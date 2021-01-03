#pragma once

#include <memory>

#include "http_defs.h"
#include "request.h"
#include "utils/memory.h"

namespace tyche::http {
    class HttpParser {
    public:
        enum [[maybe_unused]] ParseStatus{STARTING, HEADERS, BODY, FINISH,
                                          ERROR};
        static std::size_t maxUrlLength;
        static std::size_t maxHeaderLength;

        void reset();

        ParseStatus recv(const utils::shared_array<char>& buffer);

        std::shared_ptr<Request> getRequest();

        HttpParser();

    private:
        ParseStatus _status;
        std::size_t _offset{};
        std::shared_ptr<Request> _request;
        utils::shared_array<char> _method_buffer;
        utils::shared_array<char> _url_buffer;
        utils::shared_array<char> _version_buffer;
        utils::shared_array<char> _header_buffer;
        utils::shared_array<char> _body_buffer;
    };

}  // namespace tyche::http