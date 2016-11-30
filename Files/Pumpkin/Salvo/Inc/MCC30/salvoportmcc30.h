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
places(TM). Copyright (C) 1995-2007 Pumpkin, Inc. and its
Licensor(s). All Rights Reserved.

$Source: C:\\RCS\\D\\Pumpkin\\Salvo\\Inc\\MCC30\\salvoportmcc30.h,v $
$Author: aek $
$Revision: 3.9 $
$Date: 2013-08-02 16:43:03-07 $

Porting file for Microchip MLPAB-C30 and all dsPIC & PIC24
targets. For both small (default) and large code models. 

************************************************************/


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




// Use OSDispatch() method with externally (i.e. user-)
//  defined interrupt control to ensure total target
//  independence of Salvo libraries.
#define OSCTXSW_METHOD                    OSVIA_OSDISPATCH


// Because PIC24 is a 16-bit native machine, we must
//  ensure correct 8/16/32-bit typedefs.
#define OSINT_SIZE                        OSINT_IS_16BITS                


// SP is 32 bits.
#define OSBYTES_OF_FRAMEP                 4


// Saved registers are 16 bits in size ... by saving
//  registers in the tcb, we're able to simulate the
//  register-save features of a true task stack.
#define OSCTXSW_SAVES_REGS             TRUE
#define OStypeSavedRegs OStypeInt16u
#define OSCTXSW_REGS_SAVED             6


// MPLAB C30 handles large code spaces via 16-bit function
//  pointers. Low memory is a normal 16-bit pointer, and
//  high memory is a 16-bit pointer to a table of GOTOs
//  in low memory, which in turn vector to high memory.
//  For us, tcbs hold 32-bit addresses, to enable context
//  switching to anywhere in program space. Dethunking
//  ensures that task entry point is via a 16-bit pointer,
//  regardless of where the task is located.  
// To use large code models, user must define the
//  symbol OSMCC30_LARGE_CM globally in the Salvo project.
#if defined(OSMCC30_LARGE_CM)
  #define OSUSE_CUSTOM_TFP_FIELD          TRUE
  typedef unsigned long OStypeRawTFP;     
  OStypeRawTFP OSDethunkTFP (void *);
#endif


// For 8-bit delay configurations, take advantage of
//  unused room in each tcb's status word to store the
//  delay field.
#define OSSTORE_8BIT_DELAYS_IN_STATUS     TRUE


// Should work fine.
#if !defined(OSUSE_MEMSET)
  #define OSUSE_MEMSET                    TRUE
#endif


// printf() supports p (pointer) format.
//    NOTE: printf() support must be included in makefile
// and the user must provide the low-level character out
// routine. See avr-libc docs or <stdio.h> header file
// for more information.
#define OSUSE_PRINTF_P_FORMAT             TRUE
    
                                
//
// End compiler-specific macro definitions.
// ========================================================
//


/************************************************************
****                                                     ****
**                                                         **
Notes

1. OSDispatch() and OSCtxSw() are in 
MCC30\salvoportmcc30-sm.s and MCC30\salvoportmcc30-lm.s.

**                                                         **
****                                                     ****
************************************************************/
