/**CFile*******************************************************************
  PackageName [Biddy]
  Synopsis    [Biddy provides data structures and algorithms
               for the representation and manipulation of Boolean
               functions with ROBDDs. A hash table is used for quick
               search of nodes. Complement edges decreases the number
               of nodes. Function ITE is given for manipulation of
               boolean functions. An automatic garbage collection with
               a formulae counter is implemented.]

  FileName    [biddyMain.c]
  Revision    [$Revision: 53 $]
  Date        [$Date: 2013-04-29 08:28:44 +0200 (pon, 29 apr 2013) $]
  Authors     [Robert Meolic (meolic@uni-mb.si),
               Ales Casar (casar@uni-mb.si)]
  Description [The file biddyMain.c contains main functions for
               representation and manipulation of boolean functions
               with ROBDDs.]
  SeeAlso     [biddy.h, biddyInt.h]

  Copyright   [This file is part of Biddy.
               Copyright (C) 2006, 2013 UM-FERI
               UM-FERI, Smetanova ulica 17, SI-2000 Maribor, Slovenia

               Biddy is free software; you can redistribute it and/or modify
               it under the terms of the GNU General Public License as
               published by the Free Software Foundation; either version 2
               of the License, or (at your option) any later version.

               Biddy is distributed in the hope that it will be useful,
               but WITHOUT ANY WARRANTY; without even the implied warranty of
               MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
               GNU General Public License for more details.

               You should have received a copy of the GNU General Public
               License along with this program; if not, write to the Free
               Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
               Boston, MA 02110-1301 USA.]
  ************************************************************************/
#include "stdafx.h"
#include "biddyInt.h"

#pragma warning(disable: 4996)

#ifdef UNIX
#include <sys/resource.h>   /* used for setrlimit(), UNIX only */
#endif

/*-----------------------------------------------------------------------*/
/* Constant declarations                                                 */
/*-----------------------------------------------------------------------*/

/* Size of node and cache tables should be prime, e.g.:            */
/*   1009, 5003, 10007, 50021, 100043, 250007, 500057, 1000003 ... */

/* If BDD node is 24 bit (e.g. 32-bit GNU/Linux systems)           */
/* 50021 nodes is 1.2 MB                                           */
/* 1000003 nodes is 22.9 MB                                        */
/* 5000011 nodes is 114.4 MB                                       */

#define SMALL_NODE_TABLE  50021
#define BIG_NODE_TABLE    1000003
#define HUGE_NODE_TABLE   5000011

#define SMALL_ITE_CACHE  25013
#define BIG_ITE_CACHE    500057
#define HUGE_ITE_CACHE   1000003

#define SMALL_EAX_CACHE  1009
#define BIG_EAX_CACHE    100043
#define HUGE_EAX_CACHE   500057

/*-----------------------------------------------------------------------*/
/* Variable declarations                                                 */
/*-----------------------------------------------------------------------*/

/* EXPORTED VARIABLES - TERMINAL EDGES AND NULL EDGE*/

/* OLD IMPLEMENTATION */
/*
Biddy_Edge biddy_one = {NULL,FALSE};
Biddy_Edge biddy_zero = {NULL,FALSE};
Biddy_Edge biddy_null = {NULL,FALSE};
*/

/* NEW IMPLEMENTATION */
Biddy_Edge biddy_one = (Biddy_Edge) 0;
Biddy_Edge biddy_zero = (Biddy_Edge) 0;
Biddy_Edge biddy_null = (Biddy_Edge) 0;

/* INTERNAL VARIABLES */

short int biddyCount = 0; /* formulae counter, for garbage collection */

unsigned int biddyBlockSize = BIG_NODE_TABLE;
unsigned int biddyIteCacheSize = BIG_ITE_CACHE;
unsigned int biddyEAxCacheSize = BIG_EAX_CACHE;

BiddyNodeTable biddyNodeTable
               = {NULL,NULL,0,0,0,0,0,0,0,0,0,0};       /* node table */
BiddyVariableTable biddyVariableTable
               = {NULL,0};                          /* variable table */
BiddyCacheList *biddyCacheList = NULL;                  /* cache list */

BiddyNode *biddyFreeNodes = NULL;      /* used for garbage collection */
BiddyNode *biddyFirstNewNode = NULL;   /* used for garbage collection */
BiddyNode *biddyLastNewNode = NULL;    /* used for garbage collection */
BiddyNode *biddyFreshNodes = NULL;     /* used for garbage collection */
BiddyNode *biddyFortifiedNodes = NULL; /* used for garbage collection */

BiddyIteCacheTable biddyIteCache
                   = {NULL,0,0,0,0};                     /* ITE cache */
BiddyEAxCacheTable biddyEAxCache
                   = {NULL,0,0,0,0};                      /* EX cache */

/**AutomaticStart*********************************************************/

/*-----------------------------------------------------------------------*/
/* Static function prototypes                                            */
/*-----------------------------------------------------------------------*/

static void deleteVariableTable(BiddyVariableTable table);

static void exchangeEdges(Biddy_Edge *f, Biddy_Edge *g);

static void complExchangeEdges(Biddy_Edge *f, Biddy_Edge *g);

static void AddIteCache(
               Biddy_Edge a, Biddy_Edge b, Biddy_Edge c, Biddy_Edge r);

static Biddy_Boolean FindIteCache(
               Biddy_Edge a, Biddy_Edge b, Biddy_Edge c, Biddy_Edge *r);

static void IteGarbage();

static void AddEAxCache(Biddy_Edge f, Biddy_Variable v, Biddy_Edge r);

static Biddy_Boolean FindEAxCache(
               Biddy_Edge f, Biddy_Variable v, Biddy_Edge *r);

static void EAxGarbage();

static void WriteBDD(Biddy_Edge f);

/**AutomaticEnd***********************************************************/

/*-----------------------------------------------------------------------*/
/* Definition of exported functions                                      */
/*-----------------------------------------------------------------------*/

/**Function****************************************************************
  Synopsis    [Function Biddy_Init initialize BIDDY package.]
  Description [Biddy_Init creates node table (biddyNodeTable), variableTable
               (biddy_VariableTable), cache list (biddyCacheList),
               ITE cache (biddyIteCache), EAX cache (biddyEAxCache),
               and terminal nodes (biddy_one, biddy_zero). Biddy_Init
               also initialize automatic garbage collection.]
  SideEffects [Allocate a lot of memory. If function call fails,
               decrease biddyBlockSize.]
  SeeAlso     [Biddy_Exit]
  ************************************************************************/

#ifdef __cplusplus
extern "C" {
#endif

void
Biddy_Init()
{
  int i;

#ifdef UNIX
  struct rlimit rl;
#endif

#ifdef UNIX
  rl.rlim_max  = rl.rlim_cur = RLIM_INFINITY;
  if (setrlimit(RLIMIT_STACK, &rl)) {
    fprintf(stderr,"ERROR: setrlimit\n");
    perror("setrlimit");
    exit(1);
  }
#endif

  /* INITIALIZATION OF NODE TABLE */
  biddyNodeTable.size = biddyBlockSize;
  if (!(biddyNodeTable.table = (BiddyNode **)
        malloc((biddyNodeTable.size+1) * sizeof(BiddyNode *)))) {
    fprintf(stderr,"Biddy_Init (Node table): Out of memoy!\n");
    exit(1);
  }

  for (i=0;i<biddyNodeTable.size+1;i++) {
    biddyNodeTable.table[i] = NULL;
  }

  /* INITIALIZATION OF VARIABLE TABLE */
  biddyVariableTable.size = 1;
  biddyVariableTable.table =
    (BiddyVariable *) malloc(biddyVariableTable.size * sizeof(BiddyVariable));
  biddyVariableTable.table[0].name = strdup("1");
  biddyVariableTable.table[0].order = 0;
  biddyVariableTable.table[0].term = biddy_null;
  biddyVariableTable.table[0].value = TRUE;

  /* VARIABLE TABLE COULD USE A RED-BLACK TREE FOR EFFICIENT SEARCHING */
  /* http://mathworld.wolfram.com/Red-BlackTree.html */
  /* 1. Every node has two children, each colored either red or black. */
  /* 2. Every tree leaf node is colored black. */
  /* 3. Every red node has both of its children colored black. */
  /* 4. Every path from the root contains the same number of black nodes. */

  /* MAKE TERMINAL NODE AND TERMINAL EDGES */
  biddyNodeTable.table[biddyNodeTable.size] =
    (BiddyNode *) malloc(1 * sizeof(BiddyNode));
  biddyNodeTable.table[biddyNodeTable.size]->prev = 0;
  biddyNodeTable.table[biddyNodeTable.size]->next = 0;
  biddyNodeTable.table[biddyNodeTable.size]->list = 0;
  biddyNodeTable.table[biddyNodeTable.size]->f = biddy_null;
  biddyNodeTable.table[biddyNodeTable.size]->t = biddy_null;
  biddyNodeTable.table[biddyNodeTable.size]->v = 0;
  biddyNodeTable.table[biddyNodeTable.size]->count = 0; /*terminal node is fortied*/

  /* OLD IMPLEMENTATION */
  /*
  biddy_one.p = (void *) biddyNodeTable.table[biddyNodeTable.size];
  biddy_one.mark = FALSE;
  biddy_zero = biddy_one;
  biddy_zero.mark = TRUE;
  */

  /* NEW IMPLEMENTATION */
  biddy_one = (void *) biddyNodeTable.table[biddyNodeTable.size];
  biddy_zero = (void *) ((uintptr_t) biddy_one | (uintptr_t) 1);

  /* DEBUGGING WITH GDB AND DDD */
  /* use "print sizeof(BiddyNode)" */
  /* use "print sizeof(((BiddyNode *) biddy_one).f)" */
  /* use "graph display *((BiddyNode *) biddy_one)" */

  /* INITIALIZATION OF NODE TABLE - CONTINUED */
  biddyNodeTable.max = 1;
  biddyNodeTable.num = 1;
  biddyNodeTable.numf = 1;
  biddyNodeTable.foa = 0;
  biddyNodeTable.compare = 0;
  biddyNodeTable.add = 0;
  biddyNodeTable.garbage = 0;

  /* INITIALIZATION OF MEMORY MANAGEMENT AND GARBAGE COLLECTION */
  /* terminal node should not be added to List of fortified nodes */
  if (!(biddyFreeNodes = (BiddyNode *)
        malloc((biddyBlockSize) * sizeof(BiddyNode)))) {
    fprintf(stderr,"Biddy_Init (Nodes): Out of memoy!\n");
    exit(1);
  }
  biddyNodeTable.blocktable = (BiddyNode **) malloc(sizeof(BiddyNode *));
  biddyNodeTable.blocktable[0] = biddyFreeNodes;
  for (i=0; i<biddyBlockSize-1; i++) {
    biddyFreeNodes[i].list = &biddyFreeNodes[i+1];
  }
  biddyFreeNodes[biddyBlockSize-1].list = NULL;
  biddyFirstNewNode = biddyFreshNodes = biddyLastNewNode =
    (BiddyNode *) Biddy_GetPointer(biddy_one);
  biddyCount = 1;

  /* INITIALIZATION OF NODE TABLE - CONTINUED */
  biddyNodeTable.generated = biddyBlockSize;
  biddyNodeTable.blocknumber = 1;

  /* INITIALIZATION OF CACHE LIST */
  biddyCacheList = NULL;

  /* INITIALIZATION OF ITE CACHE - USED FOR ITE OPERATION */
  biddyIteCache.size = biddyIteCacheSize;
  biddyIteCache.search = biddyIteCache.find = biddyIteCache.overwrite = 0;
  if (!(biddyIteCache.table = (BiddyIteCache *)
  malloc(biddyIteCache.size * sizeof(BiddyIteCache)))) {
    fprintf(stderr,"Biddy_Init (ITE CACHE): Out of memoy!\n");
    exit(1);
  }
  for (i=0;i<biddyIteCache.size;i++) {
    biddyIteCache.table[i].ok = FALSE;
  }
  Biddy_AddCache(IteGarbage);

  /* INITIALIZATION OF EAX CACHE - USED FOR QUANTIFICATIONS */
  biddyEAxCache.size = biddyEAxCacheSize;
  biddyEAxCache.search = biddyEAxCache.find = biddyEAxCache.overwrite = 0;
  if (!(biddyEAxCache.table = (BiddyEAxCache *)
  malloc(biddyEAxCache.size * sizeof(BiddyEAxCache)))) {
    fprintf(stderr,"Biddy_Init (EAx CACHE): Out of memoy!\n");
    exit(1);
  }
  for (i=0;i<biddyEAxCache.size;i++) {
    biddyEAxCache.table[i].ok = FALSE;
  }
  Biddy_AddCache(EAxGarbage);
}

#ifdef __cplusplus
}
#endif

/**Function****************************************************************
  Synopsis    [Function Biddy_Exit]
  Description [Biddy_Exit deallocates all memory allocated by Biddy_Init,
               Biddy_FoaNode, Biddy_FoaTerminal etc.]
  SideEffects []
  SeeAlso     [Biddy_Init]
  ************************************************************************/

#ifdef __cplusplus
extern "C" {
#endif

void
Biddy_Exit()
{
  int i;
  BiddyCacheList *sup1, *sup2;

  /* DELETE NODES */
  /*
  printf("Delete nodes...\n");
  */
  free(biddyNodeTable.table[biddyNodeTable.size]);
  for (i=0; i<biddyNodeTable.blocknumber; i++) {
    free(biddyNodeTable.blocktable[i]);
  }

  /* DELETE BLOCK TABLE */
  /*
  printf("Delete block table...\n");
  */
  free(biddyNodeTable.blocktable);

  /* DELETE NODE TABLE */
  /*
  printf("Delete node table...\n");
  */
  free(biddyNodeTable.table);

  /* DELETE SYMBOL TREE */
  /*
  printf("Delete symbol tree...\n");
  */
  deleteVariableTable(biddyVariableTable);

  /* DELETE CACHE LIST */
  /*
  printf("Delete cache list...\n");
  */
  sup1 = biddyCacheList;
  while (sup1) {
    sup2 = sup1->next;
    free(sup1);
    sup1 = sup2;
  }

  /* DELETE ITE CACHE */
  /*
  printf("Delete ITE cache...\n");
  */
  free(biddyIteCache.table);

  /* DELETE EAX CACHE TABLE */
  /*
  printf("Delete EAX cache...\n");
  */
  free(biddyEAxCache.table);

  /* USER SHOULD DELETE HIS OWN CACHES */
}

#ifdef __cplusplus
}
#endif

/**Function****************************************************************
  Synopsis    [Function Biddy_About]
  Description [Biddy_About reports version of Biddy package.]
  SideEffects []
  SeeAlso     []
  ************************************************************************/

#ifdef __cplusplus
extern "C" {
#endif

#ifndef BIDDYVERSION
#define BIDDYVERSION "biddy1_1"
#endif

char *
Biddy_About() {
  return strdup(BIDDYVERSION);
}

#ifdef __cplusplus
}
#endif

/**Function****************************************************************
  Synopsis    [Function Biddy_isOK]
  Description [Biddy_isOK return TRUE iff given node is not bad.]
  SideEffects []
  SeeAlso     []
  ************************************************************************/

#ifdef __cplusplus
extern "C" {
#endif

Biddy_Boolean
Biddy_isOK(Biddy_Edge f)
{
  return (
    (((BiddyNode *) Biddy_GetPointer(f))->count == 0) ||
    (((BiddyNode *) Biddy_GetPointer(f))->count == biddyCount)
  );
}

#ifdef __cplusplus
}
#endif

/**Function****************************************************************
  Synopsis    [Function Biddy_GetThen]
  Description [Biddy_GetThen return THEN successor.]
  SideEffects []
  SeeAlso     []
  ************************************************************************/

#ifdef __cplusplus
extern "C" {
#endif

Biddy_Edge
Biddy_GetThen(Biddy_Edge f)
{
  return ((BiddyNode *) Biddy_GetPointer(f))->t;
}

#ifdef __cplusplus
}
#endif

/**Function****************************************************************
  Synopsis    [Function Biddy_GetElse]
  Description [Biddy_GetElse return ELSE successor.]
  SideEffects []
  SeeAlso     []
  ************************************************************************/

#ifdef __cplusplus
extern "C" {
#endif

Biddy_Edge
Biddy_GetElse(Biddy_Edge f)
{
  return ((BiddyNode *) Biddy_GetPointer(f))->f;
}

#ifdef __cplusplus
}
#endif

/**Function****************************************************************
  Synopsis    [Function Biddy_GetVariable]
  Description [Biddy_GetVariable return the index of top variable.]
  SideEffects []
  SeeAlso     []
  ************************************************************************/

#ifdef __cplusplus
extern "C" {
#endif

Biddy_Variable
Biddy_GetVariable(Biddy_Edge f)
{
  return ((BiddyNode *) Biddy_GetPointer(f))->v;
}

#ifdef __cplusplus
}
#endif

/**Function****************************************************************
  Synopsis    [Function Biddy_GetVariableName]
  Description [Biddy_GetVariableName return the name of top variable.]
  SideEffects []
  SeeAlso     []
  ************************************************************************/

#ifdef __cplusplus
extern "C" {
#endif

Biddy_String
Biddy_GetVariableName(Biddy_Edge f)
{
  return biddyVariableTable.table[((BiddyNode *) Biddy_GetPointer(f))->v].name;
}

#ifdef __cplusplus
}
#endif

/**Function****************************************************************
  Synopsis    [Function Biddy_GetVariableChar]
  Description [Biddy_GetVariableChar return the first character in the name
               of top variable.]
  SideEffects []
  SeeAlso     []
  ************************************************************************/

#ifdef __cplusplus
extern "C" {
#endif

char
Biddy_GetVariableChar(Biddy_Edge f)
{
  return biddyVariableTable.table[((BiddyNode *) Biddy_GetPointer(f))->v].name[0];
}

#ifdef __cplusplus
}
#endif

/**Function****************************************************************
  Synopsis    [Function Biddy_GetIthVariableName]
  Description [Biddy_GetIthVariableName return the name of ith variable
               in the current order.]
  SideEffects [Assumes that variables are ordered as stored in table!]
  SeeAlso     []
  ************************************************************************/

#ifdef __cplusplus
extern "C" {
#endif

Biddy_String
Biddy_GetIthVariableName(int i)
{
  return biddyVariableTable.table[i].name;
}

#ifdef __cplusplus
}
#endif

/**Function****************************************************************
  Synopsis    [Function Biddy_GetVariableOrder]
  Description [Biddy_GetVariableOrder return the order of top variable.]
  SideEffects [VariableOrder is not the same type as Biddy_Variable.]
  SeeAlso     []
  ************************************************************************/

#ifdef __cplusplus
extern "C" {
#endif

int
Biddy_GetVariableOrder(Biddy_Edge f)
{
  return biddyVariableTable.table[((BiddyNode *) Biddy_GetPointer(f))->v].order;
}

#ifdef __cplusplus
}
#endif

/**Function****************************************************************
  Synopsis    [Function Biddy_SelectNode]
  Description [Biddy_SelectNode select the top node of given edge.]
  SideEffects []
  SeeAlso     []
  ************************************************************************/

#ifdef __cplusplus
extern "C" {
#endif

void
Biddy_SelectNode(Biddy_Edge f)
{
  if (((BiddyNode *) Biddy_GetPointer(f))->count >= 0)
    ((BiddyNode *) Biddy_GetPointer(f))->count =
      ~((BiddyNode *) Biddy_GetPointer(f))->count;
}

#ifdef __cplusplus
}
#endif

/**Function****************************************************************
  Synopsis    [Function Biddy_DeselectNode]
  Description [Biddy_DeselectNode deselect the top node of given edge.]
  SideEffects []
  SeeAlso     []
  ************************************************************************/

#ifdef __cplusplus
extern "C" {
#endif

void
Biddy_DeselectNode(Biddy_Edge f)
{
  if (((BiddyNode *) Biddy_GetPointer(f))->count < 0)
    ((BiddyNode *) Biddy_GetPointer(f))->count =
      ~((BiddyNode *) Biddy_GetPointer(f))->count;
}

#ifdef __cplusplus
}
#endif

/**Function****************************************************************
  Synopsis    [Function  Biddy_isSelected]
  Description [Biddy_isSelected return TRUE iff the top node of given edge
               is selected.]
  SideEffects []
  SeeAlso     []
  ************************************************************************/

#ifdef __cplusplus
extern "C" {
#endif

Biddy_Boolean
Biddy_isSelected(Biddy_Edge f)
{
  return (((BiddyNode *) Biddy_GetPointer(f))->count < 0);
}

#ifdef __cplusplus
}
#endif

/**Function****************************************************************
  Synopsis    [Function Biddy_FindTerminal]
  Description [Biddy_FindTerminal find a terminal node.
               Terminal node is a node with given variable and successors
               biddy_zero and biddy_one. If such terminal node
               does not exists, function returns biddy_null.]
  SideEffects [Variable table should use some kind of searching tree.]
  SeeAlso     [Biddy_FoaTerminal]
  ************************************************************************/

#ifdef __cplusplus
extern "C" {
#endif

Biddy_Edge
Biddy_FindTerminal(Biddy_String x)
{
  int i;
  Biddy_Boolean find;

  /* VARIABLE TABLE IS NEVER EMPTY. AT LEAST, THERE IS ELEMENT '1' AT INDEX [0] */
  i = 0;
  find = FALSE;
  while (!find && i<biddyVariableTable.size) {
    if (strcmp(x, biddyVariableTable.table[i].name)) {
      find = TRUE;
    } else {
      i++;
    }
  }

  /* IF NOT FOUND... */
  if (!find) return biddy_null;

  /* RETURN THE EDGE, WHICH HAS BEEN LOOKED FOR */
  return biddyVariableTable.table[i].term;
}

#ifdef __cplusplus
}
#endif

/**Function****************************************************************
  Synopsis    [Function Biddy_FoaTerminal]
  Description [Biddy_FoaTerminal find or add terminal node.
               Terminal node is a node with given variable and successors
               biddy_zero and biddy_one. If such terminal node already
               exists, function return it and does not create the new one.]
  SideEffects [Variable table should use some kind of searching tree.]
  SeeAlso     [Biddy_FindTerminal, Biddy_FoaNode]
  ************************************************************************/

#ifdef __cplusplus
extern "C" {
#endif

Biddy_Edge
Biddy_FoaTerminal(Biddy_String x)
{
  int i,n;
  Biddy_Boolean find;

  /* VARIABLE TABLE IS NEVER EMPTY. AT LEAST, THERE IS ELEMENT '1' AT INDEX [0] */
  i = 0;
  find = FALSE;
  while (!find && i<biddyVariableTable.size) {
    if (!strcmp(x, biddyVariableTable.table[i].name)) {
      find = TRUE;
    } else {
      i++;
    }
  }

  /* IF THE ELEMENT WAS NOT FOUND... */
  if (!find) {

    /*
    int j;
    for (j=0;j<biddyVariableTable.size;j++) {
      fprintf(stderr,"TABLE: %d %x = %x = %s\n",j,&biddyVariableTable.table[j],biddyVariableTable.table[j].name,biddyVariableTable.table[j].name);
    }
    */

    i = biddyVariableTable.size++;
    if (!(biddyVariableTable.table = (BiddyVariable *)
	  realloc(biddyVariableTable.table, biddyVariableTable.size * sizeof(BiddyVariable)))) {
      fprintf(stderr,"\nBIDDY (Biddy_FoaTerminal): Out of memory error!\n");
      fprintf(stderr,"Currently, there exist %d nodes.\n",biddyNodeTable.num);
      exit(1);
    }

    biddyVariableTable.table[i].name = strdup(x);
    n = biddyVariableTable.table[0].order;
    biddyVariableTable.table[i].order = n;
    biddyVariableTable.table[0].order = n+1;
    biddyVariableTable.table[i].term = Biddy_FoaNode(i, biddy_zero, biddy_one);
    ((BiddyNode *) Biddy_GetPointer(biddyVariableTable.table[i].term))->count = 0;
    biddyNodeTable.numf++;
    biddyVariableTable.table[i].value = FALSE;

  }

  /* RETURN THE EDGE, WHICH HAS BEEN LOOKED FOR */
  return biddyVariableTable.table[i].term;
}


#ifdef __cplusplus
}
#endif

/**Function****************************************************************
  Synopsis    [Function Biddy_FoaNode]
  Description [Biddy_FoaNode find or add new node with given variable
               and successors. If such node already exists, function
               return it and does not create the new one.]
  SideEffects [Using Biddy_FoaNode you can create node with bad ordering.
               It is much more safe to use Biddy_ITE.]
  SeeAlso     [Biddy_FoaTerminal, Biddy_ITE, Biddy_Garbage]
  ************************************************************************/

#ifdef __cplusplus
extern "C" {
#endif

Biddy_Edge
Biddy_FoaNode(Biddy_Variable v, Biddy_Edge pf, Biddy_Edge pt)
{
  unsigned long hash;
  Biddy_Edge edge;
  BiddyNode *sup, *sup1;
  Biddy_Boolean complementedResult;
  int i;

  biddyNodeTable.foa++;

  /* MINIMIZATION */

  if (Biddy_isEqv(pf,pt)) return pf;

  /* NORMALIZATION - EDGE 'THEN' MAY NOT BE COMPLEMENTED ! */
  if (Biddy_GetMark(pt)) {
    Biddy_InvertMark(pf);
    Biddy_ClearMark(pt);
    complementedResult = TRUE;
  } else {
    complementedResult = FALSE;
  }

  /* THIS STATEMENT ENABLES GARBAGE COLLECTION */
  /* IF THERE IS SOMETHING TO DELETE AND IF ALL GENERATED */
  /* NODES ARE USED THEN TRY GARBAGE COLLECTION           */
  /**/
  if ( (biddyNodeTable.num >= biddyNodeTable.generated) &&
       (biddyFirstNewNode != biddyFreshNodes) ) Biddy_Garbage();
  /**/

  /* THIS IS HASH FUNCTION FOR NODE TABLE */
  /* BECAUSE OF USED TRICKS, HASH FUNCTION MUST NEVER RETURN ZERO! */

  if (!(hash = (((unsigned long) v +
                 (uintptr_t) Biddy_Address(pf) +
                 (uintptr_t) Biddy_Address(pt)
               ) % biddyNodeTable.size))) hash=1;

  /* FIND OR ADD - THERE IS A HASH TABLE WITH CHAINING */

  sup = biddyNodeTable.table[hash];
  sup1 = NULL;

  /* ********************************************************************* */
  /* BEGIN OF OPTIONS                                                      */
  /* ********************************************************************* */

  /* ********************************************************************* */
  /* OPTION                                                                */
  /* USE THIS IF YOU HAVE ORDERED CHAINS IN HASH TABLE                     */
  /* IT MUST BE COMBINED WITH OPTION "INSERTING NODES AFTER sup1"          */
  /* ********************************************************************* */

  while (sup && (v < sup->v))
  {
    biddyNodeTable.compare++;
    sup1 = sup;
    sup = sup->next;
  }

  while (sup &&
          (v == sup->v) &&
          (!Biddy_isEqv(pf,sup->f) || !Biddy_isEqv(pt,sup->t))
        )

  /* ********************************************************************* */
  /* OPTION                                                                */
  /* USE THIS IF ORDERING IS NOT ASSURED                                   */
  /* FOR EXAMPLE, IF YOU INSERT AT THE BEGINNING OR ON THE END OF CHAIN    */
  /* ********************************************************************* */

  /*
  while (sup && (
          !Biddy_isEqv(pf,sup->f) ||
          !Biddy_isEqv(pt,sup->t) ||
          (v != sup->v)
        ))
  */

  /* ********************************************************************* */
  /* END OF OPTIONS                                                        */
  /* ********************************************************************* */

  {
    biddyNodeTable.compare++;
    sup1 = sup;
    sup = sup->next;
  }

  if ((!sup) || (v != sup->v)) {

    /* NEW NODE MUST BE ADDED */

    /* ONE FRESH NODE WILL BE ADDED */
    biddyNodeTable.num++;
    biddyNodeTable.add++;
    if (biddyNodeTable.num > biddyNodeTable.max)
      biddyNodeTable.max = biddyNodeTable.num;

    if (!biddyFreeNodes) {

      /* THESE LINES PREVENT ALLOCATION OF MEMORY AFTER INITIALIZATION */
      /*
      fprintf(stderr,"\nBIDDY (Biddy_FoaNode): Allocated node table is full!\n");
      fprintf(stderr,"Currently, there exist %d nodes.\n",biddyNodeTable.num);
      exit(1);
      */

      if (!(biddyFreeNodes = (BiddyNode *)
            malloc((biddyBlockSize) * sizeof(BiddyNode))))
      {
        fprintf(stderr,"\nBIDDY (Biddy_FoaNode): Out of memory error!\n");
        fprintf(stderr,"Currently, there exist %d nodes.\n",biddyNodeTable.num);
        exit(1);
      }

      biddyNodeTable.blocknumber++;
      if (!(biddyNodeTable.blocktable = (BiddyNode **) realloc(biddyNodeTable.blocktable,
            biddyNodeTable.blocknumber * sizeof(BiddyNode *))))
      {
        fprintf(stderr,"\nBIDDY (Biddy_FoaNode): Out of memory error!\n");
        fprintf(stderr,"Currently, there exist %d nodes.\n",biddyNodeTable.num);
        exit(1);
      }
      biddyNodeTable.blocktable[biddyNodeTable.blocknumber-1] = biddyFreeNodes;

      for (i=0; i<biddyBlockSize-1; i++) {
        biddyFreeNodes[i].list = &biddyFreeNodes[i+1];
      }
      biddyFreeNodes[biddyBlockSize-1].list = NULL;

      biddyNodeTable.generated = biddyNodeTable.generated + biddyBlockSize;
    }

    sup = biddyFreeNodes;
    biddyFreeNodes = sup->list;

    /* ******************************************************************* */
    /* OPTION                                                              */
    /* USE THIS TO INSERT NODE AFTER sup1                                  */
    /* IF sup1 = NULL THEN THIS WILL BE THE FIRST NODE IN A CHAIN          */
    /* ******************************************************************* */

    /* THE FIRST ELEMENT OF EACH CHAIN SHOULD HAVE A SPECIAL VALUE */
    /* FOR ITS PREV ELEMENT TO ALLOW TRICKY BUT EFFICIENT DELETING */

    sup->prev = sup1 ? sup1 : (BiddyNode *) &biddyNodeTable.table[hash-1];
    sup->next = sup->prev->next;
    sup->prev->next = sup;
    if (sup->next) sup->next->prev = sup;

    /* ******************************************************************* */
    /* OPTION                                                              */
    /* USE THIS TO INSERT NODE AT THE FIRST PLACE IN THE CHAIN             */
    /* ******************************************************************* */

    /* THE FIRST ELEMENT OF EACH CHAIN SHOULD HAVE A SPECIAL VALUE */
    /* FOR ITS PREV ELEMENT TO ALLOW TRICKY BUT EFFICIENT DELETING */

    /*
    sup->prev = (BiddyNode *) &biddyNodeTable.table[hash-1];
    sup->next = biddyNodeTable.table[hash];
    biddyNodeTable.table[hash] = sup;
    if (sup->next) sup->next->prev = sup;
    */

    /* ******************************************************************* */
    /* END OF OPTIONS                                                      */
    /* ******************************************************************* */

    biddyLastNewNode->list = sup;
    biddyLastNewNode = sup;

    /* BE CAREFULL !!!! */
    /* you can create node with bad ordering */

    sup->f = pf;
    sup->t = pt;
    sup->v = v;
    sup->count = biddyCount;

  }

  /* OLD IMPLEMENTATION */
  /*
  Biddy_SetPointer(edge,sup);
  if (complementedResult) Biddy_SetMark(edge); else Biddy_ClearMark(edge);
  */

  /* NEW IMPLEMENTATION */
  Biddy_SetPointer(edge,sup);
  if (complementedResult) Biddy_SetMark(edge);

  /* MAKE SURE THAT THE RETURNED EDGE IS NOT BAD */
  if (!Biddy_isOK(edge)) Biddy_Fresh(edge);

  return edge;
}

#ifdef __cplusplus
}
#endif

/**Function****************************************************************
  Synopsis    [Function Biddy_NOT]
  Description [Biddy_NOT complements Boolean function.]
  SideEffects []
  SeeAlso     [Biddy_ITE]
  ************************************************************************/

#ifdef __cplusplus
extern "C" {
#endif

Biddy_Edge
Biddy_NOT(Biddy_Edge f)
{
  Biddy_InvertMark(f);
  return f;
}

#ifdef __cplusplus
}
#endif

/**Function****************************************************************
  Synopsis    [Function Biddy_TransferMark]
  Description [Biddy_TransferMark complements Boolean function if
               given parameter mark is TRUE.]
  SideEffects []
  SeeAlso     []
  ************************************************************************/

#ifdef __cplusplus
extern "C" {
#endif

Biddy_Edge
Biddy_TransferMark(Biddy_Edge f, Biddy_Boolean mark)
{
  if (Biddy_GetMark(f) != mark) Biddy_SetMark(f); else Biddy_ClearMark(f);
  return f;
}

#ifdef __cplusplus
}
#endif

/**Function****************************************************************
  Synopsis    [Function Biddy_ITE]
  Description [Biddy_ITE calculates ITE operation of three Boolean functions]
  SideEffects []
  SeeAlso     [Biddy_NOT]
  ************************************************************************/

#ifdef __cplusplus
extern "C" {
#endif

Biddy_Edge
Biddy_ITE(Biddy_Edge f, Biddy_Edge g, Biddy_Edge h)
{
  Biddy_Boolean negation;
  Biddy_Edge r, T, E, Fv, Gv, Hv, Fneg_v, Gneg_v, Hneg_v;
  Biddy_Variable v;

  static Biddy_Boolean terminal; /* STATIC TO MINIMIZE STACK PROBLEMS */
  static Biddy_Variable topF;    /* STATIC TO MINIMIZE STACK PROBLEMS */
  static Biddy_Variable topG;    /* STATIC TO MINIMIZE STACK PROBLEMS */
  static Biddy_Variable topH;    /* STATIC TO MINIMIZE STACK PROBLEMS */

  /* DEBUGGING */
  /*
  static int mm = 0;
  int nn;
  nn = ++mm;
  fprintf(stdout,"Biddy_ITE (%d) F\n",nn);
  WriteBDD(f);
  fprintf(stdout,"\n");
  fprintf(stdout,"Biddy_ITE (%d) G\n",nn);
  WriteBDD(g);
  fprintf(stdout,"\n");
  fprintf(stdout,"Biddy_ITE (%d) H\n",nn);
  WriteBDD(h);
  fprintf(stdout,"\n");
  mm = nn;
  */

  if (!Biddy_Address(f)) {
    fprintf(stdout,"ERROR (Biddy_ITE): f = NULL!\n");
    sleep(5);
    exit(1);
  }

  if (!Biddy_Address(g)) {
    fprintf(stdout,"ERROR (Biddy_ITE): g = NULL!\n");
    sleep(5);
    exit(1);
  }

  if (!Biddy_Address(h)) {
    fprintf(stdout,"ERROR (Biddy_ITE): h = NULL!\n");
    sleep(5);
    exit(1);
  }

  if (!Biddy_isOK(f)) {
    fprintf(stdout,"ERROR (Biddy_ITE): Bad f\n");
    sleep(5);
    exit(1);
  }

  if (!Biddy_isOK(g)) {
    fprintf(stdout,"ERROR (Biddy_ITE): Bad g\n");
    sleep(5);
    exit(1);
  }

  if (!Biddy_isOK(h)) {
    fprintf(stdout,"ERROR (Biddy_ITE): Bad h\n");
    sleep(5);
    exit(1);
  }

  /* NORMALIZATION - STEP 1 */

  if (Biddy_isEqvPointer(f,g)) {
    if (!Biddy_isEqvPointerEqvEdge(f,g)) g = biddy_zero; else g = biddy_one;
  }

  if (Biddy_isEqvPointer(f,h)) {
    if (Biddy_isEqvPointerEqvEdge(f,h)) h = biddy_zero; else h = biddy_one;
  }

  negation = FALSE;

  /* LOOKING FOR TERMINAL CASE */

  terminal = FALSE;
  if (Biddy_isTerminal(f)) {
    terminal = TRUE;
    r = Biddy_GetMark(f) ? h : g;
  } else {
    if (Biddy_isEqvPointer(g,h)) {
      if (Biddy_isEqvPointerEqvEdge(g,h)) {
        terminal = TRUE;
        r = g;
      } else {
        if (Biddy_isTerminal(g)) {
          terminal = TRUE;
          r = f;
          negation = Biddy_GetMark(g);
        }
      }
    }
  }

  /* IF NOT TERMINAL CASE */
  if (!terminal) {

    /* NORMALIZATION - STEP 2 */

    if (Biddy_isTerminal(g)) {
      if ((uintptr_t) Biddy_Address(f) > (uintptr_t) Biddy_Address(h)) {
        if (Biddy_GetMark(g)) {
          complExchangeEdges(&f, &h);
        } else {
          exchangeEdges(&f, &h);
        }
      }
    }

    if (Biddy_isTerminal(h)) {
      if ((uintptr_t) Biddy_Address(f) > (uintptr_t) Biddy_Address(g)) {
        if (Biddy_GetMark(h)) {
          exchangeEdges(&f, &g);
        } else {
          complExchangeEdges(&f, &g);
        }
      }
    }

    /* WE KNOW, THAT G AND H ARE NOT EQUAL, BUT THEY CAN BE INVERTED! */
    if (Biddy_isEqvPointer(g,h)) {
      if ((uintptr_t) Biddy_Address(f) > (uintptr_t) Biddy_Address(g)) {
        h = f;
        f = g;
        g = h;
        Biddy_InvertMark(h);
      }
    }

    /* NORMALIZATION - STEP 3 */

    if (Biddy_GetMark(f)) {
      if (Biddy_GetMark(h)) {
        Biddy_ClearMark(f);
        negation = TRUE;
        complExchangeEdges(&g, &h);
      } else {
        Biddy_ClearMark(f);
        exchangeEdges(&g, &h);
      }
    } else {
      if (Biddy_GetMark(g)) {
        negation = TRUE;
        Biddy_ClearMark(g);
        Biddy_InvertMark(h);
      }
    }

    /* IF RESULT IS NOT IN THE CACHE TABLE... */
    if (!FindIteCache(f, g, h, &r)) {

      /* LOOKING FOR THE SMALLES TOP VARIABLE */
      topF = Biddy_GetVariable(f);
      topG = Biddy_GetVariable(g);
      topH = Biddy_GetVariable(h);

      if ((biddyVariableTable.table[topF].order < biddyVariableTable.table[topG].order) || Biddy_isTerminal(g)) {
       v = ((biddyVariableTable.table[topF].order < biddyVariableTable.table[topH].order) || Biddy_isTerminal(h)) ?
             topF : topH;
      } else {
       v = ((biddyVariableTable.table[topH].order < biddyVariableTable.table[topG].order) && !Biddy_isTerminal(h)) ?
             topH : topG;
      }

      /* DETERMINING PARAMETERS FOR RECURSIVE CALLS */
      if (topF == v) {
       if (Biddy_GetMark(f)) {
         Fv = Biddy_NOT(Biddy_GetThen(f));
         Fneg_v = Biddy_NOT(Biddy_GetElse(f));
       } else {
         Fv = Biddy_GetThen(f);
         Fneg_v = Biddy_GetElse(f);
       }
      } else {
       Fneg_v = Fv = f;
      }

      if (topG == v) {
        if (Biddy_GetMark(g)) {
         Gv = Biddy_NOT(Biddy_GetThen(g));
         Gneg_v = Biddy_NOT(Biddy_GetElse(g));
        } else {
         Gv = Biddy_GetThen(g);
         Gneg_v = Biddy_GetElse(g);
        }
      } else {
        Gneg_v = Gv = g;
      }

      if (topH == v) {
        if (Biddy_GetMark(h)) {
         Hv = Biddy_NOT(Biddy_GetThen(h));
         Hneg_v = Biddy_NOT(Biddy_GetElse(h));
        } else {
         Hv = Biddy_GetThen(h);
         Hneg_v = Biddy_GetElse(h);
        }
      } else {
        Hneg_v = Hv = h;
      }

      /* REKURSIVE CALLS */
      T = Biddy_ITE(Fv, Gv, Hv);
      E = Biddy_ITE(Fneg_v, Gneg_v, Hneg_v);

      r = Biddy_FoaNode(v, E, T);
      AddIteCache(f, g, h, r);

    } else {

      /* IF THE RESULT IS FROM CACHE TABLE, FRESH IT! */
      Biddy_Fresh(r);

    }
  }

  if (negation) {
    Biddy_InvertMark(r);
  }

  /* DEBUGGING */
  /*
  fprintf(stdout,"Biddy_ITE (%d) R\n",nn);
  if (terminal) fprintf(stdout,"TERMINAL CASE\n");
  WriteBDD(r);
  fprintf(stdout,"\n");
  */

  return r;
}

#ifdef __cplusplus
}
#endif

/**Function****************************************************************
  Synopsis    [Function Biddy_Restrict]
  Description [Biddy_Restrict calculates a restriction of Boolean function.
               Function does not use hash table.
               It uses optimization: F(A=x) == NOT((NOT F)(A=x))]
  SideEffects []
  SeeAlso     [Biddy_Compose, Biddy_E, Biddy_A]
  ************************************************************************/

#ifdef __cplusplus
extern "C" {
#endif

Biddy_Edge
Biddy_Restrict(Biddy_Edge f, Biddy_Variable v, Biddy_Boolean value)
{
  Biddy_Edge e, t, r;
  Biddy_Variable fv;

  if (!f) {
    fprintf(stdout,"ERROR (Biddy_Restrict): f = NULL!\n");
    sleep(5);
    exit(1);
  }

  if (!Biddy_isOK(f)) {
    fprintf(stdout,"ERROR (Biddy_Restrict): Bad f\n");
    sleep(5);
    exit(1);
  }

  if (Biddy_isTerminal(f)) return f;
  if (biddyVariableTable.table[fv=(Biddy_GetVariable(f))].order > biddyVariableTable.table[v].order) return f;
  if (fv == v) {
    if (value) return Biddy_TransferMark(Biddy_GetThen(f), Biddy_GetMark(f));
    else return Biddy_TransferMark(Biddy_GetElse(f), Biddy_GetMark(f));
  }

  e = Biddy_Restrict(Biddy_GetElse(f), v, value);
  t = Biddy_Restrict(Biddy_GetThen(f), v, value);
  r =  Biddy_TransferMark(Biddy_FoaNode(Biddy_GetVariable(f), e, t), Biddy_GetMark(f));

  return r;
}

#ifdef __cplusplus
}
#endif

/**Function****************************************************************
  Synopsis    [Function Biddy_Compose]
  Description [Biddy_Compose calculates a composition of two Boolean
               functions. Function does not use hash table.
               It uses optimization: F(A=G) == NOT((NOT F)(A=G))]
  SideEffects []
  SeeAlso     [Biddy_Restrict, Biddy_E, Biddy_A]
  ************************************************************************/

#ifdef __cplusplus
extern "C" {
#endif

Biddy_Edge
Biddy_Compose(Biddy_Edge f, Biddy_Variable v, Biddy_Edge g)
{
  Biddy_Edge e, t, r;
  Biddy_Variable fv;

  if (!f) {
    fprintf(stdout,"ERROR (Biddy_Compose): f = NULL!\n");
    sleep(5);
    exit(1);
  }

  if (!g) {
    fprintf(stdout,"ERROR (Biddy_Compose): g = NULL!\n");
    sleep(5);
    exit(1);
  }

  if (!Biddy_isOK(f)) {
    fprintf(stdout,"ERROR (Biddy_Compose): Bad f\n");
    sleep(5);
    exit(1);
  }

  if (!Biddy_isOK(g)) {
    fprintf(stdout,"ERROR (Biddy_Compose): Bad g\n");
    sleep(5);
    exit(1);
  }

  if (Biddy_isTerminal(f)) return f;
  if (biddyVariableTable.table[fv=(Biddy_GetVariable(f))].order > biddyVariableTable.table[v].order) return f;
  if (fv == v) {
    return Biddy_TransferMark(Biddy_ITE(g, Biddy_GetThen(f), Biddy_GetElse(f)), Biddy_GetMark(f));
  }

  e = Biddy_Compose(Biddy_GetElse(f), v, g);
  t = Biddy_Compose(Biddy_GetThen(f), v, g);
  r = Biddy_TransferMark(Biddy_FoaNode(Biddy_GetVariable(f), e, t), Biddy_GetMark(f));

  return r;
}

#ifdef __cplusplus
}
#endif

/**Function****************************************************************
  Synopsis    [Function Biddy_E]
  Description [Biddy_E calculates an existential quantification of
               Boolean function. Function does not use hash table.
               Be careful: ExA F != NOT(ExA (NOT F))
               counterexample Exb (AND (NOT a) b c)]
  SideEffects []
  SeeAlso     [Biddy_Restrict, Biddy_Compose, Biddy_A]
  ************************************************************************/

#ifdef __cplusplus
extern "C" {
#endif

Biddy_Edge
Biddy_E(Biddy_Edge f, Biddy_Variable v)
{
  Biddy_Edge e, t, r;
  Biddy_Variable fv;

  if (!f) {
    fprintf(stdout,"ERROR (Biddy_E): f = NULL!\n");
    sleep(5);
    exit(1);
  }

  if (!Biddy_isOK(f)) {
    fprintf(stdout,"ERROR (Biddy_E): Bad f\n");
    sleep(5);
    exit(1);
  }

  /* LOOKING FOR TERMINAL CASE */

  if (Biddy_isTerminal(f)) return f;
  if (biddyVariableTable.table[fv=(Biddy_GetVariable(f))].order > biddyVariableTable.table[v].order) return f;

  /* THE USAGE OF EAX CACHE IS NOT IMPLEMENTED, YET */

  if (fv == v) {
    return Biddy_ITE(Biddy_TransferMark(Biddy_GetThen(f), Biddy_GetMark(f)),
                     biddy_one,
                     Biddy_TransferMark(Biddy_GetElse(f), Biddy_GetMark(f)));
  }

  e = Biddy_E(Biddy_TransferMark(Biddy_GetElse(f),Biddy_GetMark(f)), v);
  t = Biddy_E(Biddy_TransferMark(Biddy_GetThen(f),Biddy_GetMark(f)), v);
  r = Biddy_ITE(biddyVariableTable.table[fv].term,t,e);

  return r;
}

#ifdef __cplusplus
}
#endif

/**Function****************************************************************
  Synopsis    [Function Biddy_A]
  Description [Biddy_A calculates an universal quantification of
               Boolean function. Function does not use hash table.
               Be careful: AxA F != NOT(AxA (NOT F))
               counterexample Axb (AND (NOT a) b c)]
  SideEffects []
  SeeAlso     [Biddy_Restrict, Biddy_Compose, Biddy_A]
  ************************************************************************/

#ifdef __cplusplus
extern "C" {
#endif

Biddy_Edge
Biddy_A(Biddy_Edge f, Biddy_Variable v)
{
  Biddy_Edge e, t, r;
  Biddy_Variable fv;

  if (!f) {
    fprintf(stdout,"ERROR (Biddy_A): f = NULL!\n");
    sleep(5);
    exit(1);
  }

  if (!Biddy_isOK(f)) {
    fprintf(stdout,"ERROR (Biddy_A): Bad f\n");
    sleep(5);
    exit(1);
  }

  /* LOOKING FOR TERMINAL CASE */

  if (Biddy_isTerminal(f)) return f;
  if (biddyVariableTable.table[fv=(Biddy_GetVariable(f))].order > biddyVariableTable.table[v].order) return f;

  /* THE USAGE OF EAX CACHE IS NOT IMPLEMENTED, YET */

  if (fv == v) {
    return Biddy_ITE(Biddy_TransferMark(Biddy_GetThen(f), Biddy_GetMark(f)),
                     Biddy_TransferMark(Biddy_GetElse(f), Biddy_GetMark(f)),
                     biddy_zero);
  }

  e = Biddy_A(Biddy_TransferMark(Biddy_GetElse(f),Biddy_GetMark(f)), v);
  t = Biddy_A(Biddy_TransferMark(Biddy_GetThen(f),Biddy_GetMark(f)), v);
  r = Biddy_ITE(biddyVariableTable.table[fv].term,t,e);

  return r;
}

#ifdef __cplusplus
}
#endif

/**Function****************************************************************
  Synopsis    [Function Biddy_IncCounter]
  Description [Biddy_IncCounter increment variable biddyCount by 1.]
  SideEffects []
  SeeAlso     [Biddy_Garbage]
  ************************************************************************/

#ifdef __cplusplus
extern "C" {
#endif

void
Biddy_IncCounter()
{
  if (++biddyCount <= 0) {
    fprintf(stdout,"WARNING: biddyCount is back to 1");
    biddyCount = 1;
  }
  biddyFreshNodes = biddyLastNewNode;
}

#ifdef __cplusplus
}
#endif

/**Function****************************************************************
  Synopsis    [Function Biddy_Fresh]
  Description [Biddy_Fresh fresh all non-fortified nodes in a function.]
  SideEffects []
  SeeAlso     [Biddy_Garbage]
  ************************************************************************/

#ifdef __cplusplus
extern "C" {
#endif

void
Biddy_Fresh(Biddy_Edge f)
{
  if (!Biddy_isOK(f)) {
    ((BiddyNode *) Biddy_GetPointer(f))->count = biddyCount;
    Biddy_Fresh(Biddy_GetElse(f));
    Biddy_Fresh(Biddy_GetThen(f));
  }
}

#ifdef __cplusplus
}
#endif

/**Function****************************************************************
  Synopsis    [Function Biddy_Fortify]
  Description [Biddy_Fortify fortify all nodes in a function.]
  SideEffects []
  SeeAlso     [Biddy_Garbage]
  ************************************************************************/

#ifdef __cplusplus
extern "C" {
#endif

void
Biddy_Fortify(Biddy_Edge f)
{
  if (((BiddyNode *) Biddy_GetPointer(f))->count) {
    ((BiddyNode *) Biddy_GetPointer(f))->count = 0;
    biddyNodeTable.numf++;
    Biddy_Fortify(Biddy_GetElse(f));
    Biddy_Fortify(Biddy_GetThen(f));
  }
}

#ifdef __cplusplus
}
#endif

/**Function****************************************************************
  Synopsis    [Function Biddy_Garbage]
  Description [Biddy_Garbage perform garbage collection.]
  SideEffects [The first element of each chain in a node table
               should have a special value for its 'prev' element to
               allow tricky but efficient deleting. Moreover,
               'prev' and 'next' should be the first and the second
               element in the structure BiddyNode, respectively.]
  SeeAlso     [Biddy_FoaNode]
  ************************************************************************/

#ifdef __cplusplus
extern "C" {
#endif

void
Biddy_Garbage()
{
  BiddyNode *sup;
  BiddyCacheList *c;

  /*
  static int n = 0;
  fprintf(stdout,"Garbage (%d)...\n",++n);
  */

  /* BE CAREFULLY WITH TYPE BiddyNode !!!! */
  /* THIS COMPUTATION USE SOME TRICKS! */
  /* FIELDS 'prev' AND 'next' HAVE TO BE THE FIRST AND THE SECOND! */

  if (biddyFirstNewNode != biddyFreshNodes) {
    biddyNodeTable.garbage++;

    /* Updating cache tables */
    c = biddyCacheList;
    while (c) {
      c->gc();
      c = c->next;
    }

    /* Updating unique table */
    while (biddyFirstNewNode != biddyFreshNodes) {
      if (!biddyFirstNewNode->count) { /* fortified node */
        /* NOT IMPLEMENTED */
        /* fortified nodes should be added to List of fortified nodes */
        biddyFirstNewNode = biddyFirstNewNode->list;
      } else {
        if (biddyFirstNewNode->count == biddyCount) { /* fresh node */
          biddyLastNewNode->list = biddyFirstNewNode;
          biddyLastNewNode = biddyFirstNewNode;
          biddyFirstNewNode = biddyFirstNewNode->list;
        } else { /* bad node */
          biddyFirstNewNode->prev->next = biddyFirstNewNode->next;
          if (biddyFirstNewNode->next)
            biddyFirstNewNode->next->prev = biddyFirstNewNode->prev;
          sup = biddyFirstNewNode->list;
          biddyFirstNewNode->list = biddyFreeNodes;
          biddyFreeNodes = biddyFirstNewNode;
          biddyFirstNewNode = sup;
          biddyNodeTable.num--;
        }
      }
    }

  }
}

#ifdef __cplusplus
}
#endif

/**Function****************************************************************
  Synopsis    [Function Biddy_AddCache]
  Description [Biddy_AddCache add cache to the end of Cache list]
  SideEffects [If biddyCacheList does not exist, function create it]
  SeeAlso     [Biddy_Garbage]
  ************************************************************************/

#ifdef __cplusplus
extern "C" {
#endif

void
Biddy_AddCache(Biddy_VoidFunction gc)
{
  BiddyCacheList *sup;

  if (!biddyCacheList) {

    if (!(biddyCacheList = (BiddyCacheList *) malloc(sizeof(BiddyCacheList)))) {
      fprintf(stdout,"Biddy_AddCache: Out of memoy!\n");
      exit(1);
    }

    biddyCacheList->gc = gc;
    biddyCacheList->next = NULL;
  } else {
    sup = biddyCacheList;
    while (sup->next) sup = sup->next;

    if (!(sup->next = (BiddyCacheList *) malloc(sizeof(BiddyCacheList)))) {
      fprintf(stdout,"Biddy_AddCache: Out of memoy!\n");
      exit(1);
    }

    sup = sup->next;
    sup->gc = gc;
    sup->next = NULL;
  }
}

#ifdef __cplusplus
}
#endif

/**Function****************************************************************
  Synopsis    [Function Biddy_NodeSelect]
  Description [Function Biddy_NodeSelect recursively selects all nodes of a
               given function.]
  SideEffects []
  SeeAlso     [Biddy_NodeRepair, Biddy_Garbage]
  ************************************************************************/

#ifdef __cplusplus
extern "C" {
#endif

void
Biddy_NodeSelect(Biddy_Edge f)
{
  if (!Biddy_isNull(f)) {
    Biddy_SelectNode(f);
    if (!Biddy_isNull(Biddy_GetElse(f)) && !Biddy_isSelected(Biddy_GetElse(f)))
      Biddy_NodeSelect(Biddy_GetElse(f));
    if (!Biddy_isNull(Biddy_GetThen(f)) && !Biddy_isSelected(Biddy_GetThen(f)))
      Biddy_NodeSelect(Biddy_GetThen(f));
  }
}

#ifdef __cplusplus
}
#endif

/**Function****************************************************************
  Synopsis    [Function Biddy_NodeRepair]
  Description [Selected nodes used for statistics and some garbage collection
               techinques has an invalid count and has to be repaired before
               any operation on BDDs. Function Biddy_NodeRepair recursively
               repairs (deselects) all nodes of a given function.]
  SideEffects []
  SeeAlso     [Biddy_NodeSelect, Biddy_Garbage]
  ************************************************************************/

#ifdef __cplusplus
extern "C" {
#endif

void
Biddy_NodeRepair(Biddy_Edge f)
{
  if (!Biddy_isNull(f)) {
    Biddy_DeselectNode(f);
    if (!Biddy_isNull(Biddy_GetElse(f)) && Biddy_isSelected(Biddy_GetElse(f)))
      Biddy_NodeRepair(Biddy_GetElse(f));
    if (!Biddy_isNull(Biddy_GetThen(f)) && Biddy_isSelected(Biddy_GetThen(f)))
      Biddy_NodeRepair(Biddy_GetThen(f));
  }
}

#ifdef __cplusplus
}
#endif

/*-----------------------------------------------------------------------*/
/* Definition of internal functions                                      */
/*-----------------------------------------------------------------------*/

/*-----------------------------------------------------------------------*/
/* Definition of static functions                                        */
/*-----------------------------------------------------------------------*/

/**Function****************************************************************
  Synopsis    [Function deleteVariableTable]
  Description [deleteVariableTable free all memory used by variable table.]
  SideEffects []
  SeeAlso     []
  ************************************************************************/

static void
deleteVariableTable(BiddyVariableTable table)
{
}

/**Function****************************************************************
  Synopsis    [Function exchangeEdges]
  Description [exchangeEdges exchange two Boolean functions.]
  SideEffects []
  SeeAlso     []
  ************************************************************************/

static void
exchangeEdges(Biddy_Edge *f, Biddy_Edge *g)
{
  Biddy_Edge sup;

  sup = *f;
  *f = *g;
  *g = sup;
}

/**Function****************************************************************
  Synopsis    [Function complExchangeEdges]
  Description [complExchangeEdges complements and exchange two Boolean
               functions.]
  SideEffects []
  SeeAlso     []
  ************************************************************************/

static void
complExchangeEdges(Biddy_Edge *f, Biddy_Edge *g)
{
  Biddy_Edge sup;

  sup = *f;
  *f = *g;
  *g = sup;
  Biddy_InvertMark((*f));
  Biddy_InvertMark((*g));
}

/**Function****************************************************************
  Synopsis    [Function AddIteCache]
  Description [AddIteCache add parameters and result to the ITE cache.]
  SideEffects []
  SeeAlso     []
  ************************************************************************/

static void
AddIteCache(Biddy_Edge a, Biddy_Edge b, Biddy_Edge c, Biddy_Edge r)
{
  int indeks;
  BiddyIteCache *p;

  /* THIS IS HASH FUNCTION */
  /* MUST BE THE SAME AS IN FindIteCache! */

  indeks = (
             ((uintptr_t) Biddy_GetPointer(a) << 16) +
             ((uintptr_t) Biddy_GetPointer(b) << 8) +
             ((uintptr_t) Biddy_GetPointer(c))
           ) % biddyIteCache.size;

  p = &biddyIteCache.table[indeks];
  if (!p->ok) {
    p->ok = TRUE;
    p->f = (BiddyNode *) Biddy_GetPointer(a);
    p->g = (BiddyNode *) Biddy_GetPointer(b);
    p->h = (BiddyNode *) Biddy_GetPointer(c);
    p->hmark = Biddy_GetMark(c);
    p->result = r;
  }

#ifdef MINACC
  else if (p->f->count || p->g->count || p->h->count || p->result.p->count) {
    biddyIteCache.overwrite++;
    p->f = (BiddyNode *) Biddy_GetPointer(a);
    p->g = (BiddyNode *) Biddy_GetPointer(b);
    p->h = (BiddyNode *) Biddy_GetPointer(c);
    p->hmark = Biddy_GetMark(c);
    p->result = r;
  }
#endif

#ifdef MAXACC
  else {
    biddyIteCache.overwrite++;
    p->f = (BiddyNode *) Biddy_GetPointer(a);
    p->g = (BiddyNode *) Biddy_GetPointer(b);
    p->h = (BiddyNode *) Biddy_GetPointer(c);
    p->hmark = Biddy_GetMark(c);
    p->result = r;
  }
#endif

}

/**Function****************************************************************
  Synopsis    [Function FindIteCache]
  Description [FindIteCache looks for result in the ITE cache.]
  SideEffects []
  SeeAlso     []
  ************************************************************************/

static Biddy_Boolean
FindIteCache(Biddy_Edge a, Biddy_Edge b, Biddy_Edge c, Biddy_Edge *r)
{
  int indeks;
  BiddyIteCache *p;

  /* THIS IS HASH FUNCTION */
  /* MUST BE THE SAME AS IN AddIteCache! */

  indeks = (
             ((uintptr_t) Biddy_GetPointer(a) << 16) +
             ((uintptr_t) Biddy_GetPointer(b) << 8) +
             ((uintptr_t) Biddy_GetPointer(c))
           ) % biddyIteCache.size;

  biddyIteCache.search++;
  p = &biddyIteCache.table[indeks];

  if (p->ok && p->f == Biddy_GetPointer(a) &&
               p->g == Biddy_GetPointer(b) &&
               p->h == Biddy_GetPointer(c) &&
               p->hmark == Biddy_GetMark(c))
  {
    biddyIteCache.find++;
    *r = p->result;
    return TRUE;
  }
  return FALSE;
}

/**Function****************************************************************
  Synopsis    [Function IteGarbage.]
  Description [IteGarbage performs Garbage Collection for ITE cache]
  SideEffects []
  SeeAlso     []
  ************************************************************************/

static void
IteGarbage()
{
  int j;
  BiddyIteCache *c;

  for (j=0; j<biddyIteCache.size; j++) {
    if (biddyIteCache.table[j].ok) {
      c = &biddyIteCache.table[j];
      if ((c->f->count && c->f->count != biddyCount) ||
          (c->g->count && c->g->count != biddyCount) ||
          (c->h->count && c->h->count != biddyCount) ||
          !Biddy_isOK(c->result))
      {
        c->ok = FALSE;
      }
    }
  }
}

/**Function****************************************************************
  Synopsis    [Function AddEAxCache]
  Description [AddEAxCache add parameters and result to the EAX cache.]
  SideEffects []
  SeeAlso     []
  ************************************************************************/

static void
AddEAxCache(Biddy_Edge f, Biddy_Variable v, Biddy_Edge r)
{
  /* NOT IMPLEMENTED, YET */
}

/**Function****************************************************************
  Synopsis    [Function FindEAxCache]
  Description [FindEAxCache looks for result in the EAX cache.]
  SideEffects []
  SeeAlso     []
  ************************************************************************/

static Biddy_Boolean
FindEAxCache(Biddy_Edge f, Biddy_Variable v, Biddy_Edge *r)
{
  /* NOT IMPLEMENTED, YET */
  return FALSE;
}


/**Function****************************************************************
  Synopsis    [Function EAxGarbage.]
  Description [EAxGarbage performs Garbage Collection for EAx cache]
  SideEffects []
  SeeAlso     []
  ************************************************************************/

static void
EAxGarbage()
{
  int j;
  BiddyEAxCache *c;

  for (j=0; j<biddyEAxCache.size; j++) {
    if (biddyEAxCache.table[j].ok) {
      c = &biddyEAxCache.table[j];
      if ((c->f->count && c->f->count != biddyCount) ||
          !Biddy_isOK(c->result))
      {
        c->ok = FALSE;
      }
    }
  }
}

/*-----------------------------------------------------------------------*/
/* DEBUGGING                                                             */
/*-----------------------------------------------------------------------*/

static void
WriteBDD(Biddy_Edge f)
{
  if (Biddy_GetMark(f)) {
    fprintf(stdout,"* ");
  }
  fprintf(stdout,"%s",Biddy_GetVariableName(f));
  if (!Biddy_isTerminal(f)) {
    fprintf(stdout," (");
    WriteBDD(Biddy_GetElse(f));
    fprintf(stdout,") (");
    WriteBDD(Biddy_GetThen(f));
    fprintf(stdout,")");
  }
}
