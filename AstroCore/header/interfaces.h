#ifndef __INTERFACES_H__
#define __INTERFACES_H__
namespace Astrolib
{
    class Observer
    {
    public:
        virtual ~Observer(){};
        virtual void OnNotify(const Entity2D *entity, std::string eventName);
    };

    class Signaler
    {
    public:
       

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
            }
        }
        void RemoveObserver(Observer* observer, std::string eventName)
        {
            if (observerMap.find(eventName) != observerMap.end())
            {
                std::vector<Observer*> obs = observerMap[eventName];
                auto index = std::find(obs.end(), obs.begin(), observer);
            }
        }

    protected:
        std::map<std::string, std::vector<Observer*>> observerMap;
        void SendEvent(Entity2D *entity, std::string eventName)
        {
            if (observerMap.find(eventName) != observerMap.end())
            {
                for (Observer *observer : observerMap[eventName])
                {
                    observer->OnNotify(entity, eventName);
                }
            }
        }
    };
};
#endif // __INTERFACES_H__