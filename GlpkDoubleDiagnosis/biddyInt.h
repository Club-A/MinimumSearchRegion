/**CHeaderFile*************************************************************
  PackageName [Biddy]
  Synopsis    [Biddy provides data structures and algorithms
               for the representation and manipulation of Boolean
               functions with ROBDDs. A hash table is used for quick
               search of nodes. Complement edges decreases the number
               of nodes. Function ITE is given for manipulation of
               boolean functions. An automatic garbage collection with
               a formulae counter is implemented.]

  FileName    [biddyInt.h]
  Revision    [$Revision: 53 $]
  Date        [$Date: 2013-04-29 08:28:44 +0200 (pon, 29 apr 2013) $]
  Authors     [Robert Meolic (meolic@uni-mb.si),
               Ales Casar (casar@uni-mb.si)]
  Description [File biddyInt.h contains declaration of internal
               data structures.]
  SeeAlso     [biddy.h]

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

#ifndef _BIDDYINT
#define _BIDDYINT

#include "biddy.h"

/* sleep() is not included in modern version of MINGW */
/* #define sleep(sec) (Sleep ((sec) * 1000), 0) */

#ifdef WIN32
#define sleep(sec) 0
#endif

/*-----------------------------------------------------------------------*/
/* Constant declarations                                                 */
/*-----------------------------------------------------------------------*/

/*-----------------------------------------------------------------------*/
/* Macro definitions                                                     */
/*-----------------------------------------------------------------------*/

/* Biddy_Address returns the pointer used with the given edge */

/* #define Biddy_Address(f) (f.p) */
#define Biddy_Address(f) (f)

/*-----------------------------------------------------------------------*/
/* Type declarations                                                     */
/*-----------------------------------------------------------------------*/

/*-----------------------------------------------------------------------*/
/* Structure declarations                                                */
/*-----------------------------------------------------------------------*/

/* NODE TABLE = a hash table with chaining */
typedef struct BiddyNode {
  struct BiddyNode *prev, *next; /* !!!MUST BE FIRST AND SECOND */
  struct BiddyNode *list;        /* list of nodes (previously named new) */
  Biddy_Edge f, t;               /* f = else, t = then          */
  Biddy_Variable v;              /* variable                    */
  short int count;               /* formulae counter            */
} BiddyNode;

typedef struct {
  BiddyNode **table;
  BiddyNode **blocktable;    /* table of allocated memory blocks */
  unsigned long int foa;     /* number of calls to Biddy_FoaNode */
  unsigned long int compare; /* number of comparisons made by Biddy_FoaNode */
  unsigned long int add;     /* number of adds made by Biddy_FoaNode */
  unsigned int size;         /* size of node table */
  unsigned int max;          /* maximal number of nodes in node table */
  unsigned int num;          /* number of nodes currently in node table */
  unsigned int numf;         /* number of fortified nodes */
  unsigned int garbage;      /* number of garbage collections */
  unsigned int generated;    /* number of free nodes generated */
  unsigned int blocknumber;  /* number of allocated memory blocks */
} BiddyNodeTable;

/* VARIABLE TABLE = dynamicaly allocated table */
typedef struct {
  Biddy_String name;       /* name of variable                    */
  Biddy_Edge term;         /* pointer to terminal                 */
  int order;               /* order of variable != Biddy_Variable */
  Biddy_Boolean value;     /* value = 0,1                         */
} BiddyVariable;

typedef struct {
  BiddyVariable *table;
  unsigned short int size;       /* size of variable table */
} BiddyVariableTable;

/* CACHE LIST = unidirectional list */
typedef struct BiddyCacheList {
  struct BiddyCacheList *next;
  Biddy_VoidFunction gc;
} BiddyCacheList;

/* ITE Cache = a fixed-size hash table */
typedef struct {
  BiddyNode *f, *g, *h;
  Biddy_Edge result;
  Biddy_Boolean hmark;
  Biddy_Boolean ok;     /* FALSE = not valid record! */
} BiddyIteCache;

typedef struct {
  BiddyIteCache *table;
  unsigned int size;
  unsigned long int search;
  unsigned long int find;
  unsigned long int overwrite;
} BiddyIteCacheTable;

/* EAX Cache = a fixed-size hash table */
typedef struct {
  BiddyNode *f;
  Biddy_Edge result;
  BiddyVariable v;
  Biddy_Boolean fmark;
  Biddy_Boolean ok;     /* FALSE = not valid record! */
} BiddyEAxCache;

typedef struct {
  BiddyEAxCache *table;
  unsigned int size;
  unsigned long int search;
  unsigned long int find;
  unsigned long int overwrite;
} BiddyEAxCacheTable;

/*-----------------------------------------------------------------------*/
/* Variable declarations                                                 */
/*-----------------------------------------------------------------------*/

extern short int biddyCount;

extern unsigned int biddyBlockSize;

extern unsigned int biddyIteCacheSize;

extern unsigned int biddyEAxCacheSize;

extern BiddyNodeTable biddyNodeTable;

extern BiddyVariableTable biddyVariableTable;

extern BiddyCacheList *biddyCacheList;

extern BiddyNode *biddyFreeNodes;

extern BiddyNode *biddyFirstNewNode;

extern BiddyNode *biddyLastNewNode;

extern BiddyNode *biddyFreshNodes;

extern BiddyNode *biddyFortifiedNodes;

extern BiddyIteCacheTable biddyIteCache;

extern BiddyEAxCacheTable biddyEAxCache;

/**AutomaticStart*********************************************************/

/*-----------------------------------------------------------------------*/
/* Function prototypes                                                   */
/*-----------------------------------------------------------------------*/

/*-----------------------------------------------------------------------*/
/* INTERNAL FUNCTIONS DEFINED IN biddyMain.c                             */
/*-----------------------------------------------------------------------*/

extern void BiddyExchange(Biddy_Edge *f, Biddy_Edge *g);

extern void BiddyExchangeNeg(Biddy_Edge *f, Biddy_Edge *g);

/*-----------------------------------------------------------------------*/
/* INTERNAL FUNCTIONS DEFINED IN biddyStat.c                             */
/*-----------------------------------------------------------------------*/

/**AutomaticEnd***********************************************************/

#endif  /* _BIDDYINT */
