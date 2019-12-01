cmake_minimum_required(VERSION 3.10)
project(roboteam_mimir)

#find the necessary dependencies
find_package(Protobuf REQUIRED)
find_package(Qt5 COMPONENTS Core Network REQUIRED)
find_package(Bullet REQUIRED)
#only needed for interface
find_package(Qt5 COMPONENTS Widgets)
# Instruct Cmake to run the meta-object-compiler for qt5's signal slot system automatically when needed
set(CMAKE_AUTOMOC ON)
set(CMAKE_CXX_STANDARD 17)
add_subdirectory(src)
add_subdirectory(test)