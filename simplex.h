/******************************************************************************
			simplex.h  - description
			----------------------
	               L.G. Casado leo@ual.es. 2014. GPLv3.
	
     ********************************************************************	
     Purpose: Define the container simplex. 
     
     Stores:
     	NSimplex 	: Simplex's number; for drawing purposes.
	NVertex         : How many vertices are stored. It will be always
	                  equal to NDim.
        BTreeLevel	: Level of the Binary Tree where simplex is stored.
	ppV      	: Vector of vertexes the simplex consist of.
	pDistV          : Vector with distances between vertexes.
	pMaxDV      	: Vector with the max distance of each vertex to tohers.
	NVertex      	: Number of vertexes in this simplex.
	V1,V2		: Indexes of ppV, of vertexes with longest edge. 
	Size		: Size of the longest edge of the simplex.
	SQ		: Quality of the simplex.
	
    Functions:
    	
    	NewSimplex() : Creates a new simplex. Allocates memory an initialice 
		the values.
	FreeSimplex() : Free the memory allocates by simplex. 
	PrintSimplex(): Print the contents of the simplex.	
	DelSimplex(): Delete the simplex from the graphical output.	
	SizeSimplex(): Return the size of the longest edge of the simplex.
	SizeOfSimplex(): Return the bytes used by a Simplex.
	SumCostSimplex(): Adds the cost of all vertexes of the simplex.
	GetMaxDistToOtherV():Calculate the max distance of one Vertex to 
		others in the Simplex.	
		
*******************************************************************************/
#ifndef __SIMPLEX__
#define __SIMPLEX__


/*---------------------------------------------------------------------------*/
struct typesimplex
       {
        UCHAR  		NVertex;
	INT		BTreeLevel;
	LLINT 		NSimplex;
	PPVERTEX	ppV;
	PREAL		pDistV;
	PREAL		pMaxDV;
	REAL 		Size;
	REAL		SQ;
       };

typedef struct 	typesimplex 	SIMPLEX;
typedef SIMPLEX *         	PSIMPLEX; 
typedef PSIMPLEX *         	PPSIMPLEX;


/*---------------------------------------------------------------------------*/

PSIMPLEX NewSimplex (INT NDim, INT NVertex, PPREAL ppCoor, INT NSimplex);
PSIMPLEX FreeSimplex  (PSIMPLEX pS, INT NDim);
VOID     PrintSimplex (PSIMPLEX pS, INT NDim);
VOID     DrawSimplex  (PSIMPLEX pS, INT WWidth, PCHAR Color); 
VOID     DelSimplex   (PSIMPLEX pS);
REAL 	 SizeSimplex  (PSIMPLEX pS, INT NDim);
VOID     DivideSimplex(PPSIMPLEX ppS, PPSIMPLEX ppS1, INT NDim);
INT 	BytesBySimplex(INT NDim); 

#endif /*__SIMPLEX__*/
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
