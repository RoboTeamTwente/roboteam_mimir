add_library(graphics
        VisualizedSimWorld.cpp
        VisualizedSimBot.cpp include/graphics/VisualizedSimBot.h VisualizedBall.cpp include/graphics/VisualizedBall.h VisualizedBox.cpp include/graphics/VisualizedBox.h VisualizedCylinder.cpp include/graphics/VisualizedCylinder.h)
target_link_libraries(graphics
        PUBLIC simulator
        )
target_include_directories(graphics
        INTERFACE include
        PRIVATE include/graphics
        )