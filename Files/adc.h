#ifndef __adc_h
#define __adc_h

extern unsigned int RtnOnChipTemp(void);
extern float RtnBattVoltage(void);
extern float Rtn5VUSB(void);
extern float RtnVCC(void);
extern float RtnVCCMCU(void);
extern float RtnCHRG(void);
extern float RtnPROG(void);
extern float RtnVCCCurrent(void);
extern void TaskADC(void);

#define STR_TASK_ADC  "TaskADC"

#endif /* __adc_h */
