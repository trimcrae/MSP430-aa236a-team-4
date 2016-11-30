/************************************************************
Copyright (C) 1995-2007 Pumpkin, Inc. and its
Licensor(s). 

This file may be distributed under the terms of the License 
Agreement provided with this software.

THIS FILE IS PROVIDED AS IS WITH NO WARRANTY OF ANY KIND, 
INCLUDING THE WARRANTY OF DESIGN, MERCHANTABILITY AND 
FITNESS FOR A PARTICULAR PURPOSE.

$Source: C:\\RCS\\D\\Pumpkin\\Library\\MSP430\\Inc\\io.h,v $
$Author: aek $
$Revision: 3.3 $
$Date: 2009-11-01 22:53:21-08 $

************************************************************/
#ifndef __io_h
#define __io_h


/************************************************************
****                                                     ****
**                                                         **
IO_HIGH|INPUT|LOW|OUTPUT|SEL|TOGGLE|UNSEL(port, bits)

Example:  IO_TOGGLE(P4, (BIT5+BIT7));

Notes:    IO_INPUT and IO_OUTPUT are fail-safe macros. They
           redefine the PnDIR and PnSEL registers each time.
           
**                                                         **
****                                                     ****
************************************************************/

#define IO_OUTPUT(p,b)      do { (p##DIR) |=  (b); (p##SEL) &= ~(b); } while(0)
#define IO_INPUT(p,b)       do { (p##DIR) &= ~(b); (p##SEL) &= ~(b); } while(0)
#define IO_SEL(p,b)         do { (p##SEL) |=  (b); } while(0)
#define IO_UNSEL(p,b)       do { (p##SEL) &= ~(b); } while(0)


#define IO_HIGH(p,b)        do { (p##OUT) |=  (b);  } while(0)
#define IO_LOW(p,b)         do { (p##OUT) &= ~(b);  } while(0)
#define IO_TOGGLE(p,b)      do { (p##OUT) ^=  (b);  } while(0)

#define IO_IN(p, b)         ((p##IN) & (b))


#endif /* __io_h */

