/******************************************************************************
 vertex.c  - description
 ----------------------
 L.G. Casado leo@ual.es. 2014. GPLv3.
 *******************************************************************************/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <values.h>
#include "getmem.h"
#include "utils.h"
#include "vertex.h"

/*---------------------------------------------------------------------------*/
PVERTEX GetMemVertex(INT NDim)

{
	PVERTEX pV;

	pV = (PVERTEX) GetMem((SIZE) 1, (SIZE) sizeof(VERTEX), "GetMemVertex");
	pV->pX = (PREAL) GetMem((SIZE) NDim, (SIZE) sizeof(REAL), "GetMemVertex->pX");
	return pV;
}

/*---------------------------------------------------------------------------*/
PVERTEX NewVertex(INT NDim, PREAL pCoor) {

	static int NVertex = 0;

	PVERTEX pV;

	pV = GetMemVertex(NDim);

	pV->NVertex = NVertex++;

	CopyVR(pV->pX, pCoor, NDim);

	return pV;
}

/*---------------------------------------------------------------------------*/
PVERTEX FreeVertex(PVERTEX pV) {
	if (pV == NULL) {
		fputs("FreeVertex: Trying to free a null vertex pointer.", stderr);
		exit(1);
	} else {
		free((PVOID) pV->pX);
		free((PVOID) pV);
		return NULL;
	}
}

/*---------------------------------------------------------------------------*/
PVERTEX CopyVertex(PVERTEX pVO, INT NDim) {
	return NewVertex(NDim, pVO->pX);
}

/*---------------------------------------------------------------------------*/
VOID PrintVertex(PVERTEX pV, INT NDim) {
	fputs("X =\n", stderr);
	PrintVR(stderr, pV->pX, NDim);
	fprintf(stderr, "\tNVertex               = %d\n", pV->NVertex);

	fputs("\n", stderr);
}

/*---------------------------------------------------------------------------*/
VOID DrawVertex(PVERTEX pV, INT WWidth, INT NDim, PCHAR color) {
	if (PointInX123(pV->pX, NDim)) {
		printf("DrawPoint\n");

		printf("%f\n", XInWindow(pV->pX, WWidth));
		printf("%f\n", YInWindow(pV->pX, WWidth));

		printf("%s\n", color);

		printf("%d\n", pV->NVertex);
		fflush(stdout);
	}
}

/*---------------------------------------------------------------------------*/
VOID DelVertex(PVERTEX pV, INT NDim) {
	if (PointInX123(pV->pX, NDim)) {
		printf("DelPoint\n");
		printf("%d\n", pV->NVertex);
	}
}

/*---------------------------------------------------------------------------*/
INT BytesByVertex(INT NDim) {
	INT Total = 0;

	Total += sizeof(VERTEX);
	Total += NDim * sizeof(REAL);

	return Total;
}

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
