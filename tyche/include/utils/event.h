#pragma once
#include <functional>
#include <list>
#include <string>
namespace tyche::utils {
    enum EventState {
        Continue,
        Cancelled,
        Handled,
        Unhandled,
    };
    template <class... Params> class Event {
        typedef std::list<
            std::pair<std::string, std::function<EventState(Params...)>>>
            HandlerList;
        HandlerList _handlers;

    public:
        Event& subscribe(const std::string& id,
                         const std::function<EventState(Params...)>& handler);
        Event& unsubscribe(const std::string& id);

        EventState fire(Params&&... params);
        EventState fire(const std::string& id, Params&&... params);
    };
    template <class... Params>
    Event<Params...>& Event<Params...>::subscribe(
        const std::string& id,
        const std::function<EventState(Params...)>& handler) {
        _handlers.emplace_back(id, handler);
        return *this;
    }

    template <class... Params>
    Event<Params...>& Event<Params...>::unsubscribe(const std::string& id) {
        for (auto it = _handlers.begin(); it != _handlers.end(); ++it) {
            if (it->first == id) {
                _handlers.erase(_handlers.begin(), it);
            }
        }
        return *this;
    }

    template <class... Params>
    EventState Event<Params...>::fire(Params&&... params) {
        for (auto it = _handlers.begin(); it != _handlers.end(); ++it) {
            EventState state;
            if ((state = it->second(params...)) != Continue) {
                return state;
            }
        }
        return Unhandled;
    }

    template <class... Params>
    EventState Event<Params...>::fire(const std::string& id,
                                      Params&&... params) {
        for (auto it = _handlers.begin(); it != _handlers.end(); ++it) {
            if (it->first == id) {
                EventState state;
                if ((state = it->second(params...)) != Continue) {
                    return state;
                }
            }
        }
        return Unhandled;
    }
}  // namespace tyche::utils