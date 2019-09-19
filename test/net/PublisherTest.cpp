//
// Created by rolf on 18-09-19.
//

#include "gtest/gtest.h"
#include "proto/messages_robocup_ssl_wrapper.pb.h"
#include "net/Publisher.h"
#include <QHostAddress>
#include "mockReceiver.h"
// function that makes a frame with a robot of specified colour and ID at position 1.0,1.0 and a ball at (-1.0,-1.0)
SSL_DetectionFrame makeFrame(unsigned int robotID){
    SSL_DetectionFrame frame;
    // set up robot
    SSL_DetectionRobot robot;
    robot.set_orientation(0.0);
    robot.set_robot_id(robotID);
    robot.set_x(1.0);
    robot.set_y(1.0);
    //set up ball
    SSL_DetectionBall ball;
    ball.set_x(-1.0);
    ball.set_y(-1.0);

    //add robot and ball to the frame
    frame.add_robots_yellow()->CopyFrom(robot);
    frame.add_balls()->CopyFrom(ball);
    return frame;
}
SSL_GeometryData makeGeometry(unsigned int camID,int goalDepth){
    SSL_GeometryData geometryData;
    SSL_GeometryCameraCalibration cam;
    cam.set_camera_id(camID);
    geometryData.add_calib()->CopyFrom(cam);

    SSL_GeometryFieldSize field;
    field.set_goal_depth(goalDepth);
    geometryData.mutable_field()->CopyFrom(field);
    return geometryData;
}
TEST(PublisherTest, sendingTest) {
    QHostAddress localIP("127.0.0.1");
    int port=10006;
    // set up a publisher and a subscriber at desired IP
    mockReceiver receiver(localIP,port);
    net::Publisher publisher(localIP,port);
    SSL_WrapperPacket packet;
    int id = 4;
    SSL_DetectionFrame frame=makeFrame(id);
    // we send a frame
    publisher.send(frame);
    // lets check if we received it as we should have
    std::vector<SSL_WrapperPacket> messages=receiver.readMessages();
    ASSERT_EQ(messages.size(),1);
    ASSERT_EQ(messages[0].has_detection(),true);
    ASSERT_EQ(messages[0].detection().robots_yellow_size(),1);

    auto bot=messages[0].detection().robots_yellow(0);
    EXPECT_EQ(bot.robot_id(),id);
    EXPECT_FLOAT_EQ(bot.orientation(),0.0);
    EXPECT_FLOAT_EQ(bot.x(),1.0);
    EXPECT_FLOAT_EQ(bot.y(),1.0);

    ASSERT_EQ(messages[0].detection().balls_size(),1);
    EXPECT_FLOAT_EQ(messages[0].detection().balls(0).x(),-1.0);
    EXPECT_FLOAT_EQ(messages[0].detection().balls(0).y(),-1.0);

    ASSERT_EQ(messages[0].has_geometry(),false);
    //Let's check the receiver we build actually works properly and doesn't receive double or something crazy like that
    messages=receiver.readMessages();
    ASSERT_EQ(messages.size(),0);

    int camId=6;
    int goalDepth=1533;
    SSL_GeometryData data=makeGeometry(camId,goalDepth);
    publisher.send(data);
    messages=receiver.readMessages();
    ASSERT_EQ(messages.size(),1);
    ASSERT_EQ(messages[0].has_geometry(),true);
    auto geom=messages[0].geometry();
    ASSERT_EQ(geom.calib_size(),1);
    EXPECT_EQ(geom.calib(0).camera_id(),camId);

    ASSERT_EQ(geom.has_field(),true);
    EXPECT_EQ(geom.field().goal_depth(),goalDepth);
}
TEST(PublisherTest,settersGetters){
    QHostAddress address("127.0.0.1");
    unsigned int port = 10006;
    net::Publisher publisher(address, port);
    EXPECT_EQ(publisher.getPort(), port);
    EXPECT_EQ(publisher.getIP(), "127.0.0.1");

    int newPort=10005;
    publisher.setIP(QHostAddress("127.0.0.2"));
    publisher.setPort(newPort);
    EXPECT_NE(publisher.getPort(), port);
    EXPECT_NE(publisher.getIP(), "127.0.0.1");
    EXPECT_EQ(publisher.getPort(), newPort);
    EXPECT_EQ(publisher.getIP(), "127.0.0.2");

}