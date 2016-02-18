DEBUG  = -g
FLAGS  = -Wall -march=x86-64
#FLAGS  = -Wall -march=x86-64 -pg
CC     = gcc

default : FirstLEB

#-------------------------------------------------------------------------------
FirstLEB : makefile FirstLEB.c argshand.o getmem.o utils.o vertex.o \
	   simplex.o  btsimplex.o gengridpoints.o Queue.o btvertex.o
	$(CC) $(DEBUG) $(FLAGS) -o FirstLEB FirstLEB.c \
	argshand.o getmem.o utils.o vertex.o \
	simplex.o btsimplex.o gengridpoints.o Queue.o btvertex.o -L/usr/lib64 -lm

btsimplex.o : btsimplex.c btsimplex.h simplex.o
	$(CC) $(DEBUG) $(FLAGS) -c -o btsimplex.o btsimplex.c


simplex.o : simplex.c simplex.h vertex.o
	$(CC) $(DEBUG) $(FLAGS) -c -o simplex.o simplex.c

vertex.o : vertex.c vertex.h
	$(CC) $(DEBUG) $(FLAGS) -c -o vertex.o vertex.c

gengridpoints.o : gengridpoints.c gengridpoints.h
	$(CC) $(DEBUG) $(FLAGS) -c -o gengridpoints.o gengridpoints.c

Queue.o : Queue.c Queue.h
	$(CC) $(DEBUG) $(FLAGS) -c -o Queue.o Queue.c

btvertex.o : btvertex.c btvertex.h
	$(CC) $(DEBUG) $(FLAGS) -c -o btvertex.o btvertex.c

utils.o : utils.c utils.h
	$(CC) $(DEBUG) $(FLAGS) -c -o utils.o utils.c

getmem.o : getmem.c getmem.h
	$(CC) $(DEBUG) $(FLAGS) -c -o getmem.o getmem.c

argshand.o : argshand.c argshand.h
	$(CC) $(DEBUG) $(FLAGS) -c -o argshand.o argshand.c

#-------------------------------------------------------------------------------
clean:
	rm -f *.o *~ *.bck *.bak FirstLEB

#-------------------------------------------------------------------------------
#-------------------------------------------------------------------------------
