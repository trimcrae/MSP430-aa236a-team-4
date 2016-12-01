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
    command = usart_uart1_getchar();

    switch(command) {

      case 'h'  :
        sprintf(strTmp, STR_TASK_DOCMDS ": h: Commands: {h, i, r, t, v}");
        timestamp_print(strTmp);
        break; 

      case 'i'  :
        VUSB = Rtn5VUSB();
        chargeTime = checkChargeTime();
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

        acpr = check_acpr();
        fault = check_fault();
        
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
   OS_Delay(20);
 }
}

void setChargerOn(void){
  //timestamp_print("charger on");
  P2DIR |= BIT3;
  P2OUT |= BIT3;
}

void setProgOn(void){
  //timestamp_print("Prog on");
  P2DIR |=  BIT4;
  P2OUT |=  BIT4;
}

void setVCCCurrentChipOn(void){
  //timestamp_print("VCCCurrent Chip on");
  P2DIR |=  BIT5;
  P2OUT &=  ~BIT5;
}