/******************************************************************************
****                                                                       ****
**                                                                           **
Project-wide definitions and other items that might be used across a multitude
of files ... kind of a catch-all.

**                                                                           **
****                                                                       ****
******************************************************************************/
extern char strTmp[];            // Anyone can use this shared buffer

#define VERSION_NUM                     "0.0.1"

#define TIMERA0_RELOAD                  328

#define LOOP_HERE()                     do { ; } while (1) 
#define NOP		                _NOP()

// control of Watchdog timer
#define WDT_OFF				WDTCTL = WDTPW + WDTHOLD
#define WDT_ON				WDTCTL = WDT_ARST_250


