//
// Created by fox on 2021/9/15.
//

#ifndef EVENTBUS_DEMO_TEST2EVENT_H
#define EVENTBUS_DEMO_TEST2EVENT_H

#include <string>
#include "../../../common/Event.h"

class Test2Event : public Event{
public:
    std::string data;
};


#endif //EVENTBUS_DEMO_TEST2EVENT_H
