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
#include <online_tempo/joint_message.h>
#include <online_tempo/frame_message.h>
#include <online_tempo/frame_message_burst.h>
#include <online_tempo/scale_and_cast.h>
#include "headers/Movement.h"


#define NCOUNT 22

using namespace std;




static Frame frm; 
static float incomingTempo;
static int currentFrame;
static string recievedMove;
static int globalFrame = 0;

void broadcast(const online_tempo::frame_message::ConstPtr& msg){
  
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
  currentFrame = msg->currentFrame;
  recievedMove = msg->moveName;
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
        
      ros::Publisher joint_pub = n.advertise<sensor_msgs::JointState>("joint_states", 1);
    
//<origin rpy="0 -0.157079 0" xyz="0.1812 0.015 0.00013"/>
    

//xyz="-0.02297 -0.00019 0.00119"
    //DEFAULT VALUES
    //----------------
      
   
   geometry_msgs::TransformStamped rootTrans;
   rootTrans.header.frame_id = "base_link";
   rootTrans.child_frame_id = "odom";
   
   /*
   geometry_msgs::TransformStamped rootTrans2;
   rootTrans2.header.frame_id = "hip_roll";
   rootTrans2.child_frame_id = "hip_pitch";
   
   geometry_msgs::TransformStamped rootTrans3;
   rootTrans3.header.frame_id = "hip_pitch";
   rootTrans3.child_frame_id = "hip_yaw";
   */

   tf::TransformBroadcaster rootJointBroadcaster;
  

  tf::TransformBroadcaster broadcaster;
  

    // message declarations
    geometry_msgs::TransformStamped transList[NCOUNT];

      //  sensor_msgs::JointState joint_state;
    transList[0].header.frame_id = "base_link";
    transList[0].child_frame_id = "hip";
    
    transList[1].header.frame_id = "hip";
    transList[1].child_frame_id = "lower_spine";
    
    transList[2].header.frame_id = "lower_spine";
    transList[2].child_frame_id = "middle_spine";
    
    transList[3].header.frame_id = "middle_spine";
    transList[3].child_frame_id = "chest";
    //
    transList[4].header.frame_id = "chest";
    transList[4].child_frame_id = "neck";
    
    transList[5].header.frame_id = "neck";
    transList[5].child_frame_id = "head";
    //
    transList[6].header.frame_id = "chest";
    transList[6].child_frame_id = "left_clavicle";
    
    transList[7].header.frame_id = "left_clavicle";
    transList[7].child_frame_id = "left_shoulder";
    
    transList[8].header.frame_id = "left_shoulder";
    transList[8].child_frame_id = "left_forearm";
    
    transList[9].header.frame_id = "left_forearm";
    transList[9].child_frame_id = "left_hand";
    
    //
    transList[10].header.frame_id = "chest";
    transList[10].child_frame_id = "right_clavicle";
    
    transList[11].header.frame_id = "right_clavicle";
    transList[11].child_frame_id = "right_shoulder";
    
    transList[12].header.frame_id = "right_shoulder";
    transList[12].child_frame_id = "right_forearm";
    
    transList[13].header.frame_id = "right_forearm";
    transList[13].child_frame_id = "right_hand";
        
    //
    transList[14].header.frame_id = "hip";
    transList[14].child_frame_id = "right_tigh";
    
    transList[15].header.frame_id = "right_tigh";
    transList[15].child_frame_id = "right_leg";
    
    transList[16].header.frame_id = "right_leg";
    transList[16].child_frame_id = "right_foot";
    
    transList[17].header.frame_id = "right_foot";
    transList[17].child_frame_id = "right_toe";
    
    transList[18].header.frame_id = "hip";
    transList[18].child_frame_id = "left_tigh";
    
    transList[19].header.frame_id = "left_tigh";
    transList[19].child_frame_id = "left_leg";
    
    transList[20].header.frame_id = "left_leg";
    transList[20].child_frame_id = "left_foot";
    
    transList[21].header.frame_id = "left_foot";
    transList[21].child_frame_id = "left_toe";
  ros::Rate sleep_rate(30);  
  bool frameNotSent = true;
  Joint jnt;      

  geometry_msgs::TransformStamped meshedTransList[NCOUNT*3];
  /*
  meshedTransList[0].header.frame_id = "base_link";
  meshedTransList[0].child_frame_id = "hip_roll";

  meshedTransList[1].header.frame_id = "hip_roll";
  meshedTransList[1].child_frame_id = "hip_pitch";

  meshedTransList[2].header.frame_id = "hip_pitch";
  meshedTransList[2].child_frame_id = "hip_yaw";
    */
  /*
  for (int i=2; i<10; i++){
    meshedTransList[3*i].header.frame_id = transList[i].header.frame_id + "_roll";
    meshedTransList[3*i].child_frame_id = transList[i-1].child_frame_id + "_pitch";
    cout << "i: " << i << " " << meshedTransList[3*i].header.frame_id << " -> " << meshedTransList[3*i].child_frame_id << endl;
    
    meshedTransList[3*i+1].header.frame_id = transList[i].header.frame_id + "_pitch";
    meshedTransList[3*i+1].child_frame_id = transList[i-1].child_frame_id + "_yaw";
    cout << "i: " << i <<  " " << meshedTransList[3*i+1].header.frame_id << " -> " << meshedTransList[3*i+1].child_frame_id << endl;
    
    meshedTransList[3*i+2].header.frame_id = transList[i].header.frame_id + "_yaw";
    meshedTransList[3*i+2].child_frame_id = transList[i].child_frame_id + "_roll";
    cout << "i: " << i << " " << meshedTransList[3*i+2].header.frame_id << " -> " << meshedTransList[3*i+2].child_frame_id << endl;
      
  }
  */
  // i = 9;
  /*
  meshedTransList[30].header.frame_id = "left_hand_roll";
  meshedTransList[30].child_frame_id = "left_hand_pitch";
  cout << "i: " << 10 << " " << meshedTransList[30].header.frame_id << " -> " << meshedTransList[30].child_frame_id << endl;
  meshedTransList[31].header.frame_id = "left_hand_pitch";
  meshedTransList[31].child_frame_id = "left_hand_yaw";
  cout << "i: " << 10 << " " << meshedTransList[31].header.frame_id << " -> " << meshedTransList[31].child_frame_id << endl;
  meshedTransList[32].header.frame_id = "left_hand_pitch";
  meshedTransList[32].child_frame_id = "end_effector";
  cout << "end effector" << endl; 
  */

/*
  meshedTransList[63].header.frame_id = "left_foot_roll";
  meshedTransList[63].child_frame_id = "left_toe_pitch";

  meshedTransList[64].header.frame_id = "left_toe_roll";
  meshedTransList[64].child_frame_id = "left_toe_pitch";

  meshedTransList[65].header.frame_id = "left_toe_pitch";
  meshedTransList[65].child_frame_id = "left_toe_yaw";
  */

   Movement m("mala_vida", "1");
  int frameToSend = 0;
  Movement* m_p;
  m_p = &m;

  
  while (ros::ok()){

    frm = m_p->getFrame(frameToSend);
      frameToSend++;
      frameToSend = frameToSend % m_p->getFrameCount();

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


      tf::Quaternion quat;
      double roll, pitch, yaw;

      
        
      rootTrans.header.stamp = ros::Time::now();
      rootTrans.transform.translation.x = frm.joints[0].transX;
      rootTrans.transform.translation.y = frm.joints[0].transY;
      rootTrans.transform.translation.z = frm.joints[0].transZ;
      
      rootTrans.transform.rotation = tf::createQuaternionMsgFromRollPitchYaw(frm.joints[0].rotX, frm.joints[0].rotY, frm.joints[0].rotZ);
      
    //rootTrans.transform.rotation = tf::createQuaternionMsgFromRollPitchYaw(0,0,0);
      rootJointBroadcaster.sendTransform(rootTrans);

    
      transList[0].header.stamp = ros::Time::now();
      transList[0].transform.translation.x = frm.joints[0].transX;
      transList[0].transform.translation.y = frm.joints[0].transY;// + 2;
      transList[0].transform.translation.z = frm.joints[0].transZ;
      transList[0].transform.rotation = tf::createQuaternionMsgFromRollPitchYaw(frm.joints[0].rotX,frm.joints[0].rotY,
                                            frm.joints[0].rotZ);
      //broadcaster.sendTransform(transList[0]);
      

      for (int i = 0; i < 22; i++){
        /*
        tf::quaternionMsgToTF(tf::createQuaternionMsgFromRollPitchYaw(frm.joints[i].rotX,frm.joints[i].rotY,
                                            frm.joints[i].rotZ),quat);
        
        tf::Matrix3x3(quat).getRPY(roll, pitch, yaw);
       */ 
        joint_state.name[3*i] = jointNames[i] + "_roll";
        joint_state.position[3*i] = frm.joints[i].rotX * 57.29;
        joint_state.name[3*i+1] = jointNames[i] + "_pitch";
        joint_state.position[3*i+1] = frm.joints[i].rotY * 57.29;
        joint_state.name[3*i+2] = jointNames[i] + "_yaw";
        joint_state.position[3*i+2] = frm.joints[i].rotZ * 57.29;
     //   cout << "rotX: " << frm.joints[i].rotX << " roll: " << roll << endl;   
      }



    for(int i=1; i<NCOUNT; i++){
      // update transform
      //cout << "main function" << endl;
      transList[i].header.stamp = ros::Time::now();
      transList[i].transform.translation.x = frm.joints[i].transX;
      transList[i].transform.translation.y = frm.joints[i].transY;
      transList[i].transform.translation.z = frm.joints[i].transZ;
      transList[i].transform.rotation = tf::createQuaternionMsgFromRollPitchYaw(frm.joints[i].rotX,frm.joints[i].rotY,
                                            frm.joints[i].rotZ);
      
     
      
             
    }



      joint_pub.publish(joint_state);
    
  //    cout << globalFrame << " " << joint_state.position[0] << " " << joint_state.position[1] << " " << joint_state.position[2] << endl; 
//    cout << "sent transform" << endl;
      globalFrame++;
      //broadcaster.sendTransform(trans);
        
    


        
    sleep_rate.sleep();
    ros::spinOnce();
  
  }
  return 0;
}


