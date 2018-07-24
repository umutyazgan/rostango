#include "ros/ros.h"

float incomingTempo = 0;
//hark mesajı içindeki her değişkeni tanımla
void harkMsgCallback(const std_msgs::Float32MultiArray::ConstPtr& msg){
    incomingTempo = msg->data[0];
    //her değişkenden gerekli değerleri çek
}

int main(int argc, char **argv){
    ros::init(argc, argv, "message_stamper");
    ros::NodeHandle n;
    ros::Subscriber sub = n.subscribe("/HarkStdMsgs", 10000);
    ros::Publisher message_stamper_pub = n.advertise<std_msgs::Float32MultiArray>("stamped_message", 10000);
    ros::Rate sleep_rate(30);
    while (ros::ok()){
        sleep_rate.sleep();
	rostango::stamped_hark_message msg;
	msg.tempo
	// çekilen değerleri headerlı mesaja yaz
	// publish
    }
}
