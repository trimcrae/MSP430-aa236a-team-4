/******************************************************************************
****                                                                       ****
**                                                                           **
Functions available to all who include ui.h

**                                                                           **
****                                                                       ****
******************************************************************************/
#ifndef __ui_h
#define __ui_h

extern void TaskDoCmds(void);
extern void setChargerOn(void);
void setProgOn(void);
void setVCCCurrentChipOn(void);

#define STR_TASK_DOCMDS   "TaskDoCmds"

#endif /* __ui_h */
