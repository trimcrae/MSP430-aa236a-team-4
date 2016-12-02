#include "io2.h"                   // Good to self-reference
#include "main.h"                 // Application header
#include "msp430.h"               // Contains MSP430-specific definitions like P2DIR
#include "salvo.h"                // Req'd because we call OSDelay() 
#include "telem.h"                // Req'd because we call RtnStatus()
#include "timestamp_print.h"      // Req'd becayse we call timestamp_print()



/******************************************************************************
****                                                                       ****
**                                                                           **
TaskIO()

**                                                                           **
****                                                                       ****
******************************************************************************/
int status;

void TaskIO(void) {
  timestamp_print(STR_TASK_IO ": Starting.");

  while(1) {
    status = RtnStatus();
    switch (status) {
    //Status 1 = charging, charge is at 0v (constant c)
    //Status 2 = charging, charge is around 2v (constant v)
    //Status 3 = charging, charge terminated
    //Status 4 = not charging, voltage >4.13
    //Status 5 = discharging, partially charged
    //Status 6 = discharging, almost dead
    //Status 1 = discharging, is dead
      case 1:
      //blink one light
        SetLED0();
        OS_Delay(50);
        ClrLED0();
        OS_Delay(50);
        break;

      case 2:
      //blink two lights
        SetLED0();
        SetLED1();
        OS_Delay(50);
        ClrLED0();
        ClrLED1();
        OS_Delay(50);
        break; 
              
       case 3:
       //blink three lights
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
       //flash three lights
        SetLED0();
        SetLED1();
        SetLED2();
        OS_Delay(20);
        ClrLED0();
        ClrLED1();
        ClrLED2();
        OS_Delay(80);
        break;  
   
       case 5:
       //flash two lights
        SetLED0();
        SetLED1();
        OS_Delay(20);
        ClrLED0();
        ClrLED1();
        OS_Delay(80);
        break;  

       case 6:
       //flash one light
        SetLED0();
        OS_Delay(15);
        ClrLED0();
        OS_Delay(85);
        break;  

       case 7:
       //no lights
        OS_Delay(50);
        break;

       case 8:
       // no lights
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
