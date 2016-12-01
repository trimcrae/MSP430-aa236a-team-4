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
  checkChargeTime();

  usb = Rtn5VUSB();

  //Status 1 = charging, charge is at 0v (constant c)
  //Status 2 = charging, charge is around 2v (constant v)
  //Status 3 = charging, charge terminated
  //Status 4 = not charging, voltage >4.13
  //Status 5 = discharging, partially charged
  //Status 6 = discharging, almost dead
  //Status 1 = discharging, is dead
  //ONLY USED FOR TESTING - delete the fault = 0 line!!!
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
  fault = P2IN & BIT7;
  fault >> 7;
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
  if (acpr != 0){
    acpr = -1;
  }
  return acpr;
}

static long charge_start;
int checkChargeTime(void){
    int charge_time;
    int acpr;
    long current_ticks;
    acpr = check_acpr();
    current_ticks = OSGetTicks();
    if (acpr != 0 || current_ticks < 100){
       charge_start = current_ticks;
    }
    
    charge_time = (current_ticks - charge_start) / 100;
    return charge_time;
}
