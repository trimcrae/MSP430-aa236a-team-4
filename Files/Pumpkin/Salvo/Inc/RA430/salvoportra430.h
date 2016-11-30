/*************************************************************
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

$Source: C:\\RCS\\D\\Pumpkin\\Salvo\\Inc\\RA430\\salvoportra430.h,v $
$Author: aek $
$Revision: 3.13 $
$Date: 2008-06-20 11:37:38-07 $

Porting file for TI MSP430 and Rowley Associates CrossWorks
for MSP430 (RA430).

Note: RA430 supports the notion of a single header file 
msp430.h, which in turn includes the appropriate part-
specific header file based on which processor is being 
compiled for (e.g. -D__TARGET_PROCESSOR=MSP430F149).

*************************************************************/

#if OSMAKE_LIBRARY
#include <msp430.h>
#endif
#include <in430.h>
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

         
// memset() should work fine.
#define OSUSE_MEMSET                    TRUE


// printf() supports p (pointer) format.
#define OSUSE_PRINTF_P_FORMAT						TRUE


// MSP430X needs 20-bit function pointers in tFP field,
//  so declare OSDethunkTFP() and define OStypeRawTFP.
#ifdef __MSP430_CPUX
#define OSUSE_CUSTOM_TFP_FIELD					TRUE
typedef unsigned long OStypeRawTFP; 		
OStypeRawTFP OSDethunkTFP (void *);
#endif


//
// End compiler-specific macro definitions.
// ========================================================
//
 

/************************************************************ 
****                                                     ****
**                                                         **
Notes 

1. RA430 does not support bitfield packing (i.e. into
unsigned char instead of unsigned int). The high / "extra" 
byte in the status word contains the frame size (in words).

2. OSDispatch() and OSCtxSw() are in RA430/salvoportra430.asm.

**                                                         **
****                                                     ****
************************************************************/
