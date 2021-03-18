#include<vector>
#include<limits>
#include<cmath>
#include<unordered_set>
#include<iostream>

using namespace std;

struct DataPoint {
	float classType;
	vector<float> features;
};

bool CloseEnough(float A, float B) {
	return (abs(A - B) < 0.001);
}

float PointDistance (DataPoint& A, DataPoint& B, vector<int>& featureChoice) {
	float totalDist = 0;
	for (int i = 0; i < featureChoice.size(); ++i) {
		totalDist += pow(A.features.at(featureChoice.at(i)) - B.features.at(featureChoice.at(i)),2);
	}
	totalDist = sqrt(totalDist);
	return totalDist;
}

float NearestNeighbor(vector<DataPoint>& Data, DataPoint& toClassify, vector<int>& featureChoice) {
	float smallestDistance = numeric_limits<float>::max();
	int nearestPoint = -1;
	float currDist;
	for (int i = 0; i < Data.size(); ++i) {
		currDist = PointDistance(Data.at(i), toClassify, featureChoice);
		if (currDist < smallestDistance) {
			smallestDistance = currDist;
			nearestPoint = i;
		}
	}
	return Data.at(nearestPoint).classType;
}

float DefaultAccuracy(vector<DataPoint>& Data) {
	float defaultAcc;
	int numInClassOne = 0;
	for (int i = 0; i < Data.size(); ++i) {
		if (CloseEnough(Data.at(i).classType, 1)) {
			++numInClassOne;
		}
	}
	int numMajority;
	if (numInClassOne > (Data.size() / 2)) {
		numMajority = numInClassOne;
	}
	else {
		numMajority = Data.size() - numInClassOne;
	}
	defaultAcc = static_cast<float>(numMajority) / static_cast<float>(Data.size());
	return defaultAcc;
}

float EvalAccuracy(vector<DataPoint>& Data, vector<int>& featureChoice) {
	int numCorrect = 0;
	float calculatedType;
	DataPoint pointLeftOut;
	vector<DataPoint>::iterator iter;
	
	if (featureChoice.size() == 0) {
		return DefaultAccuracy(Data);
	}
	
	for (iter = Data.begin(); iter < Data.end(); ++iter) {
		pointLeftOut = *iter;
		iter = Data.erase(iter);
		calculatedType = NearestNeighbor(Data, pointLeftOut, featureChoice);
		if (CloseEnough(calculatedType, pointLeftOut.classType)) {
			++numCorrect;
		}
		iter = Data.insert(iter, pointLeftOut);
	}
	float accuracy = static_cast<float>(numCorrect) / static_cast<float>(Data.size());
	return accuracy;
}

void GenChoiceVector(unordered_set<int>& set, int numFeatures, vector<int>& choiceVec) {
	choiceVec.clear();
	for (int i = 0; i < numFeatures; ++i) {
		if (set.count(i)) {
			choiceVec.push_back(i);
		}
	}
}

void GenAddedChoiceVector(unordered_set<int>& set, int addedFeature, int numFeatures, vector<int>& choiceVec) {
	set.insert(addedFeature);
	GenChoiceVector(set, numFeatures, choiceVec);
	set.erase(addedFeature);
}

void GenRemovedChoiceVector(unordered_set<int>& set, int removedFeature, int numFeatures, vector<int>& choiceVec) {
	set.erase(removedFeature);
	GenChoiceVector(set, numFeatures, choiceVec);
	set.insert(removedFeature);
}

void printChoice(vector<int>& choice) {
	for (int i = 0; i < choice.size(); ++i) {
		cout << choice.at(i) + 1;
		if (i + 1 < choice.size()) {
			cout << ", ";
		}
	}
}