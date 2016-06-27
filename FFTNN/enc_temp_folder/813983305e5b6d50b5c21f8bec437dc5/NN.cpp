#include "../header/NN.h"

NeuralNetwork::NeuralNetwork()
	:
	bOnlyForUsage(true)
{

}

NeuralNetwork::NeuralNetwork(int in, int hidden, int out)
	:
	nInput(in), 
	nHidden(hidden),
	nOutput(out),
	epoch(0),
	logResults(false),
	logResolution(10),
	lastEpochLogged(-10),
	trainingSetAccuracy(0),
	validationSetAccuracy(0),
	generalizationSetAccuracy(0),
	trainingSetMSE(0),
	validationSetMSE(0),
	generalizationSetMSE(0),
	bOnlyForUsage(false)
{
	srand((unsigned int)time(0));
	//create neuron lists
	//--------------------------------------------------------------------------------------------------------
	inputNeurons.resize(in + 1);
	for (int i = 0; i < in; i++) inputNeurons[i] = 0;

	//create bias neuron
	inputNeurons[in] = -1;

	hiddenNeurons.resize(hidden + 1);
	for (int i = 0; i < hidden; i++) hiddenNeurons[i] = 0;

	//create bias neuron
	hiddenNeurons[hidden] = -1;

	outputNeurons.resize(out);
	for (int i = 0; i < out; i++) outputNeurons[i] = 0;

	//create weight lists (include bias neuron weights)
	//--------------------------------------------------------------------------------------------------------
	wInputHidden.resize(in + 1);
	for (int i = 0; i <= in; i++)
	{
		wInputHidden[i].resize(hidden);
		for (int j = 0; j < hidden; j++) 
			wInputHidden[i][j] = 0;
	}

	wHiddenOutput.resize(hidden + 1);
	for (int i = 0; i <= hidden; i++)
	{
		wHiddenOutput[i].resize(out);
		for (int j = 0; j < out; j++) 
			wHiddenOutput[i][j] = 0;
	}

	deltaInputHidden.resize(in + 1);
	for (int i = 0; i <= in; i++)
	{
		deltaInputHidden[i].resize(hidden);
		for (int j = 0; j < hidden; j++)
			deltaInputHidden[i][j] = 0;
	}

	deltaHiddenOutput.resize(hidden + 1);
	for (int i = 0; i <= hidden; i++)
	{
		deltaHiddenOutput[i].resize(out);
		for (int j = 0; j < out; j++)
			deltaHiddenOutput[i][j] = 0;
	}

	//create error gradient storage
	//--------------------------------------------------------------------------------------------------------
	hiddenErrorGradients.resize(hidden + 1);
	for (int i = 0; i <= hidden; i++)
		hiddenErrorGradients[i] = 0;
	 
	outputErrorGradients.resize(out + 1);
	for (int i = 0; i <= out; i++)
		outputErrorGradients[i] = 0;

	InitializeWeights();
	learningRate = LEARNING_RATE;
	momentum = MOMENTUM;

	//use stochastic learning by default
	useBatch = false;

	maxEpochs = MAX_EPOCHS;
	desiredAccuracy = DESIRED_ACCURACY;
}



bool NeuralNetwork::ReadFromFile(const wxString &filePath)
{

	std::ifstream is(filePath.c_str().AsChar(), std::ifstream::in | std::ifstream::binary);
	if (!is.is_open())
		return false;
	is >> nInput >> nHidden >> nOutput;

	inputNeurons.resize(nInput + 1);
	hiddenNeurons.resize(nHidden + 1);
	outputNeurons.resize(nOutput + 1);
	
	wInputHidden.resize(nInput+1);
	for (int i = 0; i <= nInput; i++)
	{
		wInputHidden[i].resize(nHidden);
		for (int j = 0; j < nHidden; j++)
			is >> wInputHidden[i][j];
	}

	wHiddenOutput.resize(nHidden+1);
	for (int i = 0; i <= nHidden; i++)
	{
		wHiddenOutput[i].resize(nOutput);
		for (int j = 0; j < nOutput; j++)
			is >> wHiddenOutput[i][j];
	}
	return true;
}

bool NeuralNetwork::WriteToFile(const wxString &filePath)
{
	std::ofstream ofs;
	ofs.open(filePath.c_str().AsChar(), std::ofstream::out);

	ofs << nInput << " " << nHidden << " " << nOutput << " ";

	for (int i = 0; i <= nInput; i++)
	{
		for (int j = 0; j < nHidden; j++)
			ofs << wInputHidden[i][j] << " ";
	}

	for (int i = 0; i <= nHidden; i++)
	{
		for (int j = 0; j < nOutput; j++)
			ofs << wHiddenOutput[i][j] << " ";
	}
	return true;
}




NeuralNetwork::~NeuralNetwork()
{
	if (logFile.is_open()) logFile.close();
}

void NeuralNetwork::SetLearningParameters(double lr, double m)
{
	learningRate = lr;
	momentum = m;
}

void NeuralNetwork::SetMaxEpochs(int max)
{
	maxEpochs = max;
}

void NeuralNetwork::SetDesiredAccuracy(float d)
{
	desiredAccuracy = d;
}

void NeuralNetwork::UseBatchLearning()
{
	useBatch = true;
}

void NeuralNetwork::UseStochasticLearning()
{
	useBatch = false;
}

void NeuralNetwork::EnableLogging(const char* filename, int resolution)
{
	//create log file 
	if (!logFile.is_open())
	{
		logFile.open(filename, ios::out);

		if (logFile.is_open())
		{
			//write log file header
			logFile << "Epoch,Training Set Accuracy, Generalization Set Accuracy,Training Set MSE, Generalization Set MSE" << endl;

			//enable logging
			logResults = true;

			//resolution setting;
			logResolution = resolution;
			lastEpochLogged = -resolution;
		}
	}
}

void NeuralNetwork::ResetWeights()
{
	InitializeWeights();
}

const vector<double>& NeuralNetwork::FeedInput(double* inputs)
{
	FeedForward(inputs);
	return outputNeurons;
}
 
void NeuralNetwork::TrainNetwork(vector<DataEntry*>& trainingSet,
								vector<DataEntry*>& generalizationSet, 
								vector<DataEntry*>& validationSet,
								vector<double>& trainingErrorHistory)
{

	epoch = 0;
	lastEpochLogged = -logResolution;
	trainingErrorHistory.push_back(1.);
	//vector<double> trainHistory;
	//vector<double> genHistory;
	generalizationSetMSE = 1.;
	while (((1. - generalizationSetMSE) * 100. < desiredAccuracy) && epoch < maxEpochs)
	{
		double previousTAccuracy = trainingSetAccuracy;
		double previousGAccuracy = generalizationSetAccuracy;

		RunTrainingEpoch(trainingSet);

		generalizationSetAccuracy = GetSetAccuracy(generalizationSet);
		generalizationSetMSE = GetSetMSE(generalizationSet);
	   if ((fabs(trainingErrorHistory[trainingErrorHistory.size() -1 ] - generalizationSetMSE) < 0.e-5)
			&& maxEpochs - epoch > 0.1 * maxEpochs)
			ResetWeights();
		trainingErrorHistory.push_back(generalizationSetMSE);
		//trainHistory.push_back(trainingSetAccuracy);
		//genHistory.push_back(generalizationSetAccuracy);
		epoch++;

	}
	 
	validationSetAccuracy = GetSetAccuracy(validationSet);
	validationSetMSE = GetSetMSE(validationSet);
}
 
void NeuralNetwork::InitializeWeights()
{
	for (int i = 0; i <= nInput; i++)
	{
		for (int j = 0; j < nHidden; j++)
		{
			wInputHidden[i][j] = (double)rand() / (RAND_MAX + 1) - 0.5;
			deltaInputHidden[i][j] = 0;
		}
	}

	for (int i = 0; i <= nHidden; i++)
	{
		for (int j = 0; j < nOutput; j++)
		{
			wHiddenOutput[i][j] = (double)rand() / (RAND_MAX + 1) - 0.5;
			deltaHiddenOutput[i][j] = 0;
		}
	}
}
 
void NeuralNetwork::RunTrainingEpoch(vector<DataEntry*>& trainingSet)
{
	double incorrectPatterns = 0;
	double mse = 0;
	for (int tp = 0; tp < (int)trainingSet.size(); tp++)
	{
		FeedForward(trainingSet[tp]->pattern);
		Backpropagate(trainingSet[tp]->target);

		bool patternCorrect = true;
		for (int k = 0; k < nOutput; k++)
		{
			if (fabs(outputNeurons[k] - trainingSet[tp]->target[k]) > 0.001)
				patternCorrect = false;
			mse += pow((outputNeurons[k] - trainingSet[tp]->target[k]), 2);
		}
		if (!patternCorrect) incorrectPatterns++;

	}//end for

	if (useBatch)
		UpdateWeights();
	trainingSetAccuracy = 100. - (incorrectPatterns / (double)trainingSet.size()) * 100.;
	trainingSetMSE = mse / (nOutput * trainingSet.size());
}

void NeuralNetwork::FeedForward(double *inputs)
{
	for (int i = 0; i < nInput; i++) 
		inputNeurons[i] = inputs[i];

	for (int j = 0; j < nHidden; j++)
	{
		hiddenNeurons[j] = 0;
		for (int i = 0; i <= nInput; i++)
			hiddenNeurons[j] += inputNeurons[i] * wInputHidden[i][j];
		hiddenNeurons[j] = ActivationFunction(hiddenNeurons[j]);
	}
	for (int k = 0; k < nOutput; k++)
	{
		outputNeurons[k] = 0;
		for (int j = 0; j <= nHidden; j++)
			outputNeurons[k] += hiddenNeurons[j] * wHiddenOutput[j][k];
		outputNeurons[k] = ActivationFunction(outputNeurons[k]);
	}
}
void NeuralNetwork::Backpropagate(double* desiredValues)
{
	for (int k = 0; k < nOutput; k++)
	{
		outputErrorGradients[k] = GetOutputErrorGradient(desiredValues[k], outputNeurons[k]);
		for (int j = 0; j <= nHidden; j++)
		{
			if (!useBatch)
				deltaHiddenOutput[j][k] = learningRate * hiddenNeurons[j] * outputErrorGradients[k] + momentum * deltaHiddenOutput[j][k];
			else
				deltaHiddenOutput[j][k] += learningRate * hiddenNeurons[j] * outputErrorGradients[k];
		}
	}
	for (int j = 0; j < nHidden; j++)
	{
		hiddenErrorGradients[j] = GetHiddenErrorGradient(j);
		for (int i = 0; i <= nInput; i++)
		{
			if (!useBatch)
				deltaInputHidden[i][j] = learningRate * inputNeurons[i] * hiddenErrorGradients[j] + momentum * deltaInputHidden[i][j];
			else
				deltaInputHidden[i][j] += learningRate * inputNeurons[i] * hiddenErrorGradients[j];

		}
	}
	if (!useBatch)
		UpdateWeights();
}

void NeuralNetwork::UpdateWeights()
{
	for (int i = 0; i <= nInput; i++)
	{
		for (int j = 0; j < nHidden; j++)
		{
			wInputHidden[i][j] += deltaInputHidden[i][j];
			if (useBatch)
				deltaInputHidden[i][j] = 0;
		}
	}

	//hidden -> output weights
	//--------------------------------------------------------------------------------------------------------
	for (int j = 0; j <= nHidden; j++)
	{
		for (int k = 0; k < nOutput; k++)
		{
			//update weight
			wHiddenOutput[j][k] += deltaHiddenOutput[j][k];

			//clear delta only if using batch (previous delta is needed for momentum)
			if (useBatch)
				deltaHiddenOutput[j][k] = 0;
		}
	}
}

double NeuralNetwork::ActivationFunction(double x)
{
	//sigmoid function
	return 1 / (1 + exp(-x));
}

double NeuralNetwork::GetOutputErrorGradient(double desiredValue, double outputValue)
{
	//return error gradient
	return outputValue * (1 - outputValue) * (desiredValue - outputValue);
}

double NeuralNetwork::GetHiddenErrorGradient(int j)
{
	//get sum of hidden->output weights * output error gradients
	double weightedSum = 0;
	for (int k = 0; k < nOutput; k++) weightedSum += wHiddenOutput[j][k] * outputErrorGradients[k];

	//return error gradient
	return hiddenNeurons[j] * (1 - hiddenNeurons[j]) * weightedSum;
}

int NeuralNetwork::GetRoundedOutputValue(double x)
{
	if (x < 0.1) return 0;
	else if (x > 0.9) return 1;
	else return -1;
}
double NeuralNetwork::GetSetAccuracy(vector<DataEntry*>& set)
{
	double incorrectResults = 0;
	for (int tp = 0; tp < (int)set.size(); tp++)
	{
		FeedForward(set[tp]->pattern);
		bool correctResult = true;
		for (int k = 0; k < nOutput; k++)
		{
			if (GetRoundedOutputValue(outputNeurons[k])
				!= set[tp]->target[k]) correctResult = false;
		}

		if (!correctResult)
			incorrectResults++;

	}//end for

		//calculate error and return as percentage
	return 100. - (incorrectResults / double(set.size()) * 100.);
}
double NeuralNetwork::GetSetMSE(vector<DataEntry*>& set)
{
	double mse = 0;
	for (int tp = 0; tp < (int)set.size(); tp++)
	{
		FeedForward(set[tp]->pattern);
		for (int k = 0; k < nOutput; k++)
		{
			mse += pow((outputNeurons[k] - set[tp]->target[k]), 2);
		}

	}//end for

	return mse / (nOutput * set.size());
}
