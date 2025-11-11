#include "exo.h"
#include <avr/io.h>

// (24.9.1)
void ADC_init_potentiometre(void) {
  // Select voltage reference for the ADC (we choose AVCC):
  CLEAR_BIT(ADMUX, REFS1);
  SET_BIT(ADMUX, REFS0);

  // Analog Channel Selection to ADC0
  CLEAR_BIT(ADMUX, MUX0);
  CLEAR_BIT(ADMUX, MUX1);
  CLEAR_BIT(ADMUX, MUX2);
  CLEAR_BIT(ADMUX, MUX3);

  // Set right adjustment for ADC value register
  SET_BIT(ADMUX, ADLAR);

  // Select which source will trigger an ADC conversion (we choose Free Running mode):
  CLEAR_BIT(ADCSRB, ADTS2);
  CLEAR_BIT(ADCSRB, ADTS1);
  CLEAR_BIT(ADCSRB, ADTS0);

  // Set ADC Prescaler to 128 (doc says that the successive approximation circuitry requires an input clock frequency between 50kHz and
  // 200kHz to get maximum resolution)
  SET_BIT(ADCSRA, ADPS2);
  SET_BIT(ADCSRA, ADPS1);
  SET_BIT(ADCSRA, ADPS0);

  SET_BIT(ADCSRA, ADEN); //Enables the ADC
}


uint8_t read_ADC_value(void) {
  // ADC Start conversion
  SET_BIT(ADCSRA, ADSC);
  // Wait for the end of the conversion
  while ((ADCSRA & (1 << ADIF)) == 0) {}
  return ADCH;
}

// Pentotiometre RV1 linked to PC0 : ADC0 / PCINT8
// LDR linked to PC1 : ADC1 / PCINT9
// NTC linked to PC2 : ADC2 / PCINT10
// Value is in ADCL / ADCH