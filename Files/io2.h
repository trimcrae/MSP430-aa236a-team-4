/******************************************************************************
****                                                                       ****
**                                                                           **
Functions available to all who include io.h

**                                                                           **
****                                                                       ****
******************************************************************************/
#ifndef __io2_h
#define __io2_h

extern void TaskIO(void);
extern void SetLED0(void);
extern void ClrLED0(void);
extern void SetLED1(void);
extern void ClrLED1(void);
extern void SetLED2(void);
extern void ClrLED2(void);
extern void setChargerOn(void);
extern void setProgOn(void);
extern void setVCCCurrentChipOn(void);

#define STR_TASK_IO  "TaskIO"

#endif /* __io2_h */
