#include "D:\Usuario\Escritorio\Sistemas Embebidos\Laboratorio 2\Actividad 4\adc_driver.cpp"
#include "D:\Usuario\Escritorio\Sistemas Embebidos\Laboratorio 2\Actividad 4\teclado_driver.h"
#include <avr/interrupt.h>
#include "Arduino.h"

adc_cfg adc;
int flag_adc = 0;
volatile int flag_teclado=0;
volatile int flagLCD = 0;
int flagAnterior = 100;
volatile int buttonInterrupt = 0;
volatile uint8_t portchistory = 0xFF;     // default is high because the pull-up
void (*handlerUP[9])() = {0,0,0,0,0,0,0,0,0}; 
void (*handlerDOWN[9])() = {0,0,0,0,0,0,0,0,0};

void mi_callback(){ //esta funcion de callback especifica para el LDR nos convierte el valor  
	
	flag_adc = 1;
	if(adc.adc_value <= 30){ 
		flag_teclado=9;//tecla derecha	
	}
	if(adc.adc_value > 30 && adc.adc_value <= 150){
		flag_teclado=10;// tecla arriba
	}
	if(adc.adc_value > 150 && adc.adc_value <=360){ 
		flag_teclado=11;// tecla abajo
	}
	if(adc.adc_value >360 && adc.adc_value <=535){
		flag_teclado=12;//tecla izquierda
	}
	if(adc.adc_value > 535 && adc.adc_value <= 760){
		flag_teclado=13;//tecla select	
	}
	if(adc.adc_value > 760){ 
		flag_teclado=0; //no tengo que hacer nada porque no apretaron nada
	}
	Serial.println(String(flag_teclado) + " <--flag_teclado");
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
	
	Serial.println(String(buttonInterrupt) + " <--buttonInterrupt");

	if (buttonInterrupt == 0) {//buttonInterrupt se activa si se dispara algun changePin
		adc_loop();
	}
	//buttonLCD
	if(flag_adc){
		flag_adc = 0;
		if(flag_teclado==0 && flagLCD != 0){//ninguna tecla esta presionada y se solto una tecla del LCD
			switch(flagLCD){
				//TECLAS LCD UP
				//RIGHT
				case 9: if((handlerUP[3])!= 0)(handlerUP[3])();
				break;
				//UP
				case 10: if((handlerUP[0])!= 0)(handlerUP[0])();
				break;
				//DOWN
				case 11: if((handlerUP[1])!= 0)(handlerUP[1])();
				break;
				//LEFT
				case 12: if((handlerUP[2])!= 0)(handlerUP[2])();
				break;	
				//SELECT
				case 13: if((handlerUP[4])!= 0)(handlerUP[4])();
				break;
				default: break;
			}
		flagLCD=0;
		}
		if(flag_teclado != 0){//se esta presionando algo en el LCD
			delay(50);		          // wait for debounce time
			adc_loop();
			if(flag_teclado != flagAnterior){
				flag_adc = 0;
				flagAnterior = flag_teclado;
				//selecciono el handler del button correspondiente
				switch(flag_teclado){
					//TECLAS LCD DOWN
					//RIGHT
					case 9: if((handlerDOWN[3])!= 0){(handlerDOWN[3])(); flagLCD = 9;}
					break;
					//UP
					case 10: if((handlerDOWN[0])!= 0){(handlerDOWN[0])(); flagLCD = 10;}
					break;
					//DOWN
					case 11: if((handlerDOWN[1])!= 0){(handlerDOWN[1])(); flagLCD = 11;}
					break;
					//LEFT
					case 12: if((handlerDOWN[2])!= 0){(handlerDOWN[2])(); flagLCD = 12;}
					break;	
					//SELECT
					case 13: if((handlerDOWN[4])!= 0){(handlerDOWN[4])(); flagLCD = 13;}
					break;
					default: break;
				}
			}
		}		
	}
	//button A2-A5
	if(buttonInterrupt == 1){//se presiono algun buttonPin
		Serial.println(String(flag_teclado) + " <--flag_teclado");
		switch(flag_teclado){
			//suelto A2  
			case 1: if((handlerUP[5])!= 0)(handlerUP[5])();
					buttonInterrupt=0;
					break;          
			//presionando A2
			case 2: if((handlerDOWN[5])!= 0)(handlerDOWN[5])();
					break;
			//suelto A3
			case 3: if((handlerUP[6])!= 0)(handlerUP[6])();
					buttonInterrupt=0;
					break;
			//presionando A3
			case 4: if((handlerDOWN[6])!= 0)(handlerDOWN[6])();  			
					break;
			//suelto A4
			case 5: if((handlerUP[7])!= 0)(handlerUP[7])();
					buttonInterrupt=0;
					break;
			//presionando A4
			case 6: if((handlerDOWN[7])!= 0)(handlerDOWN[7])();
			break;
			//suelto A5
			case 7: if((handlerUP[8])!= 0)(handlerUP[8])();
					buttonInterrupt=0;
					break;
			//presionando A5
			case 8: if((handlerDOWN[8])!= 0)(handlerDOWN[8])();
					break;
		}
	}
}


//este código es para cuando tenes un vector de interrupcion pero muchos pines, como dijimos para el vector pcint0 tengo pines de 0 a 7
ISR (PCINT1_vect){
	
    uint8_t changedbits;
    changedbits = PINC ^ portchistory;
    portchistory = PINC;
	Serial.println("entre");
    if(changedbits & (1 << PINC2))
    {
        //PCINT10 changed 
		if( ((PINC & (1 << PINC2))) ){
			//LOW TO HIGH
			flag_teclado = 2;
			buttonInterrupt =1;
		}else{ 
			//HIGH TO LOW
			flag_teclado = 1;
		}
    }
    /**
    if(changedbits & (1 << PINC3))
    {
        // PCINT11 changed 
		if( ((PINC & (1 << PINC3))) )
			//LOW TO HIGH
			flag_teclado = 4;
		else 
			//HIGH TO LOW
			flag_teclado = 3;
    }

    if(changedbits & (1 << PINC4))
    {
        //PCINT12 changed 
		if( ((PINC & (1 << PINC4))) )
			//LOW TO HIGH
			flag_teclado = 6;
		else 
			//HIGH TO LOW
			flag_teclado = 5;
    }
	
	if(changedbits & (1 << PINC5))
    {
        // PCINT13 changed
		if( ((PINC & (1 << PINC5))) )
			//LOW TO HIGH
			flag_teclado = 8;
		else 
			//HIGH TO LOW
			flag_teclado = 7;
    }**/
}

void key_down_callback(void (*handler)(), int tecla){
	handlerDOWN[tecla] = handler;
}
void key_up_callback(void (*handler)(), int tecla){
	handlerUP[tecla] = handler;
}