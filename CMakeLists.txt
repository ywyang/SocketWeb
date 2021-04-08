cmake_minimum_required(VERSION 2.0)
project(webcpp)

set(CMAKE_CXX_STANDARD 14)

link_libraries(pthread)
add_executable(webcpp src/main.cpp src/Server/SocketListener.cpp src/Server/SocketListener.h src/Server/ClientHandler.h src/Home.cpp src/Home.h)
