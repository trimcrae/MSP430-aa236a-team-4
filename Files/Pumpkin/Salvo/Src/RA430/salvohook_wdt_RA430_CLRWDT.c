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

$Source: C:\\RCS\\D\\Pumpkin\\Salvo\\Src\\RA430\\salvohook_wdt_RA430_CLRWDT.c,v $
$Author: aek $
$Revision: 3.4 $
$Date: 2007-11-14 10:31:10-08 $

User-defined watchdog-clearing function.

************************************************************/
#include <salvo.h>


void OSClrWDTHook(void)
{
  WDTCTL = (WDTCTL & 0x00FF) | WDTPW | WDTCNTCL;
}

