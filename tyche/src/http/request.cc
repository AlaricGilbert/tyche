#include "http/request.h"
namespace tyche::http {
    HttpMethod Request::getMethod() const noexcept { return _method; }

    const HeaderContainer& Request::getHeadersRef() const noexcept {
        return _headers;
    }

    bool Request::containsHeader(const std::string& key) const noexcept {
        return _headers.count(key) != 0;
    }

    const std::vector<std::string>& Request::operator[](
        const std::string& key) const {
        auto iterator = _headers.find(key);
        if (iterator != _headers.end()) return iterator->second;
        throw std::invalid_argument("No such key in request headers");
    }

    HttpVersion Request::getVersion() const noexcept { return _version; }

    utils::shared_array<char> Request::getUrl() const noexcept { return _url; }

    utils::shared_array<char> Request::getBody() const noexcept {
        return _body;
    }

    std::string Request::getUrlString() const noexcept {
        return std::string(_url.get(), _url.length());
    }
}  // namespace tyche::http