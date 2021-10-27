/*
 * Copyright 2021-10-21 <fox>
 */
#ifndef _HOME_FOX_CLIONPROJECTS_EVENTBUS_DEMO_APPLICATION_H_
#define _HOME_FOX_CLIONPROJECTS_EVENTBUS_DEMO_APPLICATION_H_

#include <memory>
#include <unordered_map>
#include "common/Component.h"

class Application {
 public:
    explicit Application(int argc , char* argv[]);
    virtual ~Application();
    void run();

 private:
    std::unordered_map<std::string ,std::shared_ptr<Component>> modules;
};

#endif // _HOME_FOX_CLIONPROJECTS_EVENTBUS_DEMO_APPLICATION_H_