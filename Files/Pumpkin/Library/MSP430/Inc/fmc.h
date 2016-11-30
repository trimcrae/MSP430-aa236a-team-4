/************************************************************
Copyright (C) 1995-2007 Pumpkin, Inc. and its
Licensor(s).

This file may be distributed under the terms of the License
Agreement provided with this software.

THIS FILE IS PROVIDED AS IS WITH NO WARRANTY OF ANY KIND,
INCLUDING THE WARRANTY OF DESIGN, MERCHANTABILITY AND
FITNESS FOR A PARTICULAR PURPOSE.

$Source: C:\\RCS\\D\\Pumpkin\\Library\\MSP430\\Inc\\FMC.h,v $
$Author: aek $
$Revision: 3.2 $
$Date: 2009-10-11 19:47:39-07 $

************************************************************/
#ifndef __fmc_h
#define __fmc_h

#include "config.h"


#define FMC_CLK  362000  // mean of the required 257kHz < f_FMC_clk < 476kHz

extern void         fmc_erase(unsigned int *);
extern unsigned int fmc_read(unsigned int *);
extern void         fmc_write(unsigned int, unsigned int *);


#endif /* __fmc_h */


