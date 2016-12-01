//#include <__cross_studio_io.h>    // for CrossStudio debugging

#include "adc.h"                  // Req'd because we reference TaskADC()
#include "main.h"                 // Application header
#include "salvo.h"                // 
#include "msp430.h"               // Contains MSP430-specific definitions like P2DIR
#include "timestamp_print.h"
#include "ui.h"
#include "telem.h"
#include "io2.h"      


/******************************************************************************
****                                                                       ****
**                                                                           **
TaskDoCmds()

**                                                                           **
****                                                                       ****
******************************************************************************/
  char command;
  int temp;
  float VUSB;
  int chargeTime;
  float MCU;
  float VCC;
  float VCCcurrent;
  float batt;
  float prog;
  float chrg;
  int acpr;
  int fault;


void TaskDoCmds(void){ 
  timestamp_print(STR_TASK_DOCMDS ": Starting.");

  while(1){
    OS_Delay(20);
    command = usart_uart1_getchar();

    switch(command) {

      case 'h'  :
        sprintf(strTmp, STR_TASK_DOCMDS ": h: Commands: {h, i, r, t, v}");
        timestamp_print(strTmp);
        break; 

      case 'i'  :
        VUSB = Rtn5VUSB();
        chargeTime = 10; //This needs to be fixed
        MCU = RtnVCCMCU();
        VCC = RtnVCC();
        VCCcurrent = RtnVCCCurrent();
        temp = RtnOnChipTemp();
        //Prints out the value of vcc current for debugging
        sprintf(strTmp, "after pass %.2f", VCCcurrent);
        timestamp_print(strTmp);
        sprintf(strTmp, "p2in %d", P2IN);
        timestamp_print(strTmp);
        batt = RtnBattVoltage();
        prog = RtnPROG();
        chrg = RtnCHRG();

        P2DIR &= ~BIT6;
        P2DIR &= ~BIT7;

        acpr = 1; //need to fix this
        if((P2IN && BIT6) != 0){
          acpr = 0;
        }
        fault = 1; //If the fault bit is high then there is no fault and this will end up 0

        if((P2IN && BIT7) != 0){
            fault = -1;
        }
        
        sprintf(strTmp, STR_TASK_DOCMDS ": System Info: +5V_USB: %.2f \t charge time: %d s \n MCU: %.2f \t VCC: %.2f \t VCC current: %.2f mA \t temp: %d C \n Batt: %.2f V \t PROG: %.2f V \t CHRG: %.2f V \t -ACPR: %d \t -FAULT: %d", VUSB, chargeTime, MCU, VCC, VCCcurrent, temp, batt, prog, chrg, acpr, fault);
        timestamp_print(strTmp);
        break; 

     case 'r'  :
        sprintf(strTmp, STR_TASK_DOCMDS ": r: Reset (via WDT) in 1 s.");
        timestamp_print(strTmp);
        WDTCTL = 0xDEAD; //reset via WDT
        break; 

     case 't'  :
        temp = RtnOnChipTemp();
        sprintf(strTmp, STR_TASK_DOCMDS ": Ambient temp is %d C", temp);
        timestamp_print(strTmp);
        break; 

     case 'v'  :
        timestamp_print(STR_TASK_DOCMDS ":   v" VERSION_NUM " built on "__DATE__" at "__TIME__".");
        break;

      default :
      break;
	
   }
 }
}

void setChargerOn(void){
  timestamp_print("charger off");
  P2DIR |= BIT3;
  P2OUT |= BIT3;
}

void setChargerOff(void){
  timestamp_print("charger on");
  P2DIR |=  BIT3;
  P2OUT &= ~BIT3;
}
