#include "event.h"

void Event::subscribe(const std::function<void()>& callback) 
{
    listeners.push_back(callback);
}

void Event::unsubscribe(const std::function<void()>* callback) 
{
    int removeID = -1;
    for(int i = 0; i < listeners.size(); i++)
    {
        if(&listeners[i] == callback)
        {
            removeID = i;
            break;
        }
    }
    if(removeID != -1)
    {
        listeners.erase(listeners.begin() + removeID);
    }
}

void Event::trigger() 
{
    for(auto& listener: listeners)
    {
        listener();
    }
}