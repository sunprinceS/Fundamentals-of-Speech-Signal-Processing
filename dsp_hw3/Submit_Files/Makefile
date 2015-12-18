# The following two variable will be commandline determined by TA
# For testing, you could uncomment them.
SRIPATH ?= /home/sunprinces/srilm-1.5.10
MACHINE_TYPE ?= i686-m64
LM ?= bigram.lm
SRIBIN=$(SRIPATH)/bin/$(MACHINE_TYPE)

CXX = g++
#CXXFLAGS = -g -std=c++11 -I$(SRIPATH)/include
CXXFLAGS = -O3 -std=c++11 -I$(SRIPATH)/include -w
vpath lib%.a $(SRIPATH)/lib/$(MACHINE_TYPE)

EXEC= mydisambig
CSRCS=$(wildcard *.cpp)
COBJS  = $(addsuffix .o, $(basename $(CSRCS)))
CNT = $(wildcard *.cnt)
SEG = $(wildcard testdata/*.seg *_seg.txt)
LM_FILE = $(wildcard *.lm)

TO = ZhuYin-Big5.map
FROM = Big5-ZhuYin.map
.PHONY: all clean map cleanAll run srilm

all: $(EXEC)
	@echo "Finish!"

mydisambig : $(COBJS) -loolm -ldstruct -lmisc
	@echo "> Building $@"
	$(CXX) $(LDFLAGS) $^ -o $@

%.o: %.cpp
	@echo "> Compiling $<"
	$(CXX) $(CXXFLAGS) -c $< -o $@

preprocess: corpus.txt separator_big5.pl
	@echo "Preprocessing..."
	@echo "================"
	@echo "<1> seperate corpus"
	perl separator_big5.pl corpus.txt > corpus_seg.txt
	@echo "Done!"
	@echo "<2> Building Bi-gram"
	$(SRIBIN)/ngram-count -text corpus_seg.txt -write lm2.cnt -order 2
	$(SRIBIN)/ngram-count -read lm2.cnt -lm bigram.lm -unk -order 2
	@echo "Done!"
	@echo "<3> Building Tri-gram"
	$(SRIBIN)/ngram-count -text corpus_seg.txt -write lm3.cnt -order 3
	$(SRIBIN)/ngram-count -read lm3.cnt -lm trigram.lm -unk -order 3
	@echo "Done!"
	@echo "<4> Segment testdata"
	@for i in $(shell seq 1 10) ; do \
		perl separator_big5.pl testdata/$$i.txt > testdata/$$i.seg ; \
	done;
	@echo "Preprocess finish!"
	@echo "================"
	

run: bigram.lm trigram.lm corpus_seg.txt $(TO) $(EXEC)
	@for i in $(shell seq 1 10) ; do \
		echo "Running $$i.seg"; \
		./mydisambig testdata/$$i.seg $(TO) $(LM) 2 > result2/$$i.txt; \
	done;
srilm_dis: bigram.lm trigram.lm corpus_seg.txt $(TO)
	@for i in $(shell seq 1 10) ; do \
		echo "Running $$i.seg"; \
		$(SRIBIN)/disambig -text testdata/$$i.seg -map $(TO) -lm $(LM) -order 2 > result1/$$i.txt; \
	done;

map: $(FROM) mapping
	@echo "Mapping!"
	@python2 mapping.py $(FROM) $(TO)

clean:
	$(RM) $(COBJS) $(EXEC)
cleanAll:
	$(RM) $(COBJS) $(EXEC)
	$(RM) $(LM_FILE) $(CNT) $(SEG) $(TO)
