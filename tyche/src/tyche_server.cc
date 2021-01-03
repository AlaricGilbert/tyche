#include "tyche_server.h"

#include "http/request.h"
#include "http/response.h"
using namespace tyche::http;
using namespace tyche::router;
using namespace tyche::utils;
namespace tyche {

    TycheServer::TycheServer(const std::string& address, uint16_t port) {
        _address = address;
        _port = port;
        _server.onRequestReceived.subscribe(
            "tyche",
            [this](const Request& request, Response& response) -> EventState {
                RouteContext ctx;
                if (_router.route(request.getUrlString(), request, response,
                                  ctx))
                    return Handled;
                return Unhandled;
            });
    }

    void TycheServer::tieRootService(const IRoutableService& svc) {
        svc.initRouter(_router);
    }

    void TycheServer::start() { _server.start(_address, _port); }
}  // namespace tyche