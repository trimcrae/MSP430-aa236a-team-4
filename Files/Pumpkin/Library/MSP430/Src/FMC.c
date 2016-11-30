/******************************************************************************
(C) Copyright Pumpkin, Inc. All Rights Reserved.

This file may be distributed under the terms of the License
Agreement provided with this software.

THIS FILE IS PROVIDED AS IS WITH NO WARRANTY OF ANY KIND,
INCLUDING THE WARRANTY OF DESIGN, MERCHANTABILITY AND
FITNESS FOR A PARTICULAR PURPOSE.

$Source: C:\\RCS\\D\\Pumpkin\\Library\\MSP430\\Src\\FMC.c,v $
$Author: aek $
$Revision: 3.4 $
$Date: 2009-11-01 22:53:48-08 $

******************************************************************************/
#include "config.h"
#include "IO.h"
#include "fmc.h"


/******************************************************************************
****                                                                       ****
**                                                                           **
IMPORTANT NOTE: Since interrupts are COMPLETELY DISABLED
while flash memory programming is underway, e.g. incoming Rx
chars can be missed. Keep this in mind ...

**                                                                           **
****                                                                       ****
******************************************************************************/

/******************************************************************************
****                                                                       ****
**                                                                           **
fmc_erase()

Erase one Flash memory segment.
      
Notes:    Assumes that an SMCLK greater or equal to 362kHz
             (the geometric mean of 257kHz and 476kHz) is
             available.
            Does not check for odd address.
            Does not range-check address.
            Executes from Flash (not RAM).

Returns:  --      

**                                                                           **
****                                                                       ****
******************************************************************************/
void fmc_erase(unsigned int * address) {

  unsigned int WDTCTL_saved;


  // Save config bits of WDTCTL
  WDTCTL_saved = WDTCTL & 0x00FF;

  // Required for proper operation.
  WDTCTL = WDTPW + WDTHOLD;
  __disable_interrupt();

  // Drive FMC with SMCLK at proper frequency.
  FCTL2 = FWKEY+FSSEL1+(SMCLK/FMC_CLK);

  // Clear LOCK.
  FCTL3 = FWKEY;

  // Enable segment erase.
  FCTL1 = FWKEY+ERASE;

  // Dummy write.
  *address = 0;

  // Set LOCK.
  FCTL3 = FWKEY+LOCK;

  // Restore WDTCTL
  WDTCTL = WDTPW + WDTCTL_saved;

  // Re-enable interrupts.
  __enable_interrupt();
  
} /* fmc_erase() */


/******************************************************************************
****                                                                       ****
**                                                                           **
fmc_read()

Read one 16-bit word from flash memory. Typically used to
read from Information Memory (Flash).
      
Notes:    Forces address to be even.
            Can read anywhere in memory.
            Does not check for odd address.
            Limited to 64KB address space.
            Executes from Flash (not RAM).

Returns:  16-bit word read from memory.      

**                                                                           **
****                                                                       ****
******************************************************************************/
unsigned int fmc_read(unsigned int * address) {

  // Read data word from flash.
  return (*address);

} /* fmc_read() */


/******************************************************************************
****                                                                       ****
**                                                                           **
fmc_write()

Write one 16-bit word to flash memory.
      
Notes:    Assumes that an SMCLK greater or equal to 362kHz
             (the geometric mean of 257kHz and 476kHz) is
             available.
            Does not check for odd address.
            Forces address to be even.
            Does not range-check address.
            Executes from Flash (not RAM).

Returns:  --      

**                                                                           **
****                                                                       ****
******************************************************************************/
void fmc_write(unsigned int data,
               unsigned int * address) {
               
  unsigned int WDTCTL_saved;


  // Save config bits of WDTCTL
  WDTCTL_saved = WDTCTL & 0x00FF;

  // Required for proper operation.
  WDTCTL = WDTPW + WDTHOLD;
  __disable_interrupt();

  // Drive FMC with SMCLK at proper frequency.
  FCTL2 = FWKEY+FSSEL1+(SMCLK/FMC_CLK);

  // Clear LOCK.
  FCTL3 = FWKEY;

  // Enable write.
  FCTL1 = FWKEY+WRT;

  // Write data word to flash.
  *address = data;

  // Done. Disable write.
  FCTL1 = FWKEY;

  // Set LOCK.
  FCTL3 = FWKEY+LOCK;

  // Restore WDTCTL
  WDTCTL = WDTPW + WDTCTL_saved;

  // Re-enable interrupts.
  __enable_interrupt();
  
} /* fmc_write() */
