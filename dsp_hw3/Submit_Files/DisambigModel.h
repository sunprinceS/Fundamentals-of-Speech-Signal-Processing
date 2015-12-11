#include <iostream>
#include <fstream>
#include <locale>
#include <string>
#include <vector>
#include <map>
#include <cstdint>
#include <cctype>

using namespace std;


typedef uint16_t Big5;

class DisambigModel
{
public:
	DisambigModel(string mapFile,string lmFile, size_t numOrder){
		setlocale(LC_ALL,"zh_TW.BIG5"); // WTF big5...= ="
		loadMap(mapFile);
		loadLM(lmFile);
		_numOrder = numOrder;
	}
	void loadSeg(string segFile);
	void verbose(); //for debug
private:
	void loadMap(string mapFile);
	void loadLM(string lmFile);
	vector<Big5> _lm;
	vector< vector<Big5> > _sentences;
	size_t _numOrder;
	map<Big5, vector<Big5> > _ZhuYinMap;
};
