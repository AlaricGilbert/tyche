#include "http/http_parser.h"

#include <cstring>

#include "utils/strhash.h"
namespace tyche::http {
    std::size_t HttpParser::maxUrlLength = 2048;
    std::size_t HttpParser::maxHeaderLength = 8192;

    void HttpParser::reset() {
        _status = STARTING;
        _offset = 0;
        _request = std::make_shared<Request>();
        _method_buffer = utils::shared_array<char>(8);
        _version_buffer = utils::shared_array<char>(16);
        _url_buffer = utils::shared_array<char>(maxUrlLength);
        _header_buffer = utils::shared_array<char>(maxHeaderLength);
    }

    HttpParser::ParseStatus HttpParser::recv(
        const utils::shared_array<char>& buffer) {
        using namespace tyche::utils;
        std::size_t buffer_idx = 0;
        while (_offset < buffer.length()) {
            switch (_status) {
            case STARTING: {
                std::size_t _url_buffer_length = _url_buffer.length();
                std::size_t _buffer_length = buffer.length();
                std::size_t curr_idx;
                while ((_offset < _url_buffer_length) &&
                       (buffer_idx < _buffer_length) &&
                       (_url_buffer[_offset] = buffer[buffer_idx]) != '\n')
                    _offset++, buffer_idx++;
                if (_offset == maxUrlLength) {
                    // TODO
                    _status = ERROR;
                    return ERROR;
                }
                if (buffer_idx == _buffer_length) return _status;
                _offset = 0;
                while ((_offset < 8) &&
                       (_method_buffer[_offset] = _url_buffer[_offset]) != ' ')
                    _offset++;
                if (_offset == 8) {
                    // TODO
                    _status = ERROR;
                    return ERROR;
                }
                _method_buffer[_offset] = '\0';
                switch (strhash(_method_buffer.get())) {
                case "GET"_hash: _request->_method = GET; break;
                case "HEAD"_hash: _request->_method = HEAD; break;
                case "POST"_hash: _request->_method = POST; break;
                case "PUT"_hash: _request->_method = PUT; break;
                case "DELETE"_hash: _request->_method = DELETE; break;
                case "CONNECT"_hash: _request->_method = CONNECT; break;
                case "OPTIONS"_hash: _request->_method = OPTIONS; break;
                case "TRACE"_hash: _request->_method = TRACE; break;
                case "PATCH"_hash: _request->_method = PATCH; break;
                default:
                    // TODO
                    _request->_method = UNKNOWN;
                    _status = ERROR;
                    return ERROR;
                }

                curr_idx = _offset + 1;
                while ((curr_idx < maxUrlLength) &&
                       (_url_buffer.get()[curr_idx] != ' '))
                    curr_idx++;
                _request->_url = shared_array<char>(curr_idx - _offset - 1);
                std::copy_n(_url_buffer.get() + _offset + 1,
                            _request->_url.length(), _request->_url.get());

                _offset = curr_idx + 1;
                curr_idx = 0;
                // RFC 2616 defines CR LF as EOF marker.
                while ((curr_idx < 16) && (_offset + curr_idx < maxUrlLength)) {
                    _version_buffer[curr_idx] = _url_buffer[_offset];
                    if (curr_idx > 0 && _version_buffer[curr_idx - 1] == '\r' &&
                        _version_buffer[curr_idx] == '\n') {
                        buffer_idx++;
                        break;
                    }
                    _offset++, curr_idx++;
                }
                _version_buffer[curr_idx - 1] = '\0';
                switch (strhash(_version_buffer.get())) {
                case "HTTP/1.0"_hash: _request->_version = HTTP_1_0; break;
                case "HTTP/1.1"_hash: _request->_version = HTTP_1_1; break;
                case "HTTP/2"_hash:
                case "HTTP/2.0"_hash: _request->_version = HTTP_2; break;
                default:
                    // TODO
                    _status = ERROR;
                    return ERROR;
                }
                _offset = 0;
                _status = HEADERS;
            }
            case HEADERS: {
                std::size_t _header_buffer_length = _header_buffer.length();
                std::size_t _buffer_length = buffer.length();
                while ((_offset < _header_buffer_length) &&
                       (buffer_idx < _buffer_length)) {
                    _header_buffer[_offset] = buffer[buffer_idx];
                    if ((_offset >= 3) &&
                        (strhash(_header_buffer.get() + _offset - 3, 4) ==
                         "\r\n\r\n"_hash))
                        break;
                    _offset++, buffer_idx++;
                }
                if (_offset == maxHeaderLength) {
                    // TODO
                    _status = ERROR;
                    return ERROR;
                }
                if (buffer_idx == _buffer_length) return _status;

                std::size_t header_length = _offset + 1;
                std::string key;
                std::string key_lower;
                bool parsing_key = true;
                bool contains_body = false;
                _offset = 0;

                while (_offset < header_length) {
                    if (_header_buffer[_offset] == '\n') {
                        break;
                    }
                    std::size_t line_start = _offset;
                    std::size_t value_start = _offset;
                    while (_offset < header_length) {
                        if (_header_buffer[_offset] == '\n') {
                            if (parsing_key) return ERROR;

                            _request->_headers[key].emplace_back(
                                std::string(_header_buffer.get(), value_start,
                                            _offset - value_start - 1));
                            if (key_lower == "content-length") {
                                contains_body = true;
                                std::size_t body_length =
                                    std::stoull(_request->_headers[key][0]);
                                _body_buffer = shared_array<char>(body_length);
                            }
                            break;
                        }
                        if (parsing_key) {
                            if (_header_buffer[_offset] == ':') {
                                parsing_key = false;
                                key = std::string(_header_buffer.get(),
                                                  line_start,
                                                  _offset - line_start);
                                key_lower = key;
                                std::transform(
                                    key_lower.begin(), key_lower.end(),
                                    key_lower.begin(), [](unsigned char c) {
                                        return std::tolower(c);
                                    });
                                value_start = _offset + 1;
                            }
                        } else {
                            if (_header_buffer[_offset] == ';') {
                                _request->_headers[key].emplace_back(
                                    std::string(_header_buffer.get(),
                                                value_start,
                                                _offset - value_start - 1));
                                value_start = _offset + 1;
                            }
                        }
                        _offset++;
                    }
                    _offset++;
                    parsing_key = true;
                }

                if (!contains_body) {
                    _status = FINISH;
                    return _status;
                }
                _status = BODY;
            }
            case BODY: {
                std::size_t length = buffer.length() - buffer_idx;
                std::copy_n(buffer.get() + buffer_idx, length,
                            _body_buffer.get() + _offset);
                _offset += length;
                if (_offset == _body_buffer.length() - 1) _status = FINISH;
                break;
            }
            default: break;
            }
        }
        return _status;
    }

    std::shared_ptr<Request> HttpParser::getRequest() { return _request; }

    HttpParser::HttpParser()
        : _status(STARTING),
          _offset(0),
          _request(std::make_shared<Request>()),
          _method_buffer(8),
          _version_buffer(16),
          _url_buffer(maxUrlLength),
          _header_buffer(maxHeaderLength) {}
}  // namespace tyche::http