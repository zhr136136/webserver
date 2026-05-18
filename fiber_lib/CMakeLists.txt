cmake_minimum_required(VERSION 3.10)
project(coroutine_lib)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 1. 把所有模块的头文件目录都加进来，保证编译器能找到 .h 文件
include_directories(
    ./1thread
    ./2fiber
    ./3scheduler
    ./4timer
    ./5iomanager
    ./6hook
)

# 2. 把所有模块的 .cpp 文件都列出来
set(SRC_FILES
    ./1thread/thread.cpp
    ./2fiber/fiber.cpp
    ./3scheduler/scheduler.cpp
    ./4timer/timer.cpp
    ./5iomanager/ioscheduler.cpp
    ./6hook/hook.cpp
    ./6hook/fd_manager.cpp
)

# 3. 编译成静态库 libcoroutine.a
add_library(coroutine STATIC ${SRC_FILES})
