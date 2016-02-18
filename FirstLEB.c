/*L.G.Casado leo@ual.es. 2014. GPLv3.*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>
#include <sys/times.h>
#include <unistd.h>
#include "argshand.h"
#include "getmem.h"
#include "utils.h"
#include "vertex.h"
#include "simplex.h"
#include "btsimplex.h"

#include "btvertex.h"
#include "gengridpoints.h"
#include "Queue.h"

/*----------------------------------------------------------------------------*/
void PrintTimeStuff(FILE * FOut, clock_t cIni)
{
 INT 		Hour,Min,Sec;
 clock_t   	c;
 struct tms  	t;

 c=times(&t);
 Sec=(c-cIni)/sysconf(_SC_CLK_TCK);
 Hour=Sec/3600;
 Sec=Sec%3600;
 Min=Sec/60;
 Sec=Sec%60;
 fprintf(FOut,"%2.2d:%2.2d:%2.2d\n",(int)Hour,(int)Min,(int)Sec);
}

/*---------------------------------------------------------------------------*/
PSIMPLEX TerminationTest (PBTS pbts, PBTS pbtsEnd, PSIMPLEX pS,
                      INT NDim, REAL Epsilon, PLLINT pNFinalSimplex,
                      BOOL NoStoreFinalS, BOOL Draw, INT WWidth, PBTV pbtvVertices, PBTV pbtvGridPoints)
{
  BOOL add;
  int i;
  for(i = 0; i < NDim; i++){
    // Look in pbts for non-repited vertices and compare with gridpoints
    // Check each vertex in the simplices
    VISITED(pbtvGridPoints->pFirstBTVNode, NDim, pS->ppV[i]->pX);
    // Count non-repited vertices
    InsertBTV(pbtvVertices, NDim, pS->ppV[i]->pX, &add);
    /*
    if(add == False){
      fprintf(stderr,"Repetido\n");
    }
    */
  }

 if (GT(pS->Size, Epsilon))
    {
     InsertBTS(pbts,pS);
     if (Draw)
	DrawSimplex(pS,WWidth,"Gray90");
    }
 else
    {
     (*pNFinalSimplex)++;
     if (Draw)
	DrawSimplex(pS,WWidth,"Black");

     if (NoStoreFinalS)
        pS = FreeSimplex(pS,NDim);
     else
        InsertBTS(pbtsEnd,pS);
    }

 return NULL;
}




/*----------------------------------------------------------------------------*/
PSIMPLEX GenInitialS(INT NDim)
{
 int i,j;
 PSIMPLEX pS;
 PPREAL ppCoor;

 ppCoor= (PPREAL)GetMem2D((SIZE)NDim,(SIZE)NDim,(SIZE)sizeof(REAL),
         "GenInitalS:ppCoor");

  for (i=0;i<NDim;i++) /*i: Indicates the NVertex Value*/
     for (j=0;j<NDim;j++)
         {
	  if (i!=j)
              ppCoor[i][j]=0.0;
	  else
	      ppCoor[i][i]=1.0;
	 }

 pS=NewSimplex(NDim,NDim,ppCoor,1);
 Free2D((PPVOID)ppCoor,NDim);
 return pS;
}

/*----------------------------------------------------------------------------*/
VOID DrawGridPoints(PQueue gridPoints, INT NDim, INT WWidth, PCHAR Color)
{
  //int i;
  int p = 1;
  //fprintf(stderr,"GRID POINTS\n");
  while(Front(gridPoints) != NULL){
    double* point = Front(gridPoints);
/*
    for(i = 0; i < NDim; i++){
      fprintf(stderr,"%f ", point[i]);
    }
    fprintf(stderr,"\n");
*/
    printf("DrawPoint\n");

    printf("%f\n",XInWindow(point,WWidth));
    printf("%f\n",YInWindow(point,WWidth));

    printf("%s\n",Color);
    printf("GP%d\n",p);

    // No libero porque son los mismo punteros que tiene el arbol
    //free((void*)point);
    Pop(gridPoints);

    p = p + 1;
  }

  fflush(stdout);
}

/*----------------------------------------------------------------------------*/
/*                               MAIN                                         */
/*----------------------------------------------------------------------------*/
int main(int argc,  char *argv[])
{
 REAL 		Epsilon; 		/*Final procesion on simplex's size.*/
 INT      	FinalDivPerEdge=0;      //ep =1/FinalDivPerEdge
 INT 		NDim;     		/*Dimension=NVertex*/
 LLINT 		NEvalSimplex= 0;
 LLINT		NFinalSimplex=0;
 INT 		WWidth;            	/*Graphical window's width*/
 BOOL		Draw; 			/*GRaphical output using stdout*/
 BOOL 		OutStat; 		/*OutPut statistics*/
 BOOL           NoStoreFinalS;
 INT 		ScUp;   	/*Screen Update: after select ScUp simplexes*/
 PSIMPLEX 	pS, pS1;
 PBTS		pbts=NULL;
 PBTS 		pbtsEnd=NULL;
 CHAR		Execution[256];
 FILE *		FOut;
 clock_t        c1;
 struct tms     t1;


/* Check the input parameters.-- -----------------------------------------*/
 if (ExistArg("--help",argc,argv))
    ParametersError();

 if (!ExistArg("-d",argc,argv))
    ParametersError();
 else
    NDim = atoi(GetArg("-d",argc,argv));


 //Accuracy
 if ((!ExistArg("-ep",argc,argv) && !ExistArg("-g",argc,argv)) ||
     ( ExistArg("-ep",argc,argv) &&  ExistArg("-g",argc,argv)))
   {
    fputs("Parameter -ep  xor -g are neccesary.\n",stderr);
    ParametersError();
   }

  //Number of divisions of an edge.
 if (ExistArg("-g",argc,argv))
    {
     FinalDivPerEdge = (INT)atoi(GetArg("-g",argc,argv));
     if (FinalDivPerEdge  <= 1 )
        {
	 fprintf(stderr,"-g  %d  <=1.\n",FinalDivPerEdge);
	 exit(1);
	}
     Epsilon=1.0/FinalDivPerEdge;	
    }

 //Epsilon of the grid.
 if (ExistArg("-ep",argc,argv))
    {
     Epsilon = (REAL)atof(GetArg("-ep",argc,argv));
     if (Epsilon  <= 0 || Epsilon >= 1.0)
        {
	 fprintf(stderr,"Epsilon not in (0,1). -gep %f\n",Epsilon);
	 exit(1);
	}
    }


 if (ExistArg("-w",argc,argv))
     WWidth = atoi(GetArg("-w",argc,argv));
 else
     WWidth = 400;

 if (ExistArg("-su",argc,argv))
     ScUp =  atoi(GetArg("-su",argc,argv));
 else
     ScUp = 0;

 if (!ExistArg("-out",argc,argv))
    OutStat=False;
 else
    OutStat = True;

 if (!ExistArg("-ns",argc,argv))
    NoStoreFinalS = False;
 else
    NoStoreFinalS = True;

 if (ExistArg("-tcl",argc,argv))
    if (NDim!=3)
       {
	fprintf(stderr,"Only two dimensinal graphics are allowed. Dim=%d\n",
	        NDim);
	exit(1);
       }
    else
       {
	Draw = (UCHAR) atoi(GetArg("-tcl",argc,argv));
	printf("%d\n",WWidth);
	printf("%d\n",WWidth);
	printf("0.0\n");
	printf("1.0\n");
	printf("0.0\n");
	printf("1.0\n");
	printf("%2.4f\n",Epsilon);
	printf("NDim=%d\n",NDim);
       }
 else
    Draw=False;


 if (ScUp>0)
    {
     fprintf(stderr,"\e[0;0H");
     fprintf(stderr,"\e[2J");
    }

 sprintf(Execution,"FirstLEBonGrid-1.0_-n_%d_-ep_%3.4f_-su_%d",NDim,Epsilon,ScUp);
 fprintf(stderr,"%s\n",Execution);

 c1=times(&t1);

 // Generate gridpoints
fprintf(stderr, "eps=%f\n", Epsilon);
fprintf(stderr, "grid points=%f\n", ceil(1.0/Epsilon)+1.0);
PQueue gridPoints = GenGrid(ceil(1.0/Epsilon)+1.0, NDim);

 PBTV pbtvGridPoints = NULL; //AVL tree of grid points
 pbtvGridPoints = NewBTV (pbtvGridPoints);

 BOOL add;
 do{
   double* point = Top(gridPoints);

   InsertBTV(pbtvGridPoints, NDim, point, &add);
   if(add == False){
     fprintf(stderr,"Error, ya insertado\n\n");
   }

   Next(gridPoints);
 }while(End(gridPoints) == 0);

 PBTV pbtvVertices = NULL; //AVL tree of vertices
 pbtvVertices = NewBTV (pbtvVertices);

 /*Initiate Values:*/
 Epsilon = Epsilon * sqrt(2);
 fprintf(stderr, "Accuracy=%f\n", Epsilon);

 pbts      = NewBTS (pbts);
 pbtsEnd   = NewBTS (pbtsEnd);

 pS=GenInitialS(NDim);

 NEvalSimplex++;
 pS=TerminationTest (pbts, pbtsEnd, pS, NDim, Epsilon,
                     &NFinalSimplex, NoStoreFinalS, Draw, WWidth, pbtvVertices, pbtvGridPoints);

 while (pS!=NULL || pbts->NElem > 0)
       {
        if (pS==NULL)
	    ExtractGreaterBTS(pbts,NDim,&pS);
        else
           if (Draw)
              DelSimplex(pS);
 /*
        if (pS->NSimplex< 0)
           {
            fprintf(stderr,"pS->NSimplex=%lld.\n",pS->NSimplex);
           }

        if (NEvalSimplex%1000000==1)
           {
            fprintf(stderr,"NEvalSimplex=%lld.\n",NEvalSimplex);
            fprintf(stderr,"pS->Size=%lf.\n",pS->Size);
            fprintf(stderr,"pS->BTreeLevel=%d.\n",pS->BTreeLevel);
            fprintf(stderr,"pS->NSimplex=%lld.\n",pS->NSimplex);
           }
*/

        DivideSimplex(&pS,&pS1,NDim);

        NEvalSimplex+=2;
        pS1=TerminationTest(pbts, pbtsEnd, pS1, NDim, Epsilon,
                        &NFinalSimplex,NoStoreFinalS, Draw, WWidth, pbtvVertices, pbtvGridPoints);
        if (LE(pS->Size,Epsilon))
           {
            pS=TerminationTest(pbts, pbtsEnd, pS, NDim, Epsilon,
                               &NFinalSimplex,NoStoreFinalS, Draw, WWidth, pbtvVertices, pbtvGridPoints);
           }
       }

       if(Draw)
         DrawGridPoints(gridPoints, NDim, WWidth, "Yellow");

if (OutStat)
    {
     FOut = OpenFile(GetArg("-out",argc,argv),"a");
     fprintf(FOut,"%f\t%lld\n",Epsilon,NEvalSimplex);
     fflush(FOut);
     fclose(FOut);
    }
 else
    {
     fprintf(stderr,"NEvalS     = %lld\n",NEvalSimplex);
     //VISITED(pbtvGridPoints->pFirstBTVNode, NDim, PREAL);
     //checkVertices(pbts->pFirstBTSNode, NDim, pbtvGridPoints);
     fprintf(stderr, "NVertices  = %d\n", pbtvVertices->NElem);
     fprintf(stderr,"GridPoints = %d(%d)\n", Count(pbtvGridPoints->pFirstBTVNode, NDim), CountVisited(pbtvGridPoints->pFirstBTVNode, NDim));

  //   PrintBTV(pbtvGridPoints, NDim);
     //PrintBTV(pbtvVertices, NDim);

//     fprintf(stderr,"NFinalS=%lld.\n",NFinalSimplex);
//     PrintStatBTS(stderr,pbts,"Arbol de trabajo");
//     PrintStatBTS(stderr,pbtsEnd,"Arbol Final");
     PrintTimeStuff(stderr,c1);


    }



 pbts      = FreeBTS (pbts,NDim);
 pbtsEnd   = FreeBTS (pbtsEnd,NDim);

 return 0;
}
