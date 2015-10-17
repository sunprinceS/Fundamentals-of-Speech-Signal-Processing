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

int main(int argc, const char *argv[]) {
	if(argc == 4 or argc == 5){

		string modelList(argv[1]);
		string testingData(argv[2]);
		string result(argv[3]);

		vector<HMMmodel> HMMmodelList;

		//loadModel
		ifstream modelNames;
		modelNames.open(modelList);
		string modelName;
		while(getline(modelNames,modelName)){
			HMMmodelList.push_back(HMMmodel(modelName));
		}
		modelNames.close();

		vector<string> obsSeqs;
		size_t correctNum = 0;
		vector<size_t> answers;
		ifstream testData;
		testData.open(testingData);
		ifstream answerFile;
		if(argc ==5){ //compare to answer get accuracy
			answerFile.open(string("../testinganswer.txt"));
			string answer;
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
		//init
		string obsSeq;
		size_t numTimeFrame=0;

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


		for(auto it=HMMmodelList.begin();it!=HMMmodelList.end();++it){
			it->testInit(numTimeFrame);
		}

		ofstream resultLog;
		resultLog.open(result);

		for(size_t id=0;id!=obsSeqs.size();++id){
			vector<double> probs;
			probs.resize(HMMmodelList.size());
			for(size_t i=0;i<HMMmodelList.size();++i){
				probs[i] = HMMmodelList[i].viterbi(obsSeqs[id]);
			}
			size_t argmax = (distance(probs.begin(),max_element(probs.begin(),probs.end())));
			resultLog << "model_0" << char(argmax + '1') << ".txt" << "   " << probs[argmax] << endl;
			if(argc == 5){ //record accuracy
				if( (argmax+1) == answers[id])
					++correctNum;
			}
		}
		resultLog.close();
		if(argc == 5){
			ofstream accLog;
			accLog.open(string(argv[4]));
			accLog << ((double)correctNum / answers.size()) << endl;
			accLog.close();
		}
	}
	else{
		cerr << "Usage : ./test <modelList> <testingData> <result>" << endl;
		exit(1);
	}
	return 0;
}
