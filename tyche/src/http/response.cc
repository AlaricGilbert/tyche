#include "http/response.h"

#include <cstring>
using namespace tyche::utils;
namespace tyche::http {
    std::vector<std::string>& Response::operator[](
        const std::string& key) noexcept {
        return _headers[key];
    }

    HeaderContainer& Response::getHeaderContainer() noexcept {
        return _headers;
    }

    HttpCode Response::getCode() const noexcept { return _code; }

    HttpVersion Response::getVersion() const noexcept { return _version; }

    Response::Response(HttpCode code, HttpVersion version)
        : _body(shared_array<char>(nullptr, 0)) {
        _code = code;
        _version = version;
    }

    utils::shared_array<char> Response::toResponseBuffer() noexcept {
        std::vector<std::string> content_len_vec;
        content_len_vec.emplace_back((std::to_string(_body.length())));
        _headers["Content-Length"] = content_len_vec;
        const char* version_str = get_version_str(_version);
        const char* code_msg_str = get_code_message(_code);
        std::size_t version_length = strlen(version_str);
        std::size_t code_msg_length = strlen(code_msg_str);
        std::size_t status_line_length =
            version_length + 1 + 3 + 1 + code_msg_length;
        std::size_t header_length = 0;
        for (auto& pair : _headers) {
            header_length += pair.first.length() + 3;
            for (const auto& i : pair.second) {
                header_length += i.length() + 1;
            }
            header_length -= 1;  // without last ;
            header_length += 2;  // \r\n
        }
        std::size_t full_length =
            status_line_length + 2 + header_length + 2 + _body.length();
        shared_array<char> buf(full_length);
        std::size_t idx = 0;

        std::copy_n(version_str, version_length, buf.get() + idx);
        idx += version_length;
        buf[idx++] = ' ';

        sprintf(buf.get() + idx, "%uhd", _code);
        idx += 3;
        buf[idx++] = ' ';

        std::copy_n(code_msg_str, code_msg_length, buf.get() + idx);
        idx += code_msg_length;
        buf[idx++] = '\r';
        buf[idx++] = '\n';

        for (auto& pair : _headers) {
            std::copy_n(pair.first.data(), pair.first.length(),
                        buf.get() + idx);
            idx += pair.first.length();
            buf[idx++] = ' ';
            buf[idx++] = ':';
            buf[idx++] = ' ';

            for (const auto& i : pair.second) {
                std::copy_n(i.data(), i.length(), buf.get() + idx);
                idx += i.length();
                buf[idx++] = ';';
            }
            buf[idx - 1] = '\r';
            buf[idx++] = '\n';
        }
        buf[idx++] = '\r';
        buf[idx++] = '\n';
        std::copy_n(_body.get(), _body.length(), buf.get() + idx);
        return buf;
    }

    void Response::setBody(const shared_array<char>& body) { _body = body; }
}  // namespace tyche::http