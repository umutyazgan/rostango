#include <string>
#include <fstream>
#include <vector>
//rotation files are seperated by semicolons. To read them I will replace semicolons with space characters

using namespace std;

void replaceSemicolons(string csvFileName){
	char c;
	char space = ' ';
	vector<int> semicolonLocations;
	vector<char> csvFile;
	int currentChar = 0;
	string fullPath = "C:\\Users\\7197\\Documents\\mocap_data\\" + csvFileName + ".txt";

	fstream rotationFile;
	rotationFile.open(fullPath);
	char skip;
	while (rotationFile >> noskipws >> c){		
		if (c != ';'){
			csvFile.push_back(c);			
		}
		else
			csvFile.push_back(' ');
		currentChar++;
	}
	rotationFile.close();
	string modified_fullPath = "C:\\Users\\7197\\Documents\\mocap_data\\" + csvFileName + "_noSC.txt";

	ofstream finalFile;
	finalFile.open(modified_fullPath);
	int scIndex = 0; //semicolon index
	for(int i= 0; i < csvFile.size(); i++)
		finalFile << csvFile[i];
	

	return;
}