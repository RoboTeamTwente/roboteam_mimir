//
// Created by rolf on 18-09-19.
//

#include "gtest/gtest.h"
#include "proto/messages_robocup_ssl_wrapper.pb.h"
#include "net/Publisher.h"
#include <QHostAddress>

// function that makes a frame with a robot of specified colour and ID at position 1.0,1.0 and a ball at (-1.0,-1.0)
SSL_DetectionFrame makeFrame(unsigned int robotID,bool yellow){
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
}

TEST(PublisherTest, basic) {
    QHostAddress localIP("127.0.0.1");
    int port=10006;
    // set up a publisher and a subscriber at desired IP
    net::Publisher publisher(localIP,10006);
    SSL_WrapperPacket packet;
    SSL_DetectionFrame frame=makeFrame(1,true);
    publisher.send(frame);
}