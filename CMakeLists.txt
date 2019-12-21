cmake_minimum_required(VERSION 3.10)
project(roboteam_mimir)

#find the necessary dependencies
find_package(Protobuf REQUIRED)
find_package(Qt5 COMPONENTS Core Network REQUIRED)
#find_package(Bullet REQUIRED)
#only needed for interface
find_package(Qt5 COMPONENTS Widgets)
# Instruct Cmake to run the meta-object-compiler for qt5's signal slot system automatically when needed
option(DOUBLE_PRECISION "compile using double precision physics library" ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_CXX_STANDARD 17)
if (DOUBLE_PRECISION)
    set(CMAKE_CXX_FLAGS  "-O0 -Wall -Wextra -DBT_USE_DOUBLE_PRECISION")
else()
    set(CMAKE_CXX_FLAGS  "-O0 -Wall -Wextra ")
endif()
set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake")
include(BuildBullet)
add_subdirectory(src)
add_subdirectory(test)