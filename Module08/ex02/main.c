#include "exo.h"
#include <avr/io.h>
#include <util/delay.h>


void spi_master_init() {
  SET_BIT(DDRB, DDB2); // Set SS output
  SET_BIT(DDRB, DDB3); // Set MOSI output
  SET_BIT(DDRB, DDB5); // Set SCK output

  // Enable SPI, Master, set clock rate fck/16
  SPCR = (1 << SPE)|(1 << MSTR)|(1 << SPR0);
}

void spi_master_transmit(uint8_t data) {
  SPDR = data; // Start transmission;

  // Wait for transmission complete
  while (!(SPSR & (1 << SPIF))) {}
  
}


void send_color(uint8_t r, uint8_t g, uint8_t b) {
  spi_master_transmit(0b11100001);
  spi_master_transmit(b);
  spi_master_transmit(g);
  spi_master_transmit(r);
}


void send_start_frame(void) {
  for (uint32_t i = 0; i < 4; i++) {
    spi_master_transmit(0x00);
  }
}


void send_end_frame(void) {
  for (uint32_t i = 0; i < 4; i++) {
    spi_master_transmit(0xff);
  }
}


void set_leds(uint32_t d6, uint32_t d7, uint32_t d8) {
  send_start_frame();
  send_color((uint8_t)(d6 >> 16), (uint8_t)(d6 >> 8), (uint8_t)(d6));
  send_color((uint8_t)(d7 >> 16), (uint8_t)(d7 >> 8), (uint8_t)(d7));
  send_color((uint8_t)(d8 >> 16), (uint8_t)(d8 >> 8), (uint8_t)(d8));
  send_end_frame();

}


int main() {
  spi_master_init();
  while (1) {
    set_leds(0xff0000, 0x00, 0x00);
    _delay_ms(250);
    set_leds(0x00, 0x00ff00, 0x00);
    _delay_ms(250);
    set_leds(0x00, 0x00, 0x0000ff);
    _delay_ms(250);
    set_leds(0x00, 0x00, 0x00);
    _delay_ms(250);
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
