
# define compilation flags
#
#CFLAGS += -O2
CFLAGS += -g

# define source and object files
#
SRC = run_mplpc.cc
OBJ = run_mplpc.o

# define dependencies
#
DEPS = ../Edf/Edf.h ../Fe/Fe.h ../Cmdl/Cmdl.h\
	../lib/libdsp.a \
       ./Makefile

# define include files
#
INCLUDES=-I../include/ -I/boost/current/include/ 

# define a target for the application
#
all: run_mplpc

# define a target to link the application
#
run_mplpc: $(OBJ) $(DEPS)
	g++  -I../include/ $(CFLAGS) -o run_mplpc run_mplpc.o \
	-L../lib -ldsp \
	-lm

# define a target to compile the application
#
run_mplpc.o: $(SRC) $(DEPS)
	g++ $(CFLAGS) -c $(SRC) $(INCLUDES) -o $(OBJ)

# define an installation target
#
install:
	cp run_mplpc ./bin/

# define a target to clean the directory
#
clean:
	rm -f run_mplpc run_mplpc.o

#
# end of file
