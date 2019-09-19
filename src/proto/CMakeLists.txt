#This command generates all the c++ protobuf files as defined in the .proto files
protobuf_generate_cpp(PROTO_SRCS PROTO_HDRS
        # output format (as defined by the RoboCup)
        messages_robocup_ssl_detection.proto
        messages_robocup_ssl_geometry.proto
        messages_robocup_ssl_wrapper.proto
        mimir_robotcommand.proto
        Vector2f.proto
)
add_library(mimir_proto SHARED
        ${PROTO_SRCS}
        ${PROTO_HDRS}
)
target_link_libraries(mimir_proto
        PUBLIC ${PROTOBUF_LIBRARIES}
)
target_include_directories(mimir_proto
        PUBLIC ${CMAKE_CURRENT_BINARY_DIR}/..
)