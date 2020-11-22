add_subdirectory(net)
add_subdirectory(proto)
add_subdirectory(simulator)
add_subdirectory(interface)

add_executable(Mimir
        mimir.cpp
        )
target_link_libraries(Mimir
        PRIVATE simulator
        PRIVATE interface
        )