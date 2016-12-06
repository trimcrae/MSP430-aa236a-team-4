#include <salvo.h>

#include "config.h"
#include "timestamp_print.h"  //Self reference
#include "usart_uart.h"


/**
 * @file timestamp_print.c 
 * @brief This file outputs the timestamp each time it is called.
 */
/**
 * Takes the tick units inherent to the chip and converts it into readable
 * hh:mm:ss:hh output. It is called before each task/function in the code.
 * The usart_uart1 functions are givens, and are used to retrieve the ticks.
 */
void timestamp_print(const char * cP) {

  unsigned int size;
  char strTicks[TICKS_BUFFER_SIZE];
  
  // Format strTicks to indicate which USART is talking, and display a timestamp.
  long ticks = OSGetTicks();
  int hundredths = ticks % 100;
  int seconds = (ticks % 6000) / 100;
  int minutes = (ticks % 360000) / 100 / 60;
  int hours = ticks / 100 / 60 / 60;

  //Formats time into hours minutes seconds and hundredths of a second
  sprintf(strTicks, "1|%02d:%02d:%02d.%02d ",hours, minutes, seconds, hundredths);

  // Let's find out how big this string is ...
  size = 1 + sizeof(strTicks) + strlen(cP) + sizeof(CRLF);

  // Only if it can theoretically fit AND there's room in
  //  the buffer do we output it.
  if (size < TX1_BUFF_SIZE) {
    while (usart_uart1_tx_free() < size);
    usart_uart1_puts(strTicks);
    usart_uart1_puts(cP);
    usart_uart1_puts(CRLF);
  } /* if */
  
  // Else handle error.

} /* usart_uart1_msg_ts() */
