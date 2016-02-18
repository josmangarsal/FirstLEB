DEBUG  = -g   
FLAGS  = -Wall -march=x86-64
#FLAGS  = -Wall -march=x86-64 -pg 
CC     = gcc

default : FirstLEB-VertCount

#-------------------------------------------------------------------------------
FirstLEB-VertCount : makefile FirstLEB-VertCount.c argshand.o getmem.o utils.o vertex.o \
	   btvertex.o simplex.o  btsimplex.o
	$(CC) $(DEBUG) $(FLAGS) -o FirstLEB-VertCount FirstLEB-VertCount.c \
	argshand.o getmem.o utils.o vertex.o \
	btvertex.o simplex.o btsimplex.o -L/usr/lib64 -lm

btsimplex.o : btsimplex.c btsimplex.h simplex.o
	$(CC) $(DEBUG) $(FLAGS) -c -o btsimplex.o btsimplex.c


simplex.o : simplex.c simplex.h vertex.o
	$(CC) $(DEBUG) $(FLAGS) -c -o simplex.o simplex.c

btvertex.o : btvertex.c btvertex.h 
	$(CC) $(DEBUG) $(FLAGS) -c -o btvertex.o btvertex.c

vertex.o : vertex.c vertex.h 
	$(CC) $(DEBUG) $(FLAGS) -c -o vertex.o vertex.c

utils.o : utils.c utils.h 
	$(CC) $(DEBUG) $(FLAGS) -c -o utils.o utils.c

getmem.o : getmem.c getmem.h 
	$(CC) $(DEBUG) $(FLAGS) -c -o getmem.o getmem.c

argshand.o : argshand.c argshand.h 
	$(CC) $(DEBUG) $(FLAGS) -c -o argshand.o argshand.c
	
#-------------------------------------------------------------------------------	
clean:
	rm -f *.o *~ *.bck *.bak FirstLEB-VertCount 

#-------------------------------------------------------------------------------
#-------------------------------------------------------------------------------
