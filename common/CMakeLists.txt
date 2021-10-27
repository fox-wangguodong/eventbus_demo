project(common)

add_library(Component SHARED
        Component.cpp Component.h
        EventBus.cpp EventBus.h
        Event.cpp Event.h
        ThreadPool.cpp ThreadPool.h
        EventLoopManager.cpp EventLoopManager.h
        )

add_library(SingleInstance SHARED
        SingleInstance.cpp SingleInstance.h
        )