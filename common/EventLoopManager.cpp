//
// Created by fox on 2021/10/25.
//

#include "EventLoopManager.h"

EventLoopManager* EventLoopManager::instance;
std::mutex EventLoopManager::lock;

EventLoopManager *EventLoopManager::getInstance() {
    if(instance == nullptr){
        lock.lock();
        if(instance == nullptr){
            instance = new EventLoopManager();
        }
        lock.unlock();
    }
    return instance;
}

EventLoopManager::EventLoopManager() {

}

EventLoopManager::~EventLoopManager() {

}

std::weak_ptr<muduo::net::EventLoop> EventLoopManager::CreateEventLoop(const std::string name)
{
    //返回指定组件的eventloop
    for( auto item : eventLoopList )
    {
        if(item.first == name){
            return item.second;
        }
    }

    listLock.lock();

    std::promise<std::weak_ptr<muduo::net::EventLoop>> promise;
    std::future<std::weak_ptr<muduo::net::EventLoop>> future = promise.get_future();
    std::thread componentThread([](std::promise<std::weak_ptr<muduo::net::EventLoop>> &task){
        std::shared_ptr<muduo::net::EventLoop> eventLoop = std::make_shared<muduo::net::EventLoop>();
        task.set_value(eventLoop);
        eventLoop->loop();
    },std::ref(promise));
    componentThread.detach();

    std::weak_ptr<muduo::net::EventLoop> eventLoop = future.get();
    eventLoopList.emplace(name,eventLoop);

    listLock.unlock();
    return eventLoop;
}

void EventLoopManager::DestoryEventLoop(const std::string name)
{
    listLock.lock();
    for( auto item : eventLoopList )
    {
        if(item.first == name){
            auto eventLoop = item.second.lock();
            eventLoop->runInLoop([=](){
                eventLoop->quit();
            });
            eventLoopList.erase(item.first);
        }
    }
    listLock.unlock();
}

void EventLoopManager::DestoryAllEventLoop()
{
    listLock.lock();
    for( auto item : eventLoopList )
    {
        auto eventLoop = item.second.lock();
        eventLoop->runInLoop([=](){
            eventLoop->quit();
        });
        eventLoopList.erase(item.first);
    }
    listLock.unlock();
}
