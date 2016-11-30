/************************************************************
Copyright (C) Pumpkin, Inc. and its
Licensor(s).

This file may be distributed under the terms of the License
Agreement provided with this software.

THIS FILE IS PROVIDED AS IS WITH NO WARRANTY OF ANY KIND,
INCLUDING THE WARRANTY OF DESIGN, MERCHANTABILITY AND
FITNESS FOR A PARTICULAR PURPOSE.

$Source: C:\\RCS\\D\\Pumpkin\\Salvo\\Src\\IAR430\\salvohook_wdt_IAR430_CLRWDT.c,v $
$Author: aek $
$Revision: 3.4 $
$Date: 2007-12-10 12:06:19-08 $

************************************************************/

#include <salvo.h>


void OSClrWDTHook(void)
{
  WDTCTL = (WDTCTL & 0x00FF) | WDTPW | WDTCNTCL;
}

