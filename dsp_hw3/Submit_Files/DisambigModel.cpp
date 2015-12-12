#include "DisambigModel.h"

using namespace std;

Big5 toBig5(unsigned char p1,unsigned char p2){
	return (((unsigned short)p1 << 8 | (unsigned short)p2));
}

void showBig5(Big5 word,char sep){
	char str[3] = {0};
	str[0] = (unsigned char)(word >> 8);
	str[1] = (unsigned char)(word & 0xFF);
	cout << (const char*)str << sep;
}

LogP DisambigModel::getBigramProb(VocabIndex wid1, VocabIndex wid2)
{
    if(wid1 == Vocab_None)  //OOV
        wid1 = _voc.getIndex(Vocab_Unknown);
    if(wid2 == Vocab_None)  //OOV
        wid2 = _voc.getIndex(Vocab_Unknown);

    VocabIndex context[] = { wid1, Vocab_None };
    return _lm->wordProb( wid2, context);
}

VocabIndex DisambigModel::Big5toIndex(Big5 word){
	char str[3] = {0};
	str[0] = (unsigned char)(word >> 8);
	str[1] = (unsigned char)(word & 0xFF);
	return _voc.getIndex(str);
}

vector<Big5> DisambigModel::viterbi(vector<Big5> seg){
	vector< vector<Delta> > deltas(seg.size());
	vector<Big5> possibleWords = _ZhuYinMap[seg[0]];
	vector<Delta> delta(possibleWords.size());
	vector<Big5> segAns(seg.size());

	//base case
	for(size_t j=0;j<possibleWords.size();++j){
		VocabIndex idx = Big5toIndex(possibleWords[j]);
		VocabIndex context[] = {Vocab_None};
		delta[j].word = possibleWords[j];
		if(idx == Vocab_None){
			continue;
		}
		else{
			delta[j].prob = _lm->wordProb(idx,context);
		}
	}
	deltas[0] = delta;
	//recursive case
	for(size_t i=1;i<seg.size();++i){
		possibleWords = _ZhuYinMap[seg[i]];
		delta.clear();
		delta.resize(possibleWords.size());
		size_t numFromWords = deltas[i-1].size();

		for(size_t j=0;j<possibleWords.size();++j){
			vector<LogP> probs(numFromWords);
			VocabIndex idx_now = Big5toIndex(possibleWords[j]);

			for(size_t k=0;k<numFromWords;++k){
				VocabIndex idx_prev = Big5toIndex(_ZhuYinMap[seg[i-1]][k]);

				probs[k] = deltas[i-1][k].prob + getBigramProb(idx_prev,idx_now);
			}
			//won't consider emission except first word
			delta[j].prob = *(max_element(probs.begin(),probs.end()));
			delta[j].word = possibleWords[j];
			delta[j].prevWordPtr = &(deltas[i-1][distance(probs.begin(),max_element(probs.begin(),probs.end()))]);
		}
		deltas[i] = delta;
	}
	Delta psi = *(max_element(deltas[seg.size()-1].begin(),deltas[seg.size()-1].end()));

	for(int i=seg.size()-1;i>=0;--i){
		segAns[i] = psi.word;
		if(psi.prevWordPtr!=NULL)
			psi = *(psi.prevWordPtr);
	}
	return segAns;
}

void DisambigModel::loadMap(string mapFile){
	ifstream zhuyinMap;
	string line;
	zhuyinMap.open(mapFile);

	while(getline(zhuyinMap,line)){
		const char* bstr = line.c_str();
		while(isspace(*bstr)) ++bstr; //ignore leading space
		Big5 key = toBig5(bstr[0],bstr[1]);
		bstr += 2;
		vector<Big5> value;
		while(isspace(*bstr)) ++bstr; //ignore tab between word and spell
		while(*bstr != NULL){
			value.push_back(toBig5(bstr[0],bstr[1]));
			bstr +=2;
			while(isspace(*bstr)) ++bstr; //ignore trailing space
		//for(size_t i=0;bstr[i]!='\n';i+=3){ // 2+1 (one is space )
			//value.push_back(toBig5(bstr[i],bstr[i+1]));
		//}
		_ZhuYinMap[key] = value;
		}
	}
	zhuyinMap.close();
}

void DisambigModel::loadLM(string lmfile){
	_lm = new Ngram(_voc,_numOrder);
	File lmFile(lmfile.c_str(),"r");
	_lm->read(lmFile);
	lmFile.close();
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

void DisambigModel::writeAns(){
	for(size_t i=0;i<_sentences.size();++i){
		writeSentence(viterbi(_sentences[i]));
	}
}
void DisambigModel::writeSentence(vector<Big5> ans){
	cout << Vocab_SentStart << ' ';
	for(size_t i=0;i<ans.size();++i){
		showBig5(ans[i],' ');
	}
	cout << Vocab_SentEnd << endl;
}
void DisambigModel::verbose(){
	cout << "SEGFILE" << endl;
	cout << "=======" << endl;
	for(size_t i=0;i<_sentences.size();++i){
		for(size_t j=0;j<_sentences[i].size();++j){
			showBig5(_sentences[i][j],' ');
		}
		cout << endl;
	}
	cout << "=======" << endl;
	cout << "MAPFILE" << endl;
	cout << "=======" << endl;
	for(map< Big5,vector<Big5> >::const_iterator it=_ZhuYinMap.begin();it!=_ZhuYinMap.end();++it){
		showBig5(it->first,'\t');
		for(size_t i=0;i<it->second.size();++i){
			showBig5((it->second)[i],' ');
		}
		cout << endl;
	}
	cout << "=======" << endl;
}
