############# MAKEFILE FOR GenClass PROGRAM ################################
############################################################################
include ../Makefile.inc
INC = -I../include $(INCL)

OBJS = population.o cprogram.o symbol.o rule.o program.o doublestack.o fparser.o fpoptimizer.o  get_options.o classprogram.o main.o

all: genclass

genclass: $(OBJS)
	$(CXX) $(OBJS) $(LINK) $(CFLAGS) -o genclass
	mkdir -p ../bin
	cp genclass ../bin

%.o: %.cc
	$(CXX) -c $< $(INC) $(CFLAGS)  -o $@

clean:
	rm -f $(OBJS) *~ genclass
