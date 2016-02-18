/******************************************************************************
			btsimplex.c  - description
			----------------------
                    L.G. Casado leo@ual.es. 2014. GPLv3.
	
*******************************************************************************/

#include <stdlib.h>
#include <stdio.h>
#include <values.h>
#include "getmem.h"
#include "utils.h"
#include "vertex.h"
#include "btvertex.h"
#include "simplex.h"
#include "btsimplex.h"

/*---------------------------------------------------------------------------*/
PBTSNODE NewBTSNODE(PSIMPLEX pS)
{
 PBTSNODE pBTSNode;
 
 pBTSNode	   = GetMem(1,sizeof(BTSNODE),"NewBTSNODE");
 pBTSNode->pS      = pS;
 pBTSNode->Balance = EQUAL;
 pBTSNode->pleft   = NULL;
 pBTSNode->pright  = NULL;
 return pBTSNode;
}

/*---------------------------------------------------------------------------*/
PBTSNODE FreeBTSNode(PBTSNODE pBTSNode, INT NDim)
{
 if (pBTSNode==NULL)
    {
     fprintf(stderr,"Trying to free a NULL BTSNODE pointer.\n");
     exit(1);
    }
 if (pBTSNode->pS!=NULL)
     pBTSNode->pS=FreeSimplex(pBTSNode->pS,NDim);
 free((PVOID)pBTSNode);
 pBTSNode=NULL;
 return pBTSNode;   
}

/*----------------------------------------------------------------------------*/
/*Swap the nodes, but the structure of the BLTree does not change.            */
VOID SwapBTSNode(PBTSNODE pBTSNodeO, PBTSNODE pBTSNodeT)
{
 PSIMPLEX pS;
 
 pS            = pBTSNodeO->pS;
 pBTSNodeO->pS = pBTSNodeT->pS;
 pBTSNodeT->pS = pS;
}

/*----------------------------------------------------------------------------*/
/*Swap the data and copy the Balance and pointers of the BTSNodes.            */
/*Node2 will be removed with data of node1 after exchange.                    */
/*----------------------------------------------------------------------------*/
VOID ExchangeNode(PBTSNODE node1, PBTSNODE node2)
{
 SwapBTSNode(node1,node2);
 node1->Balance = node2->Balance;
 node1->pleft   = node2->pleft;
 node1->pright  = node2->pright;

 node2->pleft   = NULL;
 node2->pright  = NULL;
} 

/*---------------------------------------------------------------------------*/
void PrintBTSSubTreeInOrder(PBTSNODE node, INT NDim)
{
 if (node!=NULL)
    {
     PrintBTSSubTreeInOrder(node->pleft,  NDim);
     fprintf(stderr,"(%d)->",node->Balance);
     PrintSimplex (node->pS,NDim);
     PrintBTSSubTreeInOrder(node->pright, NDim);
    }
  else
     fprintf(stderr,"N ");
}

/*---------------------------------------------------------------------------*/
void PrintBTSSubTree(PBTSNODE node, INT NDim)
{
 if (node!=NULL)
    {
     fprintf(stderr,"(%d)->",node->Balance);
     PrintSimplex (node->pS,NDim);
     PrintBTSSubTree(node->pleft,  NDim);
     PrintBTSSubTree(node->pright, NDim);
    }
  else
     fprintf(stderr,"N ");
}



/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
PBTS NewBTS(PBTS pBTS)
{
 pBTS                 = GetMem(1,sizeof(BTS),"NewBTS");
 pBTS->NElem          = 0;
 pBTS->MaxNElem       = 0;	
 pBTS->pFirstBTSNode  = NULL;
 return pBTS;
}

/*----------------------------------------------------------------------------*/
void FreeBTSSubTree(PBTSNODE root,INT NDim)
{
 if (root!=NULL)
    {
     FreeBTSSubTree(root->pleft,NDim);
     FreeBTSSubTree(root->pright,NDim);
     root=FreeBTSNode(root,NDim);
    }
}

/*----------------------------------------------------------------------------*/
PBTS FreeBTS(PBTS pbts,INT NDim)
{
 if (pbts!=NULL)
    {
     FreeBTSSubTree(pbts->pFirstBTSNode,NDim);
     pbts->pFirstBTSNode=NULL;
     free((PVOID)pbts);
     pbts=NULL;
    }
 return NULL;   
}

/*---------------------------------------------------------------------------*/
void PrintBTS(PBTS pbts, INT NDim)
{
 if (pbts!=NULL)
    {
     PrintBTSSubTree(pbts->pFirstBTSNode,NDim);    
     fprintf(stderr,"Number of Elements     = %d\n",pbts->NElem);
     fprintf(stderr,"Max Number of Elements = %d\n",pbts->MaxNElem);
    }
  else
     fprintf(stderr,"N ");
}


/*----------------------------------------------------------------------------*/
/*Rotate left a binary Tree                                                   */
/*----------------------------------------------------------------------------*/
PBTSNODE RotateTreeLeft(PBTSNODE root)
{
 PBTSNODE Temp;

 if (root==NULL)
    {
     fprintf(stderr,"BTSimplex::RotateTreeLeft :");;
     fprintf(stderr," Can not rotate to left an empty tree \n");
     exit(1);
    }
 else
    if (root->pright==NULL)
       {
        fprintf(stderr,"BTSimplex::RotateTreeLeft : Can not rotate to left\n");
        exit(1);
       }
    else
       {
        Temp         = root->pright;
        root->pright = Temp->pleft;
        Temp->pleft  = root;
        root=Temp;
       }
 return Temp;      
}
/*----------------------------------------------------------------------------*/
/*Rotate right a binary Tree                                                  */
/*----------------------------------------------------------------------------*/
PBTSNODE RotateTreeRight(PBTSNODE root)
{
 PBTSNODE Temp;
 
 if (root==NULL)
    {
     fprintf(stderr,"BTSimplex::RotateTreeRight :");
     fprintf(stderr," Can not rotate to right an empty tree \n");
     exit(1);
    }
 else
    if (root->pleft==NULL)
       {
        fprintf(stderr,"BTSimplex::RotateTreeRight:Can not rotate to right\n");
        exit(1);
       }
    else
       {
        Temp         = root->pleft;
        root->pleft  = Temp->pright;
        Temp->pright = root;
        root=Temp;
       }
 return Temp;      
}

/*----------------------------------------------------------------------------*/
/*Insert in the left branch maintaining the global balance. Taller noticed if */
/*the insertion has increased the heigh or not, so the balance factors can be */
/*changed appropiately and rotations may be done. Taller has efects only as   */
/* return value. We initiate it to false.                                     */
/*----------------------------------------------------------------------------*/
PBTSNODE LeftInsertTreeBalance(PBTSNODE root, PBOOL ptaller)
{
 PBTSNODE Left, LeftRight;

 Left=root->pleft;
 switch (Left->Balance)
        {
         case LEFT:
              root->Balance=EQUAL;
              Left->Balance=EQUAL;
              root=RotateTreeRight(root);
              *ptaller=False;
              break;
         case EQUAL:
              fprintf(stderr,"BTSimplex:LeftInsertTreeBalance :");
	      fprintf(stderr," The tree is already balanced\n");
              exit(1);
              break;
         case RIGHT:
              LeftRight=Left->pright;
              switch (LeftRight->Balance)
                     {
                      case LEFT:
                           root->Balance=RIGHT;
                           Left->Balance=EQUAL;
                           break;
                      case EQUAL:
                           root->Balance=EQUAL;
                           Left->Balance=EQUAL;
                           break;
                      case RIGHT:
                           root->Balance=EQUAL;
                           Left->Balance=LEFT;
                           break;
                     }
              LeftRight->Balance=EQUAL;
              root->pleft=RotateTreeLeft(root->pleft);
              root=RotateTreeRight(root);
              *ptaller=False;
              break;
        }
 return root;	
}


/*---------------------------------------------------------------------------*/
/*Analogous to previous one but on the right.                                */
/*---------------------------------------------------------------------------*/
PBTSNODE RightInsertTreeBalance(PBTSNODE root, PBOOL ptaller)
{
 PBTSNODE Right, RightLeft;

 Right=root->pright;
 switch (Right->Balance)
        {
         case LEFT:
              RightLeft=Right->pleft;
              switch (RightLeft->Balance)
                     {
                      case LEFT:
                           root->Balance=EQUAL;
                           Right->Balance=RIGHT;
                           break;
                      case EQUAL:
                           root->Balance=EQUAL;
                           Right->Balance=EQUAL;
                           break;
                      case RIGHT:
                           root->Balance=LEFT;
                           Right->Balance=EQUAL;
                           break;
                     }
              RightLeft->Balance=EQUAL;
              root->pright=RotateTreeRight(root->pright);
              root=RotateTreeLeft(root);
              *ptaller=False;
              break;
         case EQUAL:
              fprintf(stderr,"BTSimplex:RightInsertTreeBalance: ");
	      fprintf(stderr,"The tree is already balanced\n");
              exit(1);
              break;
         case RIGHT:
              root->Balance=EQUAL;
              Right->Balance=EQUAL;
              root=RotateTreeLeft(root);
              *ptaller=False;
              break;
        }
 return root;	
}


/*---------------------------------------------------------------------------*/
/*Do the actual insertion in the BTSimplex.                                  */
/*---------------------------------------------------------------------------*/ 
PBTSNODE InsertTree (PBTSNODE root, PSIMPLEX pS, PBOOL ptaller)
{
 if (root==NULL)
    {
     root     = NewBTSNODE(pS); /*Initiate the node with pS.*/
     *ptaller = True;
    }
 else
    {
     if (pS->NSimplex==root->pS->NSimplex)
        {
         fprintf(stderr,"InsertTree: Simplex %lld already exists.\n", pS->NSimplex); 
         exit(1);
        }
     else
         if (pS->NSimplex < root->pS->NSimplex)
            {
             root->pleft=InsertTree(root->pleft,pS,ptaller);
             if (*ptaller)
                switch (root->Balance)
                       {
                        case LEFT:
                                 root=LeftInsertTreeBalance(root,ptaller);
                                 break;
                        case EQUAL:
                                 root->Balance=LEFT;
                                 break;
                        case RIGHT:
                                 root->Balance=EQUAL;
                                 *ptaller=False;
                                 break;
                       }
            }
         else
            {
             root->pright=InsertTree(root->pright,pS,ptaller);
             if (*ptaller)
                switch (root->Balance)
                       {
                        case LEFT:
                                 root->Balance=EQUAL;
                                 *ptaller=False;
                                 break;
                        case EQUAL:
                                 root->Balance=RIGHT;
                                 break;
                        case RIGHT:
                                 root=RightInsertTreeBalance(root,ptaller);
                                 break;
                       }
            }
    }
 return root;   
}

/*---------------------------------------------------------------------------*/
VOID InsertBTS(PBTS pbts, PSIMPLEX pS)
{
 BOOL taller = False;

 pbts->pFirstBTSNode=InsertTree(pbts->pFirstBTSNode,pS,&taller);

 pbts->NElem++;
 
 if (pbts->NElem > pbts->MaxNElem)
     pbts->MaxNElem = pbts->NElem;
	
}


/*---------------------------------------------------------------------------*/
/*Update the balance factors and do the necesary rotations when a left node  */
/*is removed.                                                                */
/*---------------------------------------------------------------------------*/
PBTSNODE LeftDelTreeBalance (PBTSNODE root, PBOOL pshorter)
{
 PBTSNODE Left, LeftRight;

 Left=root->pleft;
 switch (Left->Balance)
        {
         case LEFT:
              root->Balance=Left->Balance=EQUAL;
              root=RotateTreeRight(root);
              break;
         case EQUAL:
              root->Balance=LEFT;
              Left->Balance=RIGHT;
              root=RotateTreeRight(root);
              *pshorter=False;
              break;
         case RIGHT:
              LeftRight=Left->pright;
              switch (LeftRight->Balance)
                     {
                      case LEFT:
                           root->Balance=RIGHT;
                           Left->Balance=EQUAL;
                           break;
                      case EQUAL:
                           root->Balance=Left->Balance=EQUAL;
                           break;
                      case RIGHT:
                           root->Balance=EQUAL;
                           Left->Balance=LEFT;
                           break;
                     }
              LeftRight->Balance=EQUAL;
              root->pleft=RotateTreeLeft(root->pleft);
              root=RotateTreeRight(root);
              break;
        }
 return root;
}

/*---------------------------------------------------------------------------*/
/*Analogous to previous one but on the right.                                */
/*---------------------------------------------------------------------------*/
PBTSNODE RightDelTreeBalance (PBTSNODE root, PBOOL pshorter)
{
 PBTSNODE Right, RightLeft;

 Right=root->pright;
 switch (Right->Balance)
        {
         case LEFT:
              RightLeft=Right->pleft;
              switch (RightLeft->Balance)
                     {
                      case LEFT:
                           root->Balance=EQUAL;
                           Right->Balance=RIGHT;
                           break;
                      case EQUAL:
                           root->Balance=Right->Balance=EQUAL;
                           break;
                      case RIGHT:
                           root->Balance=LEFT;
                           Right->Balance=EQUAL;
                           break;
                     }
              RightLeft->Balance=EQUAL;
              root->pright=RotateTreeRight(root->pright);
              root=RotateTreeLeft(root);
              break;
         case EQUAL:
              root->Balance=RIGHT;
              Right->Balance=LEFT;
              root=RotateTreeLeft(root);
              *pshorter=False;
              break;
         case RIGHT:
              root->Balance=Right->Balance=EQUAL;
              root=RotateTreeLeft(root);
              break;
        }
 return root;	
}

/*---------------------------------------------------------------------------*/
/*                                                                           */
/*---------------------------------------------------------------------------*/
PBTSNODE ExtractNodeEqual (PBTSNODE root, INT NDim, INT NSimplex, 
                           PBOOL pshorter, PPSIMPLEX ppS)
{
 PBTSNODE Temp;
 
 if (root==NULL)
    {
     fprintf(stderr,"BTSNODE,Free1: Empty BLTSimplex\n");
     exit(1);
    }     
 else  
    {     	      
     if (NSimplex==root->pS->NSimplex)
        {
	 *ppS=root->pS;
	 if (root->pleft==NULL && root->pright==NULL)
            {
             root->pS=NULL; 
	     root=FreeBTSNode(root,NDim);
             *pshorter=True;     
            } 
	 else
            if (root->pleft==NULL)
               {
        	Temp=root->pright;
        	ExchangeNode(root,Temp); 
        	*pshorter=True; 
                Temp->pS=NULL;  
        	Temp=FreeBTSNode(Temp,NDim);
               } 
            else
               if (root->pright==NULL)
                  {
                   Temp=root->pleft;
                   ExchangeNode(root,Temp);
                   *pshorter=True; 
                   Temp->pS=NULL;  
                   Temp=FreeBTSNode(Temp,NDim);
                  } 
               else
                  {
                   for (Temp=root->pright;Temp->pleft!=NULL;
		        Temp=Temp->pleft);
                   SwapBTSNode(root,Temp);  
                   root->pright=ExtractNodeEqual(root->pright, NDim, 
		                                 NSimplex, pshorter, ppS);
                   if (*pshorter)
                      switch (root->Balance)
                             {
                              case LEFT:
                                   root=LeftDelTreeBalance(root, pshorter);
                                   break;
                              case EQUAL:
                                   root->Balance=LEFT;
                                   *pshorter=False;
                                   break;
                              case RIGHT:
                                   root->Balance=EQUAL;
                                   break;
                             }
                  } 
        
	}    	      
     else
        if (NSimplex < root->pS->NSimplex)
           {
            root->pleft=ExtractNodeEqual(root->pleft, NDim, NSimplex,
	                                 pshorter,ppS);
            if (*pshorter)
               switch (root->Balance)
                      {
                       case LEFT:
                            root->Balance=EQUAL;
                            break;
                       case EQUAL:
                            root->Balance=RIGHT;
                            *pshorter=False;
                            break;
                       case RIGHT:
                            root=RightDelTreeBalance(root, pshorter);
                            break;
                      }              
           }
        else
           {
            root->pright=ExtractNodeEqual(root->pright, NDim, NSimplex,
	                                  pshorter,ppS);
            if (*pshorter)
                switch (root->Balance)
                      {
                       case LEFT:
                            root=LeftDelTreeBalance(root, pshorter);
                            break;
                       case EQUAL:
                            root->Balance=LEFT;
                            *pshorter=False;
                            break;
                       case RIGHT:
                            root->Balance=EQUAL;
                            break;
                      }
           }
    }	   
 return root;
}


/*---------------------------------------------------------------------------*/ 
VOID ExtractEqualBTS(PBTS pbts, INT NDim, INT NSimplex, PPSIMPLEX ppS)
{
 BOOL shorter=False;

 pbts->pFirstBTSNode=ExtractNodeEqual(pbts->pFirstBTSNode, NDim, NSimplex,
                                    &shorter, ppS);
 pbts->NElem--;
} 



/*---------------------------------------------------------------------------*/
/*                                                                           */
/*---------------------------------------------------------------------------*/
PBTSNODE ExtractSmallerNode(PBTSNODE root, INT NDim, PLLINT pNSimplex, 
                            PBOOL pshorter, PPSIMPLEX ppS)
{
 PBTSNODE Temp;
 
 if (root==NULL)
    {
     fprintf(stderr,"BTSNODE,ExtractSmallerNode: Empty BTS\n");
     exit(1);
    }     

 if (root->pleft!=NULL)
    root->pleft= ExtractSmallerNode(root->pleft, NDim, pNSimplex, 
     				    pshorter, ppS);	     
 else
    {
     *ppS=root->pS;
     *pNSimplex = (*ppS)->NSimplex;
    } 
    
 /*Check if the node have to be removed*/ 
 if (root->pS->NSimplex == *pNSimplex) 
    { 
     if (root->pleft==NULL && root->pright==NULL)
	{
         root->pS=NULL;
	 root=FreeBTSNode(root,NDim);
	 *pshorter=True; 
	 /*The new Smaller value was given in the path*/    
	} 
     else
	{
	 Temp=root->pright;
	 ExchangeNode(root,Temp); 
	 *pshorter=True;
         Temp->pS=NULL;   
	 Temp=FreeBTSNode(Temp,NDim);
	} 
    }    	      
 else /*root->Key > *pNSimplex */
    {
     if (*pshorter)
	switch (root->Balance)
               {
        	case LEFT:
                     root->Balance=EQUAL;
                     break;
        	case EQUAL:
                     root->Balance=RIGHT;
                     *pshorter=False;
                     break;
        	case RIGHT:
                     root=RightDelTreeBalance(root, pshorter);
                     break;
               }              
    }
    
 return root;
}

/*---------------------------------------------------------------------------*/
/*                                                                           */
/*---------------------------------------------------------------------------*/
VOID ExtractSmallerBTS(PBTS pbts, INT NDim, PPSIMPLEX ppS)
{
 BOOL shorter       = False;
 LLINT NSimplex;

 pbts->pFirstBTSNode=ExtractSmallerNode(pbts->pFirstBTSNode,NDim,&NSimplex,
                                        &shorter,ppS);
 pbts->NElem--;
}


/*---------------------------------------------------------------------------*/
/*                                                                           */
/*---------------------------------------------------------------------------*/
PBTSNODE ExtractGreaterNode(PBTSNODE root, INT NDim, PLLINT pNSimplex, 
                            PBOOL pshorter, PPSIMPLEX ppS)
{
 PBTSNODE Temp;
 
 if (root==NULL)
    {
     fprintf(stderr,"BTSNODE,ExtractGreaterNode: Empty BTS\n");
     exit(1);
    }     

 if (root->pright!=NULL)
    root->pright= ExtractGreaterNode(root->pright, NDim, pNSimplex, 
     				    pshorter, ppS);	     
 else
    {
     *ppS=root->pS;
     *pNSimplex = (*ppS)->NSimplex;
    } 
    
 /*Check if the node have to be removed*/ 
 if (root->pS->NSimplex == *pNSimplex) 
    { 
     if (root->pleft==NULL && root->pright==NULL)
	{
         root->pS=NULL;
	 root=FreeBTSNode(root,NDim);
	 *pshorter=True; 
	 /*The new Smaller value was given in the path*/    
	} 
     else
	{
	 Temp=root->pleft;
	 ExchangeNode(root,Temp); 
	 *pshorter=True;
         Temp->pS=NULL;   
	 Temp=FreeBTSNode(Temp,NDim);
	} 
    }    	      
 else /*root->Key > *pNSimplex */
    {
     if (*pshorter)
         switch (root->Balance)
               {
                case LEFT:
                     root=LeftDelTreeBalance(root, pshorter);
                     break;
                case EQUAL:
                     root->Balance=LEFT;
                     *pshorter=False;
                     break;
                case RIGHT:
                     root->Balance=EQUAL;
                     break;
               }      
    }
    
 return root;
}


/*---------------------------------------------------------------------------*/
/*                                                                           */
/*---------------------------------------------------------------------------*/
VOID ExtractGreaterBTS(PBTS pbts, INT NDim, PPSIMPLEX ppS)
{
 BOOL shorter       = False;
 LLINT NSimplex;

 pbts->pFirstBTSNode=ExtractGreaterNode(pbts->pFirstBTSNode,NDim,&NSimplex,
                                        &shorter,ppS);
 pbts->NElem--;
}

/*---------------------------------------------------------------------------*/
INT GetSmallerNSimplex(PBTS pbts)
{
 PBTSNODE Temp;

 if (pbts->pFirstBTSNode==NULL)
    {
     fputs("GetSmallerNSimplex:Empty Tree.",stderr);
     exit(1);
    }
 else
    {
     Temp=pbts->pFirstBTSNode;
     while (Temp->pleft!=NULL)
           Temp=Temp->pleft;    
     return Temp->pS->NSimplex;
    }
}

/*---------------------------------------------------------------------------*/
INT GetGreaterNSimplex(PBTS pbts)
{
 PBTSNODE Temp;

 if (pbts->pFirstBTSNode==NULL)
    {
     fputs("GetGreaterNSimplex:Empty Tree.",stderr);
     exit(1);
    }
 else
    {
     Temp=pbts->pFirstBTSNode;
     while (Temp->pright!=NULL)
           Temp=Temp->pright;    
     return Temp->pS->NSimplex;
    }
}


/*---------------------------------------------------------------------------*/
void PrintStatBTS(FILE * FOut, PBTS pbts, PCHAR String)
{
 if (pbts!=NULL)
    {
     if (FOut==stderr)
        fprintf(FOut,"\e[1;31m");
     fprintf(FOut,"           %s: \n"  ,String);
     if (FOut==stderr)
        fprintf(FOut,"\e[1;39m");	
     fprintf(FOut,"MaxEle___: ");
     fprintf(FOut,"%10d \n",pbts->MaxNElem);
     fprintf(FOut,"NElem____: ");
     fprintf(FOut,"%10d \n",pbts->NElem);
    }
  else
     fprintf(FOut,"N ");
}

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
