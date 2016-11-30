/************************************************************
The contents of this file are subject to the Pumpkin Salvo
License (the "License").  You may not use this file except
in compliance with the License. You may obtain a copy of
the License at http://www.pumpkininc.com, or from
warranty@pumpkininc.com.

Software distributed under the License is distributed on an
"AS IS" basis, WITHOUT WARRANTY OF ANY KIND, either express
or implied. See the License for specific language governing
the warranty and the rights and limitations under the
License.

The Original Code is Salvo - The RTOS that runs in tiny
places(TM). Copyright (C) 1995-2006 Pumpkin, Inc. and its
Licensor(s). All Rights Reserved.

$Source: C:\\RCS\\D\\Pumpkin\\Salvo\\Inc\\IAR430\\salvoportiar430.h,v $
$Author: aek $
$Revision: 3.14 $
$Date: 2007-11-09 18:22:50-08 $

Porting file for TI MSP430 and IAR MSP430 C compiler.

For both pre-v2 (e.g. v1.26B) and post-v2 (e.g. v2.10A)
versions of IAR MSP430 C compiler.

************************************************************/

#if OSMAKE_LIBRARY
#if (__VER__ < 300)
#include <msp430x14x.h>
#else
#include <msp430.h>
#endif
#endif

#if (__VER__ < 300)
#include <in430.h>
#else
#include <intrinsics.h>
#endif

#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include <stdlib.h>


//                                                                                                                          
// ======================================================== 
// Begin user macro definitions. If defined by the user in
//  the project's salvocfg.h configuration file, the user
//  definition will be used. If not, the default definition
//  (below) will be used.									
//


//
// End user-configurable macro definitions.
// ========================================================
//


//
// ========================================================
// Begin compiler-specific macro definitions.
//


// Standard context-switching method  												
#define OSCTXSW_METHOD									OSVIA_OSDISPATCH


// Stack pointer is 16 bits
#define OSBYTES_OF_FRAMEP								2


// MSP430 v1.x requires that extern arrays be
//  declared with size -- o/wise you get a warning #27
//  when compiling ... note required in v2.x ...
#if (__VER__ < 200)
#define	OSUSE_EXTERN_ARRAY_SIZES				TRUE
#endif


// Memset() should work fine.
#define OSUSE_MEMSET                    TRUE


// Printf() supports p (pointer) format.
#define OSUSE_PRINTF_P_FORMAT						TRUE


//
// End compiler-specific macro definitions.
// ========================================================
//


/************************************************************
****                                                     ****
**                                                         **
Notes

1. IAR MSP430 does not support bitfield packing (i.e. into
unsigned char instead of unsigned int). The high / "extra"
byte in the status word contains the frame size (in words).

2. OSDispatch() and OSCtxSw() are in IAR430/portiar430.s43.

**                                                         **
****                                                     ****
************************************************************/
