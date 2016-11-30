/************************************************************ 
The contents of this file are subject to the Pumpkin Salvo
License (the "License").  You may not use this file except
in compliance with the License. You may obtain a copy of
the License at http://www.pumpkininc.com, or from
warranty@pumpkininc.com.

Software distributed under the License is distributed on an
"AS IS" basis, WITHOUT WARRANTY OF ANY KIND, either express
or implied. See the License for specific language governing
the warranty and the rights and limitations under the
License.

The Original Code is Salvo - The RTOS that runs in tiny
places(TM). Copyright (C) 1995-2003 Pumpkin, Inc. and its
Licensor(s). All Rights Reserved.

$Source: C:\\RCS\\D\\Pumpkin\\CubeSatKit\\Example\\all\\all\\CubeSatKit_Dev_Board\\SD\\SD1\\main.h,v $
$Author: aek $
$Revision: 3.8 $
$Date: 2011-09-17 21:24:35-07 $

Header file for Pumpkin CubeSat Kit software.

************************************************************/
#ifndef __main_h
#define __main_h


// Symbols, etc.
#define STR_APP_NAME          "CubeSat Kit SD\\SD1 Application"
#define STR_VERSION           "v1.1.2" " built on " __DATE__ " at " __TIME__
#define STR_FILE_NAME         "csk_temp.txt"
#define STR_BAUD_RATE         "9600"
#define STR_1TAB              "\t"
#define STR_2TABS             "\t\t"
#define STR_CRLF              "\r\n"
  
// Other macros.
#define LOOP_HERE()            do { ; } while (1)

// Target-specific symbols.
#define NOP                    _NOP()

// Scratch.
extern char strTmp[];


#endif /* __main_h */
