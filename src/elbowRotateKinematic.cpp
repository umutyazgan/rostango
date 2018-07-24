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
#include "naoKinematics/NAOKinematics.h"
#include "naoKinematics/KMat.hpp"
#define PI 3.1415926535897932384626433832795028841971693993751058209749445923078164062862089986280348253421170679821480865132823066470938446095505822317253594081284811


#define NCOUNT 22

using namespace std;
using namespace KMath::KMat;
using namespace KDeviceLists;



static Frame frm; 
static float incomingTempo;
static int currentFrame;
static string recievedMove;
static int globalFrame = 0;

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
  currentFrame = msg->currentFrame;
  recievedMove = msg->moveName;

}


int main(int argc, char** argv){
  Movement m("mala_vida","1");
  Movement* m_p;
  m_p = &m;

  NAOKinematics nkin;
  NAOKinematics::kmatTable output1, output2, output3, output4, output5,T;
  
  std::vector<float> joints(NUMOFJOINTS);
  double pi = PI;

  //Left Hand
  joints[L_ARM+SHOULDER_PITCH]=0;
  joints[L_ARM+SHOULDER_ROLL]=0.42;
  joints[L_ARM+ELBOW_YAW]=1.20;
  joints[L_ARM+ELBOW_ROLL]=-1.29;
  joints[L_ARM+WRIST_YAW]=-0.84;
  //Right Hand
  joints[R_ARM+SHOULDER_PITCH]=0;
  joints[R_ARM+SHOULDER_ROLL]=0;
  joints[R_ARM+ELBOW_YAW]=0;
  joints[R_ARM+ELBOW_ROLL]=0;
  joints[R_ARM+WRIST_YAW]=0;
  //Left Leg
  joints[L_LEG+HIP_YAW_PITCH]=0;
  joints[L_LEG+HIP_ROLL]=0;
  joints[L_LEG+HIP_PITCH]=0;
  joints[L_LEG+KNEE_PITCH]=0;
  joints[L_LEG+ANKLE_PITCH]=0;
  joints[L_LEG+ANKLE_ROLL]=0;
  //Right Leg
  joints[R_LEG+HIP_YAW_PITCH]=0;
  joints[R_LEG+HIP_ROLL]=0;
  joints[R_LEG+HIP_PITCH]=0;
  joints[R_LEG+KNEE_PITCH]=0;
  joints[R_LEG+ANKLE_PITCH]=0;
  joints[R_LEG+ANKLE_ROLL]=0;
  //Head
  joints[HEAD+YAW]=0;
  joints[HEAD+PITCH]=0;

  nkin.setJoints(joints);
  output1= nkin.getForwardEffector((NAOKinematics::Effectors)CHAIN_L_ARM);

  joints[L_ARM+SHOULDER_PITCH]=0;
  joints[L_ARM+SHOULDER_ROLL]=0;
  joints[L_ARM+ELBOW_YAW]=0;
  joints[L_ARM+ELBOW_ROLL]=0;
  joints[L_ARM+WRIST_YAW]=0;

  ros::init(argc, argv, "state_publisher");
  
  ros::NodeHandle nb;
  ros::Subscriber sub = nb.subscribe("broadcaster", 10000, broadcast);
  sensor_msgs::JointState joint_state;

 
    ros::NodeHandle n;
    //
        
      ros::Publisher joint_pub = n.advertise<sensor_msgs::JointState>("joint_states", 1);
    

      

  
  ros::Rate sleep_rate(30);  

  

  int currentF = 0;

    joint_state.name.resize(25);
      joint_state.position.resize(25);



      

      string jointNames[] = {"HeadYaw", "HeadPitch", "LHipYawPitch", "LHipRoll", "LHipPitch", 
                         "LKneePitch", "LAnklePitch", "LAnkleRoll", "RHipRoll", "RHipPitch", 
                         "RKneePitch", "RAnklePitch", "RAnkleRoll", "LShoulderPitch", "LShoulderRoll", 
                         "LElbowYaw",  "LElbowRoll", "LWristYaw", "LHand",        "RShoulderPitch", 
                          "RShoulderRoll", "RElbowYaw", "RElbowRoll", "RWristYaw",  "RHand"};
      
//output1= nkin.getForwardEffector((NAOKinematics::Effectors)CHAIN_L_ARM);
  //cout << "output1: " << output1 << endl;
      //KMath::KMat::ATMartrix a;                    
 // x = 123.881 y = 133.774 z = -7.16733
/*
      output1(0,3) = 123.881;
      output1(1,3) = 133.774;
      output1(2,3) = -7.16733;
*/
    /*                      
      output1(0,3) = 115.078;
      output1(1,3) = 171.574;
      output1(2,3) = 205.234;
    
  */
         
 //   std::cout << "initial: " <<  "x = " << output1(0,3) << " y = " << output1(1,3) << " z = " << output1(2,3) <<  std::endl;
  
  

  

  while (ros::ok()){
    vector<vector<float> > result;
    result = nkin.inverseLeftHand(output1);
    std::cout << "x = " << output1(0,3) << " y = " << output1(1,3) << " z = " << output1(2,3) <<  std::endl;
    
    Frame f = m_p->getFrame(currentF);
    //output1= nkin.getForwardEffector((NAOKinematics::Effectors)CHAIN_L_ARM);
  

    currentF++; 
    currentF =  currentF % m_p->getFrameCount();


 //   output1(0,3) -= 1;
   // output1(1,3) += 0.1;
   // output1(2,3) += 1;

      joint_state.header.stamp = ros::Time::now();
    

        for (int i=0; i<25; i++){
          
        joint_state.name[i] = jointNames[i];
        joint_state.position[i] = 0;
        
        }
        /*
        joint_state.position[13] = joints[L_ARM+SHOULDER_PITCH];//result[0][0];
        joint_state.position[14] = joints[L_ARM+SHOULDER_ROLL];//result[0][1];
        joint_state.position[15] = joints[L_ARM+ELBOW_YAW];//result[0][2];
        joint_state.position[16] = joints[L_ARM+ELBOW_ROLL];//result[0][3];
        */
        joint_state.position[13] = result[0][0];
        joint_state.position[14] = result[0][1];
        joint_state.position[15] = result[0][2];
        joint_state.position[16] = result[0][3];
        
        for(int j=0; j<result[0].size(); j++){
          cout << "angle" << j << " = " << result[0][j] << " ";
        }
        cout << std::endl;
        /*
        joints[L_ARM+SHOULDER_PITCH]=joint_state.position[13];
        joints[L_ARM+SHOULDER_ROLL]=joint_state.position[14];
        joints[L_ARM+ELBOW_YAW]=joint_state.position[15];
        joints[L_ARM+ELBOW_ROLL]=joint_state.position[16];
        cout << "--Solution exists 1" << endl;
        
        for(int j=0; j<result[0].size(); j++){
          cout << "angle" << j << " = " << result[0][j] << " ";
        }
        cout << endl;
        */
        /*
        joint_state.position[13] = f.joints[7].rotY * 57.29;
        joint_state.position[14] = f.joints[7].rotX * 57.29;
        joint_state.position[15] = f.joints[8].rotX * 57.29;
        joint_state.position[16] = f.joints[8].rotZ * 57.29;
      */
     //   cout << "rotX: " << frm.joints[i].rotX << " roll: " << roll << endl;   
      //}



   
      
    

      joint_pub.publish(joint_state);
   
      globalFrame++;
        
    


        
    sleep_rate.sleep();
    ros::spinOnce();
  
  }
  return 0;
}


