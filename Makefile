# Linux settings.
MEX         = mex
MEXSUFFIX   = mexglx
MATLAB_HOME = /usr/local/MATLAB/R2012a
CXX         = g++
CC          = gcc
CFLAGS      = -O3 -fPIC -pthread -g  
FFLAGS      = -O3 -fPIC -g

TARGET = tn.$(MEXSUFFIX)
OBJS   = tnc.o matlabexception.o matlabscalar.o matlabstring.o   \
         matlabmatrix.o arrayofmatrices.o program.o matlabprogram.o \
         tn.o

CFLAGS += -Wall -ansi -DMATLAB_MEXFILE

all: $(TARGET)

%.o: %.cpp
	$(CXX) $(CFLAGS) -I$(MATLAB_HOME)/extern/include -o $@ -c $^

%.o: %.c
	$(CC) $(FFLAGS) -o $@ -c $^

$(TARGET): $(OBJS)
	$(MEX) -cxx CXX=$(CXX) CC=$(CXX) FC=$(FCC) LD=$(CXX) -lm \
        -O -output $@ $^

clean:
	rm -f *.o $(TARGET)

