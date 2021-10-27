//
// Created by fox on 2021/9/15.
//

#ifndef EVENTBUS_DEMO_TEST1EVENT_H
#define EVENTBUS_DEMO_TEST1EVENT_H

#include "../../../common/Event.h"

class Test1Event : public Event{
public:
    int x;
    int y;
};

#endif //EVENTBUS_DEMO_TEST1EVENT_H
