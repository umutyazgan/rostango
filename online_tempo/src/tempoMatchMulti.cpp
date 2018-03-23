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
#include "headers/Movement.h"
#include <algorithm>
#include <sstream>
#include "std_msgs/Float32.h"
#include "std_msgs/Float32MultiArray.h"


#include <online_tempo/frame_message.h>


#include <random>

#define NCOUNT 22

using namespace std;

static float currentTempo;
static bool harkSignalReady = false;


void harkBPMSignal(const std_msgs::Float32MultiArray::ConstPtr& msg)
{
  /*
  if (msg->data != 0){
    currentTempo = msg->data;
    std::cout << "Current Tempo is: " << currentTempo << std::endl;
    harkSignalReady = true;
  }
  */
  if( msg->data[1] > 0.7 && msg->data[0] != 0) //if message is reliable enough
      currentTempo = msg->data[0];
    //  std::cout << "Tempo found: " << msg->data[0] << std::endl;
      harkSignalReady = true;

//  else
//    harkSignalReady = false;
}

int main(int argc, char** argv){
 
                                    // distribution that maps to 1..6
                                    // see random number distributions
//int x = six(rng);                   // simulate rolling a die

  //movement extraction block
  /*
  Movement demo("mala_vida");
  int input;
  
  vector<int> moveStart;
  vector<int> moveEnd;
  int moveCount = 0;
  string fullPath = "/home/anil/catkin_ws/src/online_tempo/mocap_data/library/move_locations/mala_vida/move_locations.txt" ;
  ifstream move_locations(fullPath.c_str());  
  if(move_locations.is_open()){
    while(move_locations.good()){
      move_locations >> input;
      moveStart.push_back(input);
      move_locations >> input;
      moveEnd.push_back(input);
      moveCount++;
    }
    
  }
  else{
    const int last_error = errno;
    std::cerr << "failed to open move_locations.txt"
              << ": "
              << strerror(last_error)
              << std::endl;
  }
  move_locations.close();
  
  
Movement *prim = new Movement[30];
Movement **point = new Movement*[30];
string primitiveName;  
  

  
  for (int e=1; e<=30; e++){
    point[e] = &prim[e];
    demo.extractMove(moveStart[e], moveEnd[e],point[e]);
    std::ostringstream ss;
    ss << e;
    primitiveName = ss.str();
    point[e]->printMove(primitiveName);
  }

*/
  

     

  ros::init(argc, argv, "hark");
  ros::NodeHandle brd;
  ros::NodeHandle hrk;
  
  //Movement demo("mala_vida");
  
  //cout << "waiting for BPM..." << endl;
  //harkSignalReady = false;
  //int primNumber1;
  //int primNumber2;


  
  string moveName = "mala_vida";
  /*
  Movement m1(moveName, "10");
  Movement m2(moveName, "26");
  Movement m3(moveName, "20");
  Movement m4(moveName, "30");
  Movement m5(moveName, "27");
  Movement m6(moveName, "5");
  Movement m7(moveName, "25");
  Movement m8(moveName, "10");
  Movement m9(moveName, "23");
  Movement m10(moveName, "12");
  Movement m11(moveName, "27");  
  Movement m12(moveName, "9");
  Movement m13(moveName, "6");
  Movement m14(moveName, "12");
  Movement m15(moveName, "14");
  Movement m16(moveName, "5");
  Movement m17(moveName, "15");
  Movement m18(moveName, "9");
  Movement m19(moveName, "11");
  Movement m20(moveName, "3");
  

  Movement inter, inter2;
  Movement inter3, inter4;
  Movement inter5;
  Movement inter6, inter7;
  Movement inter8, inter9;
  Movement inter10;
  Movement inter11, inter12;
  Movement inter13, inter14;
  Movement inter15;
  Movement inter16, inter17;
  Movement inter18, inter19;
    
  Movement* m1_p1;
  m1_p1 = &m1;
  Movement* m2_p2;
  m2_p2 = &m2;
  Movement* m3_p3;
  m3_p3 = &m3;
  Movement* m4_p4;
  m4_p4 = &m4;
  Movement* m5_p5;
  m5_p5 = &m5;
  Movement* m6_p6;
  m6_p6 = &m6;
  Movement* m7_p7;
  m7_p7 = &m7;
  Movement* m8_p8;
  m8_p8 = &m8;
  Movement* m9_p9;
  m9_p9 = &m9;
  Movement* m10_p10;
  m10_p10 = &m10;
  Movement* m11_p11;
  m11_p11 = &m11;
  Movement* m12_p12; 
  m12_p12 = &m12;
  Movement* m13_p13;
  m13_p13 = &m13;
  Movement* m14_p14;
  m14_p14 = &m14;
  Movement* m15_p15;
  m15_p15 = &m15;
  Movement* m16_p16;
  m16_p16 = &m16;
  Movement* m17_p17;
  m17_p17 = &m17;
  Movement* m18_p18;
  m18_p18 = &m18;
  Movement* m19_p19;
  m19_p19 = &m19;
  Movement* m20_p20;
  m20_p20 = &m20;
      

  inter.interpolate(m1_p1, m2_p2, 15);
  inter2.interpolate(m2_p2, m3_p3, 15);
  inter3.interpolate(m3_p3, m4_p4, 15);
  inter4.interpolate(m4_p4, m5_p5, 15);
  
  inter5.interpolate(m5_p5, m6_p6, 15);
  inter6.interpolate(m6_p6, m7_p7, 15);
  inter7.interpolate(m7_p7, m8_p8, 15);
  inter8.interpolate(m8_p8, m9_p9, 15);
  inter9.interpolate(m9_p9, m10_p10, 15);
  inter10.interpolate(m10_p10, m11_p11, 15);
  */
  /*
  inter11.interpolate(m11_p11, m12_p12, 15);
  inter12.interpolate(m12_p12, m13_p13, 15);
  inter13.interpolate(m13_p13, m14_p14, 15);
  inter14.interpolate(m14_p14, m15_p15, 15);
  inter15.interpolate(m15_p15, m16_p16, 15);
  inter16.interpolate(m16_p16, m17_p17, 15);
  inter17.interpolate(m17_p17, m18_p18, 15);
  inter18.interpolate(m18_p18, m19_p19, 15);
  inter19.interpolate(m19_p19, m20_p20, 15);
  */      
  /*
  m1.broadcast();
  
  inter.broadcast();
  m2.broadcast();
  
    
  inter2.broadcast();
  m3.broadcast();
  
  inter3.broadcast();
  m4.broadcast();
  
  inter4.broadcast();
  m5.broadcast();
  inter5.broadcast();
  m6.broadcast();
  
  inter6.broadcast();
  
  m7.broadcast();
  
  inter7.broadcast();
  m8.broadcast();
  
  inter8.broadcast();
  m9.broadcast();
  
  inter9.broadcast();
  m10.broadcast();
  */
  /*
  inter10.broadcast();
  m11.broadcast();
  
  inter11.broadcast();
  m12.broadcast();
  inter12.broadcast();



  m13.broadcast();
  
  inter13.broadcast();
  m14.broadcast();
  
  inter14.broadcast();
  m15.broadcast();
  
  inter15.broadcast();
  m16.broadcast();
  
  inter16.broadcast();
  m17.broadcast();
  
  inter17.broadcast();
  m18.broadcast();



*/









/*
  inter.broadcast();
  inter2.broadcast();
  inter3.broadcast();
  inter4.broadcast();
  inter5.broadcast();

*/
online_tempo::frame_message f_m; 
//online_tempo::frame_message f_ms[30]; 

//online_tempo::frame_message_burst f_m_b;





ros::Subscriber sub = hrk.subscribe("/HarkStdMsgsTempoAndRely", 1000, harkBPMSignal);
  
ros::Publisher tempoMatch_pub = brd.advertise<online_tempo::frame_message>("broadcaster", 10000);
ros::Rate send_rate(30);



int primNumber = 1;
std::ostringstream ss;
int currentFrame = 0;
//string primNumbers[20] = {"1", "2", "3", "4", "5" ,"6", "7", "8", "9", "10",
//                    "11", "12", "13", "14", "15" ,"16", "17", "18", "19", "20"};    

Movement *prim = new Movement[30];
Movement *primPtr[30];
//cout << "creating primitives" << endl;
//srand (time(NULL));
cout << "loading primitives..." << endl;
for(int i=1; i <= 30; i++){ //ATTENTION 30 is primitive count, there is possibility for segFault
  std::ostringstream ss;
  ss << i;
  primPtr[i] = new Movement(moveName, ss.str());
  primPtr[i]->setTempo(108); //ATTENTION this is also mala vida specific
}
cout << "done!" << endl; 

//Movement* test = new Movement("mala_vida");

Movement* lastMove;
Movement* nextMove;
Movement* moveToSend;
Movement inter;  





//float referenceBPM = 108; //Attention this bpm is only true for Mala Vida song

bool firstMoveSent = false;
bool moveIsSent = false;
bool interpolationTurn = false;
int interWindow = 15;
float tempoRatio;
  
std::random_device rd;
std::mt19937 gen(rd());
std::uniform_int_distribution<> dis(1, 30);
//dis(gen) produces random integer on range above 
lastMove = primPtr[dis(gen)];
nextMove = primPtr[dis(gen)];  

moveToSend = lastMove;
Frame frm;
while (ros::ok()){
    //if (harkSignalReady = false)
  if(harkSignalReady == false){
    //cout << "waiting for BPM signal" << endl;
    ros::spinOnce();
  }

  else{ //hark signal is ready 
 
    frm = moveToSend->getFrame(currentFrame);
    for (int jNo = 0; jNo<NCOUNT; jNo++){
      //j = m1.getJoint(currentFrame,jNo);
      f_m.joints[jNo].transX = frm.joints[jNo].transX;
      f_m.joints[jNo].transY = frm.joints[jNo].transY;
      f_m.joints[jNo].transZ = frm.joints[jNo].transZ;
      f_m.joints[jNo].rotX = frm.joints[jNo].rotX;
      f_m.joints[jNo].rotY = frm.joints[jNo].rotY;
      f_m.joints[jNo].rotZ = frm.joints[jNo].rotZ;   
    }
    f_m.tempo = moveToSend->getTempo(); 
    if (currentTempo != 0){
      //ROS_INFO("move to send tempo %f", lastMove->getTempo() );
      ROS_INFO("moveToSend Tempo %f", moveToSend->getTempo());
      ROS_INFO("current Tempo %f", currentTempo);
      tempoRatio = moveToSend->getTempo() / currentTempo ;        
      tempoRatio =  floor((tempoRatio * pow(10, 2) + 0.5)) / pow(10, 2); //taking only 2 digits after point
      interWindow = tempoRatio * 15;  //ATTENTION 15 is mala vida specific as well    
    }
    else
      tempoRatio = 1;
   // cout << "tempoRatio = " << tempoRatio << endl;  
   // cout << "interWindow = " << interWindow << endl;   
    currentFrame++;
    if (currentFrame == moveToSend->getFrameCount()){ //move is sent completely
      currentFrame = 0;

      if(interpolationTurn){  //time to send interpolation
        inter.interpolate(lastMove, nextMove, interWindow);
        moveToSend = &inter;
        moveToSend->setTempo(currentTempo);
        ROS_INFO ("interpolation: %s is sent", moveToSend->getMoveName().c_str());
        interpolationTurn = false;
        lastMove = nextMove;
        nextMove = primPtr[dis(gen)];
      }
      else{ //time to send primitive
        if (currentTempo != lastMove->getTempo()){ // scale the moves when needed
          //double scaleBegin = ros::Time::now().toSec();  
          if (tempoRatio != 1){
            ROS_INFO( "scaling %s by %f",  lastMove->getMoveName().c_str(), tempoRatio);
            lastMove->scale(tempoRatio);
          }
          //double scaleEnd = ros::Time::now().toSec();
          //double duration = scaleEnd - scaleBegin;
          //cout << "scaling took: " << duration << endl; 
          //if(currentTempo != 0)
            //lastMove->setTempo(currentTempo);  
        }
        else
          ROS_INFO( "no need for scale");   
        moveToSend = lastMove;
        ROS_INFO("primitive: %s is sent",  moveToSend->getMoveName().c_str());
        ROS_INFO("currentTempo: %f ",  currentTempo);
        
        interpolationTurn = true;
      }


    }
    

    tempoMatch_pub.publish(f_m);
    send_rate.sleep();
    ros::spinOnce();  
  }

    /*
    if (harkSignalReady)
    {
      primNumber1 = primNumber2;


      primNumber1 = rand() % 30 + 1;
      std::ostringstream ss1;
      ss1 << primNumber1;
      //cout << "rand:" << ss1.str() << endl;
      //primNumber2 = rand() % 30 + 1;
      //std::ostringstream ss2;
      //ss2 << primNumber2;
        
      Movement m1(moveName, ss1.str());
      //Movement m2(moveName, ss2.str());
      //Movement inter;

      //cout << "currentBPM is " << currentBPM << endl;
      tempoRatio = referenceBPM / currentBPM;
      //cout << "ratio is:" << tempoRatio << endl;
      //m1.scale(tempoRatio);
      //inter.interpolate(&m1,&m2, 15);
      //m1.broadcast();
      //inter.broadcast();
      //demo.scale(tempoRatio);
      //demo.broadcast();
      //harkSignalReady = false;  
      //currentBPM = 0;    
    
    }*/
    
      
    
  }

  
 
  



  return 0;
}


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
#include "headers/Movement.h"
#include <algorithm>
#include <sstream>
#include "std_msgs/Float32.h"
#include "std_msgs/Float32MultiArray.h"


#include <online_tempo/frame_message.h>


#include <random>

#define NCOUNT 22

using namespace std;

static float currentTempo = -1;
static bool harkSignalReady = false;


void harkBPMSignal(const std_msgs::Float32MultiArray::ConstPtr& msg)
{
  /*
  if (msg->data != 0){
    currentTempo = msg->data;
    std::cout << "Current Tempo is: " << currentTempo << std::endl;
    harkSignalReady = true;
  }
  */
  if( msg->data[1] > 0.7 && msg->data[0] != 0){ //if message is reliable enough
      currentTempo = msg->data[0];
    //  std::cout << "Tempo found: " << msg->data[0] << std::endl;
      harkSignalReady = true;
  }
}
//    harkSignalReady = false;


int main(int argc, char** argv){
 
                                    // distribution that maps to 1..6
                                    // see random number distributions
//int x = six(rng);                   // simulate rolling a die

  //movement extraction block
  /*
  Movement demo("mala_vida");
  int input;
  
  vector<int> moveStart;
  vector<int> moveEnd;
  int moveCount = 0;
  string fullPath = "/home/anil/catkin_ws/src/online_tempo/mocap_data/library/move_locations/mala_vida/move_locations.txt" ;
  ifstream move_locations(fullPath.c_str());  
  if(move_locations.is_open()){
    while(move_locations.good()){
      move_locations >> input;
      moveStart.push_back(input);
      move_locations >> input;
      moveEnd.push_back(input);
      moveCount++;
    }
    
  }
  else{
    const int last_error = errno;
    std::cerr << "failed to open move_locations.txt"
              << ": "
              << strerror(last_error)
              << std::endl;
  }
  move_locations.close();
  
  
Movement *prim = new Movement[30];
Movement **point = new Movement*[30];
string primitiveName;  
  

  
  for (int e=1; e<=30; e++){
    point[e] = &prim[e];
    demo.extractMove(moveStart[e], moveEnd[e],point[e]);
    std::ostringstream ss;
    ss << e;
    primitiveName = ss.str();
    point[e]->printMove(primitiveName);
  }

*/
  

     

  ros::init(argc, argv, "hark");
  ros::NodeHandle brd;
  ros::NodeHandle hrk;
  
  //Movement demo("mala_vida");
  
  //cout << "waiting for BPM..." << endl;
  //harkSignalReady = false;
  //int primNumber1;
  //int primNumber2;


  
  string moveName = "mala_vida";
  /*
  Movement m1(moveName, "10");
  Movement m2(moveName, "26");
  Movement m3(moveName, "20");
  Movement m4(moveName, "30");
  Movement m5(moveName, "27");
  Movement m6(moveName, "5");
  Movement m7(moveName, "25");
  Movement m8(moveName, "10");
  Movement m9(moveName, "23");
  Movement m10(moveName, "12");
  Movement m11(moveName, "27");  
  Movement m12(moveName, "9");
  Movement m13(moveName, "6");
  Movement m14(moveName, "12");
  Movement m15(moveName, "14");
  Movement m16(moveName, "5");
  Movement m17(moveName, "15");
  Movement m18(moveName, "9");
  Movement m19(moveName, "11");
  Movement m20(moveName, "3");
  

  Movement inter, inter2;
  Movement inter3, inter4;
  Movement inter5;
  Movement inter6, inter7;
  Movement inter8, inter9;
  Movement inter10;
  Movement inter11, inter12;
  Movement inter13, inter14;
  Movement inter15;
  Movement inter16, inter17;
  Movement inter18, inter19;
    
  Movement* m1_p1;
  m1_p1 = &m1;
  Movement* m2_p2;
  m2_p2 = &m2;
  Movement* m3_p3;
  m3_p3 = &m3;
  Movement* m4_p4;
  m4_p4 = &m4;
  Movement* m5_p5;
  m5_p5 = &m5;
  Movement* m6_p6;
  m6_p6 = &m6;
  Movement* m7_p7;
  m7_p7 = &m7;
  Movement* m8_p8;
  m8_p8 = &m8;
  Movement* m9_p9;
  m9_p9 = &m9;
  Movement* m10_p10;
  m10_p10 = &m10;
  Movement* m11_p11;
  m11_p11 = &m11;
  Movement* m12_p12; 
  m12_p12 = &m12;
  Movement* m13_p13;
  m13_p13 = &m13;
  Movement* m14_p14;
  m14_p14 = &m14;
  Movement* m15_p15;
  m15_p15 = &m15;
  Movement* m16_p16;
  m16_p16 = &m16;
  Movement* m17_p17;
  m17_p17 = &m17;
  Movement* m18_p18;
  m18_p18 = &m18;
  Movement* m19_p19;
  m19_p19 = &m19;
  Movement* m20_p20;
  m20_p20 = &m20;
      

  inter.interpolate(m1_p1, m2_p2, 15);
  inter2.interpolate(m2_p2, m3_p3, 15);
  inter3.interpolate(m3_p3, m4_p4, 15);
  inter4.interpolate(m4_p4, m5_p5, 15);
  
  inter5.interpolate(m5_p5, m6_p6, 15);
  inter6.interpolate(m6_p6, m7_p7, 15);
  inter7.interpolate(m7_p7, m8_p8, 15);
  inter8.interpolate(m8_p8, m9_p9, 15);
  inter9.interpolate(m9_p9, m10_p10, 15);
  inter10.interpolate(m10_p10, m11_p11, 15);
  */
  /*
  inter11.interpolate(m11_p11, m12_p12, 15);
  inter12.interpolate(m12_p12, m13_p13, 15);
  inter13.interpolate(m13_p13, m14_p14, 15);
  inter14.interpolate(m14_p14, m15_p15, 15);
  inter15.interpolate(m15_p15, m16_p16, 15);
  inter16.interpolate(m16_p16, m17_p17, 15);
  inter17.interpolate(m17_p17, m18_p18, 15);
  inter18.interpolate(m18_p18, m19_p19, 15);
  inter19.interpolate(m19_p19, m20_p20, 15);
  */      
  /*
  m1.broadcast();
  
  inter.broadcast();
  m2.broadcast();
  
    
  inter2.broadcast();
  m3.broadcast();
  
  inter3.broadcast();
  m4.broadcast();
  
  inter4.broadcast();
  m5.broadcast();
  inter5.broadcast();
  m6.broadcast();
  
  inter6.broadcast();
  
  m7.broadcast();
  
  inter7.broadcast();
  m8.broadcast();
  
  inter8.broadcast();
  m9.broadcast();
  
  inter9.broadcast();
  m10.broadcast();
  */
  /*
  inter10.broadcast();
  m11.broadcast();
  
  inter11.broadcast();
  m12.broadcast();
  inter12.broadcast();



  m13.broadcast();
  
  inter13.broadcast();
  m14.broadcast();
  
  inter14.broadcast();
  m15.broadcast();
  
  inter15.broadcast();
  m16.broadcast();
  
  inter16.broadcast();
  m17.broadcast();
  
  inter17.broadcast();
  m18.broadcast();



*/









/*
  inter.broadcast();
  inter2.broadcast();
  inter3.broadcast();
  inter4.broadcast();
  inter5.broadcast();

*/
online_tempo::frame_message f_m; 
//online_tempo::frame_message f_ms[30]; 

//online_tempo::frame_message_burst f_m_b;





ros::Subscriber sub = hrk.subscribe("/HarkStdMsgsTempoAndRely", 1000, harkBPMSignal);
  
ros::Publisher tempoMatch_pub = brd.advertise<online_tempo::frame_message>("broadcaster", 10000);
ros::Rate send_rate(30);



int primNumber = 1;
std::ostringstream ss;
int currentFrame = 0;
//string primNumbers[20] = {"1", "2", "3", "4", "5" ,"6", "7", "8", "9", "10",
//                    "11", "12", "13", "14", "15" ,"16", "17", "18", "19", "20"};    

Movement *prim = new Movement[30];
Movement *primPtr[30];
//cout << "creating primitives" << endl;
//srand (time(NULL));
cout << "loading primitives..." << endl;
for(int i=1; i <= 30; i++){ //ATTENTION 30 is primitive count, there is possibility for segFault
  std::ostringstream ss;
  ss << i;
  primPtr[i] = new Movement(moveName, ss.str());
  primPtr[i]->setTempo(108); //ATTENTION this is also mala vida specific
}
cout << "done!" << endl; 

//Movement* test = new Movement("mala_vida");

Movement* lastMove;
Movement* nextMove;
Movement* moveToSend;
Movement inter;  





//float referenceBPM = 108; //Attention this bpm is only true for Mala Vida song

bool firstMoveSent = false;
bool moveIsSent = false;
bool interpolationTurn = false;
int interWindow = 15;
float tempoRatio;
  
std::random_device rd;
std::mt19937 gen(rd());
std::uniform_int_distribution<> dis(1, 30);
//dis(gen) produces random integer on range above 
lastMove = primPtr[dis(gen)];
nextMove = primPtr[dis(gen)];  
moveToSend = lastMove;
Frame frm;
Frame fr2, frm3;
while (ros::ok()){
    //if (harkSignalReady = false)
  if(harkSignalReady == false){
   // ROS_INFO ("waiting for BPM signal");
    ros::spinOnce();
  }

  else{ //hark signal is ready 
 
    
    
      //ROS_INFO("move to send tempo %f", lastMove->getTempo() );
      //tempoRatio = moveToSend->getTempo() / currentTempo;         
      //tempoRatio =  floor((tempoRatio * pow(10, 2) + 0.5)) / pow(10, 2); //taking only 2 digits after point
      //interWindow = tempoRatio * 15;  //ATTENTION 15 is mala vida specific as well    
    
    // cout << "tempoRatio = " << tempoRatio << endl;  
   // cout << "interWindow = " << interWindow << endl;   
    currentFrame++;

    if (currentFrame == moveToSend->getFrameCount()){ //move is sent completely
      
    

      currentFrame = 0;

      if(interpolationTurn){  //time to send interpolation
        if (currentTempo >= 0 ){
          tempoRatio = moveToSend->getTempo() / currentTempo;         
          interWindow = tempoRatio * 15;  //ATTENTION 15 is mala vida specific as well   
        }

        inter.interpolate(lastMove, nextMove, interWindow);
        if (currentTempo != 0){
          inter.setTempo(currentTempo);
        }
        moveToSend = &inter;
        //moveToSend->setTempo(lastMove->getTempo());
       // ROS_INFO ("interpolation: %s is sent", moveToSend->getMoveName().c_str());
        interpolationTurn = false;
        lastMove = nextMove;
        nextMove = primPtr[dis(gen)];
      }
      else{ //time to send primitive
        //if (currentTempo != lastMove->getTempo()){ // scale the moves when needed
          //double scaleBegin = ros::Time::now().toSec();  
          if (currentTempo >= 0 ){
            tempoRatio = lastMove->getTempo() / currentTempo;         
            interWindow = tempoRatio * 15;  //ATTENTION 15 is mala vida specific as well   
          }

          if (tempoRatio != 1){
            ROS_INFO( "scaling %s by %f",  lastMove->getMoveName().c_str(), tempoRatio);
            lastMove->scale(tempoRatio);
            
          }
          //double scaleEnd = ros::Time::now().toSec();
          //double duration = scaleEnd - scaleBegin;
          //cout << "scaling took: " << duration << endl; 
          //if(currentTempo != 0)
            //lastMove->setTempo(currentTempo);  
        //}
        else
          ROS_INFO( "no need for scale");   
        moveToSend = lastMove;
     
        //ROS_INFO("currentTempo: %f ",  currentTempo);
        
        interpolationTurn = true;
      }
        ROS_INFO("-------------------------------------------");
        ROS_INFO("moveToSend Tempo %f", moveToSend->getTempo());
        ROS_INFO("current Tempo %f", currentTempo);
        ROS_INFO("MOVE NAME: %s is sent",  moveToSend->getMoveName().c_str());
        ROS_INFO("-------------------------------------------");
        

    }
    
    frm = moveToSend->getFrame(currentFrame);
    for (int jNo = 0; jNo<NCOUNT; jNo++){
      //j = m1.getJoint(currentFrame,jNo);
      f_m.joints[jNo].transX = frm.joints[jNo].transX;
      f_m.joints[jNo].transY = frm.joints[jNo].transY;
      f_m.joints[jNo].transZ = frm.joints[jNo].transZ;
      f_m.joints[jNo].rotX = frm.joints[jNo].rotX;
      f_m.joints[jNo].rotY = frm.joints[jNo].rotY;
      f_m.joints[jNo].rotZ = frm.joints[jNo].rotZ;   
    }
    f_m.tempo = moveToSend->getTempo(); 

    if(currentTempo != 0){
      tempoMatch_pub.publish(f_m);
    }

    moveToSend->shiftMove(100, 100, 0); //z is upwards
    



    send_rate.sleep();
    ros::spinOnce();  
  }

    /*
    if (harkSignalReady)
    {
      primNumber1 = primNumber2;


      primNumber1 = rand() % 30 + 1;
      std::ostringstream ss1;
      ss1 << primNumber1;
      //cout << "rand:" << ss1.str() << endl;
      //primNumber2 = rand() % 30 + 1;
      //std::ostringstream ss2;
      //ss2 << primNumber2;
        
      Movement m1(moveName, ss1.str());
      //Movement m2(moveName, ss2.str());
      //Movement inter;

      //cout << "currentBPM is " << currentBPM << endl;
      tempoRatio = referenceBPM / currentBPM;
      //cout << "ratio is:" << tempoRatio << endl;
      //m1.scale(tempoRatio);
      //inter.interpolate(&m1,&m2, 15);
      //m1.broadcast();
      //inter.broadcast();
      //demo.scale(tempoRatio);
      //demo.broadcast();
      //harkSignalReady = false;  
      //currentBPM = 0;    
    
    }*/
    
      
    
  }

  
 
  



  return 0;
}


