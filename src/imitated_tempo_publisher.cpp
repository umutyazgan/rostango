#include "ros/ros.h"
#include <rostango/imitated_tempo_message.h>

float input = 60;
 
void inputCallback(const rostango::imitated_tempo_message::ConstPtr& msg){
    input = msg->tempo;
}

int main(int argc, char **argv){
    ros::init(argc, argv, "imitated_tempo_publisher");
    ros::NodeHandle n;
    ros::Subscriber sub = n.subscribe("tempo_input", 10000, inputCallback);
    ros::Publisher imitated_tempo_pub = n.advertise<rostango::imitated_tempo_message>("imitated_tempo", 10000);
    ros::Rate sleep_rate(30);
    
    while (ros::ok()){
	sleep_rate.sleep();
	rostango::imitated_tempo_message msg;
        msg.tempo = input;
        imitated_tempo_pub.publish(msg);

        ros::spinOnce();
    }
    ros::spin();

    return 0;
}
