/*
 * Copyright 2021-10-21 <fox>
 */
#include "Application.h"

#include <fstream>
#include <string>
#include <iomanip>
#include <glog/logging.h>
#include <gflags/gflags.h>
#include <muduo/net/EventLoop.h>
#include "modules/module_recv/components/MyComponent.h"
#include "modules/module_send/components/HerComponent.h"
#include "common/EventLoopManager.h"

Application::Application(int argc, char* argv[]) {
    gflags::ParseCommandLineFlags(&argc ,&argv ,true);
    google::InitGoogleLogging(argv[0]);//初始化glog日志
    google::InstallFailureSignalHandler();//捕捉系统崩溃信号
    google::InstallFailureWriter([](const char* data, int size){//打印系统崩溃信息
        std::ofstream dump(FLAGS_log_dir+"core.dump",std::ios::app);
        std::time_t nowTime = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
        dump << std::put_time(std::localtime(&nowTime), "%Y-%m-%d %H.%M.%S : ") << std::string(data,size);
        dump.close();
    });

}

Application::~Application()
{
    google::ShutdownGoogleLogging();
}

void Application::run()
{
    {
        std::weak_ptr<muduo::net::EventLoop> eventLoop = EventLoopManager::getInstance()->CreateEventLoop("MyComponent");
        std::shared_ptr<Component> component = std::make_shared<MyComponent>(eventLoop);
        modules.emplace("MyComponent",component);
    }

    {
        std::weak_ptr<muduo::net::EventLoop> eventLoop = EventLoopManager::getInstance()->CreateEventLoop("HerComponent");
        std::shared_ptr<Component> component = std::make_shared<HerComponent>(eventLoop);
        modules.emplace("HerComponent",component);
    }

    //Init
    for(auto &module : modules ) {
        auto component = module.second;
        if(auto eventLoop = GetEventLoop(component.get()).lock()){
            eventLoop->runInLoop([=](){
                component->Init();
            });
        }
    }

    std::this_thread::sleep_for(std::chrono::seconds(5));

    //UnInit
    for(auto &module : modules ) {
        auto component = module.second;
        if(auto eventLoop = GetEventLoop(component.get()).lock()){
            eventLoop->runInLoop([=](){
                component->UnInit();
            });
        }
    }

    //销毁所有的EventLoop
    for(auto &module : modules ) {
        EventLoopManager::getInstance()->DestoryEventLoop(module.first);
        modules.erase(module.first);
    }

    return;
}
