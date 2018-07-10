#include "adc_driver.h"

volatile adc_cfg *var;
volatile int flag = 0;

void adc_init(adc_cfg *cfg) {
    
    var = cfg;

    cli();
    // clear ADLAR in ADMUX (0x7C) to right-adjust the result
    // ADCL will contain lower 8 bits, ADCH upper 2 (in last two bits)
    ADMUX &= 0B11011111;
    //ADMUX &= ~(1<<ADLAR);
    // Set REFS1..0 in ADMUX (0x7C) to change reference voltage to the
    // proper source (01)
    ADMUX |= 0B01000000;
    
    // Clear MUX3..0 in ADMUX (0x7C) in preparation for setting the analog
    // input
    ADMUX &= 0B11110000;
    
    // Set MUX3..0 in ADMUX (0x7C) to read from AD8 (Internal temp)
    // Do not set above 15! You will overrun other parts of ADMUX. A full
    // list of possible inputs is available in Table 24-4 of the ATMega328
    // datasheet
    ADMUX |= var->canal;
    
    // Set ADEN in ADCSRA (0x7A) to enable the ADC.
    // Note, this instruction takes 12 ADC clocks to execute
    ADCSRA |= 0B10000000;
    
    // Set ADATE in ADCSRA (0x7A) to enable auto-triggering.
    ADCSRA |= 0B00100000;
    
    // Clear ADTS2..0 in ADCSRB (0x7B) to set trigger mode to free running.
    // This means that as soon as an ADC has finished, the next will be
    // immediately started.
    ADCSRB &= 0B11111000;
    
    // Set the Prescaler to 128 (16000KHz/128 = 125KHz)
    // Above 200KHz 10-bit results are not reliable.
    ADCSRA |= 0B00000111;
    
    // Set ADIE in ADCSRA (0x7A) to enable the ADC interrupt.
    // Without this, the internal interrupt will not trigger.
    ADCSRA |= 0B00001000;
    
    // Set ADSC in ADCSRA (0x7A) to start the ADC conversion
    ADCSRA |=0B01000000;

    sei();
}

// Interrupt service routine for the ADC completion
ISR(ADC_vect){
    flag=1;
      var->val = ADC;
}

void adc_loop() {
  
    if (flag == 1) {
        var->callback();
        flag = 0;
    }
}
