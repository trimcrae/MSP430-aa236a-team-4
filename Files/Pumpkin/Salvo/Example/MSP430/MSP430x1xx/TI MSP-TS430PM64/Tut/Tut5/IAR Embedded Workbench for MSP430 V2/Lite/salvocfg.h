/************************************************************
Copyright (C) 1995-2002 Pumpkin, Inc. and its
Licensor(s). Freely distributable.

$Source: C:\\RCS\\D\\Pumpkin\\Salvo\\Example\\MSP430\\MSP430x1xx\\TI\040MSP-TS430PM64\\Tut\\Tut5\\IAR\040Embedded\040Workbench\040for\040MSP430\040V2\\Lite\\salvocfg.h,v $
$Author: aek $
$Revision: 3.0 $
$Date: 2008-02-06 20:48:10-08 $

Header file for tutorial programs.

************************************************************/

#include <msp430x14x.h> 		// pre-v3 compilers don't support #include <msp430.h>

#define OSUSE_LIBRARY				TRUE
#define OSLIBRARY_TYPE			OSF
#define OSLIBRARY_CONFIG		OST

#define OSEVENTS					  1
#define OSEVENT_FLAGS				0
#define OSMESSAGE_QUEUES		0
#define OSTASKS             3


