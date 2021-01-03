#pragma once
#include "router/router.h"
namespace bbs::account {
    class AccountService : public tyche::router::IRoutableService {
    public:
        void initRouter(tyche::router::Router& router) const override;
    };
}  // namespace bbs::account