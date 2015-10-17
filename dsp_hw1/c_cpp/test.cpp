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

#include "hmm.h"
using namespace std;

int main(int argc, const char *argv[])
{
	if(argc ==4 or argc ==5){

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

		if(argc == 4){
			test();
		}
		else{
			test(string(argv[5]));
		}
	}
	else{
		cerr << "Usage : ./test <modelList> <testingData> <result>" << endl;
		exit(1);
	}
	return 0;

	
}
