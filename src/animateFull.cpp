#include <ros/ros.h>
#include <tf/transform_broadcaster.h>
#include <tf/transform_listener.h>

#include <vector>
#include <iostream>
#include <iterator>
#include <fstream>
#include <string>
#include <cmath>

#define NCOUNT 18
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
	BodyNode nodes[NCOUNT];				//body has 18 nodes
};

string hierarchy[][2] = {{"base_link", "hip"},
							{"hip","chest"},
							{"chest","neck"},
							{"neck","head"},							
							{"chest","left_clavicle"},
							{"left_clavicle","left_arm"},
							{"left_arm","left_forearm"},
							{"left_forearm","left_hand"},							
							{"chest","right_clavicle"},
							{"right_clavicle","right_arm"},
							{"right_arm","right_forearm"},
							{"right_forearm","right_hand"},							
							{"hip","right_tigh"},
							{"right_tigh" , "right_leg"},
							{"right_leg" , "right_foot"},							
							{"hip" , "left_tigh"},
							{"left_tigh" , "left_leg"},
							{"left_leg" , "left_foot"}};							

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
	cout << "creating movement object" << endl;
	frameCount = 0;	
	float value;
	std::ifstream moveFile;
	moveName = "/home/monster2/catkin_ws/src/rostango/motion_library/" + moveName + ".txt";
	const char* fileName = moveName.c_str();
	moveFile.open(fileName);
	/*
	std::vector<std::string> nodeNames;
	nodeNames.push_back("hip");
	nodeNames.push_back("right_tigh");
	nodeNames.push_back("right_leg");
	nodeNames.push_back("right_foot");
	nodeNames.push_back("left_tigh");
	nodeNames.push_back("left_leg");
	nodeNames.push_back("left_foot");
	 */
	Frame input;	
	float data;
	if(moveFile.is_open()){
		while(moveFile.good()){		
			for(int i = 0; i < NCOUNT; i++){  // 
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
				//input.nodes[i].nodeName = nodeNames[i];			
			}	// a frame is ready
			frameCount++;
			frames.push_back(input);

		}
	}
	else
		cout << "unable to open move file" << endl;
	moveFile.close();	
	cout << "object is created" << endl;
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
	
	float fps = 10;
	ros::Rate rate(fps);   
	int currentFrame = 0;
	
	cout << "broadcast is called" << endl; 
	int success = 0;
	int failure = 0;
	while(node.ok(), currentFrame < frameCount){
		for (int i=0; i < NCOUNT; i++){	
			transform.setOrigin( tf::Vector3(frames[currentFrame].nodes[i].transX,
											 frames[currentFrame].nodes[i].transY,
											 frames[currentFrame].nodes[i].transZ));
			transform.setRotation( frames[currentFrame].nodes[i].qtr);
			tf::TransformListener listener;
			
			br.sendTransform(tf::StampedTransform(transform, ros::Time::now(), hierarchy[i][0], hierarchy[i][1]));
			cout << "frame " << i << "is sent" << endl; 
		}	
			
		
		
		
		currentFrame++;		
		rate.sleep();
		
	}
	node.shutdown();
}

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
	

	movement demo("18Nodes");
	demo.broadcast();



	return 0;
}


