#pragma once

#include <unordered_map>
#include <vector>

namespace tyche::http {
    typedef std::unordered_map<std::string, std::vector<std::string>>
        HeaderContainer;
    enum [[maybe_unused]] HttpCode : uint16_t{
        Continue = 100,
        SwitchingProtocols = 101,
        Processing = 102,
        EarlyHints = 103,

        OK = 200,
        Created = 201,
        Accepted = 202,
        NonAuthoritativeInformation = 203,
        NoContent = 204,
        ResetContent = 205,
        PartialContent = 206,
        MultiStatus = 207,
        AlreadyReported = 208,
        IMUsed = 226,

        MultipleChoices = 300,
        MovedPermanently = 301,
        Found = 302,
        SeeOther = 303,
        NotModified = 304,
        UseProxy = 305,
        TemporaryRedirect = 307,
        PermanentRedirect = 308,

        BadRequest = 400,
        Unauthorized = 401,
        PaymentRequired = 402,
        Forbidden = 403,
        NotFound = 404,
        MethodNotAllowed = 405,
        NotAcceptable = 406,
        ProxyAuthenticationRequired = 407,
        RequestTimeout = 408,
        Conflict = 409,
        Gone = 410,
        LengthRequired = 411,
        PreconditionFailed = 412,
        PayloadTooLarge = 413,
        URITooLong = 414,
        UnsupportedMediaType = 415,
        RangeNotSatisfiable = 416,
        ExpectationFailed = 417,
        ImATeapot = 418,
        UnprocessableEntity = 422,
        Locked = 423,
        FailedDependency = 424,
        UpgradeRequired = 426,
        PreconditionRequired = 428,
        TooManyRequests = 429,
        RequestHeaderFieldsTooLarge = 431,
        UnavailableForLegalReasons = 451,

        InternalServerError = 500,
        NotImplemented = 501,
        BadGateway = 502,
        ServiceUnavailable = 503,
        GatewayTimeout = 504,
        HTTPVersionNotSupported = 505,
        VariantAlsoNegotiates = 506,
        InsufficientStorage = 507,
        LoopDetected = 508,
        NotExtended = 510,
        NetworkAuthenticationRequired = 511,

        MAX = 1023};
    enum [[maybe_unused]] HttpVersion : uint16_t{HTTP_1_0, HTTP_1_1, HTTP_2};
    enum [[maybe_unused]] HttpMethod{GET,     HEAD,    POST,  PUT,   DELETE,
                                     CONNECT, OPTIONS, TRACE, PATCH, UNKNOWN};

    [[maybe_unused]] inline bool is_informational(uint16_t code) {
        return (code >= 100 && code < 200);
    }

    [[maybe_unused]] inline bool is_successful(uint16_t code) {
        return (code >= 200 && code < 300);
    }

    [[maybe_unused]] inline bool is_redirection(uint16_t code) {
        return (code >= 300 && code < 400);
    }

    [[maybe_unused]] inline bool is_client_error(uint16_t code) {
        return (code >= 400 && code < 500);
    }

    [[maybe_unused]] inline bool is_server_error(uint16_t code) {
        return (code >= 500 && code < 600);
    }

    [[maybe_unused]] inline bool is_error(uint16_t code) {
        return (code >= 400);
    }

    [[maybe_unused]] const char* get_code_message(uint16_t code);

    [[maybe_unused]] const char* get_version_str(HttpVersion version);
}  // namespace tyche::http