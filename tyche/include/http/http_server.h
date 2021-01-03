#pragma once
#include <sys/epoll.h>

#include <memory>

#include "http_parser.h"
#include "request.h"
#include "response.h"
#include "utils/event.h"
namespace tyche::http {
    typedef std::function<utils::EventState(const Request&, Response&)>
        HttpRequestHandler;
    class HttpServer {
    private:
        HttpRequestHandler _badRequest;
        HttpRequestHandler _defaultRequest;
        int _listen_socket;
        int _epoll_fd;
        bool _running;
        uint16_t _max_connections;
        uint16_t _max_tcp_buffer_size;
        std::unique_ptr<epoll_event[]> _events;
        void addSocketEpollEvent(int socket_fd) const;

    public:
        explicit HttpServer(uint16_t maxConnections = 1024,
                            uint16_t maxTcpBufSize = 4096);
        utils::Event<const Request&, Response&> onRequestReceived;
        void start(const std::string& address, uint16_t port);
    };
}  // namespace tyche::http