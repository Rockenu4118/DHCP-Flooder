cmake_minimum_required(VERSION 3.30)

set(PROJECT_NAME "DHCP_Flooder")

project(${PROJECT_NAME})

set(CMAKE_CXX_STANDARD 23)

file(GLOB SOURCES "src/*.cpp")

add_executable(${PROJECT_NAME} ${SOURCES})
