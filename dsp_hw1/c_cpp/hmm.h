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

struct Gamma
{
	double value;
	char obs;
};

struct Delta
{
	size_t fromState;
	double prob;
};

class HMMmodel
{
public:
	HMMmodel(string fileName);
	~HMMmodel();

	//Basic I/O
	void loadModel(string fileName);
	void writeModel(string fileName)const;

	//Main
	void train(size_t numIters,string seqsFileName);
	void test(string resultFile,string seqsFileName,string accFileName=string("acc.txt"));

	void viewParam()const;

private:
	/**data member**/
	vector<double> _initProb;
	vector< vector<double> > _transProb;
	vector< vector<double> > _emisProb;

	vector< vector<double> > _alpha;
	vector< vector<double> > _beta;
	vector< vector<Gamma> > _gamma;
	vector< vector< vector<double> > >_eps;
	vector< vector<Delta> > _delta;

	size_t _numState;
	size_t _obsLen;
	size_t _numObsType;


	//learing
	void BaumWeltch(const string& obsSeq);
	void update(size_t numObsSeqs,size_t numTimeFrame);

	//find best seq
	double viterbi(const string& obsSeq);
	
	/**helper function**/
	void resetParam(size_t obsSeqLen);
	void normalizeProb();
};

#endif /* end of include guard: HMM_H */
