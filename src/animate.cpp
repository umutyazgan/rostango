#include <ros/ros.h>
#include <tf/transform_broadcaster.h>
#include <tf/transform_listener.h>

#include <vector>
#include <iostream>
#include <iterator>
#include <fstream>
#include <string>
#include <cmath>
using namespace std;


typedef struct quat{
	float xx;
	float yy;
	float zz;
	float ww;
};

typedef struct BodyNode{
	std::string nodeName;
	float transX;
	float transY;
	float transZ;
	float rotX;
	float rotY;
	float rotZ;
	tf::Quaternion qtr;		
};

typedef struct Frame{
	BodyNode nodes[7];				//body has 15 nodes
};



class movement{
	public:
		movement();		
		movement(std::string);
		Frame getFrame(int);		
		void broadcast();
		void broadcast(int, int);				//requires starting and ending frame
		int getFrameCount();
		void insertFrame(Frame);
		void updateFrameCount(int);
		quat testData(int, int);    //frame number, joint number	
	private:										//fix later needs to be at least protected
		std::vector<Frame> frames;
		int frameCount;
};

movement::movement(){

}

movement::movement(std::string moveName){
	frameCount = 0;	
	float value;
	std::ifstream moveFile;
	moveName = "/home/anil/catkin_ws/src/rostango/motion_library/" + moveName + ".txt";
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
			for(int i = 0; i < 7; i++){  // 7 nodes for now
				//ros' y coordinate correspond to motionbuilder z coordinate			
				moveFile >> data; input.nodes[i].transY = data / 100;
				moveFile >> data; input.nodes[i].transZ = data / 100;
				moveFile >> data; input.nodes[i].transX = data / 100;
				
				moveFile >> data;  input.nodes[i].rotY = data;			
				moveFile >> data;  input.nodes[i].rotZ = data;
				moveFile >> data;  input.nodes[i].rotX = data;

				input.nodes[i].qtr = tf::createQuaternionFromRPY(input.nodes[i].rotX, input.nodes[i].rotY, input.nodes[i].rotZ);

//				moveFile >> input.nodes[i].transX;
//				moveFile >> input.nodes[i].transY;
//				moveFile >> input.nodes[i].transZ;
//				moveFile >> input.nodes[i].rotX;			
	//			moveFile >> input.nodes[i].rotY;
		//		moveFile >> input.nodes[i].rotZ;	
				input.nodes[i].nodeName = nodeNames[i];			
			}	// a frame is ready
			frameCount++;
			frames.push_back(input);

		}
	}
	else
		cout << "unable to open move file" << endl;
	moveFile.close();	
	
}


Frame movement::getFrame(int frameNumber){
	return frames[frameNumber];
}


void movement::broadcast(){
	int argc = 0; 				//no idea what to do
	char** argv;	
	ros::init(argc, argv, "my_dance_broadcast");
	ros::NodeHandle node;	
	tf::Transform transform;
	tf::TransformBroadcaster br;
	
	float fps = 30;
	ros::Rate rate(fps);   
	int currentFrame = 0;
	
	cout << "broadcast is called" << endl; 
	int success = 0;
	int failure = 0;
	while(node.ok(), currentFrame < frameCount){
			
			transform.setOrigin( tf::Vector3(frames[currentFrame].nodes[0].transX,
											 frames[currentFrame].nodes[0].transY,
											 frames[currentFrame].nodes[0].transZ));
			transform.setRotation( frames[currentFrame].nodes[0].qtr);
			tf::TransformListener listener;
			
			br.sendTransform(tf::StampedTransform(transform, ros::Time::now(), "/base_link", frames[currentFrame].nodes[0].nodeName));
			//listener.lookupTransform("base_link", "hip", ros::Time::now(), ros::Duration(3.0));
			
			
		for(int bn =1; bn < 4; bn++){            //there are 7 nodes in body for now
			transform.setOrigin( tf::Vector3(frames[currentFrame].nodes[bn].transX,
											 frames[currentFrame].nodes[bn].transY,
											 frames[currentFrame].nodes[bn].transZ));
			transform.setRotation(  frames[currentFrame].nodes[bn].qtr);
			
			br.sendTransform(tf::StampedTransform(transform, ros::Time::now(), frames[currentFrame].nodes[bn-1].nodeName, frames[currentFrame].nodes[bn].nodeName));
			
		}
		
		transform.setOrigin( tf::Vector3(frames[currentFrame].nodes[4].transX,
											 frames[currentFrame].nodes[4].transY,
											 frames[currentFrame].nodes[4].transZ));
			transform.setRotation(  frames[currentFrame].nodes[4].qtr);
			br.sendTransform(tf::StampedTransform(transform, ros::Time::now(), "hip", frames[currentFrame].nodes[4].nodeName));
			
		
		for(int bn =5; bn < 7; bn++){            //there are 7 nodes in body for now
			transform.setOrigin( tf::Vector3(frames[currentFrame].nodes[bn].transX,
											 frames[currentFrame].nodes[bn].transY,
											 frames[currentFrame].nodes[bn].transZ));
			transform.setRotation(  frames[currentFrame].nodes[bn].qtr);
			br.sendTransform(tf::StampedTransform(transform, ros::Time::now(), frames[currentFrame].nodes[bn-1].nodeName, frames[currentFrame].nodes[bn].nodeName));
			
		}
		
		
		currentFrame++;		
		rate.sleep();
		
	}
	node.shutdown();
}

/*
void movement::broadcast(int frameStart, int frameEnd){
	int argc = 0; 				//no idea what to do
	char** argv;	
	ros::init(argc, argv, "my_dance_broadcast");	
	ros::NodeHandle node;
	tf::Transform transform;
	tf::TransformBroadcaster br;
	ros::Rate rate(15.0);   //will be dynamic later	currently 20 hertz
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
	
	
	movement demo("halfBody");
	demo.broadcast();
//	float t = 0.1;
//	ros::NodeHandle n;
//	ros::Timer timer1 = n.createTimer(ros::Duration(t), &movement::broadcast, &demo);
	
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
//	Frame f;
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


