/****************************************************************************
  FileName     [hmm.h]
  PackageName  [ hmm ]
  Synopsis     [ Basic HMM model ]
  Author       [ Juei-Yang (Tony) Hsu ]
****************************************************************************/

#ifndef HMM_H

#define HMM_H

#include <vector>
#include <string>

using namespace std;

class HMMmodel
{
public:
	HMMmodel(string fileName);
	~HMMmodel();

	//Basic I/O
	void loadModel(string fileName);
	void writeModel(string fileName);

	//Main
	void train(size_t numIters,string seqsFileName);
	void test();

	void viewParam();

private:
	/**data member**/
	vector<double> _initProb;
	vector< vector<double> > _transProb;
	vector< vector<double> > _emisProb;

	vector< vector<double> > _alpha;
	vector< vector<double> > _beta;
	vector< vector<double> > _gamma;
	vector< vector< vector<double> > >_eps;

	size_t _numState;
	size_t _obsLen;
	size_t _numObsType;


	//learing
	void BaumWeltch(string const& obsSeq);
	void update(size_t numObsSeqs,size_t numTimeFrame);

	//find best seq
	//viterbi();
	
	/**helper function**/
	void resetParam(size_t obsSeqLen);
};

#endif /* end of include guard: HMM_H */
