add_library(interface
        MainWindow.cpp
        include/interface/MainWindow.h
        DebugVisualization.cpp
        DebugVisualization.h
        DebugDrawer.cpp
        DebugDrawer.h
        SettingsWidget.cpp
        SettingsWidget.h
        Visualizer.cpp
        Visualizer.h
        )
target_link_libraries(interface
        PUBLIC Qt5::Core
        PUBLIC Qt5::Widgets
        PUBLIC simulator
        PRIVATE net ##TODO Fix this hack for QT autogen whining
        PRIVATE mimir_proto
        PRIVATE ${BULLET_LIBRARIES}
        )
target_include_directories(interface
        INTERFACE include
        PRIVATE include/interface
        PUBLIC ${BULLET_INCLUDE_DIRS}
        )