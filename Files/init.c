#include "init.h"                 // Good to self-reference
#include "main.h"                 // Application header
#include "msp430.h"               // Contains MSP430-specific definitions like DCOCTL
#include "usart_uart.h"           // Req'd because we call usart_uart1_open();


/******************************************************************************
****                                                                       ****
**                                                                           **

**                                                                           **
****                                                                       ****
******************************************************************************/
void Init(void) {
  unsigned int i;

  WDT_OFF;                           // Stop WDT. This app does not use the WDT.
  
  // clock initialization. Requires only 32.768kHz LFXTAL
                                     //DCO=0x03
  DCOCTL=0x60;		             //DCO.1,DCO.0
  BCSCTL2=0x00;
  BCSCTL1=0x84;		             //XT2OFF,RSEL.2
  
  BCSCTL1 &= ~XT2OFF;                // Turn on XT2
  do {
                                     // Wait for XT2 to stablize
    IFG1 &= ~OFIFG;                  // Clear OSCFault flag
    for (i = 0xFFF; i > 0; i--);     // Time for flag to set
  } while((IFG1 & OFIFG) == OFIFG);  // OSCFault flag still set?
  BCSCTL2 |= SELS;                   // SMCLK = XTAL2 (7.3728MHz)


  WDTCTL = WDTPW + WDTHOLD;       // Stop WDT
  TACTL  = TASSEL0 + TACLR;       // ACLK, clear TAR
  CCTL0  = CCIE;                  // CCR0 interrupt enabled
  CCR0   = TIMERA0_RELOAD;        // initial value
  TACTL |= MC1;                   // Start Timer_A in continuous mode 

  usart_uart1_open(USART_UART_9600_N81_ACLK); // Initialize UART1 at 9600,N,8,1
}
