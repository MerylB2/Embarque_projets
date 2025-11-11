#include "exo.h"
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

#define BUFFER_SIZE 64

char rcv_buffer[BUFFER_SIZE];
int rainbow_flag = 0;


// Handle uart receive intterupt
void __vector_18(void) __attribute__ ((signal, used, externally_visible));
void __vector_18(void) {
    rainbow_flag = 0; // Set rainbow_flag to end the loop when a char is received
    uint8_t c = UDR0; // Read the byte received to clean the register
}


uint32_t  handle_backspace(char *rcv_buffer, uint32_t count, char c) {
  if (count == 0) {
    return count;
  }
  rcv_buffer[count] = '\0';
  uart_printstr("\b \b");
  return count - 1;
}


void wheel(uint8_t pos) {
  pos = 255 - pos;
  if (pos < 85) {
    uint8_t r = 255 - pos * 3;
    uint8_t g = 0;
    uint8_t b = pos * 3;
    uint32_t color = ((uint32_t)r << 16) | ((uint32_t)g << 8) | (uint32_t)b;
    set_leds(color, color, color);
  } else if (pos < 170) {
    pos = pos - 85;
    uint8_t r = 0;
    uint8_t g = pos * 3;
    uint8_t b = 255 - pos * 3;
    uint32_t color = ((uint32_t)r << 16) | ((uint32_t)g << 8) | (uint32_t)b;
    set_leds(color, color, color);
  } else {
    pos = pos - 170;
    uint8_t r = pos * 3;;
    uint8_t g = 255 - pos * 3;
    uint8_t b = 0;
    uint32_t color = ((uint32_t)r << 16) | ((uint32_t)g << 8) | (uint32_t)b;
    set_leds(color, color, color);
  }
}


int check_hex_values() {
  for (uint8_t i = 1; i < 7; i++) {
    if (is_in(rcv_buffer[i], "0123456789abcdef") == 0) {
      return 0;
    }
  }
  return 1;
}


void color_mode(uint8_t del_num) {
  static uint32_t d6_color = 0;
  static uint32_t d7_color = 0;
  static uint32_t d8_color = 0;
  rcv_buffer[7] = '\0';
  if (del_num == '6') {
    d6_color = atoi_hexa(rcv_buffer + 1);
  } else if (del_num == '7') {
    d7_color = atoi_hexa(rcv_buffer + 1);
  } else if (del_num == '8') {
    d8_color = atoi_hexa(rcv_buffer + 1);
  }
  set_leds(d6_color, d7_color, d8_color);
}


void rainbow_mode() {
  rainbow_flag = 1;
  uint8_t pos = 0;
  sei();
  while (rainbow_flag) {
    wheel(pos);
    pos++;
    _delay_ms(20);
  }
  cli();
  color_mode(0);
}


int parse_input() {
  if (!ft_strcmp("#FULLRAINBOW", rcv_buffer)) {
    rainbow_mode();
    return 1;
  } else {
    // Check first char, D char and size
    if (rcv_buffer[0] != '#' || rcv_buffer[7] != 'D' || ft_strlen(rcv_buffer) != 9) {
      return 0;
    }
    // Check LED num
    if (rcv_buffer[8] != '6' && rcv_buffer[8] != '7' && rcv_buffer[8] != '8') {
      return 0;
    }
    uint8_t del_num = rcv_buffer[8];
    // Check hexa characters
    if (check_hex_values() == 0) {
      return 0;
    }
    color_mode(del_num);
  }
  return 1;
}


// Fill the rcv_buffer with user input
void rcv_loop() {
  char c;
  rcv_buffer[BUFFER_SIZE - 1] = '\0';
  uint32_t count = 0;
  while (1) {
    c = uart_rx();
    if (c == 0x7F) {
      count = handle_backspace(rcv_buffer, count, c);
    } else if (count < BUFFER_SIZE - 1 && c != '\r') {
      rcv_buffer[count] = c;
      uart_tx(c);
      count++;
    } else if (c == '\r') {
      rcv_buffer[count] = '\0';
      uart_printstr("\r\n");
      return;
    }
  }
}


int main() {
  spi_master_init();
  uart_init(MYUBRR);
  SET_BIT(UCSR0B, RXCIE0);
  set_leds(0x00, 0x00, 0x00);
  while (1) {
    rcv_loop();
    if (parse_input() == 0) {
      uart_printstr("wrong format\r\n");
    }
  }
}

/*SPI Protocol : High-speed synchronous data transfer

- 2 Shift Registers and 1 Master clock generator.

- Master and Slave prepare the data to be sent in their respective shift Registers, and Master
generates the required clock pulses on the SCK line.

- Data is shifted from M to S on the Master Out - Slave In (MOSI) line

- After each data packet, M will synchronize S by pulling high the Slave Select line

- After shifting one byte, the SPI clock generator stops, setting the end of Transmission Flag (SPIF)

- M may continue to shift the next byte by writing it into SPDR or signal the end of packet by pulling high the SS line

- System single buffered in the transmit direction and double buffered in the receive direction
*/
