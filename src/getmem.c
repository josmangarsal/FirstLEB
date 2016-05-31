/*L.G. Casado leo@ual.es. 2002. GPLv3.*/
/*---------------------------------------------------------------------------*/
#include <stdlib.h>
#include <stdio.h>
#include "getmem.h"

/*---------------------------------------------------------------------------*/
void * GetMem(size_t Num, size_t Tam, char * Where) {
	void * p;

	p = calloc(Num, Tam);
	if (p == NULL) {
		fprintf(stderr, "%s: Error in asigned memory.\n", Where);
		fflush(stderr);
		exit(1);
	}
	return p;
}

/*---------------------------------------------------------------------------*/
void ** GetMem2D(int rows, int columns, int sizeofTipo, char * Where) {
	int i;
	void ** h;

	/* memory assignament for pointers */
	h = (void **) GetMem((size_t) rows, (size_t) sizeof(void *), Where);

	for (i = 0; i < rows; i++)
		h[i] = (void *) GetMem((size_t) columns, (size_t) sizeofTipo, Where);
	return (h);
}

/*---------------------------------------------------------------------------*/
void *** GetMem3D(int nelem, int rows, int columns, int sizeofTipo, char * Where) {
	int i;
	void *** h;

	/* memory assignament for pointers */
	h = (void ***) GetMem((size_t) nelem, (size_t) sizeof(void *), Where);

	for (i = 0; i < nelem; i++)
		h[i] = (void **) GetMem2D(rows, columns, sizeofTipo, Where);
	return (h);
}

/*---------------------------------------------------------------------------*/
void Free3D(void *** h, int nelem, int rows) {
	int i;

	for (i = 0; i < nelem; i++)
		Free2D(h[i], rows);
	free((void *) h);
}

/*---------------------------------------------------------------------------*/
void Free2D(void ** h, int rows) {
	int i;

	for (i = 0; i < rows; i++)
		free((void *) h[i]);
	free((void *) h);
}

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
