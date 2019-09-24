add_library(interface
        MainWindow.cpp
        include/interface/MainWindow.h)
target_link_libraries(interface
        PUBLIC Qt5::Core
        PUBLIC Qt5::Widgets
        )
target_include_directories(interface
        INTERFACE include
        PRIVATE include/interface
        )