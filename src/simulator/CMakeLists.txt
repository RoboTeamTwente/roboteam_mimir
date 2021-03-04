
add_library(simulator
        SimField.cpp
        SimWorld.cpp
        settings/RobotSettings.cpp
        settings/WorldSettings.cpp
        SimBall.cpp
        SimBot.cpp
        RobotMesh.cpp
        CollisionShared.cpp
        Camera.cpp
        settings/situation/SituationWorld.cpp
        settings/situation/SituationBot.cpp
        settings/situation/SituationBall.cpp
        utilities/Random.cpp
        )
target_link_libraries(simulator
        PUBLIC mimir_proto
        PUBLIC lib::bullet
        )
target_include_directories(simulator
        INTERFACE include
        PRIVATE include/simulator
        PUBLIC ${BULLET_INCLUDE_DIRS}
)
#target_compile_options(simulator PUBLIC
#        -O3
#        -march=native
#        -flto
#        )
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
