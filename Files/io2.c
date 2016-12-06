#include "io2.h"                   // Good to self-reference
#include "main.h"                 // Application header
#include "msp430.h"               // Contains MSP430-specific definitions like P2DIR
#include "salvo.h"                // Req'd because we call OSDelay() 
#include "telem.h"                // Req'd because we call RtnStatus()
#include "timestamp_print.h"      // Req'd becayse we call timestamp_print()


/**
 * @file io2.c
 * @brief Sets the LED's based upon what the battery voltage is.
 */
/******************************************************************************
****                                                                       ****
**                                                                           **
TaskIO()

**                                                                           **
****                                                                       ****
******************************************************************************/
int status;
/**
 * TaskIO retrieves the status values detailed in telem.c, then sets the lights appropriately based upon
 * which case RtnStatus returns:
 * Status 1 = charging, charge is at 0v (constant c)
 * Status 2 = charging, charge is around 2v (constant v)
 * Status 3 = charging, charge terminated
 * Status 4 = not charging, voltage >4.13
 * Status 5 = discharging, partially charged
 * Status 6 = discharging, almost dead
 * Status 7 = discharging, is dead
 * Status 8 = no lights, fault
 */
void TaskIO(void) {
  timestamp_print(STR_TASK_IO ": Starting.");

  while(1) {
    status = RtnStatus();
    switch (status) {
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
/**
 ClrLED# and SetLED# turn off/on the LED in question.
 */
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

/**
 * The three functions below (to include setProgOn and setVCCCurrentChipOn) are all a part of 
 * startup initilization, setting the proper pins or low in order to set up high-current charge 
 * and the 'Prog' function.
 */
void setChargerOn(void){
  P2DIR |= BIT3;
  P2OUT |= BIT3;
}

void setProgOn(void){
  P2DIR |=  BIT4;
  P2OUT |=  BIT4;
}

void setVCCCurrentChipOn(void){
  P2DIR |=  BIT5;
  P2OUT &=  ~BIT5;
}