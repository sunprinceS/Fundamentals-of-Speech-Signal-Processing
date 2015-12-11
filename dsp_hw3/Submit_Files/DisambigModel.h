#include <iostream>
#include <fstream>
#include <locale>
#include <cstdio>
#include <string>
#include <vector>
#include <map>
#include <algorithm>
#include <cstdint>
#include <cctype>
#include <limits>
#include "Ngram.h"

using namespace std;
const LogP NEG_INF = numeric_limits<float>::infinity() * (-1);
typedef uint16_t Big5;

class DisambigModel
{
public:
	DisambigModel(string mapFile,string lmFile, unsigned int numOrder){
		setlocale(LC_ALL,"zh_TW.BIG5"); // WTF big5...= ="
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
	VocabIndex Big5toIndex(Big5 word);
	//vector<Big5> _lm;
	unsigned int _numOrder;
	map<Big5, vector<Big5> > _ZhuYinMap;

	Vocab _voc;
	Ngram* _lm;

	LogP getBigramProb(VocabIndex wid1,VocabIndex wid2);
	// I/O
	void writeSentence(vector<Big5>);

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
