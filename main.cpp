#include<iostream>
#include<fstream>
#include<sstream>
#include<vector>
#include<unordered_set>
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
	
	cout << "Accuracy with all " << choices.size() << " features is " << accuracy << endl;
	
	cout << "Using forward selection search ..." << endl;
	
	vector<int> testChoice, bestChoice;
	unordered_set<int> currSet, bestSet;
	int bestfeatToAdd;
	float bestAccuracy, bestAccThisPhase, currAcc;
	bestAccuracy = 0;
	for (int i = 0; i < choices.size(); ++i) {
		bestAccThisPhase = 0;
		bestfeatToAdd = 0;
		for (int j = 0; j < choices.size(); ++j) {
			if (currSet.count(j)) {
				continue;
			}
			GenAddedChoiceVector(currSet, j, choices.size(), testChoice);
			currAcc = EvalAccuracy(Data, testChoice);
			cout << "Using features {";
			printChoice(testChoice);
			cout << "} accuracy is " << currAcc << "." << endl;
			if (currAcc > bestAccThisPhase) {
				bestAccThisPhase = currAcc;
				bestfeatToAdd = j;
			}
		}
		currSet.insert(bestfeatToAdd);
		if (bestAccThisPhase > bestAccuracy) {
			bestAccuracy = bestAccThisPhase;
			bestSet = currSet;
		}
		cout << endl;
		cout << "Feature set {";
		GenChoiceVector(currSet, choices.size(), testChoice);
		printChoice(testChoice);
		cout << "} is best, accuracy is " << bestAccThisPhase << "." << endl;
		cout << endl;
	}
	
	GenChoiceVector(bestSet, choices.size(), bestChoice);
	cout << "Serach finished. Best choice of features is set {";
	printChoice(bestChoice);
	cout << "} with accuracy " << bestAccuracy << "." << endl;
}