#pragma once

//standard libraries
#include <math.h>
#include <ctime>
#include <vector>
#include <fstream>
#include <sstream>
#include <wx/dir.h>
#include <wx/log.h>
#include "../header/DataEntry.h"

using namespace std;

//Constant Defaults!
#define LEARNING_RATE 0.001
#define MOMENTUM 0.9
#define MAX_EPOCHS 1500
#define DESIRED_ACCURACY 90  

/*******************************************************************************************************************
*	NEURAL NETWORK CLASS
*	----------------------------------------------------------------------------------------------------------------
*	Classic Back-propagation Neural Network ( makes use of gradient descent )
*	Can toggle between stochastic and batch learning
*	----------------------------------------------------------------------------------------------------------------
*	Author: Bobby Anguelov (banguelov@cs.up.ac.za)
*	Downloaded From: takinginitiative.wordpress.co.za
*******************************************************************************************************************/

class NeuralNetwork
{

	//private members
	//----------------------------------------------------------------------------------------------------------------
private:


	bool  bOnlyForUsage;
	//learning parameters
	double learningRate;					// adjusts the step size of the weight update	
	double momentum;						// improves performance of stochastic learning (don't use for batch)

											//number of neurons
	int nInput, nHidden, nOutput;

	//neurons
	double* inputNeurons;
	double* hiddenNeurons;
	double* outputNeurons;

	//weights
	double** wInputHidden;
	double** wHiddenOutput;

	//epoch counter
	long epoch;
	long maxEpochs;

	//accuracy required
	double desiredAccuracy;

	//change to weights
	double** deltaInputHidden;
	double** deltaHiddenOutput;

	//error gradients
	double* hiddenErrorGradients;
	double* outputErrorGradients;

	//accuracy stats per epoch
	double trainingSetAccuracy;
	double validationSetAccuracy;
	double generalizationSetAccuracy;
	double trainingSetMSE;
	double validationSetMSE;
	double generalizationSetMSE;

	//batch learning flag
	bool useBatch;

	//log file handle
	bool logResults;
	fstream logFile;
	int logResolution;
	int lastEpochLogged;

	//public methods
	//----------------------------------------------------------------------------------------------------------------
public:
	NeuralNetwork();
	NeuralNetwork(int in, int hidden, int out);
	~NeuralNetwork();

	bool ReadFromFile(const wxString& filePath);
	bool WriteToFile(const wxString& filePath);

	void SetLearningParameters(double lr, double m);

	void SetMaxEpochs(int max);
	void SetDesiredAccuracy(float d);
	void UseBatchLearning();
	void UseStochasticLearning();
	void EnableLogging(const char* filename, int resolution = 1);
	void ResetWeights();
	double* FeedInput(double* inputs);
	void TrainNetwork(vector<DataEntry*> trainingSet, vector<DataEntry*> generalizationSet, vector<DataEntry*> validationSet);
private:

	void InitializeWeights();
	void RunTrainingEpoch(vector<DataEntry*> trainingSet);
	void FeedForward(double *inputs);
	void Backpropagate(double* desiredValues);
	void UpdateWeights();
	inline double ActivationFunction(double x);
	inline double GetOutputErrorGradient(double desiredValue, double outputValue);
	double GetHiddenErrorGradient(int j);
	int GetRoundedOutputValue(double x);
	double GetSetAccuracy(vector<DataEntry*> set);
	double GetSetMSE(vector<DataEntry*> set);
};

