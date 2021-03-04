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
        mimir/interface/Visualizer3D.cpp
        mimir/interface/Visualizer3D.h
        mimir/interface/VisualizerCamera.cpp
        mimir/interface/VisualizerCamera.h
        mimir/interface/VisualizerDebugDrawer.cpp
        mimir/interface/VisualizerDebugDrawer.h
        )
target_link_libraries(Mimir
        PRIVATE Qt5::Core
        PRIVATE Qt5::Widgets
        PRIVATE simulator
        PRIVATE net
        PRIVATE graphics
        )
target_include_directories(Mimir
        PRIVATE mimir
        )
#target_compile_options(Mimir PUBLIC
#        -O2
#        -march=native
#        -flto)