//
// Created by fox on 2021/9/15.
//

#ifndef EVENTBUS_DEMO_COMPONENT_H
#define EVENTBUS_DEMO_COMPONENT_H

#include <memory>
#include <muduo/net/EventLoop.h>

class Component
{
public:
    explicit Component(std::weak_ptr<muduo::net::EventLoop> eventLoop);
    virtual ~Component();
    /**
     * 模块创建初始化后调用
     */
    virtual void Init() = 0;

    /**
     * 模块启动运行后调用
     */
    virtual void Start() = 0;

    /**
     * 模块恢复运行后调用
     */
    virtual void Resume() = 0;

    /**
     * 模块将要停止运行前调用
     */
    virtual void Stop() = 0;

    /**
     * 模块将要被销毁前调用
     */
    virtual void UnInit() = 0;

    friend std::weak_ptr<muduo::net::EventLoop> GetEventLoop(Component *component);
protected:

    /**
     * 在主线程循环中执行
     * @param callback 执行的内容
     */
    void postRunnable(std::function<void()> callback);

    /**
     * 添加一个定时器
     * @param delaySeconds 延时时间,定时回调将在延时时间后执行,只执行一次
     * @param callback 定时回调
     * @return 定时器ID
     */
    muduo::net::TimerId addTimerDelayed(double delaySeconds,muduo::net::TimerCallback callback);

    /**
     * 添加一个定时器
     * @param time 时间点,回调函数将在指定时间点执行,只执行一次
     * @param callback 定时回调
     * @return 定时器ID
     */
    muduo::net::TimerId addTimerAt(muduo::Timestamp time,muduo::net::TimerCallback callback);

    /**
     * 添加一个定时器
     * @param intervalSeconds 定时回调的间隔时间,定时器定时执行回调
     * @param callback 定时回调
     * @return 定时器ID
     */
    muduo::net::TimerId addTimerInterval(double interval,muduo::net::TimerCallback callback);

    /**
     * 取消定时器
     * @param timerId 定时器ID
     */
    void cancelTimer(muduo::net::TimerId timerId);

private:
    std::weak_ptr<muduo::net::EventLoop> m_eventLoop;
};

#endif //EVENTBUS_DEMO_COMPONENT_H
