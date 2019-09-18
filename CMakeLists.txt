cmake_minimum_required(VERSION 3.12)
project(roboteam_mimir)

#find the necessary dependencies
find_package(Protobuf REQUIRED)
find_package(Qt5 COMPONENTS Core Network REQUIRED)

add_subdirectory(src)
add_subdirectory(test)