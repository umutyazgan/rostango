//sending frame approach
#include <ros/ros.h>
#include <tf/transform_broadcaster.h>
#include <tf/transform_listener.h>
#include <sensor_msgs/JointState.h>
#include <nav_msgs/Odometry.h>

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
      
   
   geometry_msgs::TransformStamped rootTrans;
   rootTrans.header.frame_id = "base_link";
   rootTrans.child_frame_id = "hip_roll";
   
   geometry_msgs::TransformStamped rootTrans2;
   rootTrans2.header.frame_id = "hip_roll";
   rootTrans2.child_frame_id = "hip_pitch";
   
   geometry_msgs::TransformStamped rootTrans3;
   rootTrans3.header.frame_id = "hip_pitch";
   rootTrans3.child_frame_id = "hip_yaw";
   

   tf::TransformBroadcaster rootJointBroadcaster;
  

  ros::Publisher odom_pub = n.advertise<nav_msgs::Odometry>("odom", 50);  
  while (ros::ok()){

      // update transform
      //cout << "main function" << endl;
      //ROS_INFO( "detected tempo: %f", incomingTempo);
     
      joint_state.header.stamp = ros::Time::now();
      joint_state.name.resize(66);
      joint_state.position.resize(66);



      

      string jointNames[] = {"hip", "lower_spine", "middle_spine", "chest", "neck", "head", 
      "right_clavicle", "right_shoulder", "right_elbow", "right_hand", 
      "left_clavicle", "left_shoulder", "left_elbow", "left_hand", 
      "right_thigh", "right_knee", "right_foot", "right_toe", 
      "left_thigh", "left_knee", "left_foot", "left_toe"};

      for (int i = 1; i < 22; i++){
        joint_state.name[3*i] = jointNames[i] + "_roll";
        joint_state.position[3*i] = frm.joints[i].rotX * 57.29;
        joint_state.name[3*i+1] = jointNames[i] + "_pitch";
        joint_state.position[3*i+1] = frm.joints[i].rotY * 57.29;
        joint_state.name[3*i+2] = jointNames[i] + "_yaw";
        joint_state.position[3*i+2] = frm.joints[i].rotZ * 57.29;
        
      }
            
      rootTrans.header.stamp = ros::Time::now();
      rootTrans.transform.translation.x = frm.joints[0].transX;
      rootTrans.transform.translation.y = frm.joints[0].transY;
      rootTrans.transform.translation.z = frm.joints[0].transZ;
      rootTrans.transform.rotation = tf::createQuaternionMsgFromRollPitchYaw(frm.joints[0].rotX,frm.joints[0].rotY,
                                            frm.joints[0].rotZ);
 
      rootJointBroadcaster.sendTransform(rootTrans);

      rootTrans2.header.stamp = ros::Time::now();
      rootTrans2.transform.translation.x = 0.0;
      rootTrans2.transform.translation.y = 0.0;
      rootTrans2.transform.translation.z = 0.0;
      rootTrans2.transform.rotation = tf::createQuaternionMsgFromRollPitchYaw(0.0, 0.0, 0.0);
 
      rootJointBroadcaster.sendTransform(rootTrans2);

      rootTrans3.header.stamp = ros::Time::now();
      rootTrans3.transform.translation.x = 0.0;
      rootTrans3.transform.translation.y = 0.0;
      rootTrans3.transform.translation.z = 0.0;
      rootTrans3.transform.rotation = tf::createQuaternionMsgFromRollPitchYaw(0.0, 0.0, 0.0);
 
      rootJointBroadcaster.sendTransform(rootTrans3);

      
/*
      rootTrans.header.frame_id = "hip_roll";
      rootTrans.child_frame_id = "hip_pitch";
   
      rootJointBroadcaster.sendTransform(rootTrans);
            
      rootTrans.header.frame_id = "hip_pitch";
      rootTrans.child_frame_id = "hip_yaw";
   
      rootJointBroadcaster.sendTransform(rootTrans);
*/      

/*
      nav_msgs::Odometry odom;
      odom.header.stamp = ros::Time::now();
      odom.header.frame_id = "hip_roll";

      odom.pose.pose.position.x = frm.joints[0].transX;
      odom.pose.pose.position.y = frm.joints[0].transY;
      odom.pose.pose.position.z = frm.joints[0].transZ;
      odom.pose.pose.orientation = tf::createQuaternionMsgFromRollPitchYaw(frm.joints[0].rotX,frm.joints[0].rotY,
                                            frm.joints[0].rotZ);
 
      odom_pub.publish(odom);
 */
   
/*
      hipTrans.header.stamp = ros::Time::now();
      hipTrans.transform.translation.x = frm.joints[0].transX;
      hipTrans.transform.translation.y = frm.joints[0].transY;
      hipTrans.transform.translation.z = frm.joints[0].transZ;
      hipTrans.transform.rotation = tf::createQuaternionMsgFromRollPitchYaw(frm.joints[0].rotX,frm.joints[0].rotY,
                                            frm.joints[0].rotZ);
      hipTransformBroadcast.sendTransform(hipTrans);
*/  
      joint_pub.publish(joint_state);
//    cout << "sent transform" << endl;
      //broadcaster.sendTransform(trans);
        
        
        
    sleep_rate.sleep();
    ros::spinOnce();
  
  }
  return 0;
}


