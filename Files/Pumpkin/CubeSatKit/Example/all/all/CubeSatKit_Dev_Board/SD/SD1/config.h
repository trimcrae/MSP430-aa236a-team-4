/******************************************************************************
(C) Copyright Pumpkin, Inc. All Rights Reserved.

This file may be distributed under the terms of the License
Agreement provided with this software.

THIS FILE IS PROVIDED AS IS WITH NO WARRANTY OF ANY KIND,
INCLUDING THE WARRANTY OF DESIGN, MERCHANTABILITY AND
FITNESS FOR A PARTICULAR PURPOSE.

$Source: C:\\RCS\\D\\Pumpkin\\CubeSatKit\\Example\\all\\all\\CubeSatKit_Dev_Board\\SD\\SD1\\config.h,v $
$Author: aek $
$Revision: 3.1 $
$Date: 2010-04-24 23:26:28-07 $

******************************************************************************/
#ifndef __CONFIG_H
#define __CONFIG_H

#include <msp430.h>         // This pulls in msp430.h for all project files.

// Clocks
#define XT2CLK                            7372800L    // 7.3728MHz XT2 xtal (if fitted)
#if defined(__MSP430_HAS_FLLPLUS__)
#define DCOCLK                            5120000L    // DCO clock cranked up to 5.12MHz
#elif defined(__MSP430_HAS_BASIC_CLOCK__)
#define DCOCLK                            4975000L    // Actual value of non-FLL DCO (measured)
#endif /* __MSP430_HAS_FLLPLUS__ */
#define LFXT1CLK                          32768       // 32.768kHz LFXT1 xtal

#define ACLK                              LFXT1CLK    // ACLK = LFXT1CLK
#define MCLK                              DCOCLK      // MCLK = DCOCLK
#define SMCLK                             DCOCLK      // SMCLK = DCOCLK

#define GETCHAR_PUTCHAR_RETURN_ZERO       1           // For this project, these functions return 0
                                                      //  on error

#define RX0_BUFF_SIZE                     64          // Not used
#define TX0_BUFF_SIZE                     256         // Not used
#define RX1_BUFF_SIZE                     64          // Not used
#define TX1_BUFF_SIZE                     256         // User responses are wordy
#define RX4_BUFF_SIZE                     0           // Not used
#define TX4_BUFF_SIZE                     0           // Not used

#define INIT_HEARTBEAT_LED()               do { IO_OUTPUT(P1,BIT0); } while (0)
#define TURN_HEARTBEAT_LED_ON()            do { IO_HIGH(P1,BIT0);   } while (0)
#define TURN_HEARTBEAT_LED_OFF()           do { IO_LOW(P1,BIT0);    } while (0)


#endif /* __CONFIG_H */

