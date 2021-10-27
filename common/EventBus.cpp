//
// Created by fox on 2021/9/6.
//

#include "EventBus.h"

EventBus* EventBus::instance;
std::mutex EventBus::lock;

EventBus *EventBus::getInstance() {
    if(instance == nullptr){
        lock.lock();
        if(instance == nullptr){
            instance = new EventBus();
        }
        lock.unlock();
    }
    return instance;
}

EventBus::EventBus() {

}

EventBus::~EventBus() {

}



