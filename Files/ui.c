#include <__cross_studio_io.h>    // for CrossStudio debugging

#include "adc.h"                  // Req'd because we reference TaskADC()
#include "main.h"                 // Application header
#include "salvo.h"                // 
#include "usart_uart.h"           // Req'd because we call usart_uart1_puts()
#include "usart_uart_msg_ts.h"    // Req'd because we call usart_uart1_msg_ts()
#include "ui.h"

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
int temp;
float batt;
float prog;
float chrg;
int acpr;
int fault;


void TaskDoCmds(void){
  command = usart_uart1_getchar();

  switch(command) {

    case 'h'  :
      sprintf(strTmp, STR_TASK_DOCMDS ": h: Commands: {h, i, r, t, v}");
      usart_uart1_msg_ts(strTmp);
      break; 

    case 'i'  :
      VUSB = Rtn5VUSB();
      chargeTime = 10; //This needs to be fixed
      MCU = RtnVCCMCU();
      VCC = RtnVCC();
      VCCcurrent = RtnVCCCurrent();
      temp = RtnOnChipTemp();
      batt = RtnBattVoltage();
      prog = RtnPROG();
      chrg = RtnCHRG();
      acpr = 0; //This needs to be fixed
      fault = 0; //This needs to be fixed

      sprintf(strTmp, STR_TASK_DOCMDS ": System Info: +5V_USB: %.2f \t charge time: %d s \n MCU: %.2f \t VCC: %.2f \t VCC current: %d mA \t temp: %d C \n Batt: %.2f V \t PROG: %.2f V \t CHRG: %.2f V \t -ACPR: %d \t -FAULT: %d", VUSB, chargeTime, MCU, VCC, VCCcurrent, temp, batt, prog, chrg, acpr, fault);
      usart_uart1_msg_ts(strTmp);
      break; 

   case 'r'  :
      sprintf(strTmp, STR_TASK_DOCMDS ": r: Reset (via WDT) in 1 s.");
      usart_uart1_msg_ts(strTmp);
      WDTCTL = 0xDEAD; //reset via WDT
      break; 

   case 't'  :
      temp = RtnOnChipTemp()
      sprintf(strTmp, STR_TASK_DOCMDS ": Ambient temp is %d C", temp);
      usart_uart1_msg_ts(strTmp);
      break; 

   case 'v'  :
      usart_uart1_msg_ts(STR_TASK_DOCMDS ":   v" VERSION_NUM " built on "__DATE__" at "__TIME__".");
      break; 
	
   }
   OS_Delay(10)
}