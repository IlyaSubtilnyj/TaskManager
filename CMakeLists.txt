cmake_minimum_required(VERSION 3.27.7)

set(PROJECT_NAME TM)
project(${PROJECT_NAME} LANGUAGES CXX)

find_package(Threads REQUIRED)

include_directories(src include)
set(INCLUDE_FILES include/TM.hpp include/TMConsumer.hpp include/TMJob.hpp include/ConcurrentQueue.hpp)

add_library(${PROJECT_NAME} ${INCLUDE_FILES} src/TM.cpp src/TMConsumer.cpp src/TMJob.cpp)
target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_17)