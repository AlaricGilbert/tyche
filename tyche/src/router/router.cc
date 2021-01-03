#include "router/router.h"

namespace tyche::router {

    RouterGroupControl Router::group(const std::string& group) {
        return RouterGroupControl(*this, group);
    }

    bool Router::route(std::string url, const http::Request& request,
                       http::Response& response, RouteContext& ctx) {
        auto pos = url.find('/');
        if (pos == 0) return route(url.substr(1), request, response, ctx);

        if (_route_type == MatchString) {
            std::string key;
            if (pos == std::string::npos) {
                pos = url.find("?");
                if (pos != std::string::npos)
                    key = url.substr(0, pos);
                else
                    key = url;
                if (_children.count(key) != 0)
                    return _children[key]->route(request, response, ctx);
            } else {
                key = url.substr(0, pos);
                if (_children.count(key) != 0)
                    return _children[key]->route(url.substr(pos + 1), request,
                                                 response, ctx);
            }
            if (_unhandled) {
                _unhandled(request, ctx, response);
                return true;
            }
            return false;
        } else {
            std::string value;
            if (pos == std::string::npos) {
                pos = url.find("?");
                if (pos != std::string::npos)
                    value = url.substr(0, pos);
                else
                    value = url;
                ctx[_param_key] = value;
                return _param_children->route(request, response, ctx);
            } else {
                value = url.substr(0, pos);
                ctx[_param_key] = value;
                return _param_children->route(url.substr(pos + 1), request,
                                              response, ctx);
            }
        }
    }

    bool Router::route(const http::Request& request, http::Response& response,
                       RouteContext& ctx) {
        if (_terminal.contains(request.getMethod())) {
            _terminal[request.getMethod()](request, ctx, response);
            return true;
        } else if (_unhandled) {
            _unhandled(request, ctx, response);
            return true;
        }
        return false;
    }

    void Router::unhandled(const RequestHandler& handler) {
        _unhandled = handler;
    }

    RouterGroupControl::RouterGroupControl(Router& router,
                                           const std::string& group)
        : _router(router), _group(group) {}

    void RouterGroupControl::operator<=(
        const std::function<void(Router&)>& rhs) {
        _router._route_type = Router::MatchString;
        _router._children[_group] = std::make_unique<Router>();
        rhs(*_router._children[_group]);
    }

    void RouterGroupControl::operator<=(const IRoutableService& svc) {
        _router._route_type = Router::MatchString;
        _router._children[_group] = std::make_unique<Router>();
        svc.initRouter(*_router._children[_group]);
    }

    RouterParamControl::RouterParamControl(Router& router) : _router(router) {}

    void RouterParamControl::operator<=(
        const std::function<void(Router&)>& rhs) {
        _router._route_type = Router::MatchParam;
        _router._param_children = std::make_unique<Router>();
        rhs(*_router._param_children);
    }

    void RouterParamControl::operator<=(const IRoutableService& svc) {
        _router._route_type = Router::MatchParam;
        _router._param_children = std::make_unique<Router>();
        svc.initRouter(*_router._param_children);
    }
}  // namespace tyche::router