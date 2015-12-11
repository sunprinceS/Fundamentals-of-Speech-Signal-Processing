#include <iostream>
#include <fstream>
#include <algorithm>
#include <vector>
#include <string>
#include <map>
#include <cstdlib>
#include "DisambigModel.h"

using namespace std;
#define DEBUG

int main(int argc, const char *argv[])
{
#ifdef DEBUG
	string segFile("testdata/1.seg");
	string mapFile("ZhuYin-Big5.map");
	string lmFile("bigram.lm");
	unsigned int numOrder = 2;
#endif
#ifndef DEBUG
	if(argc !=5 ){
		cerr << "Usage : ./mydisambig <SegFile> <Map> <LM> <#order>" << endl;
		exit(1);
	}
	string segFile(argv[1]);
	string mapFile(argv[2]);
	string lmFile(argv[3]);
	unsigned int numOrder = atoi(argv[4]);
#endif
	DisambigModel myModel(mapFile,lmFile,numOrder);
	myModel.loadSeg(segFile);

	myModel.writeAns();
	//myModel.verbose();
	return 0;
}
