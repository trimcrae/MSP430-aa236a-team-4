/******************************************************************************
****                                                                       ****
**                                                                           **
Functions available to all who wish to reference modules in task_io.c!
**                                                                           **
****                                                                       ****
******************************************************************************/
#ifndef __TASK_IO_H
#define __TASK_IO_H

extern void task_io(void);
extern void SetLED0(void);
extern void ClrLED0(void);

#define STR_TASK_IO  "task_io"

#endif /* __TASK_IO_H */
