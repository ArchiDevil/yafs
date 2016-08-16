#pragma once

#include <list>

template<typename T>
class observer;

template<typename T>
class notifier
{
public:
    notifier() = default;
    virtual ~notifier() = default;

    void add_observer(observer<T> * obs)
    {
        observers.push_back(obs);
    }

    void remove_observer(observer<T> * obs)
    {
        observers.remove(obs);
    }

    virtual void notifyAll(const T& ev)
    {
        for (auto * object : observers)
            object->handleEvent(ev);
    }

protected:
    std::list<observer<T>*> observers;
};

template<typename T>
class observer
{
public:
    observer() = default;
    virtual ~observer() = default;

    void subscribe(notifier<T> * notifier)
    {
        if (notifier)
            notifier->add_observer(this);
    }
    void unsubscribe(notifier<T> * notifier)
    {
        if (notifier)
            notifier->remove_observer(this);
    }

    virtual bool handleEvent(const T & event) = 0;

protected:
    notifier<T> * cur_notifier = nullptr;
};

template<typename T>
class scoped_subscriber
{
public:
    scoped_subscriber(notifier<T> * notifier, observer<T> * observer)
        : notifier(notifier)
        , observer(observer)
    {
        if (observer)
            observer->subscribe(notifier);
    }

    ~scoped_subscriber()
    {
        if (observer)
            observer->unsubscribe(notifier);
    }

    scoped_subscriber(const scoped_subscriber&) = delete;
    scoped_subscriber(scoped_subscriber&&) = delete;
    scoped_subscriber& operator=(const scoped_subscriber&) = delete;
    scoped_subscriber& operator=(scoped_subscriber&&) = delete;

private:
    notifier<T> * notifier = nullptr;
    observer<T> * observer = nullptr;

};
