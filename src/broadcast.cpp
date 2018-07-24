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
#include <online_tempo/joint_message.h>
#include <online_tempo/frame_message.h>
#include <online_tempo/frame_message_burst.h>
#include <online_tempo/scale_and_cast.h>
#include "headers/Movement.h"
#include <online_tempo/imitated_tempo_message.h>


#define NCOUNT 22

using namespace std;




static Frame frm; 
static float incomingTempo;
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

//void get_imit_tempo(const online_tempo::imitated_tempo_message::ConstPtr& msg){
//    incomingTempo = msg->tempo;
//}


int main(int argc, char** argv){
  ros::init(argc, argv, "broadcast_node");
  
  //ros::Rate mainRate(10);
  ros::NodeHandle nb;
  ros::Subscriber sub = nb.subscribe("broadcaster", 10000, broadcast);
//  ros::Subscriber sub2 = nb.subscribe("imitated_tempo", 10000, get_imit_tempo);
  //ros::Rate polling_rate(300);
  
  /*
  while (ros::ok()){
    ros::spinOnce();
    polling_rate.sleep();
    
    //mainRate.sleep();
  }
  */
  
  ros::init(argc, argv, "state_publisher");
  
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

/*
    transList[0].header.frame_id = "base_link";
    transList[0].child_frame_id = "torso";
    
    transList[1].header.frame_id = "torso";
    transList[1].child_frame_id = "Neck";
    
    transList[2].header.frame_id = "lower_spine";
    transList[2].child_frame_id = "middle_spine";
    
    transList[3].header.frame_id = "middle_spine";
    transList[3].child_frame_id = "chest";
    //
    transList[4].header.frame_id = "chest";
    transList[4].child_frame_id = "neck";
    
    transList[5].header.frame_id = "Neck";
    transList[5].child_frame_id = "Head";
    //
    transList[6].header.frame_id = "torso";
    transList[6].child_frame_id = "LShoulder";
    
    transList[7].header.frame_id = "LShoulder";
    transList[7].child_frame_id = "LElbow";
    
    transList[8].header.frame_id = "LElbow";
    transList[8].child_frame_id = "l_wrist";
    
    transList[9].header.frame_id = "l_wrist";
    transList[9].child_frame_id = "l_gripper";
    
    //
    transList[10].header.frame_id = "torso";
    transList[10].child_frame_id = "RShoulder";
    
    transList[11].header.frame_id = "RShoulder";
    transList[11].child_frame_id = "RElbow";
    
    transList[12].header.frame_id = "RElbow";
    transList[12].child_frame_id = "r_wrist";
    
    transList[13].header.frame_id = "r_wrist";
    transList[13].child_frame_id = "r_gripper";
        
    //
    transList[14].header.frame_id = "torso";
    transList[14].child_frame_id = "RPelvis";
    
    transList[15].header.frame_id = "RPelvis";
    transList[15].child_frame_id = "RTibia";
    
    transList[16].header.frame_id = "RTibia";
    transList[16].child_frame_id = "r_ankle";
    
    transList[17].header.frame_id = "r_ankle";
    transList[17].child_frame_id = "r_sole";
    
    transList[18].header.frame_id = "torso";
    transList[18].child_frame_id = "LPelvis";
    
    transList[19].header.frame_id = "LPelvis";
    transList[19].child_frame_id = "LTibia";
    
    transList[20].header.frame_id = "LTibia";
    transList[20].child_frame_id = "l_ankle";
    
    transList[21].header.frame_id = "l_ankle";
    transList[21].child_frame_id = "l_sole";
*/
  ros::Rate sleep_rate(30);  
  bool frameNotSent = true;
  Joint jnt;      
  
  while (ros::ok()){
    sleep_rate.sleep();

    for(int i=0; i<NCOUNT; i++){
      // update transform
      //cout << "main function" << endl;
      transList[i].header.stamp = ros::Time::now();
      transList[i].transform.translation.x = frm.joints[i].transX;
      transList[i].transform.translation.y = frm.joints[i].transY;
      transList[i].transform.translation.z = frm.joints[i].transZ;
      transList[i].transform.rotation = tf::createQuaternionMsgFromRollPitchYaw(frm.joints[i].rotX,frm.joints[i].rotY,
                                            frm.joints[i].rotZ);
      
      
      //cout << "sent transform" << endl;
      if (incomingTempo > 0){
        broadcaster.sendTransform(transList[i]);
        cout << "detected tempo: " << incomingTempo << endl;
      }
//      else{
//	  cout << incomingTempo << endl;
//	}      
    }
    ros::spinOnce();   
  
  }
  return 0;
}


