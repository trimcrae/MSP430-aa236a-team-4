#include "io2.h"                   // Good to self-reference
#include "main.h"                 // Application header
#include "msp430.h"               // Contains MSP430-specific definitions like P2DIR
#include "telem.h"                // Req'd because we call RtnStatus
#include "salvo.h"                // Req'd because we call e.g. OSDelay() 
#include "usart_uart_msg_ts.h"    // Req'd because we call usart_uart1_msg_ts()


/******************************************************************************
****                                                                       ****
**                                                                           **
TaskIO()

**                                                                           **
****                                                                       ****
******************************************************************************/
int status;

void TaskIO(void) {
  usart_uart1_msg_ts(STR_TASK_IO ": Starting.");

  while(1) {
    status = RtnStatus();
    sprintf(strTmp, "status %d", status);
    timestamp_print(strTmp);
    switch (status) {
    //Status 1 = charging, charge is at 0v (constant c)
    //Status 2 = charging, charge is around 2v (constant v)
    //Status 3 = charging, charge terminated
    //Status 4 = not charging, voltage >4.13
    //Status 5 = discharging, partially charged
    //Status 6 = discharging, almost dead
    //Status 1 = discharging, is dead
      case 1:
        //usart_uart1_msg_ts(STR_TASK_IO ": Turning LED0 ON.");
        SetLED0();
        OS_Delay(50);
        //usart_uart1_msg_ts(STR_TASK_IO ": Turning LED0 OFF.");
        ClrLED0();
        OS_Delay(50);
        break;

      case 2:
        SetLED0();
        SetLED1();
        OS_Delay(50);
        ClrLED0();
        ClrLED1();
        OS_Delay(50);
        break; 
              
       case 3:
        SetLED0();
        SetLED1();
        SetLED2();
        OS_Delay(50);
        ClrLED0();
        ClrLED1();
        ClrLED2();
        OS_Delay(50);
        break;    

       case 4:
        SetLED0();
        SetLED1();
        SetLED2();
        OS_Delay(15);
        ClrLED0();
        ClrLED1();
        ClrLED2();
        OS_Delay(85);
        break;  
   
       case 5:
        SetLED0();
        SetLED1();
        OS_Delay(15);
        ClrLED0();
        ClrLED1();
        OS_Delay(85);
        break;  

       case 6:
        SetLED0();
        OS_Delay(15);
        ClrLED0();
        OS_Delay(85);
        break;  

       case 7:
        ClrLED0();
        ClrLED1();
        ClrLED2();
        OS_Delay(100);
        break;

       case 8:
        ClrLED0();
        ClrLED1();
        ClrLED2();
        OS_Delay(100);
        break;
    }
  }
}

/******************************************************************************
****                                                                       ****
**                                                                           **
ClrLED0()
Turns off LED0
**                                                                           **
****                                                                       ****
******************************************************************************/
void ClrLED0(void) {
  P2DIR |=  BIT0;
  P2OUT &= ~BIT0;
}
/******************************************************************************
****                                                                       ****
**                                                                           **
SetLED0()
Turns on LED0
**                                                                           **
****                                                                       ****
******************************************************************************/
void SetLED0(void) {
  P2DIR |= BIT0;
  P2OUT |= BIT0;
}
/******************************************************************************
****                                                                       ****
**                                                                           **
ClrLED1() 
Turns off LED1
**                                                                           **
****                                                                       ****
******************************************************************************/
void ClrLED1(void) {
  P2DIR |=  BIT1;
  P2OUT &= ~BIT1;
}
/******************************************************************************
****                                                                       ****
**                                                                           **
SetLED1()
Turns on LED1
**                                                                           **
****                                                                       ****
******************************************************************************/
void SetLED1(void) {
  P2DIR |= BIT1;
  P2OUT |= BIT1;
}
/******************************************************************************
****                                                                       ****
**                                                                           **
ClrLED2()
Turns off LED2
**                                                                           **
****                                                                       ****
******************************************************************************/
void ClrLED2(void) {
  P2DIR |=  BIT2;
  P2OUT &= ~BIT2;
}
/******************************************************************************
****                                                                       ****
**                                                                           **
SetLED2()
Turns on LED2
**                                                                           **
****                                                                       ****
******************************************************************************/
void SetLED2(void) {
  P2DIR |= BIT2;
  P2OUT |= BIT2;
}
