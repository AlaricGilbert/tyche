#pragma once
#include <netinet/in.h>

#include <exception>
#include <memory>
#include <string>
#include <unordered_map>

#include "http/http_defs.h"
#include "utils/memory.h"
namespace tyche::http {
    class HttpParser;
    class HttpServer;
    class Request {
    private:
        friend HttpParser;
        friend HttpServer;
        HttpMethod _method;
        HttpVersion _version;
        HeaderContainer _headers;
        char _peerIP[INET_ADDRSTRLEN];
        uint16_t _peerPort;
        utils::shared_array<char> _url;
        utils::shared_array<char> _body;

    public:
        HttpMethod getMethod() const noexcept;
        const HeaderContainer& getHeadersRef() const noexcept;
        bool containsHeader(const std::string& key) const noexcept;
        HttpVersion getVersion() const noexcept;
        utils::shared_array<char> getUrl() const noexcept;
        utils::shared_array<char> getBody() const noexcept;
        std::string getUrlString() const noexcept;
        const std::vector<std::string>& operator[](
            const std::string& key) const;
    };
}  // namespace tyche::http