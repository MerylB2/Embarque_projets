#include "exo.h"
#include <avr/io.h>
#include <util/delay.h>

typedef enum e_primary_color {
  RED = 0,
  GREEN = 1,
  BLUE = 2,
} t_primary_color;


typedef enum e_led {
  D6 = 0,
  D7 = 1,
  D8 = 2,
} t_led;


void init_pins(void) {
  CLEAR_BIT(DDRC, DDC0); // Set PC0 (Pentotiometre) as input pin
  CLEAR_BIT(DDRD, DDD2); // Set PD2 (Switch 1) as input pin
  CLEAR_BIT(DDRD, DDD4); // Set PD4 (Switch 2) as input pin
}


uint32_t get_color_mask(uint8_t primary_color, uint8_t color_value) {
  return ((uint32_t)color_value << ((2 - primary_color) * 8));
}


void set_primary_color(uint8_t primary_color, uint8_t led) {
  static uint32_t d6_color = 0;
  static uint32_t d7_color = 0;
  static uint32_t d8_color = 0;
  uint8_t adc_value = read_ADC_value();
  uint32_t color_mask = get_color_mask(primary_color, adc_value);
  if (led == D6) {
    // Reset the color we want to modify
    d6_color &= ~((uint32_t)0xFF << ((2 - primary_color) * 8));
    d6_color |= color_mask;
  } else if (led == D7) {
    // Reset the color we want to modify
    d7_color &= ~((uint32_t)0xFF << ((2 - primary_color) * 8));
    d7_color |= color_mask;
  } else if (led == D8) {
    // Reset the color we want to modify
    d8_color &= ~((uint32_t)0xFF << ((2 - primary_color) * 8));
    d8_color |= color_mask;
  }
  set_leds(d6_color, d7_color, d8_color);
}


int main() {
  init_pins();
  spi_master_init();
  ADC_init_potentiometre();
  uint8_t current_color = RED;
  uint8_t current_led = D6;
  set_leds(0x00, 0x00, 0x00);
  while (1) {
    if ((PIND & (1 << PD2)) == 0) {
      set_primary_color(current_color, current_led);
      current_color++;
      if (current_color > BLUE) {
        current_color = RED;
      }
      while ((PIND & (1 << PD2)) == 0) {}
      _delay_ms(100);
    }
    if ((PIND & (1 << PD4)) == 0) {
      current_led++;
      if (current_led > D8) {
        current_led = D6;
      }
      while ((PIND & (1 << PD4)) == 0) {}
      _delay_ms(100);
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
