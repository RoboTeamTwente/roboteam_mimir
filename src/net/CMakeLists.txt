add_library(net STATIC
        Publisher.cpp
        include/net/Publisher.h
        Receiver.cpp
        include/net/Receiver.h
        )

target_link_libraries(net
        PUBLIC Qt5::Network
        PUBLIC mimir_proto
)
target_include_directories(net
        PRIVATE include/net
        PUBLIC include
        )