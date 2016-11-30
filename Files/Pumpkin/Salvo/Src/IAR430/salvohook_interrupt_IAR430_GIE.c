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

$Source: C:\\RCS\\D\\Pumpkin\\Salvo\\Src\\IAR430\\salvohook_interrupt_IAR430_GIE.c,v $
$Author: aek $
$Revision: 3.6 $
$Date: 2008-04-29 10:37:13-07 $

For IAR Embedded Workbench for MSP430.

Most basic example of user-defined interrupt control functions.


************************************************************/

#include <salvo.h>

/************************************************************
****                                                     ****
**                                                         **
Hooks for IAR 430 compiler -- intelligent control of GIE bit.

In mainline code, GIE is under user control, and these hooks
restore it at the end of a Salvo critical section.

In ISRs, GIE is zero, and so these hooks leave global
interrupts disabled on exit, thus avoiding nested interrupts.

Early compiler did not have __get_interrupt_state(), etc.

For reference, the net effect of OS_DisableHook() for the 
later compilers is equivalent to

				push.w SR
				dint
				pop    R15
				mov.x  R15,&s

This (and the C-language version) avoids the issue of 
OSDisableHook() itself being preempted and thereby seeing
s overwritten by the preempter. By using an auto variable 
for temporary storage of s, we're safe from this situation.

**                                                         **
****                                                     ****
************************************************************/
#if (__VER__ < 200)

static unsigned short old_SR;

void OSDisableHook(void)
{
  old_SR = BIC_SR(0x08);
}


void OSEnableHook(void)
{
  _BIS_SR(old_SR);
}

#else /* v2 and higher compilers */

static istate_t s;

void OSDisableHook(void)
{
  istate_t t = __get_interrupt_state();
  __disable_interrupt();
  s = t;
}


void OSEnableHook(void)
{
  __set_interrupt_state(s);
}

#endif

