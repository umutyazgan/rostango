#include <vector>
#include <iostream>
#include <iterator>
#include <fstream>
#include <string>
#include <cmath>
#include <random>
#include <functional> 
#include <stdio.h>
#include "Movement.h"
#include "Song.h"
#include "EditCSV.h"
#include <time.h>


#define NODECOUNT 52


using namespace std;



int main()
{
	
	
	//int input;
	//
	//vector<int> moveStart;
	//vector<int> moveEnd;
	//int moveCount = 0;
	//
	//string fullPath = "C:\\Users\\" + userName + "\\Documents\\mocap_data\\library\\raw_record\\" + moveFileName + "\\move_locations.txt" ;
	//ifstream move_locations(fullPath);	
	//if(move_locations.is_open()){
	//	while(move_locations.good()){
	//		move_locations >> input;
	//		moveStart.push_back(input);
	//		move_locations >> input;
	//		moveEnd.push_back(input);
	//		moveCount++;
	//	}
	//	
	//}
	//else
	//	cout << "unable to open the file: move locations" << endl;
	//move_locations.close();
	//
	//
	//
	//
	//Movement prim;
	//Movement *point;
	//string primitiveName;
	//for (int e=1; e<=31; e++){
	//	point = &prim;
	//	m.extractMove(moveStart[e], moveEnd[e],point);
	//	primitiveName = to_string(e);
	//	point->printMove(primitiveName);
	//}
	////
	//
	//footRegions fr;
	
	//int i = 1;
	//Movement prim("mala_vida", to_string(i));
	//footRegions f;
	//f =	prim.findFeetRegion(0);

//
	srand(time(NULL));
	Choreography choreo;
	default_random_engine generator;
	uniform_int_distribution<int> distribution(1, 30);
//	int dice_roll = distribution(generator);
//	auto dice = std::bind(distribution, generator);

	string pMoveName = "mala_vida";

	
	Movement test(pMoveName, to_string(28));
	cout << " total(LH) = " <<  test.shiftSpace("LH") << endl;
	cout << " total(RH) = " << test.shiftSpace("RH") << endl;
	cout << " total(LF) = " << test.shiftSpace("LF") << endl;
	cout << " total(RF) = " << test.shiftSpace("RF") << endl;

	//guela
	float ratio = 1.1;

	for (int c = 0; c < 50; c++){
		int number = rand() % 30 + 1;
		if (number != 2 && number != 5 && number != 9 && number != 11 && 
			number != 13 && number != 14 && number != 17 && number != 19 &&
			number != 22 && number != 25 && number != 26){ 
		Movement mv(pMoveName, to_string(number));
		mv.scale(ratio);
		choreo.insertMove(mv,15);
		}	
	}


	
	//for (int p = 0; p < 10; p++){
		string choreoName = "negra";
		choreo.printCombination(choreoName);
		
	//}
	/*
	
	footRegions f1, f2;
	
	
	vector<int> moveStart;
	vector<int> moveEnd;
	int moveCount = 0;
	string moveFileName = "mala_vida";
	int input;
	string fullPath = "C:\\Users\\" + userName + "\\Documents\\mocap_data\\library\\raw_record\\" + moveFileName + "\\move_locations.txt" ;
	ifstream move_locations(fullPath);	
	if(move_locations.is_open()){
		while(move_locations.good()){
			move_locations >> input;
			moveStart.push_back(input);
			move_locations >> input;
			moveEnd.push_back(input);
			moveCount++;
		}
		
	}
	else
		cout << "unable to open the file: move locations" << endl;
	move_locations.close();
	

	*/
	/*
	Movement m(moveFileName);
	Movement p1, p2;
	Movement *point1, *point2;
	string primitiveName;

	Movement *prim = new Movement[31];
	Movement **point = new Movement*[31];
	*/
	

	/*
	for (int e=1; e<=31; e++){
		point[e] = &prim[e];
		m.extractMove(moveStart[e], moveEnd[e],point[e]);
		primitiveName = to_string(e);
		point[e]->printMove(primitiveName);
	}
	
	*/
	

		
	//	int dice_roll = distribution(generator);
	//	auto dice = std::bind(distribution, generator);
	/*
	string s = "mala_vida";
	Movement m1(s, to_string(1));
	Movement m2(s, to_string(2));
	Movement m3(s, to_string(3));

	Choreography c;
	c.insertMove(m1);
	c.insertMove(m2);
	c.insertMove(m3);
	c.insertMove(m2);

	c.printCombination("test");*/
	return 0;
}

