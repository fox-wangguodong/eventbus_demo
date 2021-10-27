//
// Created by fox on 2021/10/6.
//

#include "MyComponent.h"
#include <glog/logging.h>

MyComponent::MyComponent(std::weak_ptr<muduo::net::EventLoop> eventLoop) : Component(eventLoop)  {
}

MyComponent::~MyComponent() {
}

void MyComponent::Init() {
    LOG(INFO) << "MyComponent Inited" << std::endl;

    timerId = addTimerInterval(1,[&](){
        std::shared_ptr<Test1Event> test1Event = std::make_shared<Test1Event>();
        test1Event->x = rand() % 100;
        test1Event->y = rand() % 100;
        LOG(INFO) << "MyComponent::PostMessage:" << test1Event->x << "," << test1Event->y << std::endl;

        EventBus::getInstance()->PostMessage<Test1Event>(test1Event);
    });
}

void MyComponent::Start() {
    LOG(INFO) << "MyComponent Started" << std::endl;
}

void MyComponent::Resume() {
    LOG(INFO) << "MyComponent Resumed" << std::endl;
}

void MyComponent::Stop() {
    LOG(INFO) << "MyComponent will be Stop" << std::endl;
}

void MyComponent::UnInit() {
    LOG(INFO) << "MyComponent will be UnInit" << std::endl;
    cancelTimer(timerId);
}
