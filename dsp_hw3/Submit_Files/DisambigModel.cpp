#include <fstream>
#include "DisambigModel.h"
#include "util.h"
Big5 toBig5(uint8_t p1,uint8_t p2){
	return (((uint16_t)p1 << 8 | (uint16_t)p2));
}
void DisambigModel::loadMap(stting mapFile){
	ifstream zhuyinMap;
	string line;
	zhuyinMap.open(zhuyinMap);
	while(getline(zhuyinMap,line)){
		const char* bstr = line.c_str();
		while(isspace(*bstr)) ++bstr; //ignore leading space
		key = toBig5(bstr[0],bstr[1]);
		bstr += 2
			vector<Big5> value;
		while(isspace(*bstr)) ++bstr; //ignore tab between word and spell
		for(size_t i=0;bstr+i!='\n';i+=3){ // 2+1 (one is space )
			value.push_back(toBig5(bstr[i],bstr[i+1]))
		}
		_ZhuYinMap[key] = value;
	}
}
void DisambigModel::loadLM(string lmFile){

}
void DisambigModel::loadSeg(string segFile){
	ifstream seg;
	string line;
	seg.open(segFile);
	size_t senttenceIdx = 0;
	while(getline(seg,line)){
		const char* bstr = line.c_str();
		while(isspace(*bstr)) ++bstr; //ignore leading space
		for(size_t i=0;bstr+i!='\n';i+=3){ // 2+1 (one is space)
			_sentence[senttenceIdx].push_back(toBig5(bstr[i],bstr[i+1]));
		}
		++senttenceIdx;
	}
	seg.close();
}
