/******************************************************************************
(C) Copyright Pumpkin, Inc. All Rights Reserved.

This file may be distributed under the terms of the License
Agreement provided with this software.

THIS FILE IS PROVIDED AS IS WITH NO WARRANTY OF ANY KIND,
INCLUDING THE WARRANTY OF DESIGN, MERCHANTABILITY AND
FITNESS FOR A PARTICULAR PURPOSE.

$Source: C:\\RCS\\D\\Pumpkin\\CubeSatKit\\Example\\PIC24\\PIC24FJ256GA110\\CubeSat_Kit_Dev_Board\\Test\\Test1\\init.c,v $
$Author: aek $
$Revision: 3.5 $
$Date: 2011-09-17 16:24:44-07 $

******************************************************************************/
#include "main.h"
#include "init.h"
#include "events.h"

// Pumpkin CubeSat Kit headers
#include "csk_hw.h" 
#include "csk_led.h"
#include "csk_mhx.h"
#include "csk_uart.h"
#include "csk_usb.h"
#include "csk_wdt.h"

// Pumpkin PIC24 Library.
#include "pic24_uart.h"

// Microchip PIC24F Peripehral Library
#include <pps.h>
#include <pwrmgnt.h>
#include <timer.h>
#include <uart.h>

#include "salvo.h"


// Ensure that correct ICD port is selected.  
_CONFIG1( ICS_PGx1 & JTAGEN_OFF )

// Select primary oscillator in XT mode. 
_CONFIG2( FNOSC_PRI & POSCMOD_XT )


/******************************************************************************
****                                                                       ****
**                                                                           **
init()

**                                                                           **
****                                                                       ****
******************************************************************************/
void init(void) {

  // Force WDT off for now ... having it on tends to confuse novice
  //   users.
  csk_wdt_off();
  

  // Keep interrupts off for now ...
  __disable_interrupt();

  
  // All CSK control signals are active LOW.
  #if   defined(__PIC24FJ256GA110__)
  // PPM D1: 
  //  RF3 <-> TX0
  //  RF7 <-> SDI0
  //  RD9 <-> SCK0

  TRISA = ~(BIT15+BIT14+BIT13+BIT12+                                          BIT3+BIT2          );
  TRISB = ~(BIT15+BIT14+BIT13+BIT12+BIT11+BIT10+BIT9+BIT8+BIT7+BIT6+BIT5+BIT4+BIT3+BIT2+BIT1+BIT0);
  TRISC = ~(                                                             BIT4+BIT3+BIT2+BIT1     );
  TRISD = ~(BIT15+BIT14+      BIT12+            BIT9+BIT8+          BIT5+BIT4+BIT3+BIT2+BIT1+BIT0);
  TRISE = ~(                                    BIT9+BIT8+BIT7+BIT6+BIT5+BIT4+BIT3+BIT2          );
  TRISF = ~(            BIT13+BIT12+                 BIT8+BIT7+BIT6+BIT5+     BIT3+     BIT1+BIT0); // leave RF2 & RF4 alone
  TRISG = ~(                                    BIT9+BIT8+BIT7+BIT6+          BIT3+BIT2+BIT1+BIT0);
  
  PORTA = 0x0000;
  PORTB = 0x0000;
  PORTC = 0x0000+BIT1;              // -OE_USB is OFF
  PORTD = 0x0000;
  PORTE = 0x0000+BIT4+BIT3+BIT2;   // -ON_SD, -ON_MHX, -OE_MHX are OFF
  PORTF = 0x0000+BIT3+BIT5;				 // TX0 & TX1 initially high.
  PORTG = 0x0000;

  AD1PCFGL = 0xFFFF;

  #elif defined(__PIC24FJ256GB210__)
  // PPM E1: 
  //  RD9 <-> TX0
  //  RD1 <-> SDI0
  //  RD2 <-> SCK0

  TRISA = ~(            BIT13+BIT12+                      BIT7+BIT6+          BIT3+BIT2          );
  TRISB = ~(BIT15+BIT14+BIT13+BIT12+BIT11+BIT10+BIT9+BIT8+BIT7+BIT6+BIT5+BIT4+BIT3+BIT2+BIT1+BIT0);
  TRISC = ~(                                                             BIT4+BIT3+BIT2+BIT1     );
  TRISD = ~(BIT15+BIT14+      BIT12+            BIT9+BIT8+BIT7+     BIT5+BIT4+BIT3+BIT2+BIT1+BIT0);
  TRISE = ~(                                    BIT9+BIT8+BIT7+BIT6+BIT5+BIT4+BIT3+BIT2          );
  TRISF = ~(            BIT13+BIT12+                 BIT8+BIT7+BIT6+BIT5+               BIT1+BIT0); // leave RF2 & RF4 alone
  TRISG = ~(BIT15+                              BIT9+BIT8+BIT7+BIT6+          BIT3+BIT2+BIT1+BIT0);

  PORTA = 0x0000;
  PORTB = 0x0000;
  PORTC = 0x0000+BIT1;           // -OE_USB is OFF
  PORTD = 0x0000+BIT9;           // TX0 initially high
  PORTE = 0x0000+BIT4+BIT3+BIT2; // -ON_SD, -ON_MHX, -OE_MHX are OFF
  PORTF = 0x0000+BIT5;					 // TX1 initially high.
  PORTG = 0x0000;

  ANSA = 0x0000;
  ANSB = 0x0000;
  ANSC = 0x0000;
  ANSD = 0x0000;
  ANSE = 0x0000;
  ANSF = 0x0000;
  ANSG = 0x0000;
  #else
  #error PIC24F device not supported by CubeSat Kit
  #endif 

  // High-level inits (works at any clock speed).
  csk_mhx_close();
  csk_mhx_pwr_off();
  csk_usb_close();
  csk_led_status_close();
  

  // Set up to run with primary oscillator.
  // See _CONFIG2 above. A configuration-word-centric setup of the
  //  oscillator(s) was chosen because of its relative simplicity.
  //  Note e.g. that PwrMgnt_OscSel() returns FALSE if clock switching
  //  (FCKSM) is disabled ...


  // Set up Timer2 to run at system tick rate                
  ConfigIntTimer2(T2_INT_ON & T2_INT_PRIOR_1);   // Timer is configured for 10 msec (100Hz), with interrupts
  OpenTimer2(T2_ON & T2_IDLE_CON & T2_GATE_OFF & T2_PS_1_1 & T2_32BIT_MODE_OFF & T2_SOURCE_INT,
             (MAIN_XTAL_FREQ/(2*100)));        // A prescalar is not required because 8E6/200 < 16 bits.


  // Configure I/O pins for UARTs via PIC24's PPS system.
  // CSK UART0 (PIC24 UART1) TX/RX = IO.4/IO.5
  // CSK UART1 (PIC24 UART2) TX/RX = IO.6/IO.7
  // RP30/RF2 & RP10/RF4 must be configured as inputs!
  iPPSInput(IN_FN_PPS_U1RX,IN_PIN_PPS_RP30);
  #if   defined(__PIC24FJ256GA110__)
  iPPSOutput(OUT_PIN_PPS_RP16,OUT_FN_PPS_U1TX);
  #elif defined(__PIC24FJ256GB210__)
  iPPSOutput(OUT_PIN_PPS_RP4,OUT_FN_PPS_U1TX);
  #else
  #error PIC24F device not supported by CubeSat Kit
  #endif 
  iPPSInput(IN_FN_PPS_U2RX,IN_PIN_PPS_RP10);
  iPPSOutput(OUT_PIN_PPS_RP17,OUT_FN_PPS_U2TX);


  // Init UARTs to 9600,N,8,1  
  // UARTs won't transmit until interrupts are enabled ...
  csk_uart0_open(CSK_UART_9600_N81);
  csk_uart1_open(CSK_UART_9600_N81);
  csk_uart0_puts(STR_CRLF STR_CRLF);
  csk_uart0_puts("Pumpkin " STR_CSK_TARGET " " STR_APP_NAME "." STR_CRLF);
  csk_uart0_puts(STR_VERSION "." STR_CRLF);
  csk_uart0_puts(STR_WARNING "." STR_CRLF);
    
} /* init() */


void __attribute__ ((interrupt,no_auto_psv)) _T2Interrupt (void)
{
   T2_Clear_Intr_Status_Bit;
   OSTimer();
}


void __attribute__ ((interrupt,no_auto_psv)) _U1TXInterrupt(void)
{
  csk_uart0_outchar();
}


void __attribute__ ((interrupt,no_auto_psv)) _U1RXInterrupt(void)
{
  csk_uart0_inchar(ReadUART1());
  OSSignalSem(SEM_CMD_CHAR_P); // commands normally come from Rx0 (terminal port on Dev Board)
}


void __attribute__ ((interrupt,no_auto_psv)) _U2TXInterrupt(void)
{
  csk_uart1_outchar();
}


void __attribute__ ((interrupt,no_auto_psv)) _U2RXInterrupt(void)
{
  csk_uart1_inchar(ReadUART2());
}

