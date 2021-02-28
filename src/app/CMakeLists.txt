add_executable(Mimir
        mimir/mimir.cpp

        mimir/interface/DebugDrawer.cpp
        mimir/interface/DebugVisualization.cpp
        mimir/interface/MainWindow.cpp
        mimir/interface/SettingsWidget.cpp
        mimir/interface/Visualizer.cpp

        mimir/ConfigWidget.cpp
        mimir/RobotConfig.cpp
        mimir/WorldConfig.cpp
        mimir/Simulator.cpp
        mimir/Situation.cpp
        mimir/Timer.cpp
        mimir/interface/Visualizer3D.cpp mimir/interface/Visualizer3D.h mimir/interface/VisualizerCamera.cpp mimir/interface/VisualizerCamera.h mimir/interface/VisualizerDebugDrawer.cpp mimir/interface/VisualizerDebugDrawer.h mimir/interface/VisualizerWorld.cpp mimir/interface/VisualizerWorld.h mimir/interface/VisualizerBall.cpp mimir/interface/VisualizerBall.h mimir/interface/VisualizerBox.cpp mimir/interface/VisualizerBox.h mimir/interface/VisualizerCylinder.cpp mimir/interface/VisualizerCylinder.h)
target_link_libraries(Mimir
        PRIVATE Qt5::Core
        PRIVATE Qt5::Widgets
        PRIVATE simulator
        PRIVATE net
        )
target_include_directories(Mimir
        PRIVATE mimir
        )
#target_compile_options(Mimir PUBLIC
#        -O2
#        -march=native
#        -flto)