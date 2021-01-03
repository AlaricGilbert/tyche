#include <iostream>

#include "service.h"
#include "tyche_server.h"

using namespace tyche::utils;
using namespace tyche::http;
using namespace tyche::router;
using namespace tyche;

int main() {
    TycheServer server("127.0.0.1", 8080);
    server.tieRootService(bbs::Service());
    server.start();
    return 0;
}