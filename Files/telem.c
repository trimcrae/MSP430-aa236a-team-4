#include <__cross_studio_io.h>    // for CrossStudio debugging

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
RtnStatus()
Returns an integer corresponding to the charge status
1: Charging in CC mode
2: Charging in CV mode
3: Charging - Fully Charged
4: Discharging - Fully Charged
5: Discharging - Partially Charged
6: Discharging - Nearly Dead
7: Discharging - Dead
8: Issue

**                                                                           **
****                                                                       ****
******************************************************************************/


int RtnStatus(void){
  float charge;
  int status;
  int fault;
  int acpr;
  float voltage;
  int test_fault;
  int test_acpr;
  float usb;
  charge = RtnCHRG();
  voltage = RtnBattVoltage();
  
  acpr = check_acpr();
  fault = check_fault();

  usb = Rtn5VUSB();

  //Status 1 = charging, charge is at 0v (constant c)
  //Status 2 = charging, charge is around 2v (constant v)
  //Status 3 = charging, charge terminated
  //Status 4 = not charging, voltage >4.13
  //Status 5 = discharging, partially charged
  //Status 6 = discharging, almost dead
  //Status 1 = discharging, is dead
  //ONLY USED FOR TESTING - delete the fault = 0 line!!!
  //acpr = 1;
  //fault = -1;
  //acpr = -1;
  //sprintf(strTmp, "\n acpr: %d", acpr);
  //debug_printf(strTmp, "\n acpr: %d", acpr);
  if (fault == -1){
    if(acpr == 0){
      if (charge <= 0.5){status = 1;}
      else if (charge <= 1.5) {status = 2;}
      else {status = 3;}
    } else {
      if (voltage > 4.13){status = 4;}
      else if (voltage > 3.6){status = 5;}
      else if (voltage > 2.9){status = 6;}
      else {status = 7;}
    }
  } else {status = 8;}
  return (status);
}

int check_fault(void){
  int fault;
  P2DIR &= ~BIT7;
  fault = P2IN && BIT7;
  fault >> 7;
  sprintf(strTmp, "p2in %d    fault bit %d  fault %d", P2IN, BIT7, fault);
  timestamp_print(strTmp);
  if(fault != 0){
     fault = -1;
  }
  return fault;
}

//Checks the acpr (whether the board is plugged in or not
int check_acpr(void){
  int acpr;
  int acpr_step1;
  //Sets the proper bit as an input
  P2DIR &= ~BIT6;
  //Gets the input bit
  acpr_step1 = P2IN & BIT6;
  acpr = acpr_step1 >> 6;
  //Shifts so that it is 0 or 1
  //acpr >> 6;
  //Prints the result
  //sprintf(strTmp, "p2in %d   bit6 %d acpr bit %d  acpr %d", P2IN, BIT6, acpr_step1, acpr);
  //debug_printf(strTmp);
  //timestamp_print(strTmp);
  //If acpr is 0, then it is plugged in
  //If acpr is 1, then return a -1
  if (acpr != 0){
    acpr = -1;
  }
  return acpr;
}
