#pragma once
#ifndef CLASS_TRAININGDATA_H
#define CLASS_TRAININGDATA_H
#include <vector>
#include <iostream>
#include <cstdlib>
#include <cassert>
#include <cmath>
#include <fstream>
#include <sstream>

using namespace std;
// ****************** class TrainingData ******************
class TrainingData
{
public:
	//Ctor TrainingData
	TrainingData(const string filename);

	// isEof- 1 if the end of the default input stream has been reached, 0 otherwise.
	bool isEof(void)
	{
		return m_trainingDataFile.eof();
	}
	// Get Topology values
	void getTopology(vector<unsigned> &topology);

	// Returns the number of input values read from the file:
	unsigned getNextInputs(vector<double> &inputVals);
	unsigned getTargetOutputs(vector<double> &targetOutputVals);

private:
	ifstream m_trainingDataFile;
};
#endif // CLASS_TRAININGDATA_H