/******************************************************************************
****                                                                       ****
**                                                                           **
Functions available to all who include telem.h

**                                                                           **
****                                                                       ****
******************************************************************************/

#ifndef __timestamp_print_h
#define __timestamp_print_h


#ifndef GETCHAR_PUTCHAR_RETURN_ZERO
#define  EOF_RETURN_CODE            EOF
#else
#define  EOF_RETURN_CODE            0
#endif

#define CRLF                                "\n\r"
#define TICKS_BUFFER_SIZE                   15

extern void timestamp_print(const char *);


#endif /* __usart_uart_timestamp_h */
