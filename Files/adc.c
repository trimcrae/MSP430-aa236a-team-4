#include "main.h"
#include "adc.h"
#include <msp430.h>
#include "salvo.h"
#include "usart_uart_msg_ts.h"
#if defined(__CROSSWORKS_MSP430)
#include <__cross_studio_io.h>
#endif

// Battery voltage levels when batteries are being measured (not while charging).
#define BATTERY_TRIP_VOLTAGE(x) ((x) * (4095 / (SCALE_BATT * REF_BATT)))
#define FULLY_CHARGED           BATTERY_TRIP_VOLTAGE(4.13)
#define PARTIALLY_CHARGED       BATTERY_TRIP_VOLTAGE(3.60)
#define NEARLY_DEAD             BATTERY_TRIP_VOLTAGE(2.90)

// LTC4053-related thresholds, with LiPo batteries.
#define VPROG_MAX               1.47    // VPROG in constant-current mode is 1.50V
#define VPROG_MIN               0.03    // VPROG in trickle or float is 0.00V
#define CHRG_CC                 0.50    // -CHRG is fully pulled down when in constant-current mode,
                                        //   4053 datasheet shows V_CHRG_LO_MAX of 0.4V, so use 0.5V
#define CHRG_FLOAT_LO           0.75    // -CHRG has a 30uA weak pull-down through 50k (5%) when
#define CHRG_FLOAT_HI           2.68    //   float mode is entered: 3.3V - ([15,50]uA * 50kOhm)
                                        //   @50uA: 0.80V -> 0.75V
                                        //   @15uA: 2.55V -> 2.68V
                                        //   typical observed value: 1.99V
#define CHRG_DONE               3.26    // -CHRG is hi-Z when a timeout has occurred 
#define FINAL_FLOAT             4.15    // anything above this means we've reached final battery float voltage

// Scale factors (inverse of resistor dividers, where present).
#define SCALE_CHRG              1       // no divider
#define SCALE_PROG              1       // no divider
#define SCALE_BATT              3       // :3 due to high end (4.2V)
#define SCALE_5V_USB            3       // :3 due to typ value (5V)
#define SCALE_VCC               2       // :2 due to typ value (3.3V)
#define SCALE_VCC_MCU           2       // :2 due to typ value (3.3V)

// (Internal) reference voltages used with ADC12.
#define REF_CHRG                3.3     // ADC12 Ch 0
#define REF_PROG                3.3     // ADC12 Ch 1
#define REF_BATT                3.3     // ADC12 Ch 2
#define REF_5V_USB              2.5     // ADC12 Ch 3
#define REF_VCC                 2.5     // ADC12 Ch 4
#define REF_VCC_MCU             2.5     // ADC12 Ch 11

// ADC12 is running in oversampling mode, as a software LP filter.
#define NUM_SAMPLES             8

// Sample arrays for each ADC channel. These need to be global otherwise, the 
//  compiler removes unused ones.
static unsigned int A0results[NUM_SAMPLES];  
static unsigned int A1results[NUM_SAMPLES]; 
static unsigned int A2results[NUM_SAMPLES];  
static unsigned int A3results[NUM_SAMPLES];  
static unsigned int A4results[NUM_SAMPLES];
static unsigned int A5results[NUM_SAMPLES];
static unsigned int A6results[NUM_SAMPLES];
static unsigned int A7results[NUM_SAMPLES];
static unsigned int A8results[NUM_SAMPLES];
static unsigned int A9results[NUM_SAMPLES];
static unsigned int A10results[NUM_SAMPLES];
static unsigned int A11results[NUM_SAMPLES];

// Placeholder for each new _averaged_ sampled value.
static unsigned int v_u16_temp;
static unsigned int v_u16_chrg;
static unsigned int v_u16_prog;
static unsigned int v_u16_batt;
static unsigned int v_u16_5v_usb;
static unsigned int v_u16_vcc;
static unsigned int v_u16_user_a5;
static unsigned int v_u16_user_a6;
static unsigned int v_u16_user_a7;
static unsigned int v_u16_user_a8;
static unsigned int v_u16_user_a9;
static unsigned int v_u16_vcc_mcu;

/******************************************************************************
****                                                                       ****
**                                                                           **
AverageADCSamples()

Return the average of the ADC samples 

**                                                                           **
****                                                                       ****
******************************************************************************/
static unsigned int AverageADCSamples(unsigned int * p) {
  unsigned int average;

  // Hard-coded for NUM_SAMPLES == 8!
  average  = *p++;
  average += *p++;
  average += *p++;
  average += *p++;
  average += *p++;
  average += *p++;
  average += *p++;
  average += *p++;

  return (average >> 3);
}

/******************************************************************************
****                                                                       ****
**                                                                           **
RtnXYZ()

Return various sampled values in a specified format.

**                                                                           **
****                                                                       ****
******************************************************************************/
unsigned int RtnOnChipTemp(void) {
  return ((((long)v_u16_temp-1615)*704)/4095);
}

float RtnBattVoltage(void) {
  return ((v_u16_batt * SCALE_BATT * REF_BATT)/4095);
}

float Rtn5VUSB(void) {
  return ((v_u16_5v_usb * SCALE_5V_USB * REF_5V_USB)/4095);
}

float RtnVCC(void) {
  return ((v_u16_vcc * SCALE_VCC * REF_VCC)/4095);
}

float RtnVCCMCU(void) {
  return ((v_u16_vcc_mcu * SCALE_VCC_MCU * REF_VCC_MCU)/4095);
}

float RtnCHRG(void) {
  return ((v_u16_chrg * SCALE_CHRG * REF_CHRG)/4095);
}

float RtnPROG(void) {
  return ((v_u16_prog * SCALE_PROG * REF_PROG)/4095);
}

/******************************************************************************
****                                                                       ****
**                                                                           **
RtnUserA5|6|7|8|9()

Return user-available ADC channels in raw format.

**                                                                           **
****                                                                       ****
******************************************************************************/
unsigned int RtnUserA5(void) {
  return (v_u16_user_a5);
}

unsigned int RtnUserA6(void) {
  return (v_u16_user_a6);
}

unsigned int RtnUserA7(void) {
  return (v_u16_user_a7);
}

unsigned int RtnUserA8(void) {
  return (v_u16_user_a8);
}

unsigned int RtnUserA9(void) {
  return (v_u16_user_a9);
}


/******************************************************************************
****                                                                       ****
**                                                                           **
RtnVCCCurrent()

Return +3.3V out current in mA. Current is computed as the difference between 
VCC_MCU and VCC, divided by the resistance of the MAX890 switch. 10mV 
difference = 27mA reported.

**                                                                           **
****                                                                       ****
******************************************************************************/
#if 0
float RtnVCCCurrent(void) {
  if (Rtn_3p3V_enabled()) {
    return (((v_u16_vcc_mcu * SCALE_VCC_MCU * REF_VCC_MCU)-(v_u16_vcc * SCALE_VCC * REF_VCC))*(1000/(0.09*4095)));
  }
  else {
    return 0;
  }
}
#endif


/******************************************************************************
****                                                                       ****
**                                                                           **
TaskMeasureVoltages()

Initializes ADC12 for a running sampling of all twelve ADC channels, with
appropriate references, and then at 1Hz, average all the samples of a given 
channel to yield a current value for that channel. 

Samples are tranferred into their holding arrays via the ISR.

**                                                                           **
****                                                                       ****
******************************************************************************/
void TaskADC(void) 
{ 
  usart_uart1_msg_ts(STR_TASK_ADC ": Starting.");

  P6SEL       =               0xFF;     // We're using all  external ADC
                                        //  inputs

  ADC12CTL0   =        ADC12ON+MSC;     // Turn on ADC12, extend sampling time
                                        //  to avoid overflow of results
  ADC12CTL0  |=      SHT1_8+SHT0_8;     // Same long S&H times for all channels
  ADC12CTL0  |=      REFON+REF2_5V;     // On-chip temp sensor needs 2.5V ref

  ADC12CTL1   =       SHP+CONSEQ_3;     // Use sampling timer, repeated sequence

  ADC12MCTL0  =             INCH_0;     // ref+=AVcc,  channel = A0 (SNS_CHRG)
  ADC12MCTL1  =             INCH_1;     // ref+=AVcc,  channel = A1 (SNS_PROG)
  ADC12MCTL2  =             INCH_2;     // ref+=AVcc,  channel = A2 (SNS_BATT) 
  ADC12MCTL3  =      SREF_1+INCH_3;     // ref+=VREF+, channel = A3 (SNS_+5V_USB)
  ADC12MCTL4  =      SREF_1+INCH_4;     // ref+=VREF+, channel = A4 (SNS_VCC_SYS)
  ADC12MCTL5  =             INCH_5;     // ref+=AVcc,  channel = A5 (unused)
  ADC12MCTL6  =             INCH_6;     // ref+=AVcc,  channel = A6 (unused)
  ADC12MCTL7  =             INCH_7;     // ref+=AVcc,  channel = A7 (unused)
  ADC12MCTL8  =             INCH_8;     // ref+=AVcc,  channel = A8 (unused)
  ADC12MCTL9  =             INCH_9;     // ref+=AVCC,  channel = A9 (unused)
  ADC12MCTL10 =     SREF_1+INCH_10;     // ref+=VREF+, channel = A10 (internal temp sensor)
  ADC12MCTL11 = EOS+SREF_1+INCH_11;     // ref+=VREF+, channel = AVCC/2 (MCC_MCU), end of conversion

  ADC12IE     =               0x80;     // Enable ADC12IFG.11
  ADC12CTL0 |= ENC;                     // Enable conversions
  ADC12CTL0 |= ADC12SC;                 // Start conversion

  while (1) {
    // Every 100 ticks, average all of the sampled voltages.
    // Break it up into a couple of ops, as it is slow
    OS_Delay(33);
    v_u16_chrg    = AverageADCSamples(A0results);
    v_u16_prog    = AverageADCSamples(A1results);
    v_u16_batt    = AverageADCSamples(A2results);
    v_u16_5v_usb  = AverageADCSamples(A3results);  
        
    OS_Delay(33);
    v_u16_vcc      = AverageADCSamples(A4results);
    v_u16_temp     = AverageADCSamples(A10results);
    v_u16_vcc_mcu  = AverageADCSamples(A11results);
    v_u16_user_a5  = AverageADCSamples(A5results);

    OS_Delay(34);
    v_u16_user_a6  = AverageADCSamples(A6results);
    v_u16_user_a7  = AverageADCSamples(A7results);
    v_u16_user_a8  = AverageADCSamples(A8results);
    v_u16_user_a9  = AverageADCSamples(A9results);
  }
}


void ISRADC (void) __interrupt[ADC_VECTOR] {
   static unsigned int index = 0;
  
  // IFG is cleared (automatically)
  // Copy latest An results to spots in arrays, 
  A0results[index]  =  ADC12MEM0;         
  A1results[index]  =  ADC12MEM1;         
  A2results[index]  =  ADC12MEM2;            
  A3results[index]  =  ADC12MEM3; 
  A4results[index]  =  ADC12MEM4; 
  A5results[index]  =  ADC12MEM5;              
  A6results[index]  =  ADC12MEM6;   
  A7results[index]  =  ADC12MEM7;   
  A8results[index]  =  ADC12MEM8;   
  A9results[index]  =  ADC12MEM9;
  A10results[index] = ADC12MEM10; 
  A11results[index] = ADC12MEM11;  
      
  // Increment results index, modulo
  index = (index+1)%NUM_SAMPLES;     
}
