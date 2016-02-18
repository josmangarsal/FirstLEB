/******************************************************************************
			btsimplex.h  - description
			----------------------
                    L.G. Casado leo@ual.es. 1997. GPLv3.

     ********************************************************************

     Purpose: Define a balance binary tree of simplexes sorted by their
              NSimpex and define the nodes of the balance binary tree.

     BTSNODE: Node of the balanced binary tree of simplexes:
     Stores:
     	pS: A simplex.
	BALANCETYPE: The balance of the subtree which has this node as root.
	pleft,pright: Pointers to the lest and right braches.

     BTS: Balanced Binary Tree of simplexes.
     Stores:
     	NElem: Number of Elemets in the tree.
	MaxNElem: Maximum number of vertexes ever in the tree.
	pFirstBTSNode: Pointer to the fisrt node in the tree.

    Functions:
	NewBTS: Gets memory for a BTS.
	PrintBTS: Print the BTS.
	InsertBTS: Insert the simplex pS in the BTS sorted by the SelectIndex.
        ExtratEqualBTS: Given a select NSimplex, look for and extract it.
        ExtractSmallerBTS: Extract the simplex with smaller NSimplex.
        ExtractGreaterBTS: Extract the simplex with greater NSimplex.
        GetSmallerNSimplex: Returns the smaller NSimplex index.
        GetGreaterNSimplex: Returns the greater NSimplex index.
        PrintStatBTS: Print BTS statistics.

*******************************************************************************/
#ifndef __BTS__
#define __BTS__

#include "btvertex.h"

#define LEFT  -1
#define EQUAL  0
#define RIGHT  1

/*---------------------------------------------------------------------------*/
struct btsnode
{
	CHAR Balance;
	PSIMPLEX pS;
	struct btsnode * pleft;
	struct btsnode * pright;
};

typedef struct btsnode BTSNODE;
typedef BTSNODE *      PBTSNODE;
typedef PBTSNODE *     PPBTSNODE;


/*---------------------------------------------------------------------------*/
struct BalancedTreeS
{
	int NElem;
	int MaxNElem;
	PBTSNODE pFirstBTSNode;
};

typedef struct BalancedTreeS BTS;
typedef BTS  *               PBTS;
typedef PBTS *               PPBTS;


/*---------------------------------------------------------------------------*/
PBTS NewBTS    			      (PBTS pBTS);
PBTS FreeBTS 			        (PBTS pbts, INT NDim);
VOID PrintBTS  			      (PBTS pbts, INT NDim);
VOID InsertBTS 			      (PBTS pbts, PSIMPLEX pS);
VOID ExtractEqualBTS      (PBTS pbts, INT NDim, INT NSimplex, PPSIMPLEX ppS);
VOID ExtractSmallerBTS		(PBTS pbts, INT NDim, PPSIMPLEX ppS);
VOID ExtractGreaterBTS		(PBTS pbts, INT NDim, PPSIMPLEX ppS);
INT  GetSmallerNSimplex		(PBTS pbts);
INT  GetGreaterNSimplex		(PBTS pbts);
VOID PrintStatBTS         (FILE * FOut, PBTS pbts, PCHAR String);
VOID checkVertices			  (PBTSNODE pNode, INT NDim, PBTV pbtvGridPoints);

#endif /*__BTS__*/
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
