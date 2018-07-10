#include "Arduino.h"
#include "adc_driver.h"
#include "keyboard_driver.h"

#define KEYS_CHANNEL  0

struct handler{
	void (*key_up)();
	void (*key_down)();
	};

struct handler handlers [NUM_KEYS];

struct adc_cfg cfg;

byte volatile presionado = 0; 
byte volatile flag_up = 0;
byte volatile flag_down = 0;
byte volatile flag_lcd = 0;
int flag_adc = 0;

int adc_key_in;
int old_adc_key_in = -10;
int key=-1;
int oldkey=-10;
int adc_key_val[5] ={30, 150, 360, 535, 760 };

void readADC(int value){
	adc_key_in = value;
	flag_adc = 1;
	//Serial.println(String(value) + " teclado");
}

// Convert ADC value to key number
int get_key(int input)
{
	int k;
	for (k = 0; k < NUM_KEYS_LCD; k++)
		if (input < adc_key_val[k])
			return k;

	if (k >= NUM_KEYS_LCD)
		k = -1;     // No valid key pressed

	return k;
}

int map_key(int key)
{
	int retorno = 0;
	switch(key){
		case 0:
			retorno = TECLA_RIGHT;
		break;
		case 1:
			retorno = TECLA_UP;
		break;
		case 2:
			retorno = TECLA_DOWN;
		break;
		case 3:
			retorno = TECLA_LEFT;
		break;
		case 4:
			retorno = TECLA_SELECT;
		break;
	}
	
	return retorno;
}

 void key_down_callback(void (*handler)(), int tecla)
 {
	handlers[tecla].key_down = handler;
 }

 void key_up_callback(void (*handler)(), int tecla)
 {
	 handlers[tecla].key_up = handler;
 }

 void keyboard_init()
 {
	 cli();
	 
	 //PULSADOR
	 
	 DDRC &= ~(1 << DDC2 | 1 << DDC3 | 1 << DDC4 | 1 << DDC5);
	
	 PORTC |= (1 << PC2) | (1 << PC3) | (1 << PC4) | (1 << PC5);        // turn On the Pull-up
	 
	 //habilito interrupciones grupo 1
	 PCICR |= (1<<PCIE1);
	 
	 //Habilito interrupciones de los pines del grupo 1 a utilizar
	 PCMSK1 |= (1<<PCINT10) | (1<<PCINT11) | (1<<PCINT12) | (1<<PCINT13);
	 //A1-A5
	 
	 cfg.channel = KEYS_CHANNEL;
	 cfg.callback = readADC;
	 
	 adc_init(&cfg);
	 
	 sei();
 }

 void keyboard_loop()
 { 
	 adc_loop();

	 if (flag_adc)
	 {
		 flag_adc = 0;
		 key = get_key(adc_key_in);	  // convert into key press

		 if( key==-1 && flag_lcd ) // Detecto cuando se suelta una tecla 
		 {
			 int shift = 0;
			 int tecla;
			 for (tecla = TECLA_UP; tecla <= TECLA_SELECT; tecla++)
			 {
				 if (flag_lcd & (1<<shift))
				 {
					 flag_lcd = 0;
					 handlers[tecla].key_up();
				 }
				 shift++;
			 }
		 }
	 	
		 if ( key != -1)		  // if keypress is detected
		 {
			 delay(50);		          // wait for debounce time
			 adc_loop();			 
			 if (flag_adc)
			 {
				 flag_adc = 0;
				 key = get_key(adc_key_in);    // convert into key press
		 
				 if (key != oldkey)
				 {			 
					 oldkey = key;
					 if (key >=0)
					 {				 
						 Serial.println(String(adc_key_in));
						 int aux = map_key(key);				 
						 flag_lcd |= (1<<aux);
						 handlers[aux].key_down();
					 }
				 }
			 }
		 }
	 }
	 //A2-A5
	 int shift = 2;
	 int tecla;	 
	 for (tecla = BOTON_A2; tecla <= BOTON_A5; tecla++)
	 {
		 if (flag_up & (1<<shift))
		 {
			flag_up &= ~(1<<shift);
			handlers[tecla].key_up();			
		 }	
		 if (flag_down & (1<<shift))
		 {
			 flag_down &= ~(1<<shift);
			 handlers[tecla].key_down();
		 }	
		 shift++;	
	 }
 }

ISR(PCINT1_vect)
{
	
	if( (PINC & (1 << PC2)) == 0 && (presionado & (1<<2)))
	{
		presionado &= ~(1<<2);
		flag_up |= (1<<2);
		Serial.println("solto 2");
	}
	if( (PINC & (1 << PC2)) == 4 && !(presionado & (1<<2)))
	{
		presionado |= (1<<2);
		flag_down |= (1<<2);
		Serial.println("presiono 2");
	}
	/*
	if( (PINC & (1 << PC3)) == 0 && (presionado & (1<<3)))
	{
		presionado &= ~(1<<3);
		flag_up |= (1<<3);
		Serial.println("solto 3");
	}
	if( (PINC & (1 << PC3)) == 8 && !(presionado & (1<<3)))
	{
		presionado |= (1<<3);
		flag_down |= (1<<3);
		Serial.println("presiono 3");
	}
	
	if( (PINC & (1 << PC4)) == 0 && (presionado & (1<<4)))
	{
		presionado &= ~(1<<4);
		flag_up |= (1<<4);
		Serial.println("solto 4");
	}
	if( (PINC & (1 << 4)) == 16 && !(presionado & (1<<4)))
	{
		presionado |= (1<<4);
		flag_down |= (1<<4);
		Serial.println("presiono 4");
	}
	
	if( (PINC & (1 << PC5)) == 0 && (presionado & (1<<5)))
	{
		presionado &= ~(1<<5);
		flag_up |= (1<<5);
		Serial.println("solto 5");
	}
	if( (PINC & (1 << PC5)) == 32 && !(presionado & (1<<5)))
	{
		presionado |= (1<<5);
		flag_down |= (1<<5);
		Serial.println("presiono 5");
	}*/
}