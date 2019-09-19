//
// Created by rolf on 19-09-19.
//

#include "net/Receiver.h"
#include "gtest/gtest.h"


TEST(ReceiverTest,receiveTest){
    //We are going to manually send commands to test the receiver
    QUdpSocket sendSocket;
    QHostAddress address("127.0.01");
    unsigned int port = 10006;
    net::Receiver receiver(address,port);
    // at this point the receiver should already be listening
    sendSocket.bind(address,port);
    mimir_robotcommand command;

    // values we use to test
    int id=7;
    bool teamIsYellow=false;
    float dribblespeed=0.5;
    float velx= 0.3;
    float vely= 0.4;
    float angle = 1.0;

    command.set_id(id);
    command.set_teamisyellow(false);
    command.set_dribblerspeed(0.5);

    globalVelocity vel;
    vel.set_angle(angle);
    vel.set_velx(velx);
    vel.set_vely(vely);
    command.mutable_globalvel()->CopyFrom(vel);

    bool chip=true;
    int geneva=4;
    float kickpower=0.8;
    Kicker kicker;
    kicker.set_chip(chip);
    kicker.set_genevaangle(geneva);
    kicker.set_kickchippower(kickpower);
    command.mutable_kicker()->CopyFrom(kicker);

    // now actually send the command twice!
    QByteArray datagram;
    datagram.resize(command.ByteSize());
    ASSERT_TRUE(command.SerializeToArray(datagram.data(),datagram.size()));
    ASSERT_EQ(sendSocket.writeDatagram(datagram,address,port),command.ByteSize());
    ASSERT_EQ(sendSocket.writeDatagram(datagram,address,port),command.ByteSize());

    auto messages=receiver.readMessages();
    ASSERT_EQ(messages.size(),2);
    auto msg=messages[0];
    EXPECT_EQ(msg.id(),id);
    EXPECT_EQ(msg.teamisyellow(),teamIsYellow);
    EXPECT_FLOAT_EQ(msg.dribblerspeed(),dribblespeed);
    EXPECT_TRUE(msg.has_globalvel());
    EXPECT_FALSE(msg.has_robotvel());
    EXPECT_FALSE(msg.has_wheels());

    EXPECT_FLOAT_EQ(msg.globalvel().vely(),vely);
    EXPECT_FLOAT_EQ(msg.globalvel().velx(),velx);
    EXPECT_EQ(msg.globalvel().angleControl_case(),msg.globalvel().kAngleFieldNumber);
    EXPECT_EQ(msg.globalvel().angle(),angle);

    EXPECT_TRUE(msg.has_kicker());
    EXPECT_EQ(msg.kicker().chip(),chip);
    EXPECT_EQ(msg.kicker().genevaangle(),geneva);
    EXPECT_FLOAT_EQ(msg.kicker().kickchippower(),kickpower);

}
