/************************************************************
Copyright (C) 1995-2007 Pumpkin, Inc. and its
Licensor(s). 

This file may be distributed under the terms of the License 
Agreement provided with this software.

THIS FILE IS PROVIDED AS IS WITH NO WARRANTY OF ANY KIND, 
INCLUDING THE WARRANTY OF DESIGN, MERCHANTABILITY AND 
FITNESS FOR A PARTICULAR PURPOSE.

$Source: C:\\RCS\\D\\Pumpkin\\Salvo\\Example\\MSP430\\MSP430x4xx\\TI\040MSP-TS430PZ100\\Tut\\Tut5\\tut5.h,v $
$Author: aek $
$Revision: 3.0 $
$Date: 2007-11-29 11:49:31-08 $

Header file for tut5.

************************************************************/

#define TMR_TICK_10ms             3360    // empirically determined, as we're
                                          //  running from DCO and not from
                                          //  an XTAL

#define LOOP_HERE()               do { ; } while (1)
