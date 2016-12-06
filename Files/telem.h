/******************************************************************************
****                                                                       ****
**                                                                           **
Functions available to all who include telem.h

**                                                                           **
****                                                                       ****
******************************************************************************/
#ifndef __telem_h
#define __telem_h

extern int RtnStatus(void);

extern int check_fault(void);
extern int check_acpr(void);
extern long checkChargeTime(void);

#endif /* __telem_h */
