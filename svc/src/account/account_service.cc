#include "account/account_service.h"
using namespace tyche::http;
using namespace tyche::router;
using namespace tyche::utils;
namespace bbs::account {
    void AccountService::initRouter(Router& router) const {
        router.handle<GET>(
            "getkey",
            [](const Request& request, const RouteContext& ctx,
               Response& response) -> void {
                response = std::move(Response(OK, HTTP_1_1));
                response["Content-Type"].emplace_back("application/json");
                response["Content-Type"].emplace_back("charset=utf-8");
                response.setBody(from_string(
                    R"({
    "code": 200,
    "data": {
        "salt":"8acd92ef",
        "key":"__SOME_RSA_PUB_KEY__"
    }
}
)"));
            });
    }
}  // namespace bbs::account