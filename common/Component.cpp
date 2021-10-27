//
// Created by fox on 2021/9/15.
//

#include "Component.h"

Component::Component(std::weak_ptr<muduo::net::EventLoop> eventLoop) {
    this->m_eventLoop = eventLoop;
}

Component::~Component(){

}

std::weak_ptr<muduo::net::EventLoop> GetEventLoop(Component *component) {
    return component->m_eventLoop;
}

void Component::postRunnable(std::function<void()> callback) {
    if(auto eventLoop = this->m_eventLoop.lock()){
        eventLoop->runInLoop(callback);
    }
}

muduo::net::TimerId Component::addTimerDelayed(double delaySeconds, std::function<void()> callback) {
    if(auto eventLoop = this->m_eventLoop.lock()){
        return eventLoop->runAfter(delaySeconds,callback);
    }
}

muduo::net::TimerId Component::addTimerAt(muduo::Timestamp time, muduo::net::TimerCallback callback) {
    if(auto eventLoop = this->m_eventLoop.lock()){
        return eventLoop->runAt(time,callback);
    }
}

muduo::net::TimerId Component::addTimerInterval(double intervalSeconds, muduo::net::TimerCallback callback) {
    if (auto eventLoop = this->m_eventLoop.lock()) {
        return eventLoop->runEvery(intervalSeconds,callback);
    }
}

void Component::cancelTimer(muduo::net::TimerId timerId) {
    if (auto eventLoop = this->m_eventLoop.lock()) {
        eventLoop->cancel(timerId);
    }
}

