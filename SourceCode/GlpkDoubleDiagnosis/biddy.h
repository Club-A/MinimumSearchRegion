/**CHeaderFile*************************************************************
  PackageName [Biddy]
  Synopsis    [Biddy provides data structures and algorithms
               for the representation and manipulation of Boolean
               functions with ROBDDs. A hash table is used for quick
               search of nodes. Complement edges decreases the number
               of nodes. Function ITE is given for manipulation of
               boolean functions. An automatic garbage collection with
               a formulae counter is implemented.]

  FileName    [bdd.h]
  Revision    [$Revision: 51 $]
  Date        [$Date: 2013-04-03 17:57:12 +0200 (sre, 03 apr 2013) $]
  Authors     [Robert Meolic (meolic@uni-mb.si),
               Ales Casar (casar@uni-mb.si)]
  Description [The file bdd.h contains declaration of all external
               data structures.]
  SeeAlso     [bddInt.h]

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

#ifndef _BIDDY
#define _BIDDY

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <ctype.h>
#include <stdarg.h>

//#include <unistd.h>
#include <math.h>

/* ON WIN32 THERE HAS TO BE DEFINED WIN32 */
/* ON UNIX THERE HAS TO BE DEFINED UNIX */
/* ON MACOSX THERE HAS TO BE DEFINED MACOSX */

#ifdef UNIX
#  ifndef EXTERN
#    define EXTERN extern
#  endif
#  ifndef DATAEXTERN
#    define DATAEXTERN extern
#  endif
#endif

#ifdef MACOSX
#  ifndef EXTERN
#    define EXTERN extern
#  endif
#  ifndef DATAEXTERN
#    define DATAEXTERN extern
#  endif
#endif

#ifdef WIN32
#  ifdef BUILD_BIDDY
#    undef EXTERN
#    define EXTERN __declspec (dllexport)
#    undef DATAEXTERN
#    define DATAEXTERN extern __declspec (dllexport)
#  else
#    ifdef USE_BIDDY
#      undef EXTERN
#      define EXTERN __declspec (dllimport)
#      undef DATAEXTERN
#      define DATAEXTERN __declspec (dllimport)
#    else
#      undef EXTERN
#      define EXTERN extern
#      undef DATAEXTERN
#      define DATAEXTERN extern
#    endif
#  endif
#endif

/*-----------------------------------------------------------------------*/
/* Constant definitions                                                  */
/*-----------------------------------------------------------------------*/

#define TRUE (0 == 0)
#define FALSE !TRUE

/*-----------------------------------------------------------------------*/
/* Macro definitions                                                     */
/*-----------------------------------------------------------------------*/

/* Biddy_isEqv returns TRUE iff given BDDs are equal */

/* #define Biddy_isEqv(f,g) ((((f).p == (g).p) && ((f).mark == (g).mark))) */
#define Biddy_isEqv(f,g) (f == g)

/* Biddy_isEqvPointer returns TRUE iff given BDDs are equal or inverted */

/* #define Biddy_isEqvPointer(f,g) ((f).p == (g).p) */
#define Biddy_isEqvPointer(f,g) (((uintptr_t) f & ~((uintptr_t) 1)) == ((uintptr_t) g & ~((uintptr_t) 1)))

/* Biddy_isEqvPointerEqvEdge returns TRUE iff given equal or inverted BDDs are equal */

/* #define Biddy_isEqvPointerEqvEdge(f,g) ((f).mark == (g).mark) */
#define Biddy_isEqvPointerEqvEdge(f,g) (f == g)

/* Biddy_Terminal returns TRUE iff given BDD is a constant */

/* #define Biddy_isTerminal(f) (((f).p == biddy_one.p)) */
#define Biddy_isTerminal(f) ((f == biddy_one || f == biddy_zero))

/* Biddy_Null returns TRUE iff given BDD is a NULL edge */

/* #define Biddy_isNull(f) ((f).p == biddy_null.p)  */
#define Biddy_isNull(f) (f == biddy_null)

/* Biddy_GetPointer returns pointer of given edge */

/* #define Biddy_GetPointer(f) (f.p) */
#define Biddy_GetPointer(f) ((void *) ((uintptr_t) f & ~((uintptr_t) 1)))

/* Biddy_SetPointer sets pointer of given edge */

/* #define Biddy_SetPointer(f,x) (f.p = x) */
#define Biddy_SetPointer(f,x) (f = x)

/* Biddy_GetMark returns TRUE iff given edge is complemented */

/* #define Biddy_GetMark(f) (f.mark) */
#define Biddy_GetMark(f) (((uintptr_t) f & (uintptr_t) 1) != 0)

/* Biddy_SetMark makes given edge complemented */

/* #define Biddy_SetMark(f) (f.mark = TRUE) */
#define Biddy_SetMark(f) (f = (void *) ((uintptr_t) f | (uintptr_t) 1))

/* Biddy_ClearMark makes given edge not-complemented */

/* #define Biddy_ClearMark(f) (f.mark = FALSE) */
#define Biddy_ClearMark(f) (f = (void *) ((uintptr_t) f & ~((uintptr_t) 1)))

/* Biddy_InvertMark changes complement bit of the given edge */

/* #define Biddy_InvertMark(f) (f.mark = !f.mark) */
#define Biddy_InvertMark(f) (f = (void *) ((uintptr_t) f ^ (uintptr_t) 1))

/* Make compatible with Biddy 1.0 */

#define Biddy_VoidFunction Biddy_UserFunction
#define biddy_one biddy_termTrue
#define biddy_zero biddy_termFalse
#define biddy_null biddy_termNull

/*-----------------------------------------------------------------------*/
/* Type declarations                                                     */
/*-----------------------------------------------------------------------*/

typedef char Biddy_Boolean;
typedef char *Biddy_String;
typedef unsigned short int Biddy_Variable;
typedef void (*Biddy_VoidFunction)();

/*-----------------------------------------------------------------------*/
/* Structure declarations                                                */
/*-----------------------------------------------------------------------*/

/* EDGE */

/* OLD IMPLEMENTATION */
/*
typedef struct Biddy_Edge {
  void *p;
  Biddy_Boolean mark;
} Biddy_Edge;
*/

/* NEW IMPLEMENTATION */
typedef void* Biddy_Edge;

/*-----------------------------------------------------------------------*/
/* Variable declarations                                                 */
/*-----------------------------------------------------------------------*/

DATAEXTERN Biddy_Edge biddy_one;            /* terminal node */

DATAEXTERN Biddy_Edge biddy_zero;           /* terminal node */

DATAEXTERN Biddy_Edge biddy_null;           /* null edge */

/**AutomaticStart*********************************************************/

/*-----------------------------------------------------------------------*/
/* Function prototypes                                                   */
/*-----------------------------------------------------------------------*/

/*-----------------------------------------------------------------------*/
/* FUNCTIONS EXPORTED FROM biddyMain.c                                   */
/*-----------------------------------------------------------------------*/

#ifdef __cplusplus
extern "C" {
#endif

EXTERN void Biddy_Init();

EXTERN void Biddy_Exit();

EXTERN char *Biddy_About();

EXTERN Biddy_Boolean Biddy_isOK(Biddy_Edge f);

EXTERN Biddy_Edge Biddy_GetThen(Biddy_Edge f);

EXTERN Biddy_Edge Biddy_GetElse(Biddy_Edge f);

EXTERN Biddy_Variable Biddy_GetVariable(Biddy_Edge f);

EXTERN Biddy_String Biddy_GetVariableName(Biddy_Edge f);

EXTERN char Biddy_GetVariableChar(Biddy_Edge f);

EXTERN Biddy_String Biddy_GetIthVariableName(int i);

EXTERN int Biddy_GetVariableOrder(Biddy_Edge f);

EXTERN void Biddy_SelectNode(Biddy_Edge f);

EXTERN void Biddy_DeselectNode(Biddy_Edge f);

EXTERN Biddy_Boolean Biddy_isSelected(Biddy_Edge f);

EXTERN Biddy_Edge Biddy_FindTerminal(Biddy_String x);

EXTERN Biddy_Edge Biddy_FoaTerminal(Biddy_String x);

EXTERN Biddy_Edge Biddy_FoaNode(Biddy_Variable v, Biddy_Edge pf, Biddy_Edge pt);

EXTERN Biddy_Edge Biddy_NOT(Biddy_Edge f);

EXTERN Biddy_Edge Biddy_TransferMark(Biddy_Edge f, Biddy_Boolean mark);

EXTERN Biddy_Edge Biddy_ITE(Biddy_Edge f, Biddy_Edge g, Biddy_Edge h);

EXTERN Biddy_Edge Biddy_Restrict(Biddy_Edge f, Biddy_Variable v, Biddy_Boolean value);

EXTERN Biddy_Edge Biddy_Compose(Biddy_Edge f, Biddy_Variable v, Biddy_Edge g);

EXTERN Biddy_Edge Biddy_E(Biddy_Edge f, Biddy_Variable v);

EXTERN Biddy_Edge Biddy_A(Biddy_Edge f, Biddy_Variable v);

EXTERN void Biddy_IncCounter();

EXTERN void Biddy_Fresh(Biddy_Edge f);

EXTERN void Biddy_Fortify(Biddy_Edge f);

EXTERN void Biddy_Garbage();

EXTERN void Biddy_AddCache(Biddy_VoidFunction gc);

EXTERN void Biddy_NodeSelect(Biddy_Edge f);

EXTERN void Biddy_NodeRepair(Biddy_Edge f);

#ifdef __cplusplus
}
#endif

/*-----------------------------------------------------------------------*/
/* FUNCTIONS EXPORTED FROM biddyStat.c                                   */
/*-----------------------------------------------------------------------*/

#ifdef __cplusplus
extern "C" {
#endif

EXTERN unsigned int Biddy_NodeTableSize();

EXTERN unsigned int Biddy_NodeTableMax();

EXTERN unsigned int Biddy_NodeTableNum();

EXTERN unsigned int Biddy_NodeTableNumF();

EXTERN unsigned long int Biddy_NodeTableFOA();

EXTERN unsigned long int Biddy_NodeTableCompare();

EXTERN unsigned long int Biddy_NodeTableAdd();

EXTERN unsigned int Biddy_NodeTableGarbage();

EXTERN unsigned int Biddy_NodeTableGenerated();

EXTERN unsigned int Biddy_NodeTableBlockNumber();

EXTERN unsigned int Biddy_ListUsed();

EXTERN unsigned int Biddy_ListMaxLength();

EXTERN float Biddy_ListAvgLength();

EXTERN unsigned long int Biddy_IteCacheFind();

EXTERN unsigned long int Biddy_IteCacheOverwrite();

EXTERN unsigned int Biddy_NodeNumber(Biddy_Edge f);

EXTERN unsigned int Biddy_NodeMaxLevel(Biddy_Edge f);

EXTERN float Biddy_NodeAvgLevel(Biddy_Edge f);

#ifdef __cplusplus
}
#endif

/**AutomaticEnd***********************************************************/

#endif  /* _BIDDY */
