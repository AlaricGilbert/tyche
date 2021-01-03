#pragma once
#include <cstddef>
#include <functional>
#include <map>
#include <memory>
#include <string>

#include "http/request.h"
#include "http/response.h"
#include "i_routable_service.h"
namespace tyche::router {
    typedef std::unordered_map<std::string, std::string> RouteContext;
    typedef std::function<void(const http::Request&, const RouteContext&,
                               http::Response&)>
        RequestHandler;
    class RouterGroupControl;
    class RouterParamControl;
    class Router {
    protected:
        friend RouterGroupControl;
        friend RouterParamControl;
        std::unordered_map<http::HttpMethod, RequestHandler> _terminal;
        std::unordered_map<std::string, std::unique_ptr<Router>> _children;
        std::unique_ptr<Router> _param_children;
        std::string _param_key;
        RequestHandler _unhandled;
        enum RouteType {
            MatchString, /*  .../MatchedString/...  */
            MatchParam,  /*  .../:paramName/...     */
        } _route_type;

    public:
        RouterGroupControl group(const std::string&);
        bool route(std::string url, const http::Request& request,
                   http::Response& response, RouteContext& ctx);
        bool route(const http::Request& request, http::Response& response,
                   RouteContext& ctx);

        template <http::HttpMethod Method>
        void handle(const std::string& key, const RequestHandler& handler);
        void unhandled(const RequestHandler& handler);
    };

    template <http::HttpMethod Method>
    void Router::handle(const std::string& key, const RequestHandler& handler) {
        Router child;
        child._terminal[Method] = handler;
        _children[key] = std::make_unique<Router>(std::move(child));
    }

    class RouterGroupControl {
    private:
        Router& _router;
        const std::string& _group;

    public:
        RouterGroupControl(Router& router, const std::string& group);
        void operator<=(const std::function<void(Router&)>& rhs);
        void operator<=(const IRoutableService& svc);
    };
    class RouterParamControl {
    private:
        Router& _router;
        explicit RouterParamControl(Router& router);
        void operator<=(const std::function<void(Router&)>& rhs);
        void operator<=(const IRoutableService& svc);
    };

}  // namespace tyche::router