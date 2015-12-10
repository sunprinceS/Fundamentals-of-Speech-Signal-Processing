# The following two variable will be commandline determined by TA
# For testing, you could uncomment them.
SRIPATH = /home/sunprinces/srilm-1.7.1
MACHINE_TYPE = i686-m64
LM = bigram.lm

CXX = g++
CXXFLAGS = -O3 -I$(SRIPATH)/include -w
vpath lib%.a $(SRIPATH)/lib/$(MACHINE_TYPE)

TARGET = mydisambig
SRC = mydisambig.cpp
OBJ = $(SRC:.cpp=.o)
TO = ZhuYin-Big5.map
FROM = Big5-ZhuYin.map
.PHONY: all clean map run

all: $(TARGET)

$(TARGET): $(OBJ) -loolm -ldstruct -lmisc
	$(CXX) $(LDFLAGS) -o $@ $^

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $<
run:
	@#TODO How to run your code toward different txt?
	@for i in $(shell seq 1 10) ; do \
		echo "Running $$i.txt"; \
		./mydisambig -text testdata/$$i.txt -map $(TO) -lm $(LM) -order 2 > result2/$$i.txt; \
	done;
map:
	@echo "Mapping!"
	iconv -f BIG5 -t UTF-8 $(FROM) -o $(FROM)_utf8
	@python2 mapping.py $(FROM)_utf8 $(TO)_utf8
	iconv -f UTF-8 -t BIG5 $(TO)_utf8 -o $(TO)
	@$(RM) $(FROM)_utf8 $(TO)_utf8 
clean:
	$(RM) $(OBJ) $(TARGET)

