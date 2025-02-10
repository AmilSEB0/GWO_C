cmake_minimum_required(VERSION 3.30)
project(GWO_C)

set(CMAKE_CXX_STANDARD 20)

add_executable(GWO_C main.cpp
        GWO.h
        GWO.cpp
        Optimizer.h
        Optimizer.cpp
        utils/Agent.h
        utils/Agent.cpp
        utils/Target.h
        utils/Target.cpp
        utils/Space.cpp
        utils/Space.h)
