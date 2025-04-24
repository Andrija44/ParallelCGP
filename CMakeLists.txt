cmake_minimum_required(VERSION 3.10.0)
project(ParallelCGP VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

include_directories(include)

find_package(OpenMP REQUIRED)

add_executable(${PROJECT_NAME}
        cgp/CGP.cpp cgp/CGPIndividual.cpp boolProblem/BoolProblem.cpp main.cpp adProblem/ADProblem.cpp funcProblem/FuncProblem.cpp waitProblem/WaitProblem.cpp)

target_link_libraries(${PROJECT_NAME} OpenMP::OpenMP_CXX)