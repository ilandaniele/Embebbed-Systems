#include "keyboard_driver.h"

volatile int buttonState = 0;
volatile int buttonSelect;
volatile int buttonInterrupt = 0;
adc_cfg *adc;
volatile uint8_t portchistory = 0xFF;     // default is high because the pull-up
void (*handler_up[9])();
void (*handler_down[9])();

void keyboard_driver_init() {

  adc->callback = callback_keyboard;
  adc->canal = 0;
  adc_init(adc);

  cli();          // disable global interrupts
  //Enable interrupts on pin A2
  DDRC &= ~(1 << DDC2); 
  PORTC |= (1 << PORTC2);        
  PCICR |= (1 << PCIE1);     
  PCMSK1 |= (1 << PCINT10);   
  ///////////////////////////////
  //Enable interrupts on pin A3
  DDRC &= ~(1 << DDC3); 
  PORTC |= (1 << PORTC3);        
  PCICR |= (1 << PCIE1);     
  PCMSK1 |= (1 << PCINT11);   
  ///////////////////////////////
  //Enable interrupts on pin A4
  DDRC &= ~(1 << DDC4); 
  PORTC |= (1 << PORTC4);        
  PCICR |= (1 << PCIE1);     
  PCMSK1 |= (1 << PCINT12);   
  ///////////////////////////////
  //Enable interrupts on pin A5
  DDRC &= ~(1 << DDC4); 
  PORTC |= (1 << PORTC5);        
  PCICR |= (1 << PCIE1);     
  PCMSK1 |= (1 << PCINT13);   
  ///////////////////////////////

  sei();
}

void keyboard_loop() {
  if (buttonInterrupt == 0) {
    adc_loop();
  } 
  if (buttonState) {
    switch (buttonSelect) {
        case TECLA_UP:
            (*handler_down[TECLA_UP])();
            break;
        case TECLA_DOWN:
            (*handler_down[TECLA_DOWN])();
            break;
        case TECLA_LEFT:
            (*handler_down[TECLA_LEFT])();
            break;
        case TECLA_RIGHT:
            (*handler_down[TECLA_RIGHT])();
            break;
        case TECLA_SELECT:
            (*handler_down[TECLA_SELECT])();
            break;
        case BOTON_A2:
            (*handler_down[BOTON_A2])();
            break;
        case BOTON_A3:
            (*handler_down[BOTON_A3])();
            break;
        case BOTON_A4:
            (*handler_down[BOTON_A4])();
            break;
        case BOTON_A5:
            (*handler_down[BOTON_A5])();
            break;
        default:
          break;
    }
  }

  else {
    switch (buttonSelect) {
        case TECLA_UP:
            (*handler_up[TECLA_UP])();
            break;
        case TECLA_DOWN:
            (*handler_up[TECLA_DOWN])();
            break;
        case TECLA_LEFT:
            (*handler_up[TECLA_LEFT])();
            break;
        case TECLA_RIGHT:
            (*handler_up[TECLA_RIGHT])();
            break;
        case TECLA_SELECT:
            (*handler_up[TECLA_SELECT])();
            break;
        case BOTON_A2:
            (*handler_up[BOTON_A2])();
            break;
        case BOTON_A3:
            (*handler_up[BOTON_A3])();
            break;
        case BOTON_A4:
            (*handler_up[BOTON_A4])();
            break;
        case BOTON_A5:
            (*handler_up[BOTON_A5])();
            break;
        default:
          break;
    }
  }
  
}

void key_down_callback(void (*handler)(), int tecla) { 
  handler_down[tecla] = handler;
}

void key_up_callback(void (*handler)(), int tecla) {  
  handler_up[tecla] = handler;
}

void callback_keyboard() {
  
  if (adc->val <= 50) {
    buttonSelect = TECLA_RIGHT;
    buttonState = 1;
  }
  if (adc->val > 50 && adc->val <= 200) {
    buttonSelect = TECLA_UP;
    buttonState = 1;
  }
  if (adc->val > 200 && adc->val <= 400 ) {
    buttonSelect = TECLA_DOWN;
    buttonState = 1;
  }
  if (adc->val > 400 && adc->val <= 600) {
    buttonSelect = TECLA_LEFT;
    buttonState = 1;
  }
  if (adc->val > 600 && adc->val <= 800) {
    buttonSelect = TECLA_SELECT;
    buttonState = 1;
  }

  if (adc->val > 800) {
    buttonState = 0;
  }
}


ISR (PCINT1_vect)
{
    uint8_t changedbits;


    changedbits = PINC ^ portchistory;
    portchistory = PINC;

    
    if(changedbits & (1 << PINC2))
    {
        if(! (PINC & (1 << PINC2)) == 1 )
        {
          buttonState=0;
          buttonSelect = BOTON_A2;
          buttonInterrupt = 0;
        }
        else
        {
          buttonState=1;
          buttonSelect = BOTON_A2;
          buttonInterrupt = 1;
        }
    }
    
    if(changedbits & (1 << PINC3))
    {
        
        if( !(PINC & (1 << PINC3)) == 1 )
        {
          buttonState=0;
          buttonSelect = BOTON_A3;
          buttonInterrupt = 0;
        }
        else
        {
          buttonState=1;
          buttonSelect = BOTON_A3;
          buttonInterrupt = 1;
        }
    }

    if(changedbits & (1 << PINC4))
    {
        if(! (PINC & (1 << PINC4)) == 1 )
        {
          buttonState=0;
          buttonSelect = BOTON_A4;
          buttonInterrupt = 0;
        }
        else
        {
          buttonState=1;
          buttonSelect = BOTON_A4;
          buttonInterrupt = 1;
        }
    }
    if(changedbits & (1 << PINC5))
    {
        if(! (PINC & (1 << PINC5)) == 1 )
        {
          buttonState=0;
          buttonSelect = BOTON_A5;
          buttonInterrupt = 0;
        }
        else
        {
          buttonState=1;
          buttonSelect = BOTON_A5;
          buttonInterrupt = 1;
        }
    }
    
}
