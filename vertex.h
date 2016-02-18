/******************************************************************************
			vertex.h  - description
			----------------------
	              L.G. Casado leo@ual.es. 2014. GPLv3.

     ********************************************************************
     Purpose: Define the container vertex.

     Stores:
        NPoint: Identify the vertex for drawing purposes.
        pX : Coordinates of the vertex.

    Functions:
    	NewVertex() : Creates a new vertex. Allocates memory an initialice
		the values.

	FreeVertex() : Free the memory allocates by NewVertex.
	PrintVertex(): Print the contents of the Vertex.
	DrawVertex(): Generates a graphical output of the vertex with the
		      given color.
	DelBVertex(): Deletes the graphical output of the vertex.
	DistVertex(): Retun the distance between two vertexes.
	SizeOfVertex(): Return the bytes used by a vertex.


*******************************************************************************/
#ifndef __VERTEX__
#define __VERTEX__

/*---------------------------------------------------------------------------*/
struct typevertex
{
	INT   NVertex;
	PREAL pX;
};

typedef struct 	typevertex 	VERTEX;
typedef VERTEX 	*          	PVERTEX;
typedef PVERTEX *         	PPVERTEX;


/*---------------------------------------------------------------------------*/
PVERTEX GetMemVertex  (INT NDim);			 
PVERTEX	NewVertex     (INT NDim, PREAL Coor);
PVERTEX	FreeVertex    (PVERTEX pV);
PVERTEX CopyVertex    (PVERTEX pVO, INT NDim);
VOID	PrintVertex   (PVERTEX pV, INT NDim);
VOID    DrawVertex    (PVERTEX pV, INT WWidth, INT NDim, PCHAR color);
VOID	DelVertex     (PVERTEX pV, INT NDim);
INT     BytesByVertex (INT NDim);


#endif /*__VERTEX__*/
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
