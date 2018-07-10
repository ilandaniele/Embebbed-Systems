#include "D:\Usuario\Escritorio\Sistemas Embebidos\Laboratorio 2\Actividad 5\adc_driver.cpp"
#include "D:\Usuario\Escritorio\Sistemas Embebidos\Laboratorio 2\Actividad 5\teclado_driver.h"
#include <avr/interrupt.h>
#include "Arduino.h"

adc_cfg adc;

volatile int buttonState = 0;
volatile int flag_teclado;
volatile int buttonInterrupt = 0;

volatile uint8_t portchistory = 0xFF;     // default is high because the pull-up
void (*handlerUP[9])() = {0,0,0,0,0,0,0,0,0}; 
void (*handlerDOWN[9])() = {0,0,0,0,0,0,0,0,0};

void mi_callback(){ //esta funcion de callback especifica para el LDR nos convierte el valor  
	
	if(adc.adc_value <= 30){ 
		Serial.println("entre tecla derecha");
		flag_teclado=3;//tecla derecha	
		buttonState=1;
		
	}
	if(adc.adc_value > 30 && adc.adc_value <= 150){
		flag_teclado=0;// tecla arriba
		buttonState=1;
		Serial.println("entre tecla arriba");
	}
	if(adc.adc_value > 150 && adc.adc_value <=360){ 
		flag_teclado=1;// tecla abajo
		buttonState=1;
	}
	if(adc.adc_value >360 && adc.adc_value <=535){
		flag_teclado=2;//tecla izquierda
		buttonState=1;
		Serial.println("entre tecla izquierda");
	}
	if(adc.adc_value > 535 && adc.adc_value <= 760){
		flag_teclado=4;//tecla select
		buttonState=1;
	}
	if(adc.adc_value > 760){ 
		 //no tengo que hacer nada porque no apretaron nada
		 //Serial.println("entre");
		buttonState=0;
		
	}	
}

void teclado_init(){
		cli(); 
		//ADC
		adc.canal=0;
		adc.callback = mi_callback;
		adc_init(&adc);
		
		//configuro pin change interrupts
		DDRC &= ~( (1 << DDC2) | (1 << DDC3) | (1 << DDC4) | (1 << DDC5));         // Setea en 0 los pines PC2, PC3, PC4 Y PC5
		// PC2 (PCINT10 pin), PC3 (PCINT11 pin), etc is now an input
		//PCINT10, PCINT11, PCINT12 y PCINT13		
		PORTC |= (1 << PORTC2) | (1 << PORTC3) | (1 << PORTC4) | (1 << PORTC5);        // turn On the Pull-up
		// PC2 is now an input with pull-up enabled
		PCICR |= (1 << PCIE1);     // set PCIE1 to enable PCMSK1 scan
		PCMSK1 |= (1 << PCINT10) | (1 << PCINT11) | (1 << PCINT12) | (1 << PCINT13) ;   // set PCINT10 to trigger an interrupt on state change
		sei();
	
}
void teclado_loop(){
	if (buttonInterrupt == 0) {
    adc_loop();
	//delay(50);
  } 
  if (buttonState) {
    switch (flag_teclado) {
        case 0:
            (handlerDOWN[0])();
            break;
        case 1:
            (handlerDOWN[1])();
            break;
        case 2:
            (handlerDOWN[2])();
            break;
        case 3:
            (handlerDOWN[3])();
            break;
        case 4:
            (handlerDOWN[4])();
            break;
        case 5:
            (handlerDOWN[5])();
            break;
        case 6:
            (handlerDOWN[6])();
            break;
        case 7:
            (handlerDOWN[7])();
            break;
        case 8:
            (handlerDOWN[8])();
            break;
        default:
          break;
    }
  }

  else {
    switch (flag_teclado) {
        case 0: //UP
            (handlerUP[0])();
            break;
        case 1: //DOWN
            (handlerUP[1])();
            break;
        case 2: //LEFT
            (handlerUP[2])();
            break;
        case 3: //RIGHT
            (handlerUP[3])();
            break;
        case 4: //SELECT
            (handlerUP[4])();
            break;
        case 5: //A2
            (handlerUP[5])();
            break;
        case 6: //A3
            (handlerUP[6])();
            break;
        case 7: //aa4
            (handlerUP[7])();
            break;
        case 8: //a5
            (handlerUP[8])();
            break;
        default:
          break;
    }
  }
  
	
}


//este código es para cuando tenes un vector de interrupcion pero muchos pines, como dijimos para el vector pcint0 tengo pines de 0 a 7
ISR (PCINT1_vect){
    uint8_t changedbits;
    changedbits = PINC ^ portchistory;
    portchistory = PINC;   	
	
    if(changedbits & (1 << PINC2))
    {
        //PCINT10 changed 
		if( ((PINC & (1 << PINC2))) ){
			//LOW TO HIGH
			flag_teclado = 5;
			buttonState=1;
			buttonInterrupt=1;
		}else{
			//HIGH TO LOW
			flag_teclado = 5;
			buttonState=0;
			buttonInterrupt=0;
		}
    }
    
	
    if(changedbits & (1 << PINC3))
    {
        // PCINT11 changed 
		if( ((PINC & (1 << PINC3))) ){
			//LOW TO HIGH
			flag_teclado = 6;
			buttonState=1;
			buttonInterrupt=1;
		}else{ 
			//HIGH TO LOW
			flag_teclado = 6;
			buttonState=0;
			buttonInterrupt=0;
		}
    }

    if(changedbits & (1 << PINC4))
    {
        //PCINT12 changed 
		if( ((PINC & (1 << PINC4))) ){
			//LOW TO HIGH
			flag_teclado = 7;
			buttonState=1;
			buttonInterrupt=1;
		}else{ 
			//HIGH TO LOW
			flag_teclado = 7;
			buttonState=0;
			buttonInterrupt=0;
		}
    }
	
	if(changedbits & (1 << PINC5))
    {
        // PCINT13 changed
		if( ((PINC & (1 << PINC5))) ){
			//LOW TO HIGH
			flag_teclado = 8;
			buttonState=1;
			buttonInterrupt=1;
		}else{
			//HIGH TO LOW
			flag_teclado = 8;
			buttonState=0;
			buttonInterrupt=0;
		}
    }
}

void key_down_callback(void (*handler)(), int tecla){
	handlerDOWN[tecla] = handler;
}
void key_up_callback(void (*handler)(), int tecla){
	handlerUP[tecla] = handler;
}