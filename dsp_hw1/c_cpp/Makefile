.PHONY: all clean

CXX = g++

##Note: $^ all the dependency , $< the first dependency
#CFLAGS = -O3 -std=C++0x
CFLAGS = -g -Wall -std=c++0x
CSRCS     = $(wildcard *.cpp)
COBJS  = $(addsuffix .o, $(basename $(CSRCS)))
EXEC = test train

all: $(EXEC)
	@echo "Finish!"

train: train.o hmm.o
	@echo "> Building $@"
	@$(CXX) $(CFLAGS) $^ -o $@

test: test.o hmm.o
	@echo "> Building $@"
	@$(CXX) $(CFLAGS) $^ -o $@

%.o: %.cpp
	@echo "> compiling: $<"
	@$(CXX) $(CFLAGS) -c $< -o $@

clean:
	$(RM) $(COBJS)
	$(RM) $(EXEC)
