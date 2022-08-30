/**CFile*******************************************************************
  PackageName [Biddy]
  Synopsis    [Biddy provides data structures and algorithms
               for the representation and manipulation of Boolean
               functions with ROBDDs. A hash table is used for quick
               search of nodes. Complement edges decreases the number
               of nodes. Function ITE is given for manipulation of
               boolean functions. An automatic garbage collection with
               a formulae counter is implemented.]

  FileName    [bddStat.c]
  Revision    [$Revision: 51 $]
  Date        [$Date: 2013-04-03 17:57:12 +0200 (sre, 03 apr 2013) $]
  Authors     [Robert Meolic (meolic@uni-mb.si),
               Ales Casar (casar@uni-mb.si)]
  Description [The file bddStat.c contains statistical functions.]
  SeeAlso     [bdd.h, bddInt.h]

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

#include "StdAfx.h"
#include "biddyInt.h"

/**AutomaticStart*********************************************************/

/*-----------------------------------------------------------------------*/
/* Static function prototypes                                            */
/*-----------------------------------------------------------------------*/

static void NodeNumber(Biddy_Edge f, unsigned int *i);

static void MaxLevel(Biddy_Edge f, unsigned int *max, unsigned int i);

static void AvgLevel(Biddy_Edge f, float *sum, unsigned int *n, unsigned int i);

/**AutomaticEnd***********************************************************/

/*-----------------------------------------------------------------------*/
/* Definition of exported functions                                      */
/*-----------------------------------------------------------------------*/

/**Function****************************************************************
  Synopsis    []
  Description []
  SideEffects []
  SeeAlso     []
  ************************************************************************/

#ifdef __cplusplus
extern "C" {
#endif

unsigned int
Biddy_NodeTableSize()
{
  return biddyNodeTable.size;
}

#ifdef __cplusplus
}
#endif

/**Function****************************************************************
  Synopsis    []
  Description []
  SideEffects []
  SeeAlso     []
  ************************************************************************/

#ifdef __cplusplus
extern "C" {
#endif

unsigned int
Biddy_NodeTableMax()
{
  return biddyNodeTable.max;
}

#ifdef __cplusplus
}
#endif

/**Function****************************************************************
  Synopsis    []
  Description []
  SideEffects []
  SeeAlso     []
  ************************************************************************/

#ifdef __cplusplus
extern "C" {
#endif

unsigned int
Biddy_NodeTableNum()
{
  return biddyNodeTable.num;
}

#ifdef __cplusplus
}
#endif

/**Function****************************************************************
  Synopsis    []
  Description []
  SideEffects []
  SeeAlso     []
  ************************************************************************/

#ifdef __cplusplus
extern "C" {
#endif

unsigned int
Biddy_NodeTableNumF()
{
  return biddyNodeTable.numf;
}

#ifdef __cplusplus
}
#endif

/**Function****************************************************************
  Synopsis    []
  Description []
  SideEffects []
  SeeAlso     []
  ************************************************************************/

#ifdef __cplusplus
extern "C" {
#endif

unsigned long int
Biddy_NodeTableFOA()
{
  return biddyNodeTable.foa;
}

#ifdef __cplusplus
}
#endif

/**Function****************************************************************
  Synopsis    []
  Description []
  SideEffects []
  SeeAlso     []
  ************************************************************************/

#ifdef __cplusplus
extern "C" {
#endif

unsigned long int
Biddy_NodeTableCompare()
{
  return biddyNodeTable.compare;
}

#ifdef __cplusplus
}
#endif

/**Function****************************************************************
  Synopsis    []
  Description []
  SideEffects []
  SeeAlso     []
  ************************************************************************/

#ifdef __cplusplus
extern "C" {
#endif

unsigned long int
Biddy_NodeTableAdd()
{
  return biddyNodeTable.add;
}

#ifdef __cplusplus
}
#endif

/**Function****************************************************************
  Synopsis    []
  Description []
  SideEffects []
  SeeAlso     []
  ************************************************************************/

#ifdef __cplusplus
extern "C" {
#endif

unsigned int
Biddy_NodeTableGarbage()
{
  return biddyNodeTable.garbage;
}

#ifdef __cplusplus
}
#endif

/**Function****************************************************************
  Synopsis    []
  Description []
  SideEffects []
  SeeAlso     []
  ************************************************************************/

#ifdef __cplusplus
extern "C" {
#endif

unsigned int
Biddy_NodeTableGenerated()
{
  return biddyNodeTable.generated;
}

#ifdef __cplusplus
}
#endif

/**Function****************************************************************
  Synopsis    []
  Description []
  SideEffects []
  SeeAlso     []
  ************************************************************************/

#ifdef __cplusplus
extern "C" {
#endif

unsigned int
Biddy_NodeTableBlockNumber()
{
  return biddyNodeTable.blocknumber;
}

#ifdef __cplusplus
}
#endif

/**Function****************************************************************
  Synopsis    []
  Description []
  SideEffects []
  SeeAlso     []
  ************************************************************************/

#ifdef __cplusplus
extern "C" {
#endif

unsigned int
Biddy_ListUsed()
{
  unsigned int i,n;

  n = 0;
  for (i=0; i<biddyNodeTable.size; i++) {
    if (biddyNodeTable.table[i] != NULL) {
      n++;
    }
  }
  return n;
}

#ifdef __cplusplus
}
#endif

/**Function****************************************************************
  Synopsis    []
  Description []
  SideEffects []
  SeeAlso     []
  ************************************************************************/

#ifdef __cplusplus
extern "C" {
#endif

unsigned int
Biddy_ListMaxLength()
{
  unsigned int i,n,max;
  BiddyNode *sup;

  max = 0;
  for (i=0; i<biddyNodeTable.size; i++) {
    if (biddyNodeTable.table[i] != NULL) {
      n = 0;
      sup = biddyNodeTable.table[i];
      while (sup != NULL) {
        n++;
        sup = sup->next;
      }
      if (n > max) max = n;
    }
  }
  return max;
}

#ifdef __cplusplus
}
#endif

/**Function****************************************************************
  Synopsis    []
  Description []
  SideEffects []
  SeeAlso     []
  ************************************************************************/

#ifdef __cplusplus
extern "C" {
#endif

float
Biddy_ListAvgLength()
{
  float sum;
  unsigned int i,n;
  BiddyNode *sup;

  sum = 0;
  n = 0;
  for (i=0; i<biddyNodeTable.size; i++) {
    if (biddyNodeTable.table[i] != NULL) {
      n++;
      sup = biddyNodeTable.table[i];
      while (sup != NULL) {
        sum = sum + 1;
        sup = sup->next;
      }
    }
  }
  return (sum/n);
}

#ifdef __cplusplus
}
#endif

/**Function****************************************************************
  Synopsis    []
  Description []
  SideEffects []
  SeeAlso     []
  ************************************************************************/

#ifdef __cplusplus
extern "C" {
#endif

unsigned long int
Biddy_IteCacheFind()
{
  return biddyIteCache.find;
}

#ifdef __cplusplus
}
#endif

/**Function****************************************************************
  Synopsis    []
  Description []
  SideEffects []
  SeeAlso     []
  ************************************************************************/

#ifdef __cplusplus
extern "C" {
#endif

unsigned long int
Biddy_IteCacheOverwrite()
{
  return biddyIteCache.overwrite;
}

#ifdef __cplusplus
}
#endif

/**Function****************************************************************
  Synopsis    []
  Description []
  SideEffects []
  SeeAlso     []
  ************************************************************************/

#ifdef __cplusplus
extern "C" {
#endif

unsigned int
Biddy_NodeNumber(Biddy_Edge f)
{
  unsigned int i;

  i = 0;
  NodeNumber(f,&i);
  Biddy_NodeRepair(f);
  return i;
}

#ifdef __cplusplus
}
#endif

/**Function****************************************************************
  Synopsis    []
  Description []
  SideEffects []
  SeeAlso     []
  ************************************************************************/

#ifdef __cplusplus
extern "C" {
#endif

unsigned int
Biddy_NodeMaxLevel(Biddy_Edge f)
{
  unsigned int max;

  max = 0;
  MaxLevel(f,&max,0);
  return max;
}

#ifdef __cplusplus
}
#endif

/**Function****************************************************************
  Synopsis    []
  Description []
  SideEffects []
  SeeAlso     []
  ************************************************************************/

#ifdef __cplusplus
extern "C" {
#endif

float
Biddy_NodeAvgLevel(Biddy_Edge f)
{
  float sum;
  unsigned int n;

  sum = 0;
  n = 0;
  AvgLevel(f,&sum,&n,0);
  return (sum/n);
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

static void
NodeNumber(Biddy_Edge f, unsigned int *i)
{
  if (!Biddy_isNull(f)) {
    Biddy_SelectNode(f);
    (*i)++;
    if (!Biddy_isNull(Biddy_GetElse(f)) && !Biddy_isSelected(Biddy_GetElse(f)))
      NodeNumber(Biddy_GetElse(f),i);
    if (!Biddy_isNull(Biddy_GetThen(f)) && !Biddy_isSelected(Biddy_GetThen(f)))
      NodeNumber(Biddy_GetThen(f),i);
  }
}

static void
MaxLevel(Biddy_Edge f, unsigned int *max, unsigned int i)
{
  if (!Biddy_isNull(f)) {
    i++;
    if (i > *max) *max = i;
    if (!Biddy_isNull(Biddy_GetElse(f))) MaxLevel(Biddy_GetElse(f),max,i);
    if (!Biddy_isNull(Biddy_GetThen(f))) MaxLevel(Biddy_GetThen(f),max,i);
  }
}

static void
AvgLevel(Biddy_Edge f, float *sum, unsigned int *n, unsigned int i)
{
  if (!Biddy_isNull(f)) {
    i++;
    (*n)++;
    (*sum) = (*sum) + i;
    if (!Biddy_isNull(Biddy_GetElse(f))) AvgLevel(Biddy_GetElse(f),sum,n,i);
    if (!Biddy_isNull(Biddy_GetThen(f))) AvgLevel(Biddy_GetThen(f),sum,n,i);
  }
}
