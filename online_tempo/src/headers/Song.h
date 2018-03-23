#include <vector>
#include <string>
#include <iostream>
#include <fstream>


using namespace std;

class Song{
	public:
		Song(string);
		double giveBPM();		//BPM stands for beat per minute
		double giveBeatTime(int);
		string giveSongName();
private:
		double inductionTime;	//marsyas takes around 5 seconds to induce phase and tempo
		double beatPerMinute;
		vector<double> beatTime;
		string songName;
};

Song::Song(string songBPMFile){
	songName = songBPMFile; //song file name also gives the song's name
	
	string fullPath_beatTime = "C:\\Users\\anil\\Music\\Choreo_Songs\\" + songBPMFile + ".txt";
	string fullPath_medianBPM = "C:\\Users\\anil\\Music\\Choreo_Songs\\" + songBPMFile + "_medianTempo.txt";
	ifstream beatTime_file(fullPath_beatTime);
	ifstream BPM_file(fullPath_medianBPM);
	double value;
	double currentBPM;
	if(beatTime_file.is_open()){
		beatTime_file >> inductionTime;  //first line of the file is the induction time, it is supposed value is around 5
		while(beatTime_file.good()){
			beatTime_file >> value;
			beatTime.push_back(value);
			beatTime_file >> currentBPM;
		}
	}
	else
		cout << "unable to open song file meta" << endl;
	beatTime_file.close();
	if(BPM_file.is_open())
		BPM_file >> beatPerMinute;
	BPM_file.close();
}

double Song::giveBPM(){
	return beatPerMinute;
}

double Song::giveBeatTime(int beatNumber){
	return beatTime[beatNumber];
}

string Song::giveSongName(){
	return songName;
}