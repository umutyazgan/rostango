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
#include "spline.h"
#include <algorithm>
#include "std_msgs/String.h"
#include "std_msgs/Float32.h"
#include <sstream>
#include <time.h>

#define NCOUNT 22

using namespace std;

struct Joint{
  std::string nodeName;
  double transX;
  double transY;
  double transZ;
  double rotX;
  double rotY;
  double rotZ;
};

struct Frame{
  Joint joints[NCOUNT];       
};



class Movement{
public:
  Movement();
  Movement(string);   //this is for raw records
  Movement(string, string); //this is for primitives, move name + primitive number
  ~Movement();
  Frame getFrame(int);    
  void extractMove(int,int,Movement*); //give starting and ending of move 
  void printMove(string);
  void scale(float);     //scale the movement in time
  Joint getJoint(int,int);  //frame no, joint no
  string getMoveName();
  void setMoveName(string);
  void setInterWindow(int);
  int getInterWindow();
  int shiftSpace(string);    
  int getFrameCount();
  void insertFrame(Frame);
  void updateFrameCount(int);
  void interpolate(Movement*, Movement*, int); //last frame, first frame, length
  void shiftMove(float, float, float);
  void rotateMove(double);
  void fillMove(string, string); //ATTENTION this might be redundant in the future
  void clearMove();
  float getTempo();
  void setTempo(float);
private:                    
  std::vector<Frame> frames;
  int frameCount;
  int interWindow;
  string moveName;
  float tempo;
};

void Movement::rotateMove(double rotationDifference){
  int currentFrame = 0;
  //shift second move to end of first move. Hip joint is selected as pivot joint
  int pivotJoint = 0; // Hip is joint 0. Will be used for pivot 
  
  //cout << "shifting the move by xyz: " << shiftX << " " << shiftY << " " << shiftZ << endl; 
  while(currentFrame < getFrameCount()){
    this->frames[currentFrame].joints[pivotJoint].rotZ -= rotationDifference;
    //this->frames[currentFrame].joints[pivotJoint].transZ -= shiftZ; // no need to shift on Z actor's height should not change
    currentFrame++;
  }
  currentFrame = 0;
  /*
  while (currentFrame < getFrameCount()){
    for (int i = 1; i < NCOUNT; i++){
      double pastTransX = frames[currentFrame].joints[i].transX;
      double pastTransY = frames[currentFrame].joints[i].transY;
      frames[currentFrame].joints[i].transX = pastTransX * cos(rotationDifference) -
                                                    pastTransY * sin(rotationDifference);
      frames[currentFrame].joints[i].transY = pastTransX * sin(rotationDifference) + 
                                                    pastTransY * cos(rotationDifference);          
    }
    currentFrame++;
  }
  */
}

float Movement::getTempo(){
  return tempo;
}

void Movement::setTempo(float inputTempo){
  tempo = inputTempo;
}

void Movement::clearMove(){
  frames.clear();
}

void Movement::fillMove(string moveName, string primitiveNumber){
  frames.clear();
  this->moveName = moveName + "_" + primitiveNumber;
  string moveFileLocation;
  moveFileLocation = "/home/umut/catkin_ws/src/online_tempo/mocap_data/library/primitives/" + moveName + "/" + primitiveNumber + ".bvh";
  int headerLineCount = 316;
  frameCount = 0;

  ifstream moveFile(moveFileLocation.c_str());

  //skipping header
  ifstream inFile(moveFileLocation.c_str());
  int lineCount = std::count(std::istreambuf_iterator<char>(inFile),
    std::istreambuf_iterator<char>(), '\n');

  //cout << "lineCount: " << lineCount << endl;
  frames.resize(lineCount);
  Frame input;

  int currentFrame = 0;

  if (moveFile.is_open()){

    while (currentFrame < lineCount){
      for (int i = 0; i<NCOUNT; i++){
        moveFile >> frames[currentFrame].joints[i].transX;
        moveFile >> frames[currentFrame].joints[i].transY;
        moveFile >> frames[currentFrame].joints[i].transZ;
        moveFile >> frames[currentFrame].joints[i].rotX;
        moveFile >> frames[currentFrame].joints[i].rotY;
        moveFile >> frames[currentFrame].joints[i].rotZ;
        //cout << "join" << i << ".tX" << frames[currentFrame].joints[i].transX << endl;
      }
      currentFrame++;

    }
  }
  else{
    const int last_error = errno;
    std::cerr << "failed to open primitive file"
              << ": "
              << strerror(last_error)
              << std::endl;
  }

  inFile.close();
  moveFile.close();
  //cout << "primitve's frameCount: " << frames.size() << endl;
}


Movement::Movement(){
  //cout << "Movement() object created " << endl;
}

Movement::~Movement(){
  cout << "deleted object" << endl;
}

Movement::Movement(string moveName, string primitiveNumber){          //primitive constructor
  //cout << "movement primitive constructor is called " << endl;
  this->moveName = moveName + "_" + primitiveNumber;
  string moveFileLocation;
  moveFileLocation = "/home/umut/catkin_ws/src/online_tempo/mocap_data/library/primitives/" + moveName + "/" + primitiveNumber + ".bvh";
  int headerLineCount = 316;
  frameCount = 0;

  ifstream moveFile(moveFileLocation.c_str());

  //skipping header
  ifstream inFile(moveFileLocation.c_str());
  int lineCount = std::count(std::istreambuf_iterator<char>(inFile),
    std::istreambuf_iterator<char>(), '\n');

  //cout << "lineCount: " << lineCount << endl;
  frames.resize(lineCount);
  Frame input;

  int currentFrame = 0;

  if (moveFile.is_open()){

    while (currentFrame < lineCount){
      for (int i = 0; i<NCOUNT; i++){
        moveFile >> frames[currentFrame].joints[i].transX;
        moveFile >> frames[currentFrame].joints[i].transY;    //ATTENTION y and z might interchange for ros!!!
        moveFile >> frames[currentFrame].joints[i].transZ;
        moveFile >> frames[currentFrame].joints[i].rotX;
        moveFile >> frames[currentFrame].joints[i].rotY;      //ATTENTION y and z might interchange for ros!!!
        moveFile >> frames[currentFrame].joints[i].rotZ;    
        //cout << "join" << i << ".tX" << frames[currentFrame].joints[i].transX << endl;
      }
      currentFrame++;

    }
  }
  else{
    const int last_error = errno;
    std::cerr << "failed to open primitive file"
              << ": "
              << strerror(last_error)
              << std::endl;
  }

  inFile.close();
  moveFile.close();
  frameCount = frames.size();
  //cout << "primitve's frameCount: " << frames.size() << endl;
}


void Movement::shiftMove(float shiftX, float shiftY, float shiftZ){
  int currentFrame = 0;
  //shift second move to end of first move. Hip joint is selected as pivot joint
  int pivotJoint = 0; // Hip is joint 0. Will be used for pivot 
  
  //cout << "shifting the move by xyz: " << shiftX << " " << shiftY << " " << shiftZ << endl; 
  while(currentFrame < getFrameCount()){
    this->frames[currentFrame].joints[pivotJoint].transX -= shiftX;
    this->frames[currentFrame].joints[pivotJoint].transY -= shiftY;
    //this->frames[currentFrame].joints[pivotJoint].transZ -= shiftZ; // no need to shift on Z actor's height should not change
    currentFrame++;
  }
}

Joint Movement::getJoint(int frameNo, int jointNo){
  return frames[frameNo].joints[jointNo];
}

string Movement::getMoveName(){
  return moveName;
}

void Movement::interpolate(Movement* lastMove, Movement* nextMove, int interpolationLength){
  this->moveName = lastMove->getMoveName() + "_inter_" + nextMove->getMoveName();
  frames.clear();
 // cout << "interpolation is called" << endl;
  struct tempMove{
      vector<double> transX, transY, transZ;
      vector<double> rotX, rotY, rotZ;
      vector<double> index;
  };
  tempMove tMove;

  int pivotJoint; //pivotjoint will be left foot or right foot
      //Pivot Joint selection 
      //----------------
  int lastFrame = lastMove->frames.size() - 1;
  
  Joint LL; //LastLeft
  LL.transX = lastMove->getJoint(lastFrame, 0).transX;
  LL.transY = lastMove->getJoint(lastFrame, 0).transY;
  
  for (int i = 18; i <= 21; i++){
    LL.transX += lastMove->getJoint(lastFrame, i).transX;
    LL.transY += lastMove->getJoint(lastFrame, i).transY;
  }

  Joint LR; //LastRight
  LR.transX = lastMove->getJoint(lastFrame, 0).transX;
  LR.transY = lastMove->getJoint(lastFrame, 0).transY;
  for (int i = 14; i <= 17; i++){
    LR.transX += lastMove->getJoint(lastFrame, i).transX;
    LR.transY += lastMove->getJoint(lastFrame, i).transY;
  }
  Joint NL; //NextLeft
  NL.transX = nextMove->getJoint(0, 0).transX;
  NL.transY = nextMove->getJoint(0, 0).transY;
  for (int i = 18; i <= 20; i++){
    NL.transX += nextMove->getJoint(0, i).transX;
    NL.transY += nextMove->getJoint(0, i).transY;
  }
  Joint NR; //NextRight
  NR.transX = nextMove->getJoint(0, 0).transX;
  NR.transY = nextMove->getJoint(0, 0).transY;
  for (int i = 14; i <= 16; i++){
    NR.transX += nextMove->getJoint(0, i).transX;
    NR.transY += nextMove->getJoint(0, i).transY;
  }

  double distanceLeft , distanceRight;
  distanceLeft = sqrt(pow((NL.transX - LL.transX), 2) + pow((NL.transY - LL.transY), 2));
  distanceRight = sqrt(pow((NR.transX - LR.transX), 2) + pow((NR.transY - LR.transY), 2));
  double shiftX;
  double shiftY;
  double shiftZ = 0;
  if (distanceLeft < distanceRight){
    pivotJoint = 21; //left foot
    shiftX = NR.transX - LR.transX;
    shiftY = NR.transY - LR.transY;
  }
  else{
    pivotJoint = 17; //right foot
    shiftX = NL.transX - LL.transX;
    shiftY = NL.transY - LL.transY;
  }
    //---------------
  Frame targetFrame = nextMove->getFrame(0);   //interpolation will end at the beginning of new move
  Frame refFrame = lastMove->getFrame(lastMove->frames.size() - 1); //interpolation starts at the last frame of last move
  
  double shiftRotZ = targetFrame.joints[0].rotZ - refFrame.joints[0].rotZ;
  nextMove->rotateMove(shiftRotZ);
  
  nextMove->shiftMove(shiftX, shiftY, shiftZ);
  //Next move is shifted. Time for interpolation
  
  this->setInterWindow(interpolationLength);
  this->updateFrameCount(interpolationLength);
  //this->setMoveName("interpolation");
  
  this->frames.resize(interpolationLength);
  //allocating vectors before initialization saves run time
  tMove.transX.resize(lastMove->frames.size() + nextMove->frames.size());
  tMove.transY.resize(lastMove->frames.size() + nextMove->frames.size());
  tMove.transZ.resize(lastMove->frames.size() + nextMove->frames.size());
  tMove.rotX.resize(lastMove->frames.size() + nextMove->frames.size());
  tMove.rotY.resize(lastMove->frames.size() + nextMove->frames.size());
  tMove.rotZ.resize(lastMove->frames.size() + nextMove->frames.size());
  tMove.index.resize(lastMove->frames.size() + nextMove->frames.size());

  for (int n = 0; n < NCOUNT; n++){
    for (int i = 0; i < lastMove->frames.size(); i++){
      tMove.transX[i] = lastMove->getFrame(i).joints[n].transX;
      tMove.transY[i] = lastMove->getFrame(i).joints[n].transY;
      tMove.transZ[i] = lastMove->getFrame(i).joints[n].transZ;
      tMove.rotX[i] = lastMove->getFrame(i).joints[n].rotX;
      tMove.rotY[i] = lastMove->getFrame(i).joints[n].rotY;
      tMove.rotZ[i] = lastMove->getFrame(i).joints[n].rotZ;
      tMove.index[i] = i;
    }
    for (int i = lastMove->frames.size();
      i < lastMove->frames.size() + nextMove->frames.size(); i++){
      tMove.transX[i] = nextMove->getFrame(i - lastMove->frames.size()).joints[n].transX;
      tMove.transY[i] = nextMove->getFrame(i - lastMove->frames.size()).joints[n].transY;
      tMove.transZ[i] = nextMove->getFrame(i - lastMove->frames.size()).joints[n].transZ;
      tMove.rotX[i] = nextMove->getFrame(i - lastMove->frames.size()).joints[n].rotX;
      tMove.rotY[i] = nextMove->getFrame(i - lastMove->frames.size()).joints[n].rotY;
      tMove.rotZ[i] = nextMove->getFrame(i - lastMove->frames.size()).joints[n].rotZ;
      tMove.index[i] = i + interpolationLength;
    }
    tk::spline spTransX, spTransY, spTransZ;
    tk::spline spRotX, spRotY, spRotZ;
    spTransX.set_points(tMove.index, tMove.transX);
    spTransY.set_points(tMove.index, tMove.transY);
    spTransZ.set_points(tMove.index, tMove.transZ);
    spRotX.set_points(tMove.index, tMove.rotX);
    spRotY.set_points(tMove.index, tMove.rotY);
    spRotZ.set_points(tMove.index, tMove.rotZ);
    for (int i = 0; i < interpolationLength; i++){ 
      this->frames[i].joints[n].transX = spTransX(lastMove->frames.size() + i);
      this->frames[i].joints[n].transY = spTransY(lastMove->frames.size() + i);
      this->frames[i].joints[n].transZ = spTransZ(lastMove->frames.size() + i);
      this->frames[i].joints[n].rotX = spRotX(lastMove->frames.size() + i);
      this->frames[i].joints[n].rotY = spRotY(lastMove->frames.size() + i);
      this->frames[i].joints[n].rotZ = spRotZ(lastMove->frames.size() + i);
    }
  } // end of for (int n=0; n<NODECOUNT..
  
 

 
}
//Attention!!! check the function or main test program
//this extracts same move over over and over
void Movement::extractMove(int frameStart, int frameEnd, Movement* subMove){
  Frame inputFrame;
//  cout << "frame start: " << frameStart << "frame end: " << frameEnd << endl;
  for(int i = frameStart; i <= frameEnd; i++){
//      cout << "inserted frame: " << i << endl;
      inputFrame = getFrame(i);
      subMove->insertFrame(inputFrame);
  }
  int totalFrame = frameEnd - frameStart + 1;
  subMove->updateFrameCount(totalFrame);
  subMove->moveName = moveName;
  //cout << "extracted " << frameStart << " " << frameEnd << endl;
}

void Movement::printMove(string primitiveName){
  // header for rviz is quite different and unneccessary, skipping it
  /*
  string headerPath = "/home/umut/catkin_ws/src/online_tempo/mocap_data/library/header.txt";
  string content;
  std::ifstream ifs(headerPath.c_str());
  if(ifs.is_open()){
    content.assign( (std::istreambuf_iterator<char>(ifs) ),
                (std::istreambuf_iterator<char>()    ) );
  }
  else  
    cout << "unable to open header file" << endl;

  string motionFullPath = "/home/umut/catkin_ws/src/online_tempo/mocap_data/library/primitives/" + moveName + "/" + primitiveName + ".bvh" ;
  ofstream primitiveFile(motionFullPath.c_str()); 

  string frameRateInfo = "Frame Time: 0.0349650000"; //roughly 30 fps
*/
  int currentFrame = 0;
  string motionFullPath = "/home/umut/catkin_ws/src/online_tempo/mocap_data/library/primitives/" + moveName + "/" + primitiveName + ".bvh" ;
  ofstream primitiveFile(motionFullPath.c_str()); 


  if (primitiveFile.is_open()){
    //primitiveFile << content << ' ' << frameCount << "\n";
    //primitiveFile << frameRateInfo << "\n";
    Frame input_frame;
    int ff = 0;
    
    while (currentFrame < frameCount){
      input_frame = this->getFrame(ff);
      for (int nn = 0; nn < NCOUNT; nn++){
        primitiveFile << input_frame.joints[nn].transX << ' ' << 
                    input_frame.joints[nn].transY << ' ' <<
                    input_frame.joints[nn].transZ << ' ' <<
                    input_frame.joints[nn].rotX << ' ' <<
                    input_frame.joints[nn].rotY << ' ' <<
                    input_frame.joints[nn].rotZ << ' ';
      }
      primitiveFile << "\n"; 
      ff++;
      currentFrame++;
    }
  }
  else {
    const int last_error = errno;
    std::cerr << "failed to open primitive file"
              << ": "
              << strerror(last_error)
              << std::endl;
  }
  primitiveFile.close();
}

Movement::Movement(string moveName){   //record constuctor
  this->moveName = moveName;
  string moveFileLocation;
  //Attention!!! figure out how to give relative path
  moveFileLocation = "/home/umut/catkin_ws/src/online_tempo/mocap_data/library/raw_record/" + moveName + "/record.txt";
  
  frameCount = 0;
  ifstream moveFile(moveFileLocation.c_str());
  ifstream inFile(moveFileLocation.c_str()); 
  int lineCount = std::count(std::istreambuf_iterator<char>(inFile), 
            std::istreambuf_iterator<char>(), '\n');

  frames.resize(lineCount);
  double data;
  
  if(moveFile.is_open()){
    while(frameCount < lineCount){   
      for(int i = 0; i < NCOUNT; i++){  
        //ros' y coordinate correspond to motionbuilder z coordinate      
        //cout << "getting paramters" << endl;
        moveFile >> data; frames[frameCount].joints[i].transY = data / 100; //meter to cm
        moveFile >> data; frames[frameCount].joints[i].transZ = data / 100;
        moveFile >> data; frames[frameCount].joints[i].transX = data / 100;
        
        moveFile >> data;  frames[frameCount].joints[i].rotY = data / 57.2957;  //radian coefficients   
        moveFile >> data;  frames[frameCount].joints[i].rotZ = data / 57.2957;
        moveFile >> data;  frames[frameCount].joints[i].rotX = data / 57.2957;
        //cout << "parameter set " << i << "is gotten" << endl;
        //cout << "frame count: " << frameCount << endl; 
      } // a frame is ready
      frameCount++;
      
    }
  }
  else{
    const int last_error = errno;
    std::cerr << "failed to open record.txt"
              << ": "
              << strerror(last_error)
              << std::endl;
  }
    
  

  inFile.close();
  moveFile.close(); 
  //cout << "Movement: "<< moveName << " raw record is created" << endl;
}


Frame Movement::getFrame(int frameNumber){
  return frames[frameNumber];
}

void Movement::scale(float scaleRatio){


  int scaledFrameCount = scaleRatio * frameCount;
  

  struct tempMove{
    vector<double> transX, transY, transZ;
    vector<double> rotX, rotY, rotZ;
    vector<double> index;
  };
  tempMove tMove;
  tempMove sMove;

  tMove.transX.resize(frameCount);
  tMove.transY.resize(frameCount);
  tMove.transZ.resize(frameCount);
  tMove.rotX.resize(frameCount);
  tMove.rotY.resize(frameCount);
  tMove.rotZ.resize(frameCount);
  tMove.index.resize(frameCount);
  if(frameCount < scaledFrameCount)
    this->frames.resize(scaledFrameCount);

  for (int n=0; n<NCOUNT; n++){
    for (int i=0; i<frameCount; i++){
      tMove.transX[i] = frames[i].joints[n].transX;
      tMove.transY[i] = frames[i].joints[n].transY;
      tMove.transZ[i] = frames[i].joints[n].transZ;
      tMove.rotX[i] = frames[i].joints[n].rotX;
      tMove.rotY[i] = frames[i].joints[n].rotY;
      tMove.rotZ[i] = frames[i].joints[n].rotZ; 
      tMove.index[i] = i * scaleRatio;
    }
    tk::spline spTransX, spTransY, spTransZ;
    tk::spline spRotX, spRotY, spRotZ;
   
    spTransX.set_points(tMove.index, tMove.transX);
    spTransY.set_points(tMove.index, tMove.transY);
    spTransZ.set_points(tMove.index, tMove.transZ);
    spRotX.set_points(tMove.index, tMove.rotX);
    spRotY.set_points(tMove.index, tMove.rotY);
    spRotZ.set_points(tMove.index, tMove.rotZ);
   
    for (int si=0; si<scaledFrameCount; si++){
      frames[si].joints[n].transX = spTransX(si);
      frames[si].joints[n].transY = spTransY(si);
      frames[si].joints[n].transZ = spTransZ(si);
      frames[si].joints[n].rotX = spRotX(si);
      frames[si].joints[n].rotY = spRotY(si);
      frames[si].joints[n].rotZ = spRotZ(si);
    }

  }
  //if(frameCount > scaledFrameCount)
    this->frames.resize(scaledFrameCount);
  
  frameCount = scaledFrameCount;
  this->tempo = tempo / scaleRatio;
}

void Movement::setInterWindow(int newWindow){
  interWindow = newWindow;
}



int Movement::getFrameCount(){
  return frames.size();
}

void Movement::insertFrame(Frame input){
  frames.push_back(input);
}

void Movement::updateFrameCount(int count){
  frameCount = count;
}
/*
quat Movement::testData(int currentFrame, int bn){
  
  return q; 
}
*/



//Moved harkBpm to TempoMatch
/*
void harkBPMSignal(const std_msgs::Float32::ConstPtr& msg){
  //cout << "inside signal" << endl;
  if (msg->data != 0) {
    //cout << "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!" << endl;
    //cout << "BPM info came, current BPM = " << msg->data << endl;
    //cout << "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!" << endl;
    currentBPM = msg-> data;
    harkSignalReady = true;    
    //cout << "harkSignalReady is: " << harkSignalReady << endl;  
    //ros::shutdown();
  }
  else{
    harkSignalReady = false;
    //cout << "harkSignalReady is: " << harkSignalReady << endl; 
  }
  //cout << "msg->data" << msg->data << endl;
  //harkSignalReady = true;
  //cout << "harkSignalReady is " << harkSignalReady << endl;
   
}

*/
