add_library(simulator
        SimWorld.cpp
        include/simulator/SimWorld.h
        )
target_link_libraries(simulator
        PRIVATE ${BULLET_LIBRARIES}
        )
target_include_directories(simulator
        INTERFACE include
        PRIVATE include/simulator
        PUBLIC ${BULLET_INCLUDE_DIRS}
)