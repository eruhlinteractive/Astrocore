#ifndef __INTERFACES_H__
#define __INTERFACES_H__
#include <vector>
#include <algorithm>
#include <map>
namespace Astrocore
{
    class Signaler;
    class Observer
    {
    public:
        virtual ~Observer(){};
        virtual void OnNotify(const Signaler *signaler, std::string eventName){};
    };

    class Signaler
    {
    public:
        // Constructor
        Signaler()
        {
            observerMap = std::map<std::string, std::vector<Observer *>>();
        }

        /// @brief Get the amount of observers for a given event
        /// @param eventName The name of the event
        /// @return The number of observers of the event, or 0
        int GetObserverCount(std::string eventName)
        {
            return (observerMap.find(eventName) != observerMap.end() ? observerMap[eventName].size() : 0);
        };

        void AddObserver(Observer *observer, std::string eventName)
        {
            if (observerMap.find(eventName) != observerMap.end())
            {
                observerMap[eventName].push_back(observer);
            }
            else
            {
                observerMap.insert(std::pair{eventName, std::vector<Observer *>()});
                observerMap[eventName].push_back(observer);
            }
        }

        /// @brief Remove an observer of this signaler
        /// @param observer The observer to remove
        /// @param eventName The name of the event to remove the observer from
        void RemoveObserver(Observer *observer, std::string eventName)
        {
            if (observerMap.find(eventName) != observerMap.end())
            {
                std::vector<Observer *>::iterator index = std::find(observerMap[eventName].begin(), observerMap[eventName].end(), observer);

                // Note: Index is *not* and int, but an iterator
                if (index != observerMap[eventName].end())
                {
                    observerMap[eventName].erase(index);
                }
            }
        }

        /// @brief Is there an observer listening for this event
        /// @param observer The observer that might be watching the event
        /// @param eventName The name of the event
        /// @return TRUE if the observer is watching the event
        bool IsObservingEvent(Observer *observer, std::string eventName)
        {
            if (observerMap.find(eventName) != observerMap.end())
            {
                std::vector<Observer *>::iterator index = std::find(observerMap[eventName].begin(), observerMap[eventName].end(), observer);

                return index != observerMap[eventName].end();
            }
        }

    protected:
        std::map<std::string, std::vector<Observer *>> observerMap;

        void SendEvent(std::string eventName)
        {
            if (observerMap.find(eventName) != observerMap.end())
            {
                for (Observer *observer : observerMap[eventName])
                {
                    observer->OnNotify(this, eventName);
                }
            }
        }
    };

};
#endif // __INTERFACES_H__