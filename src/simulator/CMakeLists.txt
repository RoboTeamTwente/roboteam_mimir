add_subdirectory(net)

add_library(simulator
        physics/SimField.cpp
        physics/SimField.h
        physics/SimWorld.cpp
        physics/SimWorld.h
        Simulator.cpp
        include/simulator/Simulator.h
        config/RobotSettings.h
        ConfigWidget.cpp
        ConfigWidget.h
        config/WorldConfig.h
        config/WorldConfig.cpp
        config/RobotConfig.cpp
        config/RobotConfig.h
        config/RobotSettings.cpp
        config/WorldSettings.cpp
        config/WorldSettings.h
        physics/SimBall.cpp
        physics/SimBall.h
        physics/SimBot.cpp
        physics/SimBot.h
        physics/BaseSimBot.h
        physics/RobotMesh.cpp
        physics/RobotMesh.h
        physics/CollisionShared.cpp
        physics/CollisionShared.h
        physics/Camera.cpp
        physics/Camera.h
        config/SituationWorld.cpp
        config/SituationWorld.h
        config/SituationBot.cpp
        config/SituationBot.h
        config/SituationBall.cpp
        config/SituationBall.h
        config/Situation.cpp config/Situation.h)
target_link_libraries(simulator
        PRIVATE net
        PRIVATE Qt5::Core
        PRIVATE ${BULLET_LIBRARIES}
        )
target_include_directories(simulator
        INTERFACE include
        PRIVATE include/simulator
        PUBLIC ${BULLET_INCLUDE_DIRS}
)