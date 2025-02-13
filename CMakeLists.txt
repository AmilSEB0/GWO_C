cmake_minimum_required(VERSION 3.30)
project(GWO_C)

set(CMAKE_CXX_STANDARD 20)

add_executable(GWO_C main.cpp
        Optimizer.h
        Optimizer.cpp
        utils/Agent.h
        utils/Agent.cpp
        utils/Target.h
        utils/Target.cpp
        utils/FloatVar.h
        utils/FloatVar.cpp
        utils/Problem.h
        utils/Problem.cpp
        OriginalGWO.h
        OriginalGWO.cpp)
