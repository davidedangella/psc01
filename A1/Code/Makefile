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
OUT_BINCONV = binconv

CFLAGS+=$(EXTRAFLAGS)

LIBPOS=libpos.a
AR = ar
ARFLAGS = rv

SRCS = initialization.c compute_solution.c finalization.c util_read_files.c util_write_files.c vol2mesh.c
OBJS =  $(addsuffix .o, $(basename $(SRCS)))

all: gccg binconv

%.o: s%.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

gccg: gccg.c $(LIBPOS)
	$(CC) -o $(OUT) $^ $(CFLAGS) $(LIBS)

binconv:  binconv.c
	$(CC) -o $(OUT_BINCONV) $(CFLAGS) $^

$(LIBPOS) : $(OBJS)
	$(AR) $(ARFLAGS) $@ $+

clean:
	rm -rf *.o gccg binconv $(LIBPOS)



