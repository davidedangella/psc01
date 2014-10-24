####################################################################
#                                                                  #
#             Makefile for FIRE Solver Benchmarks                  #
#                                                                  #
####################################################################

CC = mpicc 
CFLAGS = -Wall $(PAPI_INC)
EXTRAFLAGS =
LIBS = -lm $(PAPI_LIB)
OUT = gccg

LIBPOS=libpos.a
AR = ar
ARFLAGS = rv

SRCS = initialization.c compute_solution.c finalization.c util_read_files.c util_write_files.c
OBJS =  $(addsuffix .o, $(basename $(SRCS)))

all: gccg 

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS) $(EXTRAFLAGS)

gccg: gccg.c $(LIBPOS)
	$(CC) -o $(OUT) $^ $(CFLAGS) $(EXTRAFLAGS) $(LIBS)

$(LIBPOS) : $(OBJS)
	$(AR) $(ARFLAGS) $@ $+

clean:
	rm -rf *.o gccg $(LIBPOS)
