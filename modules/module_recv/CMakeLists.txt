project(module_recv)

add_library(MyComponent SHARED
        components/MyComponent.cpp components/MyComponent.h
        events/Test1Event.cpp events/Test1Event.h)
target_link_libraries(MyComponent
        Component
        )

add_executable(MyComponent_test
        components/MyComponent_test.cpp)
target_link_libraries(MyComponent_test
        MyComponent
        glog
        gflags
        gtest
        gtest_main
        -lmuduo_net
        -lmuduo_base
        -lpthread
        )
add_test(NAME MyComponent_test
        COMMAND MyComponent_test)