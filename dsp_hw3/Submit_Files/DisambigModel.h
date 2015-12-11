#include <locale>
typedef uint16_t Big5;
class DisambigModel
{
public:
	DisambigModel(string mapFile,string lmFile, size_t numOrder){
		setlocale(LC_ALL,"zh_TW.big5"); // WTF big5...= ="
		loadMap(mapFile);
		loadLM(lmFile);
		_numOrder = numOrder;
	}
	void loadSeg(string segFile);
private:
	void loadMap(string mapFile);
	void loadLM(string lmFile);
	vector<Big5> _lm;
	vector<vector<Big5> > _sentence;
	size_t _numOrder;
	map<Big5,vector<Big5> > _ZhuYinMap;
};
