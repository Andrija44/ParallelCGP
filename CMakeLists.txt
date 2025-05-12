cmake_minimum_required(VERSION 3.10.0)
project(ParallelCGP VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(Boost_USE_STATIC_LIBS ON)

include_directories(include)

find_package(OpenMP REQUIRED)
find_package(Boost CONFIG COMPONENTS program_options random REQUIRED)

add_executable(${PROJECT_NAME}
        cgp/CGP.cpp cgp/CGPIndividual.cpp boolProblem/BoolProblem.cpp main.cpp adProblem/ADProblem.cpp funcProblem/FuncProblem.cpp waitProblem/WaitProblem.cpp)

if (WIN32)
        target_link_libraries(${PROJECT_NAME} -static -fopenmp)
else()
        target_link_libraries(${PROJECT_NAME} -lgomp)
endif()
target_link_libraries(${PROJECT_NAME} ${Boost_LIBRARIES})