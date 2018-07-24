//sending frame approach
#include <ros/ros.h>
#include <tf/transform_broadcaster.h>
#include <tf/transform_listener.h>
#include <sensor_msgs/JointState.h>
#include <vector>
#include <iostream>
#include <iterator>
#include <fstream>
#include <string>
#include <cmath>
#include "headers/spline.h"
#include <algorithm>
#include "std_msgs/String.h"
#include "std_msgs/Float32.h"
#include <sstream>
#include <time.h>
#include <rostango/joint_message.h>
#include <rostango/frame_message.h>
#include <rostango/frame_message_burst.h>
#include <rostango/scale_and_cast.h>
#include "headers/Movement.h"


#define NCOUNT 22

using namespace std;




static Frame frm; 
static float incomingTempo;
void broadcast(const rostango::frame_message::ConstPtr& msg){
  
  //cout << "creating Primative: " << msg->primNumber << endl;
  //Movement* m_ptr = new Movement(msg->moveName, msg->primNumber);
  for (int jNo = 0; jNo < NCOUNT; jNo++){
    frm.joints[jNo].transX = msg->joints[jNo].transX;
    frm.joints[jNo].transY = msg->joints[jNo].transY;
    frm.joints[jNo].transZ = msg->joints[jNo].transZ;
    frm.joints[jNo].rotX = msg->joints[jNo].rotX;
    frm.joints[jNo].rotY = msg->joints[jNo].rotY;
    frm.joints[jNo].rotZ = msg->joints[jNo].rotZ;
     

  }
  incomingTempo = msg->tempo;
  //cout << "inside broadcaster" << endl;
  //cout << "did you say " << msg->data << endl;

  
        //timeLine++;
        //frameNotSent = false;
        //cout << "slept for 1/30 sec" << endl;
        //currentFrameNo++;
        //cout << "Frame: " << currentFrame << " is broadcasted" << endl;

   // end of while
 // cout << "frame" << currentFrameNo << "broadcasted " << endl; 
 // cout << "sent transform" << endl;
  
  
}


int main(int argc, char** argv){
  
  //ros::Subscriber sub = nb.subscribe("broadcaster", 10000, broadcast);
  
  //ros::Rate polling_rate(300);
  
  /*
  while (ros::ok()){
    ros::spinOnce();
    polling_rate.sleep();
    
    //mainRate.sleep();
  }
  */
  
  ros::init(argc, argv, "state_publisher");
  
  ros::NodeHandle nb;
  ros::Subscriber sub = nb.subscribe("broadcaster", 10000, broadcast);
  sensor_msgs::JointState joint_state;

 
    ros::NodeHandle n;
    //
      ros::Rate sleep_rate(30);  
      ros::Publisher joint_pub = n.advertise<sensor_msgs::JointState>("joint_states", 1);
    
//<origin rpy="0 -0.157079 0" xyz="0.1812 0.015 0.00013"/>
    

//xyz="-0.02297 -0.00019 0.00119"
    //DEFAULT VALUES
    //----------------
      
      
        
  while (ros::ok()){

      // update transform
      //cout << "main function" << endl;
      ROS_INFO( "detected tempo: %f", incomingTempo);
     
      joint_state.header.stamp = ros::Time::now();
      joint_state.name.resize(18);
      joint_state.position.resize(18);


      joint_state.name[0] ="HeadYaw";
      joint_state.position[0] = frm.joints[6].rotZ * 57.2957795;
      
      joint_state.name[1] ="HeadPitch";
      joint_state.position[1] = frm.joints[6].rotY * 57.2957795; 
      
      joint_state.name[2] ="HipRoll";
      joint_state.position[2] = frm.joints[14].rotX * 57.2957795;
      
      joint_state.name[3] ="HipPitch";
      joint_state.position[3] = frm.joints[14].rotY * 57.2957795;

      joint_state.name[4] ="KneePitch";
      joint_state.position[4] = 0;

      joint_state.name[5] ="LShoulderPitch";
      joint_state.position[5] = frm.joints[6].rotY * 57.2957795;
      
      joint_state.name[6] ="LShoulderRoll";
      joint_state.position[6] = frm.joints[6].rotX * 57.2957795;

      joint_state.name[7] ="LElbowYaw";
      joint_state.position[7] = frm.joints[7].rotX * 57.2957795;

      joint_state.name[9] ="LElbowRoll";
      joint_state.position[9] = frm.joints[7].rotZ * 57.2957795;
   
      joint_state.name[10] ="LWristYaw";
      joint_state.position[10] = frm.joints[8].rotZ * 57.2957795;
      
      joint_state.name[11] ="LHand";
      joint_state.position[11] = frm.joints[9].rotX * 57.2957795;
      


      joint_state.name[12] ="RShoulderPitch";
      joint_state.position[12] = frm.joints[10].rotY * 57.2957795;
      
      joint_state.name[13] ="RShoulderRoll";
      joint_state.position[13] = frm.joints[10].rotX * 57.2957795;

      joint_state.name[14] ="RElbowYaw";
      joint_state.position[14] = frm.joints[11].rotX * 57.2957795;
      
      joint_state.name[15] ="RElbowRoll";
      joint_state.position[15] = frm.joints[11].rotZ * 57.2957795;
      
      joint_state.name[16] ="RWristYaw";
      joint_state.position[16] = frm.joints[12].rotZ * 57.2957795;
      
      joint_state.name[17] ="RHand";
      joint_state.position[17] = frm.joints[13].rotX * 57.2957795;

      //ame: ['', '', '', '', '', '', '', '', '', '', '', '', '', '', '', '', '', 'RFinger41', 'LFinger42', 'RFinger11', 'RFinger12', 'LFinger33', 'RFinger31', 'LFinger21', 'RFinger32', 'LFinger13', 'LFinger32', 'LFinger11', 'RFinger22', 'RFinger13', 'LFinger22', 'RFinger21', 'LFinger41', 'LFinger12', 'RFinger23', 'LFinger23', 'LFinger43', 'RFinger43', 'RFinger42', 'LFinger31', 'RFinger33', 'LThumb1', 'RThumb2', 'RThumb1', 'LThumb2', 'WheelB', 'WheelFL', 'WheelFR'
     

     
        joint_pub.publish(joint_state);
  //    cout << "sent transform" << endl;
        //broadcaster.sendTransform(trans);
        
        
        
    sleep_rate.sleep();
    ros::spinOnce();
  
  }
  return 0;
}


