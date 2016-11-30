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

$Source: C:\\RCS\\D\\Pumpkin\\Salvo\\Src\\RA430\\salvohook_interrupt_RA430_GIE.c,v $
$Author: aek $
$Revision: 3.7 $
$Date: 2008-02-06 20:53:39-08 $

Most basic example of user-defined interrupt control functions. 
Can be replaced by user functions that control interrupts 
associated with Salvo services.

************************************************************/

#include <salvo.h>

/************************************************************
****                                                     ****
**                                                         **
Hooks for Rowley CrossWorks for MSP430 C compiler -- 
intelligent control of GIE bit.

In mainline code, GIE is under user control, and these hooks
restore it at the end of a Salvo critical section.

In ISRs, GIE is zero, and so these hooks leave global
interrupts disabled on exit, thus avoiding nested interrupts.

**                                                         **
****                                                     ****
************************************************************/

static unsigned s;

void OSDisableHook(void)
{
  s = __disable_interrupt();
}


void OSEnableHook(void)
{
  __set_interrupt(s);
}
