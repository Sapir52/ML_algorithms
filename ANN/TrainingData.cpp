#include "TrainingData.h"
#include <vector>
#include <iostream>
#include <cstdlib>
#include <cassert>
#include <cmath>
#include <fstream>
#include <sstream>

using namespace std;
void TrainingData::getTopology(vector<unsigned> &topology)
{
	string line;
	string label;
	//Get the first line of the file
	getline(m_trainingDataFile, line);
	cout << "line: " << line << "!\n";

	//Copy the line variable into the ss variable
	stringstream ss(line);
	//Copy the "topology:" into the label variable
	ss >> label;
	//If the file is not opened or label!="topology!" then abort()
	if (this->isEof() || label.compare("topology:") != 0)
	{
		abort();
	}
	//Read the rest of the entries from ss
	while (!ss.eof())
	{
		unsigned n;
		ss >> n;
		cout << "n: " << n << "!\n";
		topology.push_back(n);
	}
	return;
}

TrainingData::TrainingData(const string filename)
{
	// Inserting data from a file into the m_trainingDataFile variable
	m_trainingDataFile.open(filename.c_str());
}


unsigned TrainingData::getNextInputs(vector<double> &inputVals)
{
	inputVals.clear();

	string line;
	//Get the contents of the file into a line variable
	getline(m_trainingDataFile, line);
	//Copy the line variable into the ss variable
	stringstream ss(line);

	string label;
	//Copy the data into the label variable
	ss >> label;
	// If the line in the label(file) starts with "in:"
	if (label.compare("in:") == 0) {
		double oneValue;
		// Insert the contents of the line "in:" into a variable oneValue
		while (ss >> oneValue) {
			inputVals.push_back(oneValue);
		}
	}

	return inputVals.size();
}

unsigned TrainingData::getTargetOutputs(vector<double> &targetOutputVals)
{
	targetOutputVals.clear();

	string line;
	//Get the contents of the file into a line variable
	getline(m_trainingDataFile, line);
	//Copy the line variable into the ss variable
	stringstream ss(line);
	string label;
	//Copy the data into the label variable
	ss >> label;
	// If the line in the label(file) starts with "out:"
	if (label.compare("out:") == 0) {
		double oneValue;
		// Insert the contents of the line "out:" into a variable oneValue
		while (ss >> oneValue) {
			targetOutputVals.push_back(oneValue);
		}
	}

	return targetOutputVals.size();
}
