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

}


int main(int argc, char** argv){
  

  
  ros::init(argc, argv, "state_publisher");
  
  ros::NodeHandle nb;
  ros::Subscriber sub = nb.subscribe("broadcaster", 10000, broadcast);
  sensor_msgs::JointState joint_state;

 
    ros::NodeHandle n;
    //
        
      ros::Publisher joint_pub = n.advertise<sensor_msgs::JointState>("joint_states", 1);
    

      
   
   geometry_msgs::TransformStamped rootTrans;
   rootTrans.header.frame_id = "base_link";
   rootTrans.child_frame_id = "odom";
   


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
  

  Movement m("mala_vida","30");
  Movement* m_p;
  m_p = &m;
  int currentF = 0;
  while (ros::ok()){
    Frame f = m_p->getFrame(currentF);
    
    
    currentF++; 
    currentF =  currentF % m_p->getFrameCount();
      // update transform
      //cout << "main function" << endl;
      //ROS_INFO( "detected tempo: %f", incomingTempo);
     
      joint_state.header.stamp = ros::Time::now();
      joint_state.name.resize(25);
      joint_state.position.resize(25);



      

      string jointNames[] = {"HeadYaw", "HeadPitch", "LHipYawPitch", "LHipRoll", "LHipPitch", 
                         "LKneePitch", "LAnklePitch", "LAnkleRoll", "RHipRoll", "RHipPitch", 
                         "RKneePitch", "RAnklePitch", "RAnkleRoll", "LShoulderPitch", "LShoulderRoll", 
                         "LElbowYaw",  "LElbowRoll", "LWristYaw", "LHand",        "RShoulderPitch", 
                          "RShoulderRoll", "RElbowYaw", "RElbowRoll", "RWristYaw",  "RHand"};


      tf::Quaternion quat;
      double roll, pitch, yaw;

      
        
      rootTrans.header.stamp = ros::Time::now();
      rootTrans.transform.translation.x = frm.joints[0].transX;
      rootTrans.transform.translation.y = frm.joints[0].transY;
      rootTrans.transform.translation.z = frm.joints[0].transZ;
      
      rootTrans.transform.rotation = tf::createQuaternionMsgFromRollPitchYaw(frm.joints[0].rotX, frm.joints[0].rotY, frm.joints[0].rotZ);
      
	 
     
      

     // for (int i = 0; i < 22; i++){
        /*
        tf::quaternionMsgToTF(tf::createQuaternionMsgFromRollPitchYaw(frm.joints[i].rotX,frm.joints[i].rotY,
                                            frm.joints[i].rotZ),quat);
        
        tf::Matrix3x3(quat).getRPY(roll, pitch, yaw);
       */ 

        for (int i=0; i<25; i++){
          
        joint_state.name[i] = jointNames[i];
        joint_state.position[i] = 0;
        
        }

        joint_state.position[13] = f.joints[11].rotY * 57.29;  //Lshoulder
        joint_state.position[14] = f.joints[11].rotX * 57.29 + 90;
        joint_state.position[15] = f.joints[12].rotX * 57.29;  //Lelbow
        joint_state.position[16] = f.joints[12].rotZ * 57.29;
        joint_state.position[17] = f.joints[13].rotX * 57.29;  //LwristYaw (x)

        joint_state.position[19] = f.joints[7].rotY * 57.29;  //Rshoulder
        joint_state.position[20] = f.joints[7].rotX * 57.29 - 90;
        joint_state.position[21] = f.joints[8].rotX * 57.29;  //Relbow
        joint_state.position[22] = f.joints[8].rotZ * 57.29;
        joint_state.position[23] = f.joints[9].rotX * 57.29;  //RwristYaw (x)



        joint_state.position[8] = f.joints[14].rotX * 57.29;  //RHipRoll
        joint_state.position[9] = f.joints[14].rotY * 57.29;  //RHipPitch
        joint_state.position[10] = f.joints[15].rotY * 57.29;  //RKneePitch
        joint_state.position[11] = f.joints[16].rotY * 57.29;  //RAnklePitch
        joint_state.position[12] = f.joints[16].rotX * 57.29;  //RAnkleRoll

       // joint_state.position[2] = f.joints[18].rotY * 57.29 * 0.707 - f.joints[18].rotZ * 57.29 * 0.707;  //LHipYawPitch
        
        joint_state.position[3] = f.joints[18].rotX * 57.29;  //LHipRoll
        joint_state.position[4] = f.joints[18].rotY * 57.29;  //LHipPitch
        joint_state.position[5] = f.joints[19].rotY * 57.29;  //LKneePitch
        joint_state.position[6] = f.joints[20].rotY * 57.29;  //LAnklePitch
        joint_state.position[7] = f.joints[20].rotX * 57.29;  //LAnkleRoll



     //   cout << "rotX: " << frm.joints[i].rotX << " roll: " << roll << endl;   
      //}



    cout << "currentF: " << currentF << endl;
      
    

      joint_pub.publish(joint_state);
   
      globalFrame++;
        
    


        
    sleep_rate.sleep();
    ros::spinOnce();
  
  }
  return 0;
}


