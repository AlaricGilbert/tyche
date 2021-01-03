#pragma once
#include "http_defs.h"
#include "utils/memory.h"
namespace tyche::http {
    class Response {
    private:
        HttpCode _code;
        HttpVersion _version;
        HeaderContainer _headers;
        utils::shared_array<char> _body;

    public:
        utils::shared_array<char> toResponseBuffer() noexcept;
        std::vector<std::string>& operator[](const std::string& key) noexcept;
        HeaderContainer& getHeaderContainer() noexcept;
        HttpCode getCode() const noexcept;
        HttpVersion getVersion() const noexcept;
        Response() = delete;
        Response(HttpCode code, HttpVersion version);
        void setBody(const utils::shared_array<char>& body);
    };
}  // namespace tyche::http