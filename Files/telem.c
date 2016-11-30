#include <__cross_studio_io.h>    // for CrossStudio debugging

#include "adc.h"                  // Req'd because we reference TaskADC()
#include "main.h"                 // Application header
#include "telem.h"



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
  float usb;
  charge = RtnCHRG();
  voltage = RtnBattVoltage();
  fault = 0; //need to fix this
  ACPR = 0; //need to fix this
  usb = Rtn5VUSB();

  if (-fault == 1 && -ACPR == 0){
    if(usb > 2.5){
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
