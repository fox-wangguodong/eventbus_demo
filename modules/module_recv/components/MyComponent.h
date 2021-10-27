//
// Created by fox on 2021/10/6.
//

#ifndef EVENTBUS_DEMO_MYCOMPONENT_H
#define EVENTBUS_DEMO_MYCOMPONENT_H

#include "muduo/net/EventLoop.h"
#include "../../../common/EventBus.h"
#include "../events/Test1Event.h"

class MyComponent : public Component {
public:
    explicit MyComponent(std::weak_ptr<muduo::net::EventLoop> eventLoop);
    ~MyComponent() override;

    void Init() override;
    void Start() override;
    void Resume() override;
    void Stop() override;
    void UnInit() override;

private:
    muduo::net::TimerId timerId;
};


#endif //EVENTBUS_DEMO_MYCOMPONENT_H
