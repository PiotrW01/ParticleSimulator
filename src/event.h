#pragma once
#include <vector>
#include <functional>

class Event
{
    public:
        void subscribe(const std::function<void()>& callback);
        void unsubscribe(const std::function<void()>* callback);
        void trigger();
    private:
        std::vector<std::function<void()>> listeners;      
};