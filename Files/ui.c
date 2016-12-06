//#include <__cross_studio_io.h>    // for CrossStudio debugging
/**
 * @file ui.c
 * @brief Reads user input and responds to it.
 */

#include "adc.h"                  // Req'd because we reference TaskADC()
#include "main.h"                 // Application header
#include "msp430.h"               // Contains MSP430-specific definitions like P2DIR
#include "salvo.h"                // Calls OSDelay
#include "telem.h"                // Calls e.g. chackFault    
#include "timestamp_print.h"      // Calls timestamp_print()
#include "ui.h"                   // Self reference



/******************************************************************************
****                                                                       ****
**                                                                           **
TaskDoCmds()
Executes user input

**                                                                           **
****                                                                       ****
******************************************************************************/

/**
 * TaskDoCmds reads the users keystrokes and responds appropriately.
 * 'h' returns help, showing you what commands are available for the user.
 * 'i' returns system information, to include (in this order) 'VUSB' (Voltage of USB in),
 * 'chargeTime' (total charge time of current charge), 'MCU' (Voltage of MCU), 
 * 'VCC' (Voltage Constant Current), 'VCCcurrent' (current at constant current),
 * 'temp' (temperature on chip), 'batt' (current battery voltage), 'prog' 
 * (essentially the status of the charge based on this voltage value, at 1.5V
 * it is just beginning charging, at .15V it switches to constant voltage charging),
 * 'chrg' (Similar to prog in terms of indicating value, 0 at CC, 2 at CV, 3.3 at complete
 * charge), 'acpr' (0 if plugged in to charge, 1 if not plugged in), 'fault' (if 0 there is an
 * issue, if -1 there is no issue)
 */
void TaskDoCmds(void){ 
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
  //Message that task is starting
  timestamp_print(STR_TASK_DOCMDS ": Starting.");

  while(1){
    //read user input
    command = usart_uart1_getchar();

    switch(command) {
      
      //Give help
      case 'h'  :
        sprintf(strTmp, STR_TASK_DOCMDS ": h: Commands: {h, i, r, t, v}");
        timestamp_print(strTmp);
        break; 

      //Give system information
      case 'i'  :
        VUSB = Rtn5VUSB();
        chargeTime = checkChargeTime();
        MCU = RtnVCCMCU();
        VCC = RtnVCC();
        VCCcurrent = RtnVCCCurrent();
        VCCcurrent = VCCcurrent * -1;
        temp = RtnOnChipTemp();
        //Prints out the value of vcc current for debugging
        //sprintf(strTmp, "after pass %.2f", VCCcurrent);
        //timestamp_print(strTmp);
        batt = RtnBattVoltage();
        prog = RtnPROG();
        chrg = RtnCHRG();

        acpr = check_acpr();
        fault = check_fault();
        
        sprintf(strTmp, STR_TASK_DOCMDS ": System Info: +5V_USB: %.2f \t charge time: %d s \n MCU: %.2f V \t VCC: %.2f V \t VCC current: %.2f mA \t temp: %d C \n Batt: %.2f V \t PROG: %.2f V \t CHRG: %.2f V \t -ACPR: %d \t -FAULT: %d", VUSB, chargeTime, MCU, VCC, VCCcurrent, temp, batt, prog, chrg, acpr, fault);
        timestamp_print(strTmp);
        break; 

     //Resets system
     case 'r'  :
        sprintf(strTmp, STR_TASK_DOCMDS ": r: Reset (via WDT) in 1 s.");
        timestamp_print(strTmp);
        WDTCTL = 0xDEAD; //reset via WDT
        break; 

    //Gives system temperature
     case 't'  :
        temp = RtnOnChipTemp();
        sprintf(strTmp, STR_TASK_DOCMDS ": Ambient temp is %d C", temp);
        timestamp_print(strTmp);
        break; 

    //Gives Version Number
     case 'v'  :
        timestamp_print(STR_TASK_DOCMDS ":   v" VERSION_NUM " built on "__DATE__" at "__TIME__".");
        break;

      default :
      break;
	
   }
   OS_Delay(20);
 }
}
