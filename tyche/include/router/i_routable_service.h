#pragma once
namespace tyche::router {
    class Router;
    class IRoutableService {
    public:
        virtual void initRouter(Router& router) const = 0;
    };
}  // namespace tyche::router