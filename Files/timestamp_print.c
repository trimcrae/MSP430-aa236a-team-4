#include "config.h"
#include "timestamp_print.h"
#include "usart_uart.h"
#include <salvo.h>


/******************************************************************************
****                                                                       ****
**                                                                           **
usart_uart1_msg_ts()

Output a time-stamped message.

**                                                                           **
****                                                                       ****
******************************************************************************/
void timestamp_print(const char * cP) {

  unsigned int size;
  char strTicks[TICKS_BUFFER_SIZE];
  
  // Format strTicks to indicate which USART is talking, and display a timestamp.
  long ticks = OSGetTicks();
  int hundredths = ticks % 100;
  int seconds = (ticks % 6000) / 100;
  int minutes = (ticks % 360000) / 100 / 60;
  int hours = ticks / 100 / 60 / 60;

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
