CPP=g++
LD=g++
SPECIALFLAGS=-O3 -std=c++11 -stdlib=libc++
#SPECIALFLAGS=-g -O0
ROOTCFLAGS=$(shell root-config --cflags)
ROOTLIBS=$(shell root-config --libs)

CFLAGS = $(SPECIALFLAGS) -I.
LFLAGS = -L. 

RCXX=$(CFLAGS) $(ROOTCFLAGS)
RLXX=$(LFLAGS) $(ROOTLIBS)

SRC=d.cpp

all: $(SRC:.cpp=.o)
	$(LD) $(SRC:.cpp=.o) $(RLXX) -lMinuit -o dana

d.o: d.cpp
	$(CPP) $(RCXX) -c d.cpp

clean:
	rm -f *~ *.o
	rm -f dana
