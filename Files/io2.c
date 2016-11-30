#include "io2.h"                   // Good to self-reference
#include "main.h"                 // Application header
#include "msp430.h"               // Contains MSP430-specific definitions like P2DIR
#include "salvo.h"                // Req'd because we call e.g. OSDelay() 
#include "timestamp_print.h"


/******************************************************************************
****                                                                       ****
**                                                                           **
TaskIO()

**                                                                           **
****                                                                       ****
******************************************************************************/
void TaskIO(void) {
  timestamp_print(STR_TASK_IO ": Starting.");

  while(1) {
    timestamp_print(STR_TASK_IO ": Turning LED0 ON.");
    SetLED0();
    OS_Delay(50);
    timestamp_print(STR_TASK_IO ": Turning LED0 OFF.");
    ClrLED0();
    OS_Delay(50);
  }
}

/******************************************************************************
****                                                                       ****
**                                                                           **
ClrLED0()

**                                                                           **
****                                                                       ****
******************************************************************************/
void ClrLED0(void) {
  P2DIR |=  BIT0;
  P2OUT &= ~BIT0;
}

void SetLED0(void) {
  P2DIR |= BIT0;
  P2OUT |= BIT0;
}






