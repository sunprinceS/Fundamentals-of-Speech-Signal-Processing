#include "DisambigModel.h"
//#include "util.h"
using namespace std;
typedef uint16_t Big5;

Big5 toBig5(uint8_t p1,uint8_t p2){
	return (((uint16_t)p1 << 8 | (uint16_t)p2));
}
void DisambigModel::loadMap(string mapFile){
	//ifstream zhuyinMap;
	//string line;
	//zhuyinMap.open(mapFile);
	//while(getline(zhuyinMap,line)){
		//const char* bstr = line.c_str();
		//while(isspace(*bstr)) ++bstr; //ignore leading space
		//Big5 key = toBig5(bstr[0],bstr[1]);
		//bstr += 2;
		//vector<Big5> value;
		//while(isspace(*bstr)) ++bstr; //ignore tab between word and spell
		//for(size_t i=0;bstr[i]!='\n';i+=3){ // 2+1 (one is space )
			//value.push_back(toBig5(bstr[i],bstr[i+1]));
		//}
		//_ZhuYinMap[key] = value;
	//}
}
void DisambigModel::loadLM(string lmFile){
}
void DisambigModel::loadSeg(string segFile){
	ifstream seg;
	string line;
	seg.open(segFile);

 	while(getline(seg,line)){
		const char* bstr = line.c_str();
		while(isspace(*bstr)) ++bstr; //ignore leading space
		vector<Big5> sentence;
		//for(size_t i=0;bstr[i]!=NULL;i+=3){
			//Big5 v = toBig5(bstr[i],bstr[i+1]);
			//sentence.push_back(v);
		//}
		//space is not equal distribute...= =
		while(*bstr != NULL){
			sentence.push_back(toBig5(bstr[0],bstr[1]));
			bstr +=2;
			while(isspace(*bstr)) ++bstr; //ignore trailing space
			
		}
		_sentences.push_back(sentence);
	}
	seg.close();
}
void DisambigModel::verbose(){
	cout << "SEGFILE" << endl;
	cout << "=======" << endl;
	for(size_t i=0;i<_sentences.size();++i){
		for(size_t j=0;j<_sentences[i].size();++j){
			Big5 tmp = _sentences[i][j];
			char str[3] = {0};
			str[0] = (char)(tmp >> 8);
			str[1] = (char)(tmp & 0xFF);
			cout << (const char*)str;
		}
		cout << endl;
	}
}
