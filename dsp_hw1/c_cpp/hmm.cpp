 /****************************************************************************
  FileName     [hmm.cpp]
  PackageName  [ hmm ]
  Synopsis     [Basic HMM model ]
  Author       [ Juei-Yang (Tony) Hsu ]
****************************************************************************/
#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <cstdlib>
#include <algorithm>
//#include <iterator>

#include "hmm.h"
using namespace std;

HMMmodel::HMMmodel(string fileName){
	loadModel(fileName);
}
HMMmodel::HMMmodel(const HMMmodel& rhs){
	*this = rhs;
}
HMMmodel::~HMMmodel(){};
void HMMmodel::loadModel(string fileName){
	cout << fileName << endl;
	ifstream model;
	model.open(fileName);
	string line;
	while(getline(model,line)){
		cout << line << endl;
		istringstream token(line);
		string paramType;
		size_t param;
		token >> paramType >> param;
		if (paramType =="initial:"){
			_numState = param;
			_initProb.resize(_numState);
			//get the value
			getline(model,line);
			istringstream token(line);
			for(size_t i=0;i<_numState;++i){
				token >> _initProb[i];
			}
		}
		else if(paramType=="transition:"){
			if(param != _numState){
				cerr << "Number of State is not consistent!\nleaving..." << endl;
				exit(1);
			}

			_transProb.resize(_numState);
			for(size_t i=0;i<_numState;++i)
				_transProb[i].resize(_numState);
			
			for(size_t i=0;i<_numState;++i){
				getline(model,line);
				istringstream token(line);
				for(size_t j=0;j<_numState;++j)
					token >> _transProb[i][j];
			}
		}
		else if(paramType=="observation:"){
			_numObsType = param; //??? Is it right?

			_emisProb.resize(_numObsType);
			for(size_t i=0;i<_numObsType;++i)
				_emisProb[i].resize(_numState);
			
			for(size_t i=0;i<_numObsType;++i){
				getline(model,line);
				istringstream token(line);
				for(size_t j=0;j<_numState;++j)
					token >> _emisProb[i][j];
			}
		}
		else{ // 空白行
			continue;
		}
	}
	model.close();
}

void HMMmodel::trainInit(size_t numTimeFrame){

	_AccumulateEmisProb.resize(_numObsType);
	for(size_t i=0;i<_numObsType;++i)
		_AccumulateEmisProb[i].resize(_numState);
	for(size_t i=0;i<_numObsType;++i){
		for(size_t j=0;j<_numState;++j)
			_AccumulateEmisProb[i][j] = 0;
	}

	_alpha.resize(numTimeFrame);
	for(size_t i=0;i<numTimeFrame;++i)
		_alpha[i].resize(_numState);

	_beta.resize(numTimeFrame);
	for(size_t i=0;i<numTimeFrame;++i)
		_beta[i].resize(_numState);

	_gamma.resize(numTimeFrame);
	for(size_t i=0;i<numTimeFrame;++i)
		_gamma[i].resize(_numState);

	_eps.resize(numTimeFrame-1);
	for(size_t i=0;i<numTimeFrame-1;++i){
		_eps[i].resize(_numState);
		for(size_t j=0;j<_numState;++j)
			_eps[i][j].resize(_numState);
	}
}

void HMMmodel::train(size_t numIters,string seqsFileName){
	ifstream trainData;
	vector<string> obsSeqs;
	string obsSeq;
	size_t numTimeFrame=0;

	trainData.open(seqsFileName);
	while(getline(trainData,obsSeq)){
		obsSeqs.push_back(obsSeq);
	}
	
	if(obsSeqs.size() !=0){
		numTimeFrame = obsSeqs[0].length();
	}
	else{
		cerr << "No training data when training...\nleaving";
		exit(1);
	}
	

	for(size_t iter=0;iter<numIters;++iter){
		trainInit(numTimeFrame);
		for(auto it=obsSeqs.begin();it!=obsSeqs.end();++it){
			BaumWeltch(*it);
			resetParam(it->length());
		}
		update(obsSeqs.size(),numTimeFrame);
		//viewParam();
		normalizeProb();
	}

	trainData.close();
}
void HMMmodel::testInit(size_t numTimeFrame){
	_delta.resize(numTimeFrame);
	for(size_t i=0;i<numTimeFrame;++i)
		_delta[i].resize(_numState);
}

double HMMmodel::viterbi(string const& obsSeq){
	//calculate delta
	
	//base case
	for(size_t state=0;state<_numState;++state){
		//_delta[0][state].prob = _initProb[state] * _emisProb[obsSeq[0]-'A'][state]
		_delta[0][state] = _initProb[state] * _emisProb[obsSeq[0]-'A'][state];
	}

	//recursive case
	 for(size_t time=1;time<obsSeq.length();++time){
		for(size_t state=0;state<_numState;++state){
			vector<double> probs;
			probs.resize(_numState);

			for(size_t fromState;fromState<_numState;++fromState){
				probs[fromState] = (_delta[time-1][fromState]*_transProb[state][fromState]);
			}
			//_delta[time][state].prob = max(probs) * _emisProb[obsSeq[time]-'A'][state];
			_delta[time][state] = *(max_element(probs.begin(),probs.end())) * _emisProb[obsSeq[time]-'A'][state];
			//_delta[time][state].fromStatePtr = &(_delta[time-1][distance(probs.begin(),max_element(probs.begin(),probs.end()))])
		}
	}
	//Return the largest prob.
	return *(max_element(_delta[obsSeq.length()-1].begin(),_delta[obsSeq.length()-1].end()));
}

void HMMmodel::writeModel(string fileName)const{
	ofstream storage;
	storage.open(fileName);

	storage << "initial: " << _numState << endl;
 	for(size_t  i=0;i<_numState ;++i){
		storage << _initProb[i] << '\t';
	}
	storage << endl << endl;

	storage << "transition: " << _numState << endl;
	for(size_t i=0;i<_numState;++i){
		for(size_t j=0;j<_numState;++j)
			storage << _transProb[i][j] << '\t';
		storage << endl;
	}
	storage << endl;

	storage << "observation: " << _numObsType << endl;
 	for(size_t i=0;i<_numObsType;++i){
		for(size_t j=0;j<_numState;++j)
			storage << _emisProb[i][j] << '\t';
		storage << endl;
	}
	storage.close();

}

void HMMmodel::BaumWeltch(const string& obsSeq){
	//forward --> calculate alpha

	//base case
	for(size_t i=0;i<_numState;++i)
		_alpha[0][i] = _initProb[i];
	//recursive case
 	for(size_t timeFrame=1;timeFrame<obsSeq.length();++timeFrame){
		for(size_t desState=0;desState<_numState;++desState){
			for(size_t fromState=0;fromState<_numState;++fromState){
				_alpha[timeFrame][desState]+=_alpha[timeFrame-1][fromState] * _transProb[desState][fromState];
			}
			_alpha[timeFrame][desState] *= _emisProb[obsSeq[timeFrame]-'A'][desState];
		}
	}

	//backward --> calculate beta
	//
	//base case
	for(size_t i=0;i<_numState;++i)
		_beta[obsSeq.length()-1][i] = 1;

	//recursive case
	//!!!!!MUST USE INT!!!!!
	for(int timeFrame=obsSeq.length()-2;timeFrame>=0;--timeFrame){
		for(size_t fromState=0;fromState<_numState;++fromState){
			for(size_t desState=0;desState<_numState;++desState){
				_beta[timeFrame][fromState] += (_beta[timeFrame+1][desState] * _transProb[desState][fromState] * _emisProb[obsSeq[timeFrame+1]-'A'][desState]);
			}
		}
	}

	//Accumulate updating param : gamma and epsilon
	
	//find gamma
	for(size_t timeFrame=0;timeFrame<obsSeq.length();++timeFrame){
		double normFactor=0.0;
		for(size_t state=0;state<_numState;++state)
			normFactor +=(_alpha[timeFrame][state] * _beta[timeFrame][state]);
		
		for(size_t state=0;state<_numState;++state){
			_gamma[timeFrame][state].value = ((_alpha[timeFrame][state] * _beta[timeFrame][state]) / normFactor);
			_gamma[timeFrame][state].obs = obsSeq[timeFrame];
		}
	}

	//find epsilon
	for(size_t timeFrame=0;timeFrame<obsSeq.length()-1;++timeFrame){
		double normFactor=0.0;
		for(size_t from=0;from<_numState;++from){
			for(size_t to=0;to<_numState;++to){
				normFactor += (_alpha[timeFrame][from] * _transProb[to][from] * _emisProb[obsSeq[timeFrame+1]-'A'][to] * _beta[timeFrame+1][to]);
			}

			for(size_t from=0;from<_numState;++from){
				for(size_t to=0;to<_numState;++to){
					_eps[timeFrame][to][from] = (_alpha[timeFrame][from] * _transProb[to][from] * _emisProb[obsSeq[timeFrame+1]-'A'][to] * _beta[timeFrame+1][to]) / normFactor;
				}
			}
		}
	}

	//collect emis prob.
	for(size_t state=0;state<_numState;++state){
		double denominator = 0.0;
		double numerator[_numObsType];

		for(size_t i=0;i<_numObsType;++i)
			numerator[i] = 0.0;

		for(size_t time=0;time<obsSeq.length();++time){
			numerator[_gamma[time][state].obs-'A'] += _gamma[time][state].value;
			denominator += _gamma[time][state].value;
		}
		
		for(size_t obs=0;obs<_numObsType;++obs){
			_AccumulateEmisProb[obs][state] += (numerator[obs] / denominator);
		}
	}
}
void HMMmodel::update(size_t numObsSeqs,size_t numTimeFrame){
	//new initial prob.
	for(size_t  i=0;i<_numState;++i){
		_initProb[i] = (_gamma[0][i].value / numObsSeqs);
	}

	//new transition prob.
	for(size_t from= 0;from<_numState;++from){
		double denominator = 0.0;
		for(size_t timeFrame=0;timeFrame<numTimeFrame-1;++timeFrame){
			denominator += _gamma[timeFrame][from].value;
		}
		for(size_t to=0;to<_numState;++to){
			double numerator = 0.0;
			for(size_t timeFrame=0;timeFrame<numTimeFrame-1;++timeFrame){
				numerator += _eps[timeFrame][to][from];
			}
			_transProb[to][from] = (numerator / denominator);
		}
	}

	//new emission prob.
	for(size_t obs=0;obs<_numObsType;++obs){
		for(size_t state=0;state<_numState;++state)
			_emisProb[obs][state] = _AccumulateEmisProb[obs][state] / numObsSeqs;
	}
}

void HMMmodel::normalizeProb(){

    //init prob
	double normFactor=0.0;
	for(size_t i=0;i<_numState;++i){
		normFactor += _initProb[i];
	}
	for(size_t i=0;i<_numState;++i){
		_initProb[i] /= normFactor;
	}

    //transition prob
	for(size_t from=0;from<_numState;++from){
		double normFactor=0.0;
		for(size_t to=0;to<_numState;++to){
			normFactor += _transProb[to][from];
		}

		for(size_t to=0;to<_numState;++to){
			_transProb[to][from] /= normFactor;
		}
	}

    //emission prob
	for(size_t state=0;state <_numObsType;++state){
		double normFactor=0.0;
		for(size_t obs=0;obs<_numObsType;++obs){
			normFactor += _emisProb[obs][state];
		}

		for(size_t obs=0;obs<_numObsType;++obs){
			_emisProb[obs][state] /= normFactor;
		}
	}
}

void HMMmodel::viewParam()const{
	
	cout << "Gamma" << endl;
	for(size_t t=0;t<_gamma.size();++t){
		for(size_t n=0;n<_numState;++n)
			cout << _gamma[t][n].value << '\t';
		cout << endl;
	}

	cout << "Eps" << endl;
	for(size_t t=0;t<_eps.size();++t){
		cout << "Time " << t << endl;
		for(size_t i=0;i<_numState;++i){
			for(size_t j=0;j<_numState;++j)
				cout << _eps[t][i][j] << '\t';
			cout << endl;
		}
		cout << endl;
	}
}
void HMMmodel::resetParam(size_t obsSeqLen){
	for(size_t t=0;t<obsSeqLen;++t){
		for(size_t state=0;state<_numState;++state){
			_alpha[t][state] = 0;
			_beta[t][state] = 0;
		}
	}
}
