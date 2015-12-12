#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <algorithm>
#include <cstdio>
#include <limits>
#include <cctype>
#include "Ngram.h"

using namespace std;

const LogP NEG_INF = numeric_limits<float>::infinity() * (-1);

typedef unsigned short Big5;

class DisambigModel
{
public:
	DisambigModel(string mapFile,string lmFile, unsigned int numOrder){
		loadMap(mapFile);
		_numOrder = numOrder;
		loadLM(lmFile);
	}
	~DisambigModel(){
		delete _lm;
	}

	// I/O
	void loadSeg(string segFile);
	void loadMap(string mapFile);
	void loadLM(string lmfile);
	void writeAns();

	//viterbi
	vector<Big5> viterbi(vector<Big5> seg);

	//debug
	void verbose();

private:
	vector< vector<Big5> > _sentences;
	map<Big5, vector<Big5> > _ZhuYinMap;
	unsigned int _numOrder;
	
	Vocab _voc;
	Ngram* _lm;

	// I/O
	void writeSentence(vector<Big5>);

	//util
	VocabIndex Big5toIndex(Big5 word);
	LogP getBigramProb(VocabIndex wid1,VocabIndex wid2);

};

class Delta
{
public:
	friend class DisambigModel;
	Delta(){
		prevWordPtr = NULL;
		prob = NEG_INF;
		word = 0;
	}
	bool operator<(const Delta& rhs){return (this->prob < rhs.prob);}
private:
	Big5 word;
	Delta* prevWordPtr;
	LogP prob;
};
