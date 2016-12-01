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
  int ACPR;
  float voltage;
  int test_fault;
  int test_acpr;
  float usb;
  charge = RtnCHRG();
  voltage = RtnBattVoltage();
  fault = 1; //If the fault bit is high then there is no fault and this will end up -1
  
  
  test_fault = P2IN && BIT7;
  test_fault = P2IN && BIT7;
  sprintf(strTmp, "p2in %d    bit7 %d  test %d", P2IN, BIT7, test_fault);
  timestamp_print(strTmp);
  if(test_fault != 0){
     fault = -1;
  }
  ACPR = -1; //need to fix this
  //ACPR bit is high if the board is plugged in
  //This will result in the ACPR value being 0 if the bit is high
  //Otherwise the ACPR value will be -1
  test_acpr = P2IN && BIT6;
  if(test_acpr != 0){
     ACPR = 0;
  }


  usb = Rtn5VUSB();

  //Status 1 = charging, charge is at 0v (constant c)
  //Status 2 = charging, charge is around 2v (constant v)
  //Status 3 = charging, charge terminated
  //Status 4 = not charging, voltage >4.13
  //Status 5 = discharging, partially charged
  //Status 6 = discharging, almost dead
  //Status 1 = discharging, is dead
  //ONLY USED FOR TESTING - delete the fault = 0 line!!!
  fault = -1;
  if (fault == -1){
    if(ACPR == 0){
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
