add_library(graphics
        VisualizedSimWorld.cpp
        )
target_link_libraries(graphics
        PUBLIC simulator
        )
target_include_directories(graphics
        INTERFACE include
        PRIVATE include/graphics
        )