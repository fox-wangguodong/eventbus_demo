//
// Created by fox on 2021/10/25.
//

#ifndef EVENTBUS_DEMO_EVENTLOOPMANAGER_H
#define EVENTBUS_DEMO_EVENTLOOPMANAGER_H

#include <thread>
#include <future>
#include <mutex>
#include <muduo/net/EventLoop.h>
#include "Component.h"

class EventLoopManager {
public:
    static EventLoopManager* getInstance();

    std::weak_ptr<muduo::net::EventLoop> CreateEventLoop(const std::string name);
    void DestoryEventLoop(const std::string name);
    void DestoryAllEventLoop();

protected:
    explicit EventLoopManager();
    virtual ~EventLoopManager();

private:
    static EventLoopManager* instance;
    static std::mutex lock;

    std::mutex listLock;
    std::unordered_map<std::string,std::weak_ptr<muduo::net::EventLoop>> eventLoopList;
};


#endif //EVENTBUS_DEMO_EVENTLOOPMANAGER_H
