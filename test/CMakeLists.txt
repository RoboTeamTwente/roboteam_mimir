find_package(GTest REQUIRED)

add_executable(mimir_tests
        main.cpp
        net/PublisherTest.cpp
        net/mockReceiver.h
        net/ReceiverTest.cpp)

target_link_libraries(mimir_tests
        PRIVATE net
        PRIVATE gtest
        PRIVATE gtest_main
        PRIVATE pthread #GTest needs pthread for some reason.
        )
