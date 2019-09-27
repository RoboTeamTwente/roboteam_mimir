add_library(simulator
        SimWorld.cpp
        include/simulator/SimWorld.h
        Simulator.cpp
        Simulator.h
        RobotConfigWidget.cpp
        RobotConfigWidget.h
        RobotSettings.h)
target_link_libraries(simulator
        PRIVATE Qt5::Core
        PRIVATE ${BULLET_LIBRARIES}
        )
target_include_directories(simulator
        INTERFACE include
        PRIVATE include/simulator
        PUBLIC ${BULLET_INCLUDE_DIRS}
)