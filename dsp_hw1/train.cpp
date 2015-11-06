/****************************************************************************
  FileName     [train.cpp]
  PackageName  [ hmm ]
  Synopsis     [ train the model by training sequence ]
  Author       [ Juei-Yang (Tony) Hsu ]
****************************************************************************/


#include <iostream>
#include <cstdlib>
#include <string>

#include "hmm.h"
using namespace std;
//#define DEBUG
int main(int argc, const char *argv[])
{
#ifndef DEBUG
	if  (argc != 5){
		cerr << "Usage : ./train <#iters> <preTrainedModel> <trainingData> <modelSave>" << endl;
		exit(1);
	}
	size_t numIters = atoi(argv[1]);
	string modelInitFileName(argv[2]);
	string trainingData(argv[3]);
	string saveModel(argv[4]);

	HMMmodel trainModel(modelInitFileName);
	trainModel.train(numIters,trainingData);
	trainModel.writeModel(saveModel);
#endif
#ifdef DEBUG
	cout << "DEBUG mode" << endl;
	size_t numIters = 2;
	string modelInitFileName(string("../model_init.txt"));
	string trainingData(string("../seq_model_01.txt"));
	string saveModel("BBB");

	HMMmodel trainModel(modelInitFileName);
	trainModel.train(numIters,trainingData);
	trainModel.writeModel(saveModel);
#endif
	return 0;
}
