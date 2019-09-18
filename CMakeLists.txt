cmake_minimum_required(VERSION 3.12)
project(roboteam_mimir)

#find the necessary dependencies
find_package(Protobuf REQUIRED)

add_subdirectory(src)