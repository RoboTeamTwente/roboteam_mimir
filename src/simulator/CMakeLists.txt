add_library(simulator
        physics/SimField.cpp
        physics/SimField.h
        physics/SimWorld.cpp
        physics/SimWorld.h
        Simulator.cpp
        Simulator.h
        RobotSettings.h
        ConfigWidget.cpp ConfigWidget.h WorldConfig.h WorldConfig.cpp RobotConfig.cpp RobotConfig.h RobotSettings.cpp WorldSettings.cpp WorldSettings.h)
target_link_libraries(simulator
        PRIVATE Qt5::Core
        PRIVATE ${BULLET_LIBRARIES}
        )
target_include_directories(simulator
        INTERFACE include
        PRIVATE include/simulator
        PUBLIC ${BULLET_INCLUDE_DIRS}
)