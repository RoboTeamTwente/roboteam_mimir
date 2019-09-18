add_library(net STATIC
        Publisher.cpp
        Publisher.h
        Receiver.cpp
        Receiver.h
        )

target_link_libraries(net
        PRIVATE Qt5::Network
        PUBLIC mimir_proto
)