#include<vector>
#include<limits>
#include<cmath>
#include<unordered_set>

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

float EvalAccuracy(vector<DataPoint>& Data, vector<int>& featureChoice) {
	int numCorrect = 0;
	float calculatedType;
	DataPoint pointLeftOut;
	vector<DataPoint>::iterator iter;
	for (iter = Data.begin(); iter < Data.end(); ++iter) {
		pointLeftOut = *iter;
		iter = Data.erase(iter);
		calculatedType = NearestNeighbor(Data, pointLeftOut, featureChoice);
		if (CloseEnough(calculatedType, pointLeftOut.classType)) {
			++numCorrect;
		}
		iter = Data.insert(iter, pointLeftOut);
	}
	//cout << "Calculation is " << numCorrect << " / " << Data.size() << endl;
	float accuracy = static_cast<float>(numCorrect) / static_cast<float>(Data.size());
	return accuracy;
}