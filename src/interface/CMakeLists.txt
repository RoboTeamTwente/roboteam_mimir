add_library(interface
        MainWindow.cpp
        include/interface/MainWindow.h
        DebugVisualization.cpp
        DebugVisualization.h
        DebugDrawer.cpp
        DebugDrawer.h)
target_link_libraries(interface
        PUBLIC Qt5::Core
        PUBLIC Qt5::Widgets
        PRIVATE ${BULLET_LIBRARIES}
        )
target_include_directories(interface
        INTERFACE include
        PRIVATE include/interface
        PUBLIC ${BULLET_INCLUDE_DIRS}
        )