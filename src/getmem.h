/******************************************************************************
 getmem.h  - description
 ----------------------
 L.G.Casado leo@ual.es. 2002. GPLv3.
 
 ********************************************************************	
 Purpose: Rutines to get and free memory.
 
 Routines:
 GetMem():   Get memory for a one dimensional vector. Where is a string
 which usualy contains the caller routine name to know where was
 the error.

 GetMem2D(): Get memory for a two dimensional vector using GetMem.
 Free2D(): Free memory allocated with GetMem2D.
 

 *******************************************************************************/
#ifndef __GETMEM__
#define __GETMEM__

/*----------------------------------------------------------------------------*/
void * GetMem(size_t Num, size_t Tam, char * Where);
void ** GetMem2D(int rows, int columns, int sizeofTipo, char * Where);
void *** GetMem3D(int nelem, int rows, int columns, int sizeofTipo, char * Where);
void Free3D(void *** h, int nelem, int rows);
void Free2D(void ** h, int rows);

#endif /*__GETMEM__*/
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
