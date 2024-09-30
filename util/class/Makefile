# define the object files (this must go first)
#
OBJ = mplpc_00.o mplpc_01.o mplpc_02.o mplpc_03.o mplpc_04.o
#mplpc_03.o mplpc_04.o
#mplpc_05.o mplpc_06.o mplpc_07.o

# define a dummy target (this must go next)
#
all: $(OBJ)

# define compilation, linking and archiving flags
#
CFLAGS += -O2 -c
#CFLAGS += -g -c
AR = gcc-ar rvs

# define dependencies
#
DEPS = Mplpc.h ../Edf/Edf.h ./Makefile

# define include files
#
INCLUDES = -I../../../include/ -I../../../pkgs/boost/ 

# define a target to make sure all source files are compiled
#
%.o: %.cc $(DEPS)
	g++ $(CFLAGS) $(INCLUDES) -o $@ $< 

# define a special target to install the code
#
install:
	cp Mplpc.h ../../../include/
	$(AR) ../../../lib/libdsp.a mplpc_??.o 

# define a special target to clean the directory
#
clean:
	rm -f mplpc_??.o

#
# end of file
