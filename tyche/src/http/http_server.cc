#include "http/http_server.h"

#include <arpa/inet.h>
#include <fcntl.h>
#include <sys/socket.h>
#include <unistd.h>

#include <cstring>
using namespace tyche::utils;
namespace tyche::http {

    void HttpServer::start(const std::string& ip, uint16_t port) {
        sockaddr_in address{0};
        address.sin_family = AF_INET;
        address.sin_port = htons(port);
        inet_pton(AF_INET, ip.data(), &address.sin_addr);

        _listen_socket = socket(AF_INET, SOCK_STREAM, 0);
        // TODO: ERROR handling
        if (_listen_socket == -1) return;
        if (bind(_listen_socket, (sockaddr*)&address, sizeof(address)) == -1)
            return;
        if (listen(_listen_socket, _max_connections) != 0) return;

        _events = std::make_unique<epoll_event[]>(_max_connections);
        _epoll_fd = epoll_create(_max_connections);
        addSocketEpollEvent(_listen_socket);

        _running = true;
        while (_running) {
            int waiting_events =
                epoll_wait(_epoll_fd, _events.get(), _max_connections, -1);
            if (waiting_events < 0) {
                close(_listen_socket);
                close(_epoll_fd);
                return;
            }
            for (int i = 0; i < waiting_events; ++i) {
                if (_events[i].data.fd == _listen_socket) {
                    sockaddr peer_socket_addr{};
                    socklen_t peer_socket_length = sizeof(sockaddr);
                    int peer_socket_fd = accept(
                        _listen_socket, &peer_socket_addr, &peer_socket_length);
                    if (peer_socket_fd != -1)
                        addSocketEpollEvent(peer_socket_fd);
                } else if (_events[i].events & EPOLLIN) {
                    char buf[_max_tcp_buffer_size];
                    HttpParser p;
                    while (true) {
                        std::fill_n(buf, _max_tcp_buffer_size, 0);
                        int rcv_size = recv(_events[i].data.fd, buf,
                                            _max_tcp_buffer_size - 1, 0);
                        if (rcv_size < 0) {
                            if ((errno == EAGAIN) || (errno == EWOULDBLOCK)) {
                                break;
                            }
                            close(_events[i].data.fd);
                            break;
                        } else if (rcv_size == 0) {
                            close(_events[i].data.fd);
                        } else {
                            sockaddr_in client{0};
                            socklen_t client_len = sizeof(client);
                            getpeername(_events[i].data.fd,
                                        (struct sockaddr*)&client, &client_len);
                            auto request = p.getRequest();

                            Response r(static_cast<HttpCode>(200),
                                       request->_version);
                            inet_ntop(AF_INET, &client.sin_addr,
                                      request->_peerIP, INET_ADDRSTRLEN);
                            request->_peerPort = ntohs(client.sin_port);

                            switch (p.recv(make_shared(buf, rcv_size))) {
                            case HttpParser::STARTING:
                            case HttpParser::HEADERS:
                            case HttpParser::BODY: break;
                            case HttpParser::FINISH:
                            case HttpParser::ERROR: {
                                if (onRequestReceived.fire(*request, r) !=
                                    Handled) {
                                    _defaultRequest(*request, r);
                                }
                                auto responseBuf = r.toResponseBuffer();
                                send(_events[i].data.fd, responseBuf.get(),
                                     responseBuf.length(), 0);
                            }
                            }
                        }
                    }
                }
            }
        }
    }

    void HttpServer::addSocketEpollEvent(int socket_fd) const {
        epoll_event event{};
        event.data.fd = socket_fd;
        event.events = EPOLLIN | EPOLLET;
        epoll_ctl(_epoll_fd, EPOLL_CTL_ADD, socket_fd, &event);

        int old_option = fcntl(socket_fd, F_GETFL);
        int new_option = old_option | O_NONBLOCK;
        fcntl(socket_fd, F_SETFL, new_option);
    }

    HttpServer::HttpServer(uint16_t maxConnections, uint16_t maxTcpBufSize) {
        _max_connections = maxConnections;
        _max_tcp_buffer_size = maxTcpBufSize;

        _badRequest = [](const Request& request,
                         Response& response) -> EventState {
            response = std::move(Response(HttpCode::BadRequest, HTTP_1_1));
            return EventState::Handled;
        };
        _defaultRequest = [](const Request& request,
                             Response& response) -> EventState {
            response = std::move(Response(HttpCode::NotFound, HTTP_1_1));
            response.setBody(
                from_string(R"(
<!DOCTYPE html>
<html>
    <head>
        <meta charset=\"utf-8\">
        <title>Not Found.</title>
    </head>
    <body>
        <p><h1>404 Not Found.</h1></p>
        <p>The requested url <code>)" +
                            request.getUrlString() +
                            R"(</code> was not found on this server.</p>
        <hr>
        <p>Powered by Tyche web framework.</p>
    </body>
</html>)"));
            return EventState::Handled;
        };
    }
}  // namespace tyche::http