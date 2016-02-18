/******************************************************************************
                        simplex.c  - description
                        ----------------------
                    L.G.Casado leo@ual.es. 2014. GPLv3.

*******************************************************************************/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include "getmem.h"
#include "utils.h"
#include "vertex.h"
#include "simplex.h"



/*---------------------------------------------------------------------------*/
REAL SizeSimplex(PSIMPLEX pS, INT NDim)
{
 int NumberOfDist[]={0,0,1,3,6,10,15,21,28,36,45,55,66,78,91,105,120};
 REAL Size;
 INT i;
 INT NDist;
 
 NDist = NumberOfDist[pS->NVertex];
 Size  = 0.0;

 for (i=0;i<NDist;i++)
     if (GE(pS->pDistV[i],Size))
         Size=pS->pDistV[i];
	 	 
 return Size;	 
}


/*---------------------------------------------------------------------------*/
PSIMPLEX NewSimplex (INT NDim, INT NVertex, PPREAL ppCoor, INT NSimplex)
{
 int NumberOfDist[]={0,0,1,3,6,10,15,21,28,36,45,55,66,78,91,105,120};
 PSIMPLEX pS;
 INT i,NDist;
 /*INT x,y;*/
 
 NDist=NumberOfDist[NVertex];

 pS         = (PSIMPLEX)GetMem((SIZE)1,      (SIZE)sizeof(SIMPLEX),
                              "NewSimplex");
 pS->ppV    = (PPVERTEX)GetMem((SIZE)NVertex,(SIZE)sizeof(PVERTEX),
                              "NewSimplex->ppV");
 pS->pDistV = (PREAL)   GetMem((SIZE)NDist,  (SIZE)sizeof(REAL),
                              "NewSimplex->pDistV");
 pS->pMaxDV = (PREAL)   GetMem((SIZE)NVertex,  (SIZE)sizeof(REAL),
                              "NewSimplex->pMaxDV");
			      
 pS->NVertex=NVertex;			      
 pS->NSimplex=NSimplex;
 pS->BTreeLevel=(int)log2((double)NSimplex);
 
 for (i=0;i<NVertex;i++)
      pS->ppV[i]=NewVertex(NDim,ppCoor[i]);
 
    
 /*NewSimplex is called only by GenInitials->all vertexes are at same dist.*/
 if (NSimplex==1)
    {
     pS->pDistV[0]=DistPoints(pS->ppV[0]->pX,pS->ppV[1]->pX,NDim);    
     for (i=1;i<NDist;i++)
	  pS->pDistV[i]=pS->pDistV[0];
    }
    
 /*Initiate the max distance from one vertex to others*/
 for (i=0;i<NVertex;i++)
     pS->pMaxDV[i]= pS->pDistV[0];
         	
 pS->Size=SizeSimplex(pS,NDim);

 /*HACER: funcio evaluar SQ*/  

 return pS;
}

/*---------------------------------------------------------------------------*/
PSIMPLEX FreeSimplex (PSIMPLEX pS, INT NDim)
{
 INT i;

 if (pS==NULL)
    {
     fputs("FreeSimplex:Trying to free a null simplex pointer.",stderr);
     exit(1);
    }
 else    
    {
     for (i=0;i<pS->NVertex;i++)
	 FreeVertex(pS->ppV[i]);
     free((PVOID)pS->pMaxDV);	 
     free((PVOID)pS->pDistV);	 
     free((PVOID)pS->ppV);	 
     free((PVOID)pS);
     return NULL;	 
    }
}

/*---------------------------------------------------------------------------*/
VOID PrintSimplex(PSIMPLEX pS, INT NDim) 
{
 INT i,x,y;
 INT NDist;
 int NumberOfDist[]={0,0,1,3,6,10,15,21,28,36,45,55,66,78,91,105,120};
 
 NDist = NumberOfDist[pS->NVertex];
  
 fputs("\n===============Simplex=============================\n",stderr);
 fprintf(stderr,"NSimplex	              = %lld\n",pS->NSimplex);
 fprintf(stderr,"Number of Vertexes           = %d\n",pS->NVertex);
 fprintf(stderr,"Size                         = %f\n",pS->Size);
 fprintf(stderr,"BTreeLevel                   = %d\n",pS->BTreeLevel);
 for (i=0;i<NDist;i++)
     {
      IndexToCombN2(pS->NVertex,i,&x,&y);
      fprintf(stderr,"Distance[%d,%d]=%f.\n",x,y,pS->pDistV[i]);
     } 
 for (i=0;i<pS->NVertex;i++)
     fprintf(stderr,"MaxDist[%d]=%f.\n",i,pS->pMaxDV[i]);
 
 for (i=0;i<pS->NVertex;i++)
     PrintVertex(pS->ppV[i],NDim); 
 
 fputs("\n",stderr);
 fflush(stderr);
}


/*---------------------------------------------------------------------------*/
VOID DrawSimplex(PSIMPLEX pS, INT WWidth, PCHAR Color) 
{
 INT i;
 
 if (pS==NULL)
    {
     fprintf(stderr,"DrawSimplex; Null pointer to a simplex\n");
     exit(1);
    }
 
 switch (pS->NVertex)
        {
	 case 1:
	      DrawVertex(pS->ppV[0],WWidth,Color);  
	      break; 
	 case 2:
	      printf("DrawLine\n");
	      break;
	 case 3:
	      printf("DrawTriangle\n");
	      break;
	 default:
	      fprintf(stderr,"DrawSimplex: pS->NVertex=%d.\n",pS->NVertex);
	      exit(1);
	      break;
	}
	
 if (pS->NVertex!=1)
    {	
     for (i=0;i<pS->NVertex;i++)
	 {
	  printf("%f\n",XInWindow(pS->ppV[i]->pX,WWidth));
	  printf("%f\n",YInWindow(pS->ppV[i]->pX,WWidth));
	 } 	      
     printf("%s\n",Color);
     printf("%lld\n",pS->NSimplex);
     fflush(stdout);

     for (i=0;i<pS->NVertex;i++)
	 DrawVertex(pS->ppV[i],WWidth,"Red");
    }	 
     
}

/*---------------------------------------------------------------------------*/
VOID DelSimplex(PSIMPLEX pS)
{
 INT i;
 
  if (pS==NULL)
    {
     fprintf(stderr,"DelSimplex; Null pointer to a simplex\n");
     exit(1);
    }
 
for (i=0;i<pS->NVertex;i++)
    DelVertex(pS->ppV[i]);
      
 switch (pS->NVertex)
        {
	 case 2:
	      printf("DelLine\n");
	      break;
	 case 3:
	      printf("DelTriangle\n");
	      break;
	}
 printf("%lld\n",pS->NSimplex);
 fflush(stdout);
}

/*---------------------------------------------------------------------------*/
INT EdgeToBisect(PSIMPLEX pS, PINT pV1, PINT pV2)
{
 /*V1,V2 are Indexes of ppV, of vertexes with longest edge and min-max cost.*/
 int NumberOfDist[]={0,0,1,3,6,10,15,21,28,36,45,55,66,78,91,105,120};
 INT i, x, y;
 INT NDist;
 BOOL Found=False;

 
 NDist=NumberOfDist[pS->NVertex];

 if (pS->NVertex==1)
    {
     *pV1=0;
     *pV2=0;
     return 0; 
    }
    
 if (pS->NVertex==2)
    {
     *pV1=0;
     *pV2=1;
     return 0;
    }   

 //FIRST Longest Edge
 for (i=0;i<NDist;i++)
     if (EQ(pS->pDistV[i],pS->Size))
        {	  
         IndexToCombN2(pS->NVertex,i,&x,&y);   			  
         *pV1=y;
         *pV2=x;
         Found=True;
         break;
        } 
 if (!Found)
    {
     fprintf(stderr,"EdgeToBisect not found.\n");
     exit(1);
    }

 return 1;    
}

/*---------------------------------------------------------------------------*/
/*Calculate the max distance of one Vertex to others in the Simplex.         */
REAL GetMaxDistToOtherV(INT Vertex, PSIMPLEX pS) 
{
 INT i;
 REAL MaxDist=0.0;
 REAL Dist;
 
 for (i=0;i < pS->NVertex; i++)
     if (i!=Vertex)
       {
	Dist=pS->pDistV[CombN2ToIndex(pS->NVertex,Vertex,i)];
	if (Dist==pS->Size)
	   return Dist;
	if (Dist>MaxDist)
	   MaxDist=Dist;
       }
 return MaxDist;   
}

/*---------------------------------------------------------------------------*/
PSIMPLEX CopySimplex(PSIMPLEX pSO, INT NDim, INT NVertex, INT NDist)
{
 int i;
 PSIMPLEX pSD;

 pSD             = (PSIMPLEX) GetMem((SIZE)1,             
                                     (SIZE) sizeof(SIMPLEX),
                                     "CopySimplex:pSD");
 pSD->ppV        = (PPVERTEX) GetMem((SIZE) NVertex, 
                                     (SIZE) sizeof(PVERTEX),
                                     "CopySimplex: pSD->ppV");
 pSD->pDistV     = (PREAL)    GetMem((SIZE) NDist, 
                                     (SIZE) sizeof(REAL),
                                     "CopySimplex: pSD->pDistV");
 pSD->pMaxDV     = (PREAL)    GetMem((SIZE)NVertex, 
                                     (SIZE) sizeof(REAL),
                                     "CopySimplex: pSD->pMaxDV");

 pSD->NVertex    = pSO->NVertex;
 pSD->BTreeLevel = pSO->BTreeLevel;
 pSD->NSimplex   = pSO->NSimplex;
 pSD->Size       = pSO->Size;
 pSD->SQ         = pSO->SQ;

 for (i=0;i<NVertex;i++)
     {
      pSD->ppV[i]=NewVertex(NDim,pSO->ppV[i]->pX);
      pSD->pMaxDV[i]=pSO->pMaxDV[i];
     }
  
 for (i=0;i<NDist;i++)
     pSD->pDistV[i]=pSO->pDistV[i];

 return pSD;
}


/*---------------------------------------------------------------------------*/
VOID DivideSimplex(PPSIMPLEX ppS, PPSIMPLEX ppS1, INT NDim)
{
 int NumberOfDist[]={0,0,1,3,6,10,15,21,28,36,45,55,66,78,91,105,120};

 REAL pRV[16],Dist; /*pRV coordenadas nuevo vertice*/
 REAL MaxDV; /*Max Distance of New Vertex to other vertexes*/

 INT i,V1,V2,NDist,Ind, NVertex;
 
 if (NDim>16)
    {
     fprintf(stderr,"DivideSimplex: NDim=%d.\n",NDim);
     fprintf(stderr,"The Static data is up to 16.\n");
     exit(1);     
    }
 
 if ((*ppS)->NVertex == 1)
    {
     fprintf(stderr,"DivideSimplex: NVertex=%d.\n",(*ppS)->NVertex);
     exit(1);
    }
 
 NDist   = NumberOfDist[(*ppS)->NVertex];
 NVertex = (*ppS)->NVertex;

 (*ppS1)=CopySimplex((*ppS), NDim, NVertex, NDist); 
				 
 (*ppS1)->NSimplex  = 1+2*((*ppS)->NSimplex);
 (*ppS1)->BTreeLevel=(int)log2((double)(*ppS1)->NSimplex);
 
 (*ppS)->NSimplex = 2*((*ppS)->NSimplex);
 (*ppS)->BTreeLevel=(int)log2((double)(*ppS)->NSimplex);
 
 EdgeToBisect(*ppS, &V1, &V2);

 /*Calculate the middle point between vertex V1 and V2.                       */
 MiddlePoint((*ppS)->ppV[V1]->pX,(*ppS)->ppV[V2]->pX,pRV,NDim);


 CopyVR((*ppS1)->ppV[V1]->pX,pRV,NDim); 
 CopyVR((*ppS) ->ppV[V2]->pX,pRV,NDim);
 
 /*The edge has now half size*/
 Ind=CombN2ToIndex(NVertex, V1, V2);
 (*ppS) ->pDistV[Ind] /=2.0;
 (*ppS1)->pDistV[Ind] /=2.0;
 
 /*Initiate the Max Distance of V1 and V2 to other vertexes*/
 /*New edge has the smaller distance*/
 MaxDV=(*ppS)->pDistV[Ind]; 
 
 /*V1 in pS1 and V2 in pS share the distance to other vertexes.*/
 for (i=0;i<NVertex;i++)
     {
      if (i==V1 || i==V2)
         continue;
      Ind=CombN2ToIndex(NVertex, V1, i); 
      Dist=DistPoints((*ppS1)->ppV[V1]->pX,(*ppS1)->ppV[i]->pX,NDim);
      
      if (Dist > MaxDV)
          MaxDV=Dist;
      
      (*ppS1)->pDistV[Ind]=Dist;
      
      Ind=CombN2ToIndex(NVertex, V2, i);
      (*ppS)->pDistV[Ind]=Dist; 
     } 
 
 (*ppS1)->pMaxDV[V1] = MaxDV;
 (*ppS) ->pMaxDV[V2] = MaxDV;
 
 (*ppS1)->Size = SizeSimplex(*ppS1,NDim);
 (*ppS)->Size  = SizeSimplex(*ppS,NDim);
 
 /*We have to update the max distance to other vertexes after*/
 /*Evaluate SizeSimplex because it is used in GetMaxDistToOtherV*/
 
 for (i=0;i<NVertex;i++)
     {
      if (i!=V1)
         (*ppS1)->pMaxDV[i] = GetMaxDistToOtherV(i,(*ppS1));
      if (i!=V2)
         (*ppS)-> pMaxDV[i] = GetMaxDistToOtherV(i,(*ppS));
     }
}

/*---------------------------------------------------------------------------*/
INT BytesBySimplex(INT NDim) /*Overstimates the number of vertexes by simplex*/
{
 INT Total=0;
 
 Total += sizeof(SIMPLEX);
 Total += NDim*sizeof(PVERTEX);/*ppV*/
 Total += BytesByVertex(NDim);
 Total += NDim*sizeof(REAL); /*pMaxDv*/
 Total += NDim*(NDim-1)/2*sizeof(REAL); /*pDistV*/
 
 return Total;  
}

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
