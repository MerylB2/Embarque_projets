#include "exo.h"
#include <avr/io.h>


char uart_rx(void) {
  // Wait for data to be received (20.7.1)
  while (!(UCSR0A & (1 << RXC0))) {}
  // Get and return received data from buffer (20.7.1)
  return UDR0;
}


void  uart_tx(unsigned char c) {
  // Wait for empty transmit buffer (20.6.1)
  while (!(UCSR0A & (1 << UDRE0))) {}
  // Put data into buffer, send the data (20.6.1)
  UDR0 = c;
}


void uart_printstr(const char *str) {
  while (*str) {uart_tx(*str++);}
}


void	uart_putnbr(uint16_t n) {
	if (n / 10 > 0)
		uart_putnbr(n / 10);
	uart_tx('0' + (n % 10));
}


void  uart_printstatus(uint16_t code, char error) {
  if (error) {
    uart_printstr("Error : ");
  }
  uart_putnbr(code);
  uart_printstr("\r\n");
}


void uart_init(unsigned int ubrr) {
  // Activate double speed (20.3.1)
  UCSR0A |= (1 << U2X0);

  // Set baud rate (20.5)
  UBRR0H = (unsigned char) (ubrr >> 8);
  UBRR0L = (unsigned char) ubrr;

  // Enable trasmitter and receiver (20.5)
  UCSR0B = (1 << TXEN0) | (1 << RXEN0);

  // Enable 8N1 (20.5)
  CLEAR_BIT(UCSR0C, UPM00);
  CLEAR_BIT(UCSR0C, UPM01);
  CLEAR_BIT(UCSR0C, USBS0);
  SET_BIT(UCSR0C, UCSZ00);
  SET_BIT(UCSR0C, UCSZ01);
  CLEAR_BIT(UCSR0B, UCSZ02);
}