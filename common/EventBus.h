//
// Created by fox on 2021/9/6.
//

#ifndef EVENTBUS_DEMO_EVENTBUS_H
#define EVENTBUS_DEMO_EVENTBUS_H

#include <unordered_map>
#include <any>
#include <memory>
#include <mutex>

#include "Event.h"
#include "Component.h"
#include "ThreadPool.h"

struct Subscriber{
    std::string name;//订阅者名称:typeid(Component).name
    std::weak_ptr<muduo::net::EventLoop> mainEventLoop;//组件的mainEventLoop
    std::unordered_map<std::string ,std::any> methods;//订阅者的回调方法:<topic,callback>
};

class EventBus {
public:
    static EventBus* getInstance();

    template<typename Topic>
    void Subscribe(Component *context, std::function<void(std::shared_ptr<Topic>)> callback)
    {
        m_subscriber_lock.lock();

        std::shared_ptr<Subscriber> target_subscriber;
        //查询当前Component是否已经注册该Topic
        for(auto iter_subscriber = m_subscribers.begin(); iter_subscriber != m_subscribers.end(); ++iter_subscriber)
        {
            std::string topic = iter_subscriber->first;
            std::shared_ptr<Subscriber> subscriber = iter_subscriber->second;
            if(topic == typeid(Topic).name() && subscriber->name == typeid(*context).name()){
                target_subscriber = subscriber;
                break;
            }
        }

        //若当前Component没有订阅过该Topic,则创建后放入订阅者列表
        if(target_subscriber == nullptr){
            target_subscriber = std::make_shared<Subscriber>();
            target_subscriber->name = typeid(*context).name();
            target_subscriber->mainEventLoop = GetEventLoop(context);
            m_subscribers.insert(std::make_pair(typeid(Topic).name(),target_subscriber));
        }

        //查询是否已经添加过当前Topic的回调函数,若之前已经添加过回调函数,则删除回调函数
        for(auto iter_method = target_subscriber->methods.begin(); iter_method != target_subscriber->methods.end(); ++iter_method)
        {
            if(iter_method->first == typeid(Topic).name()){
                target_subscriber->methods.erase(iter_method);
                break;
            }
        }

        //添加新的回调函数
        target_subscriber->methods.insert(std::make_pair(typeid(Topic).name(), callback));

        m_subscriber_lock.unlock();
    }

    template<typename Topic>
    void UnSubscribe(Component *context){
        m_subscriber_lock.lock();

        std::shared_ptr<Subscriber> target_subscriber;
        std::unordered_multimap<std::basic_string<char>, std::shared_ptr<Subscriber>>::iterator iter_target_subscriber;
        //查询当前Component是否已经注册该Topic
        for(auto iter_subscriber = m_subscribers.begin(); iter_subscriber != m_subscribers.end(); ++iter_subscriber)
        {
            std::string topic = iter_subscriber->first;
            std::shared_ptr<Subscriber> subscriber = iter_subscriber->second;
            if(topic == typeid(Topic).name() && subscriber->name == typeid(*context).name()){
                target_subscriber = subscriber;
                iter_target_subscriber = iter_subscriber;//记录这个subscriber
                break;
            }
        }

        //若当前Component没有订阅过该Topic,则直接返回
        if(target_subscriber == nullptr){
            m_subscriber_lock.unlock();
            return;
        }

        //若当前Component订阅过当前Topic,则遍历所有方法
        for(auto iter_method = target_subscriber->methods.begin(); iter_method != target_subscriber->methods.end(); ++iter_method)
        {
            if(iter_method->first == typeid(Topic).name()){
                target_subscriber->methods.erase(iter_method);
                break;
            }
        }

        //若当前订阅的Component未订阅过任何Topic则删除这个Component
        if(target_subscriber->methods.size() == 0){
            if(iter_target_subscriber != m_subscribers.end()){
                m_subscribers.erase(iter_target_subscriber);
            }
        }

        m_subscriber_lock.unlock();
    }

    template<typename Topic>
    void PostMessage(std::shared_ptr<Topic> baseEvent){
        m_subscriber_lock.lock();
        for(auto iter_subscriber = m_subscribers.begin(); iter_subscriber != m_subscribers.end(); ++iter_subscriber)
        {
            std::string topic = iter_subscriber->first;
            std::shared_ptr<Subscriber> subscriber = iter_subscriber->second;
            if(topic == typeid(Topic).name()){
                auto callback = subscriber->methods[typeid(Topic).name()];
                if(auto mainEventLoop = subscriber->mainEventLoop.lock()){
                    mainEventLoop->queueInLoop([=](){
                        std::shared_ptr<Topic> event = baseEvent;
                        std::invoke(std::any_cast<std::function<void(std::shared_ptr<Topic>)>>(callback),std::forward<std::shared_ptr<Topic>>(event));
                    });
                }
            }
        }
        m_subscriber_lock.unlock();
    }

private:
    explicit EventBus();
    virtual ~EventBus();
    std::mutex m_subscriber_lock;
    std::unordered_multimap<std::string,std::shared_ptr<Subscriber>> m_subscribers;//存储某个Topic的所有订阅者<Topic类型,订阅者>

private:
    static EventBus* instance;
    static std::mutex lock;
};



#endif //EVENTBUS_DEMO_EVENTBUS_H
