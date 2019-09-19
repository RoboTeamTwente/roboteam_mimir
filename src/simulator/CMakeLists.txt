add_library(simulator
        simWorld.cpp
        simWorld.h
        )
target_link_libraries(simulator
        ${BULLET_LIBRARIES}
)
target_include_directories(simulator
        PRIVATE ${BULLET_INCLUDE_DIRS}
)