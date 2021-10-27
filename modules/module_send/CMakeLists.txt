project(module_send)



add_library(HerComponent SHARED
        components/HerComponent.cpp components/HerComponent.h
        events/Test2Event.cpp events/Test2Event.h)
target_link_libraries(HerComponent
        Component)