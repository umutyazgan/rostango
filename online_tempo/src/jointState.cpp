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

#define NCOUNT 22

using namespace std;


struct quat{
  float xx;
  float yy;
  float zz;
  float ww;
};

struct BodyNode{
  std::string nodeName;
  float transX;
  float transY;
  float transZ;
  float rotX;
  float rotY;
  float rotZ;
  tf::Quaternion qtr;   
};

struct Frame{
  BodyNode nodes[NCOUNT];       //body has 15 nodes
};



class movement{
  public:
    movement();   
    movement(std::string);
    Frame getFrame(int);    
    void broadcast();
    void broadcast(int, int);       //requires starting and ending frame
    int getFrameCount();
    void insertFrame(Frame);
    void updateFrameCount(int);
    quat testData(int, int);    //frame number, joint number  
  private:                    //fix later needs to be at least protected
    std::vector<Frame> frames;
    int frameCount;
};

movement::movement(){

}

movement::movement(std::string moveName){
  frameCount = 0; 
  float value;
  std::ifstream moveFile;
  moveName = "/home/anil/catkin_ws/src/online_tempo/motion_library/" + moveName + ".txt";
  const char* fileName = moveName.c_str();
  moveFile.open(fileName);
  
  std::vector<std::string> nodeNames;
  nodeNames.push_back("hip");
  nodeNames.push_back("right_tigh");
  nodeNames.push_back("right_leg");
  nodeNames.push_back("right_foot");
  nodeNames.push_back("left_tigh");
  nodeNames.push_back("left_leg");
  nodeNames.push_back("left_foot");
   
  Frame input;  
  float data;
  if(moveFile.is_open()){
    while(moveFile.good()){   
      for(int i = 0; i < NCOUNT; i++){  // 7 nodes for now
        //ros' y coordinate correspond to motionbuilder z coordinate      
        moveFile >> data; input.nodes[i].transY = data / 100; //meter to cm
        moveFile >> data; input.nodes[i].transZ = data / 100;
        moveFile >> data; input.nodes[i].transX = data / 100;
        
        moveFile >> data;  input.nodes[i].rotY = data / 57.2957;  //radian coefficients   
        moveFile >> data;  input.nodes[i].rotZ = data / 57.2957;
        moveFile >> data;  input.nodes[i].rotX = data / 57.2957;

      //  input.nodes[i].qtr = tf::createQuaternionMsgFromRollPitchYaw(input.nodes[i].rotX, input.nodes[i].rotY, input.nodes[i].rotZ);

//        moveFile >> input.nodes[i].transX;
//        moveFile >> input.nodes[i].transY;
//        moveFile >> input.nodes[i].transZ;
//        moveFile >> input.nodes[i].rotX;      
  //      moveFile >> input.nodes[i].rotY;
    //    moveFile >> input.nodes[i].rotZ;  
    //    input.nodes[i].nodeName = nodeNames[i];     
      } // a frame is ready
      frameCount++;
      frames.push_back(input);

    }
  }
  else
    cout << "unable to open move file" << endl;
  moveFile.close(); 
  //cout << "move has " << frameCount << " frames" << endl;
}


Frame movement::getFrame(int frameNumber){
  return frames[frameNumber];
}


void movement::broadcast(){
  int argc = 0;         //no idea what to do
  char** argv;  
  ros::init(argc, argv, "state_publisher");
  ros::NodeHandle n;  
  ros::Publisher joint_pub = n.advertise<sensor_msgs::JointState>("joint_states",1);
  tf::TransformBroadcaster broadcaster;
  ros::Rate loop_rate(30);
  

    // message declarations
    geometry_msgs::TransformStamped transList[NCOUNT];
    
    geometry_msgs::TransformStamped odom_trans;
    sensor_msgs::JointState joint_state;
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
    
    
    //transList[18].header.frame_id = "right_foot";
    //transList[18].child_frame_id = "right_foot_end";
    
    
    transList[18].header.frame_id = "hip";
    transList[18].child_frame_id = "left_tigh";
    
    transList[19].header.frame_id = "left_tigh";
    transList[19].child_frame_id = "left_leg";
    
    transList[20].header.frame_id = "left_leg";
    transList[20].child_frame_id = "left_foot";
    
    transList[21].header.frame_id = "left_foot";
    transList[21].child_frame_id = "left_toe";
    
    
  joint_state.name.resize(3);
  joint_state.position.resize(3);
  joint_state.name[0] ="hip_to_right_tigh";
  //joint_state.name[1] ="hip_to_right_tigh_y";
  //joint_state.name[2] ="hip_to_right_tigh_z";
      
      
  int r = 0;    
  
  int currentFrame = 0;
    while (ros::ok()) {
        //update joint_state
        r++;
    
    for(int i=0; i<NCOUNT; i++){
      joint_state.header.stamp = ros::Time::now();
      
      joint_state.position[0] = r;
    //  joint_state.position[1] = 100;
    //  joint_state.position[2] = 100;

        
        
        // update transform
        // (moving in a circle with radius=2)
      transList[i].header.stamp = ros::Time::now();
      transList[i].transform.translation.x = frames[currentFrame].nodes[i].transX;
      transList[i].transform.translation.y = frames[currentFrame].nodes[i].transY;
      transList[i].transform.translation.z = frames[currentFrame].nodes[i].transZ;
      transList[i].transform.rotation = tf::createQuaternionMsgFromRollPitchYaw(frames[currentFrame].nodes[i].rotX,frames[currentFrame].nodes[i].rotY,
                                            frames[currentFrame].nodes[i].rotZ);
  
        //send the joint state and transform
      joint_pub.publish(joint_state);
      broadcaster.sendTransform(transList[i]);
    }
    
        // This will adjust as needed per iteration
        loop_rate.sleep();
        currentFrame++;
       // cout << "Frame: " << currentFrame << " is broadcasted" << endl; 
  }
}
/*
void movement::broadcast(int frameStart, int frameEnd){
  int argc = 0;         //no idea what to do
  char** argv;  
  ros::init(argc, argv, "my_dance_broadcast");  
  ros::NodeHandle node;
  tf::Transform transform;
  tf::TransformBroadcaster br;
  ros::Rate rate(15.0);   //will be dynamic later currently 20 hertz
  int currentFrame = frameStart;
  while(node.ok(), currentFrame < frameEnd){
    for(int bn =0; bn < 15; bn++){            //there are 15 nodes in body
      transform.setOrigin( tf::Vector3(frames[currentFrame].nodes[bn].transX,
                       frames[currentFrame].nodes[bn].transY,
                       frames[currentFrame].nodes[bn].transZ));
      transform.setRotation( tf::Quaternion(frames[currentFrame].nodes[bn].rotX,
                          frames[currentFrame].nodes[bn].rotY,
                          frames[currentFrame].nodes[bn].rotZ));
      br.sendTransform(tf::StampedTransform(transform, ros::Time::now(), "openni_depth_frame", frames[currentFrame].nodes[bn].nodeName));
    }     
    currentFrame++;   
    
    rate.sleep();
  }
  node.shutdown();
}

*/
int movement::getFrameCount(){
  return frameCount;
}

void movement::insertFrame(Frame input){
  frames.push_back(input);
}

void movement::updateFrameCount(int count){
  frameCount = count;
}
/*
quat movement::testData(int currentFrame, int bn){
  
  return q; 
}
*/



int main(int argc, char** argv){
  
  
  movement demo("wholeBody");
  demo.broadcast();
//  float t = 0.1;
//  ros::NodeHandle n;
//  ros::Timer timer1 = n.createTimer(ros::Duration(t), &movement::broadcast, &demo);
  
/*
  std::ofstream dataFile;
  std::string fullpath = "/home/anil/Documents/tf/check.txt";
  const char* fileName = fullpath.c_str();
  dataFile.open(fileName);
  
  for (int i = 0; i < demo.getFrameCount(); i++){
    Frame f = demo.getFrame(i);
    for (int j = 0; j < 7; j++){
      dataFile << f.nodes[j].transX << " " << f.nodes[j].transY << " " << f.nodes[j].transZ << " " <<
          f.nodes[j].rotX << " " << f.nodes[j].rotY << " " << f.nodes[j].rotZ << " "; 
    }
    dataFile << endl;
  }
  dataFile.close();
  */
//  Frame f;
/*
  for (int i = 0; i < 200; i++){
    for (int j = 0; j < 7; j++){
      f = demo.getFrame(i);
      cout << "tx: " << f.nodes[j].transX << "ty: " << f.nodes[j].transY << "tz: " << f.nodes[j].transZ << endl;
    }
  }
*/
  



  return 0;
}


