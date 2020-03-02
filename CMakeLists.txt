cmake_minimum_required(VERSION 3.1)
project(BST)

set(CMAKE_CXX_STANDARD 14)
set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall")

add_executable(BST main.cpp BST.h)