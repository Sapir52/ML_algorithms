﻿#include "TrainingData.h"
#include <vector>
#include <iostream>
#include <cstdlib>
#include <cassert>
#include <cmath>
#include <fstream>
#include <sstream>

using namespace std;

struct Connection
{
	double weight;
	double deltaWeight;
};

class Neuron;

typedef vector<Neuron> Layer;

// ****************** class Neuron ******************
class Neuron
{
public:
	Neuron(unsigned numOutputs, unsigned myIndex);
	void setOutputVal(double val) { m_outputVal = val; }
	double getOutputVal(void) const { return m_outputVal; }
	void feedForward(const Layer &prevLayer);
	void calcOutputGradients(double targetVals);
	void calcHiddenGradients(const Layer &nextLayer);
	void updateInputWeights(Layer &prevLayer);
private:
	static double eta; // [0.0...1.0] overall net training rate
	static double alpha; // [0.0...n] multiplier of last weight change [momentum]
	static double transferFunction(double x);
	static double transferFunctionDerivative(double x);
	// randomWeight: 0 - 1
	static double randomWeight(void) { return rand() / double(RAND_MAX); }
	double sumDOW(const Layer &nextLayer) const;
	double m_outputVal;
	vector<Connection> m_outputWeights;
	unsigned m_myIndex;
	double m_gradient;
};

double Neuron::eta = 0.15; // overall net learning rate
double Neuron::alpha = 0.5; // momentum, multiplier of last deltaWeight, [0.0..n]


void Neuron::updateInputWeights(Layer &prevLayer)
{
	// The weights to be updated are in the Connection container
	// in the nuerons in the preceding layer
							// Previous layer

	for (unsigned n = 0; n < prevLayer.size(); ++n)
	{
		Neuron &neuron = prevLayer[n];
		double oldDeltaWeight = neuron.m_outputWeights[m_myIndex].deltaWeight;

		double newDeltaWeight =
			// Individual input, magnified by the gradient and train rate:
			eta * neuron.getOutputVal() * m_gradient
			// Also add momentum = a fraction of the previous delta weight
			+ alpha * oldDeltaWeight;
		neuron.m_outputWeights[m_myIndex].deltaWeight = newDeltaWeight;
		neuron.m_outputWeights[m_myIndex].weight += newDeltaWeight;
	}
}
double Neuron::sumDOW(const Layer &nextLayer) const
{
	double sum = 0.0;
	// Sum our contributions of the errors at the nodes we feed
	for (unsigned n = 0; n < nextLayer.size() - 1; ++n)
		sum += m_outputWeights[n].weight * nextLayer[n].m_gradient;
	return sum;
}

void Neuron::calcHiddenGradients(const Layer &nextLayer)
{
	double dow = sumDOW(nextLayer);
	m_gradient = dow * Neuron::transferFunctionDerivative(m_outputVal);
}
void Neuron::calcOutputGradients(double targetVals)
{
	double delta = targetVals - m_outputVal;
	m_gradient = delta * Neuron::transferFunctionDerivative(m_outputVal);
}

double Neuron::transferFunction(double x)
{
	// tanh - output range [-1.0..1.0]
	return tanh(x);
}

double Neuron::transferFunctionDerivative(double x)
{
	// tanh derivative
	return 1.0 - x * x;
}

void Neuron::feedForward(const Layer &prevLayer)
{
	double sum = 0.0;
	// Sum the previous layer's outputs (which are our inputs)
	// Include the bias node from the previous layer.
	for (unsigned n = 0; n < prevLayer.size(); ++n)
		sum += prevLayer[n].getOutputVal() * prevLayer[n].m_outputWeights[m_myIndex].weight;
	m_outputVal = Neuron::transferFunction(sum);
}

//Ctor of Neuron class
Neuron::Neuron(unsigned numOutputs, unsigned myIndex)
{
	for (unsigned c = 0; c < numOutputs; ++c) {
		m_outputWeights.push_back(Connection());
		m_outputWeights.back().weight = randomWeight();
	}

	m_myIndex = myIndex;
}
// ****************** class Net ******************
class Net
{
public:
	Net(const vector<unsigned> &topology);
	void feedForward(const vector<double> &inputVals);
	void backProp(const vector<double> &targetVals);
	void getResults(vector<double> &resultVals) const;
	double getRecentAverageError(void) const { return m_recentAverageError; }

private:
	vector<Layer> m_layers; //m_layers[layerNum][neuronNum]
	double m_error;
	double m_recentAverageError;
	static double m_recentAverageSmoothingFactor;
};

double Net::m_recentAverageSmoothingFactor = 100.0; // Number of training samples to average over

//Get the results of the network
void Net::getResults(vector<double> &resultVals) const
{
	resultVals.clear();
	for (unsigned n = 0; n < m_layers.back().size() - 1; ++n)
		resultVals.push_back(m_layers.back()[n].getOutputVal());
}

/* 
In machine learning, backpropagation is a widely used algorithm for training feedforward neural networks. 
Generalizations of backpropagation exist for other artificial neural networks, and for functions generally. 
*/
void Net::backProp(const std::vector<double> &targetVals)
{
	// Calculate overal net error (RMS of output neuron errors)
	Layer &outputLayer = m_layers.back();
	m_error = 0.0;
	for (unsigned n = 0; n < outputLayer.size() - 1; ++n)
	{
		double delta = targetVals[n] - outputLayer[n].getOutputVal();
		m_error += delta *delta;
	}
	m_error /= outputLayer.size() - 1; // get average error squared
	m_error = sqrt(m_error); // RMS

							 // Implement a recent average measurement:
	m_recentAverageError = (m_recentAverageError * m_recentAverageSmoothingFactor + m_error) / (m_recentAverageSmoothingFactor + 1.0);

	// Calculate output layer gradients
	for (unsigned n = 0; n < outputLayer.size() - 1; ++n)
		outputLayer[n].calcOutputGradients(targetVals[n]);

	// Calculate gradients on hidden layers
	for (unsigned layerNum = m_layers.size() - 2; layerNum > 0; --layerNum)
	{
		Layer &hiddenLayer = m_layers[layerNum];
		Layer &nextLayer = m_layers[layerNum + 1];
		for (unsigned n = 0; n < hiddenLayer.size(); ++n)
			hiddenLayer[n].calcHiddenGradients(nextLayer);
	}

	// For all layers from outputs to first hidden layer,
	// update connection weights
	for (unsigned layerNum = m_layers.size() - 1; layerNum > 0; --layerNum)
	{
		Layer &layer = m_layers[layerNum];
		Layer &prevLayer = m_layers[layerNum - 1];
		for (unsigned n = 0; n < layer.size() - 1; ++n)
			layer[n].updateInputWeights(prevLayer);
	}
}

// A feedforward neural network is an artificial neural network wherein connections between the nodes do not form a cycle.
void Net::feedForward(const vector<double> &inputVals)
{
	// Check the num of inputVals euqal to neuronnum expect bias
	assert(inputVals.size() == m_layers[0].size() - 1);

	// Assign {latch} the input values into the input neurons
	for (unsigned i = 0; i < inputVals.size(); ++i)
		m_layers[0][i].setOutputVal(inputVals[i]);

	// Forward propagate
	for (unsigned layerNum = 1; layerNum < m_layers.size(); ++layerNum) {
		Layer &prevLayer = m_layers[layerNum - 1];
		for (unsigned n = 0; n < m_layers[layerNum].size() - 1; ++n) {
			m_layers[layerNum][n].feedForward(prevLayer);
		}
	}
}

//Ctor of Net class
Net::Net(const vector<unsigned> &topology)
{
	unsigned numLayers = topology.size();
	for (unsigned layerNum = 0; layerNum < numLayers; ++layerNum) {
		m_layers.push_back(Layer());
		// numOutputs of layer[i] is the numInputs of layer[i+1]
		// numOutputs of last layer is 0
		unsigned numOutputs = layerNum == topology.size() - 1 ? 0 : topology[layerNum + 1];

		// We have made a new Layer, now fill it ith neurons, and
		// add a bias neuron to the layer:
		for (unsigned neuronNum = 0; neuronNum <= topology[layerNum]; ++neuronNum) {
			m_layers.back().push_back(Neuron(numOutputs, neuronNum));
			cout << "Mad a Neuron!" << endl;
		}

		// Force the bias node's output value to 1.0. It's the last neuron created above
		m_layers.back().back().setOutputVal(1.0);
	}
}

//Display the values ​​of a label
void showVectorVals(string label, vector<double> &v)
{
	cout << label << " ";
	for (unsigned i = 0; i < v.size(); ++i)
	{
		cout << v[i] << " ";
	}
	cout << endl;
}

// Main function
int main()
{
	TrainingData trainData("train.txt");
	vector<unsigned> topology;

	trainData.getTopology(topology);
	Net myNet(topology);

	vector<double> inputVals, targetVals, resultVals;
	int trainingPass = 0;
	while (!trainData.isEof())
	{
		++trainingPass;
		// Get new input data and feed it forward:
		if (trainData.getNextInputs(inputVals) != topology[0])
			break;
		else {
			cout << endl << "Pass" << trainingPass;
			showVectorVals(": Inputs :", inputVals);
			myNet.feedForward(inputVals);

			// Collect the net's actual results:
			myNet.getResults(resultVals);
			showVectorVals("Outputs:", resultVals);

			// Train the net what the outputs should have been:
			trainData.getTargetOutputs(targetVals);
			showVectorVals("Targets:", targetVals);
			assert(targetVals.size() == topology.back());

			myNet.backProp(targetVals);

			// Report how well the training is working, average over recnet
			cout << "Net recent average error: "
				<< myNet.getRecentAverageError() << endl;
		}
	}

}
