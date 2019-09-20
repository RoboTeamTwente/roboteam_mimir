add_subdirectory(proto)
add_subdirectory(net)
add_subdirectory(simulator)

add_executable(Mimir
        mimir.cpp
        )
target_link_libraries(Mimir
        PRIVATE simulator
        )