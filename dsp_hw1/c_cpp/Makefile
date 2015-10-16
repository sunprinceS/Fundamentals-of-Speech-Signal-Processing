.PHONY: all clean

CXX = g++

##Note: $^ all the dependency , $< the first dependency
#CFLAGS = -O3 -std=C++0x
CFLAGS = -g -Wall -std=c++0x
CSRCS     = $(wildcard *.cpp)
COBJS  = $(addsuffix .o, $(basename $(CSRCS)))

all: train test

train: train.o hmm.o
	@$(CXX) $(CFLAGS) $^ -o $@
	@echo "> $@ is ready!"

test: test.o hmm.o
	@$(CXX) $(CFLAGS) $^ -o $@
	@echo "> $@ is ready!"

%.o: %.cpp
	@echo "> compiling: $<"
	@$(CXX) $(CFLAGS) -c $< -o $@

clean:
	@$(RM) $(COBJS)   # type make clean to remove the compiled file
