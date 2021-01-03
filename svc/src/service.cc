#include "service.h"

#include "account/account_service.h"

using namespace tyche::http;
using namespace tyche::router;
using namespace tyche::utils;
using namespace bbs::account;
namespace bbs {
    void Service::initRouter(Router& router) const {
        router.group("account") <= AccountService();
    }
}  // namespace bbs