#pragma once
#include "router/router.h"
namespace bbs {
    class Service : public tyche::router::IRoutableService {
    public:
        void initRouter(tyche::router::Router& router) const override;
    };
}  // namespace bbs