//
// Created by fox on 2021/10/6.
//

#include "HerComponent.h"
#include "../../../common/EventBus.h"
#include "../../module_recv/events/Test1Event.h"
#include <glog/logging.h>


HerComponent::HerComponent(std::weak_ptr<muduo::net::EventLoop> eventLoop) : Component(eventLoop) {
    threadPool = std::make_shared<ThreadPool>(5);
}

HerComponent::~HerComponent() {
}

void HerComponent::Init() {
    LOG(INFO) << "HerComponent::Init" << std::endl;

    EventBus::getInstance()->Subscribe<Test1Event>(this,[&](std::shared_ptr<Test1Event> event){
        LOG(INFO) << "HerComponent::Subscribe:" << event->x << "," << event->y << std::endl;

        auto task = threadPool->enqueue([](int x,int y){
            std::this_thread::sleep_for(std::chrono::milliseconds (1000));
            return x+y;
        },event->x , event->y);

        LOG(INFO) << "Calc :" << task.get();
    });
}

void HerComponent::Start() {
    LOG(INFO) << "HerComponent::Start" << std::endl;
}

void HerComponent::Resume() {
    LOG(INFO) << "HerComponent::Resume" << std::endl;
}

void HerComponent::Stop() {
    LOG(INFO) << "HerComponent::Stop" << std::endl;
}

void HerComponent::UnInit() {
    LOG(INFO) << "HerComponent::UnInit" << std::endl;
    EventBus::getInstance()->UnSubscribe<Test1Event>(this);
}

int HerComponent::add(int x, int y) {
    int sum = x + y;
    this->sum += sum;
    return sum;
}
