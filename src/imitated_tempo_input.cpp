#include "ros/ros.h"
#include <rostango/imitated_tempo_message.h>
#include <iostream>

int main(int argc, char **argv){
    ros::init(argc, argv, "imitated_tempo_input");
    ros::NodeHandle n;
    ros::Publisher tempo_input_pub = n.advertise<rostango::imitated_tempo_message>("tempo_input", 10000);
    ros::Rate sleep_rate(30);
    
    float input = 59;
    while(ros::ok()){
	sleep_rate.sleep();
	rostango::imitated_tempo_message msg;
	std::cout << "tempo: ";
	std::cin >> input;
	if(input == -1)
	    break;
	msg.tempo = input;
	tempo_input_pub.publish(msg);

	ros::spinOnce();
    }

    return 0;
}
