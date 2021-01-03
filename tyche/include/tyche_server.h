#pragma once
#include <string>

#include "http/http_server.h"
#include "router/router.h"
namespace tyche {
    class TycheServer {
    private:
        std::string _address;
        uint16_t _port;
        http::HttpServer _server;
        router::Router _router;

    public:
        TycheServer(const std::string& address, uint16_t port);
        void tieRootService(const router::IRoutableService& svc);
        void start();
    };
}  // namespace tyche