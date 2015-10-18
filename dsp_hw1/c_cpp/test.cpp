/****************************************************************************
  FileName     [test.cpp]
  PackageName  [ hmm ]
  Synopsis     [ predict the result sequence by pretrained model ]
  Author       [ Juei-Yang (Tony) Hsu ]
****************************************************************************/

#include <iostream>
#include <cstdlib>
#include <string>
#include <fstream>
#include <sstream>
#include <vector>
#include <algorithm>
#include <iterator>

#include "hmm.h"
using namespace std;
//#define DEBUG

int main(int argc, const char *argv[]) {

#ifndef DEBUG
	bool bAccuracy=false;
	if(argc == 4 or argc == 5){
		string modelList(argv[1]);
		string testingData(argv[2]);
		string result(argv[3]);
		string accLogFile;
		vector<HMMmodel> HMMmodelList;
		vector<string> obsSeqs;
		vector<size_t> answers; // used in accuracy estimation

		ifstream modelNames;
		ifstream testData;
		ifstream answerFile; // used in accuracy estimation
		ofstream resultLog;

		string modelName;
		string obsSeq;

		size_t correctNum = 0;
		size_t numTimeFrame=0;

		if(argc == 5){
			accLogFile = string(argv[4]);
			bAccuracy = true;
		}

		//loadModel
		modelNames.open(modelList);
		while(getline(modelNames,modelName)){
			HMMmodelList.push_back(HMMmodel(modelName));
		}
		modelNames.close();

		//load sequences
		testData.open(testingData);
		while(getline(testData,obsSeq)){
			obsSeqs.push_back(obsSeq);
		}
		testData.close();
		if(obsSeqs.size() !=0){
			numTimeFrame = obsSeqs[0].length();
		}
		else{
			cerr << "No training data when training...\nleaving";
			exit(1);
		}

		if(bAccuracy){
			//load answer
			string answer;
			answerFile.open(string("../testing_answer.txt"));

			while(getline(answerFile,answer)){
				for(size_t i=1;i<=HMMmodelList.size();++i){
					if(answer == (string("model_0") + char(i+'0') + string(".txt"))){
						answers.push_back(i);
						break;
					}
				}
			}
			answerFile.close();
		}


		for(auto it=HMMmodelList.begin();it!=HMMmodelList.end();++it){
			it->testInit(numTimeFrame);
		}

		//record result <modelName> <prob.>
		resultLog.open(result);
		for(size_t id=0;id<obsSeqs.size();++id){
			vector<double> probs;
			probs.resize(HMMmodelList.size());
			for(size_t i=0;i<HMMmodelList.size();++i){
				probs[i] = HMMmodelList[i].viterbi(obsSeqs[id]);
			}
			size_t argmax = (distance(probs.begin(),max_element(probs.begin(),probs.end())));
				resultLog << "model_0" << char(argmax + '1') << ".txt" << "   " << probs[argmax] << endl;
			if(bAccuracy){ //record accuracy
				if( (argmax+1) == answers[id])
					++correctNum;
			}
		}
		resultLog.close();

		//record accuracy
		if(bAccuracy){
			ofstream accLog;
			accLog.open(accLogFile);
			accLog << ((double)correctNum / answers.size()) << endl;
			accLog.close();
		}
	}
	else{
		cerr << "Usage : ./test <modelList> <testingData> <result> []" << endl;
		exit(1);
	}
#endif
#ifdef DEBUG
		string modelList("modellist.txt");
		string testingData("../testing_data1.txt");
		string result("DEBUG_RESULT");
		string accLogFile("DEBUG_ACCLOG");
		vector<HMMmodel> HMMmodelList;
		vector<string> obsSeqs;
		vector<size_t> answers; // used in accuracy estimation

		ifstream modelNames;
		ifstream testData;
		ifstream answerFile; // used in accuracy estimation
		ofstream resultLog;

		string modelName;
		string obsSeq;

		size_t correctNum = 0;
		size_t numTimeFrame=0;


		//loadModel
		modelNames.open(modelList);
		while(getline(modelNames,modelName)){
			//auto model = HMMmodel(modelName);
			//HMMmodelList.push_back(model);
			HMMmodelList.push_back(HMMmodel(modelName));
		}
		modelNames.close();
		//
		//load sequences
		testData.open(testingData);
		while(getline(testData,obsSeq)){
			obsSeqs.push_back(obsSeq);
		}
		testData.close();
		if(obsSeqs.size() !=0){
			numTimeFrame = obsSeqs[0].length();
		}
		else{
			cerr << "No training data when training...\nleaving";
			exit(1);
		}

			//load answer
			string answer;
			answerFile.open(string("../testing_answer.txt"));

			while(getline(answerFile,answer)){
				for(size_t i=1;i<=HMMmodelList.size();++i){
					if(answer == (string("model_0") + char(i+'0') + string(".txt"))){
						answers.push_back(i);
						break;
					}
				}
			}
			answerFile.close();

		for(auto it=HMMmodelList.begin();it!=HMMmodelList.end();++it){
			it->testInit(numTimeFrame);
		}

		//record result <modelName> <prob.>
		resultLog.open(result);
		for(size_t id=0;id<obsSeqs.size();++id){
			vector<double> probs;
			probs.resize(HMMmodelList.size());
			for(size_t i=0;i<HMMmodelList.size();++i){
				probs[i] = HMMmodelList[i].viterbi(obsSeqs[id]);
			}
			size_t argmax = (distance(probs.begin(),max_element(probs.begin(),probs.end())));
				resultLog << "model_0" << char(argmax + '1') << ".txt" << "   " << probs[argmax] << endl;
			if( (argmax+1) == answers[id])
				++correctNum;
		}
		resultLog.close();

		//record accuracy
		ofstream accLog;
		accLog.open(accLogFile);
		accLog << ((double)correctNum / answers.size()) << endl;
		accLog.close();
#endif
	return 0;
}
