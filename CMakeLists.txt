cmake_minimum_required(VERSION 3.20)
project(eventbus_demo)

set(CMAKE_CXX_STANDARD 17)

#third part : muduo glog gflags
include_directories(/usr/local/include)
link_directories(/usr/local/lib)


add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/common)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/modules/module_send)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/modules/module_recv)


add_executable(Demo main.cpp Application.cpp Application.h)
target_link_libraries(Demo
        SingleInstance
        MyComponent
        HerComponent
        glog
        gflags
        gtest
        -lmuduo_net
        -lmuduo_base
        -lpthread
        )
