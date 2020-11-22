
add_library(simulator
        SimField.cpp
        SimField.h
        SimWorld.cpp
        SimWorld.h
        settings/RobotSettings.h
        settings/RobotSettings.cpp
        settings/WorldSettings.cpp
        settings/WorldSettings.h
        SimBall.cpp
        SimBall.h
        SimBot.cpp
        SimBot.h
        BaseSimBot.h
        RobotMesh.cpp
        RobotMesh.h
        CollisionShared.cpp
        CollisionShared.h
        Camera.cpp
        Camera.h
        settings/situation/SituationWorld.cpp
        settings/situation/SituationWorld.h
        settings/situation/SituationBot.cpp
        settings/situation/SituationBot.h
        settings/situation/SituationBall.cpp
        settings/situation/SituationBall.h
        utilities/Random.cpp
        utilities/Random.h
        )
target_link_libraries(simulator
        PRIVATE net
        PRIVATE Qt5::Core
        PRIVATE lib::bullet
        )
target_include_directories(simulator
        INTERFACE include
        PRIVATE include/simulator
        PUBLIC ${BULLET_INCLUDE_DIRS}
)