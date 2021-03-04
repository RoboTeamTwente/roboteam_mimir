add_library(graphics
        VisualizedSimWorld.cpp
        VisualizedSimBot.cpp
        include/graphics/VisualizedSimBot.h
        VisualizedSimBall.cpp
        include/graphics/VisualizedSimBall.h
        VisualizedSimField.cpp
        include/graphics/VisualizedSimField.h
        VisualizerBall.cpp
        VisualizerBox.cpp
        VisualizerCylinder.cpp
        include/graphics/VisualizerUtil.h
        TexturedGroundPlane.cpp
        )

target_link_libraries(graphics
        PUBLIC simulator
        PUBLIC Qt5::Core
        PUBLIC Qt5::Widgets
        )
target_include_directories(graphics
        INTERFACE include
        PRIVATE include/graphics
        )