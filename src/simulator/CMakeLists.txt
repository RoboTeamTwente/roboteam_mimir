add_library(simulator
        SimWorld.cpp
        SimWorld.h
        )
target_link_libraries(simulator
        PRIVATE ${BULLET_LIBRARIES}
        )
target_include_directories(simulator
        PUBLIC ${BULLET_INCLUDE_DIRS}
)