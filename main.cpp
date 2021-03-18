#include<iostream>
#include<fstream>
#include<sstream>
#include<vector>
#include "NearestNeighbor.h"

using namespace std;

int main(int argc, char* argv[]) {
	if (argc < 2) {
		cout << "Error: Must provide input file name" << endl;
		return 1;
	}
	
	fstream inputFile;
	inputFile.open(argv[1]);
	
	if (!inputFile.is_open()) {
		cout << "Error: Could not open file \"" << argv[1] << "\"" << endl;
		return 1;
	}
	
	vector<DataPoint> Data;
	DataPoint currPoint;
	float temp;
	string currLine;
	while(getline(inputFile, currLine)) {
		stringstream ss;
		ss << currLine;
		ss >> currPoint.classType;
		while (ss >> temp) {
			currPoint.features.push_back(temp);
		}
		Data.push_back(currPoint);
		currPoint.features.clear();
	}
	
	cout << "There are " << Data.size() << " data points with " << Data.at(0).features.size() << " choices of features." << endl;
	
	vector<int> choices;
	for (int i = 0; i < Data.at(0).features.size(); ++i) {
		choices.push_back(i);
	}

	float accuracy = EvalAccuracy(Data, choices);
	
	cout << "Accuracy with all features is " << accuracy << endl;
}