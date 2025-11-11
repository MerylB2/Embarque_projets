#include "exo.h"
#include <avr/io.h>
#include <util/delay.h>

void init_pins(void) {
  CLEAR_BIT(DDRC, DDC0); // Set PC0 (Pentotiometre) as input pin
}


void digital_gauge(uint8_t adc_value) {
  if (adc_value == 0xff) {
    set_leds(0xff0000, 0xff0000, 0xff0000);
  } else if (adc_value >= 0xff * 66 / 100) {
    set_leds(0xff0000, 0xff0000, 0x00);
  } else if (adc_value >= 0xff * 33 / 100) {
    set_leds(0xff0000, 0x00, 0x00);
  } else {
    set_leds(0x00, 0x00, 0x00);
  }
}


int main() {
  init_pins();
  spi_master_init();
  ADC_init_potentiometre();
  set_leds(0x00, 0x00, 0x00);
  uint8_t old_value = 0;
  while (1) {
    uint8_t adc_value = read_ADC_value();
    if (adc_value != old_value) {
      digital_gauge(adc_value);
      old_value = adc_value;
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
