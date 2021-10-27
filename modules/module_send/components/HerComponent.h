//
// Created by fox on 2021/10/6.
//

#ifndef EVENTBUS_DEMO_HERCOMPONENT_H
#define EVENTBUS_DEMO_HERCOMPONENT_H

#include "muduo/net/EventLoop.h"
#include "../../../common/Component.h"
#include "../../../common/ThreadPool.h"

class HerComponent : public Component {
public:
    explicit HerComponent(std::weak_ptr<muduo::net::EventLoop> eventLoop);
    ~HerComponent() override;

    void Init() override ;
    void Start() override;
    void Resume() override;
    void Stop() override;
    void UnInit() override;

    int add(int x,int y);
    int sum = 0;
private:
    std::shared_ptr<ThreadPool> threadPool;
};

#endif //EVENTBUS_DEMO_HERCOMPONENT_H
