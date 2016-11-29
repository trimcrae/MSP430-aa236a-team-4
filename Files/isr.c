#include "main.h"
#include "isr.h"
#include "salvo.h"          // Req'd because we call OSTimer() 
#include "usart_uart.h"     // Req'd because we call usart_uart1_in|outchar();


/************************************************************
****                                                     ****
**                                                         **
TimerA ISR

**                                                         **
****                                                     ****
************************************************************/
void Timer_A (void) __interrupt[TIMERA0_VECTOR] {
  CCR0 += TIMERA0_RELOAD;
  OSTimer(); 
}                      


/************************************************************
****                                                     ****
**                                                         **
Tx0 ISR

**                                                         **
****                                                     ****
************************************************************/
void ISRTx0 (void) __interrupt[UART0TX_VECTOR] {
  ;
} 


/************************************************************
****                                                     ****
**                                                         **
Tx1 ISR

**                                                         **
****                                                     ****
************************************************************/
void ISRTx1 (void) __interrupt[UART1TX_VECTOR] {
  usart_uart1_outchar();
} 


/************************************************************
****                                                     ****
**                                                         **
Rx0 ISR

**                                                         **
****                                                     ****
************************************************************/
void ISRRx0 (void) __interrupt[UART0RX_VECTOR] {
  ;
}


/************************************************************
****                                                     ****
**                                                         **
Rx1 ISR

**                                                         **
****                                                     ****
************************************************************/
void ISRRx1 (void) __interrupt[UART1RX_VECTOR] {
  usart_uart1_inchar(RXBUF1);
}


/************************************************************
****                                                     ****
Other (unused) ISRs -- trap for debugging purposes.

**                                                         **
****                                                     ****
************************************************************/
void ISRPort2 (void) __interrupt[PORT2_VECTOR] {
  LOOP_HERE();
}


void ISRPort1 (void) __interrupt[PORT1_VECTOR] {
  LOOP_HERE();
}


void ISRTimerA1 (void) __interrupt[TIMERA1_VECTOR] {
  LOOP_HERE();
}

void ISRWDT (void) __interrupt[WDT_VECTOR] {
  LOOP_HERE();
}


void ISRCMPA (void) __interrupt[COMPARATORA_VECTOR] {
  LOOP_HERE();
}


void ISRTimerB1 (void) __interrupt[TIMERB1_VECTOR] {
  LOOP_HERE();
}


void ISRTimerB0 (void) __interrupt[TIMERB0_VECTOR] {
  LOOP_HERE();
}


void ISRNMI (void) __interrupt[NMI_VECTOR] {
  LOOP_HERE();
}
