//#include <__cross_studio_io.h>    // for CrossStudio debugging

//#include "adc.h"                  // Req'd because we reference TaskADC()
#include "main.h"                 // Application header
#include "salvo.h"                // 
#include "msp430.h"               // Contains MSP430-specific definitions like P2DIR
#include "timestamp_print.h"
#include "ui.h"
//#include "telem.h"
//#include "io2.h"             


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
  int VCCcurrent;
  float batt;
  float prog;
  float chrg;
  int acpr;
  int fault;


void TaskDoCmds(void){ 
  timestamp_print(STR_TASK_DOCMDS ": Starting.");

  while(1){
    OS_Delay(100);
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
        //VCCcurrent = RtnVCCCurrent();
        temp = RtnOnChipTemp();
        batt = RtnBattVoltage();
        prog = RtnPROG();
        chrg = RtnCHRG();
        acpr = 0; //This needs to be fixed
        fault = 0; //This needs to be fixed
        
        sprintf(strTmp, STR_TASK_DOCMDS ": info");
        //sprintf(strTmp, STR_TASK_DOCMDS ": System Info: +5V_USB: %.2f \t charge time: %d s \n MCU: %.2f \t VCC: %.2f \t VCC current: %d mA \t temp: %d C \n Batt: %.2f V \t PROG: %.2f V \t CHRG: %.2f V \t -ACPR: %d \t -FAULT: %d", VUSB, chargeTime, MCU, VCC, VCCcurrent, temp, batt, prog, chrg, acpr, fault);
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