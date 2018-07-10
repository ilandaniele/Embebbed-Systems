
/************************************************************************/
//ACTIVIDAD 4/
/* Primer inciso, sin serial, tan solamente utilizando las funciones de alto nivel
#include <avr/io.h>
#include <util/delay.h>
#include "Arduino.h"

// Variables:
// constants won't change. They're used here to
// set pin numbers:
const int buttonPin = 2;     // the number of the pushbutton pin
const int ledPin =  13;      // the number of the LED pin

int buttonState;       // the current reading from the input pin
int lastButtonState = 0;   // the previous reading from the input pin
const int debounceDelay = 50;    // the debounce time; increase if the output flickers
int estadoActual = 6;

void setup() {
	
	
	// initialize the LED pin as an output:
	pinMode(ledPin, OUTPUT);
	// initialize the pushbutton pin as an input:
	pinMode(buttonPin, INPUT);
}

void loop(){
	//Leer el estado del pulsador
	buttonState =  digitalRead(buttonPin);
	
	//Si cambió el estado del pulsador...
	if (buttonState != lastButtonState)
	{
		//Se espera un tiempo para evitar leer el ruido del rebote del pulsador
		delay(debounceDelay);

		//Se vuelve a leer el estado del pulsador
		buttonState =  digitalRead(buttonPin);

		//Si el cambio se mantiene, se interpreta como evento de keydown/keyup válido
		if (buttonState != lastButtonState)
		{
			//El pulso es válido, se implementa la lógica del sistema

			//Si no está presionado el pulsador (se detectó un keyup)...
			if (!buttonState)
			{
				switch(estadoActual){
					//aqui entro para cambiar de un estado a otro
					case 1:
					case 2:
					case 3:
					case 4:
					estadoActual++;
					break;
					case 5:{
						//medio segundo encendido y un segundo apagado
						//debo apagar el led
						digitalWrite(ledPin, LOW);
						estadoActual++;
					}break;
					
					case 6:{
						//apagado
						//debo prender el led
						estadoActual=1;
						digitalWrite(ledPin, HIGH);
					}break;
				}
			}
			//Si hubo cambios válidos actualizo el valor de lectura anterior
			lastButtonState = buttonState;
		}
	}
	else{
		//lastButtonState es igual a buttonState (no cambiaron)
		switch(estadoActual){
			case 2:{
				//titilando a 0.5Hz, 2 segundos
				delay(2000);
				digitalWrite(ledPin, LOW); //Apaga el Led
				delay(2000);
				digitalWrite(ledPin, HIGH);	//Enciende el Led
			}break;
			case 3:{
				//titilando a 1Hz, 1 segundo
				delay(1000);
				digitalWrite(ledPin, LOW);  //Apaga el Led
				delay(1000);
				digitalWrite(ledPin, HIGH);	//Enciende el Led
			}break;
			case 4:{
				//titilando a 2Hz, 0.5 segundos
				delay(500);
				digitalWrite(ledPin, LOW);  //Apaga el Led
				delay(500);
				digitalWrite(ledPin, HIGH);	//Enciende el Led
			}break;
			case 5:{
				//medio segundo encendido y un segundo apagado
				delay(500);
				digitalWrite(ledPin, LOW);  //Apaga el Led
				delay(1000);
				digitalWrite(ledPin, HIGH);	//Enciende el Led
			}break;
		}
	}
}
*/

/************************************************************************/

/*
//ACTIVIDAD 4 con Serial utilizando el pulsador
#include <avr/io.h>
#include <util/delay.h>
#include "Arduino.h"

// Variables:
// constants won't change. They're used here to
// set pin numbers:
const int buttonPin = 2;     // the number of the pushbutton pin
const int ledPin =  13;      // the number of the LED pin

int buttonState;       // the current reading from the input pin
int lastButtonState = 0;   // the previous reading from the input pin
const int debounceDelay = 50;    // the debounce time; increase if the output flickers
int estadoActual = 6;

void setup() {
	
	Serial.begin(9600);
	// initialize the LED pin as an output:
	pinMode(ledPin, OUTPUT);
	// initialize the pushbutton pin as an input:
	pinMode(buttonPin, INPUT);
}

void loop(){
	//Leer el estado del pulsador
	buttonState =   digitalRead(buttonPin);
	
	//Si cambió el estado del pulsador...
	if (buttonState != lastButtonState)
	{
		//Se espera un tiempo para evitar leer el ruido del rebote del pulsador
		delay(debounceDelay);

		//Se vuelve a leer el estado del pulsador
		buttonState =   digitalRead(buttonPin);

		//Si el cambio se mantiene, se interpreta como evento de keydown/keyup válido
		if (buttonState != lastButtonState)
		{
			//El pulso es válido, se implementa la lógica del sistema

			//Si no está presionado el pulsador (se detectó un keyup)...
			if (!buttonState)
			{
				switch(estadoActual){
					//aqui entro para cambiar de un estado a otro
					case 1:
					case 2:
					case 3:
					case 4:
					estadoActual++;
					break;
					case 5:{
						//medio segundo encendido y un segundo apagado
						//debo apagar el led
						Serial.println("Led apagado");
						digitalWrite(ledPin, LOW);
						estadoActual++;
					}break;
					
					case 6:{
						//apagado
						//debo prender el led
						estadoActual=1;
						Serial.println("Led prendido");
						digitalWrite(ledPin, HIGH);
					}break;
				}
			}
			//Si hubo cambios válidos actualizo el valor de lectura anterior
			lastButtonState = buttonState;
		}
	}
	else{
		//lastButtonState es igual a buttonState (no cambiaron)
		switch(estadoActual){
			case 2:{
				//titilando a 0.5Hz, 2 segundos
				delay(2000);
				Serial.println("Led a 0,5Hz");
				digitalWrite(ledPin, LOW); //Apaga el Led
				delay(2000);
				digitalWrite(ledPin, HIGH);	//Enciende el Led
			}break;
			case 3:{
				//titilando a 1Hz, 1 segundo
				delay(1000);
				Serial.println("Led a 1Hz");
				digitalWrite(ledPin, LOW);  //Apaga el Led
				delay(1000);
				digitalWrite(ledPin, HIGH);	//Enciende el Led
			}break;
			case 4:{
				//titilando a 2Hz, 0.5 segundos
				delay(500);
				Serial.println("Led a 2Hz");
				digitalWrite(ledPin, LOW);  //Apaga el Led
				delay(500);
				digitalWrite(ledPin, HIGH);	//Enciende el Led
			}break;
			case 5:{
				//medio segundo encendido y un segundo apagado
				delay(500);
				Serial.println("Led un segundo encendido y un segundo apagado");
				digitalWrite(ledPin, LOW);  //Apaga el Led
				delay(1000);
				digitalWrite(ledPin, HIGH);	//Enciende el Led
			}break;
		}
	}
}
*/
//ACTIVDAD 4 Con serial, utilizando entrada serial desde el teclado
#include <avr/io.h>
#include <util/delay.h>
#include "Arduino.h"

// Variables:
// constants won't change. They're used here to
// set pin numbers:
const int buttonPin = 2;     // the number of the pushbutton pin
const int ledPin =  13;      // the number of the LED pin


const int debounceDelay = 50;    // the debounce time; increase if the output flickers
int estadoActual = 0;
int entrada = 1;
void setup() {
	
	Serial.begin(9600);
	// initialize the LED pin as an output:
	pinMode(ledPin, OUTPUT);
	// initialize the pushbutton pin as an input:
	pinMode(buttonPin, INPUT);
}
void modo(int estadoActual){
	switch(estadoActual){
		case 0:{
			Serial.println("Led apagado");
			digitalWrite(ledPin, LOW);
			
		}break;
		case 1:{
			Serial.println("Led prendido");
			digitalWrite(ledPin, HIGH);
			
		}break;
		case 2:{	//titilando a 0.5Hz, 2 segundos
			delay(2000);
			Serial.println("Led a 0,5Hz");
			digitalWrite(ledPin, LOW); //Apaga el Led
			delay(2000);
			digitalWrite(ledPin, HIGH);	//Enciende el Led
			
		}break;
		case 3:{
			//titilando a 1Hz, 1 segundo
			delay(1000);
			Serial.println("Led a 1Hz");
			digitalWrite(ledPin, LOW);  //Apaga el Led
			delay(1000);
			digitalWrite(ledPin, HIGH);	//Enciende el Led
			
		}
		break;
		case 4:{
			//titilando a 2Hz, 0.5 segundos
			delay(500);
			Serial.println("Led a 2Hz");
			digitalWrite(ledPin, LOW);  //Apaga el Led
			delay(500);
			digitalWrite(ledPin, HIGH);	//Enciende el Led
			
		}break;
		case 5: {
			//medio segundo encendido y un segundo apagado
			delay(500);
			Serial.println("Led un segundo encendido y un segundo apagado");
			digitalWrite(ledPin, LOW);  //Apaga el Led
			delay(1000);
			digitalWrite(ledPin, HIGH);	//Enciende el Led
			
		}
		break;
		
	}

}

void loop(){
	//Leer el estado del pulsador
	entrada = Serial.read();
	entrada = entrada % 6;
	if(entrada== 1 ||entrada== 2 || entrada== 3 || entrada== 4 || entrada== 5 || entrada== 0 ){ 
		estadoActual=entrada;
	}
	modo(estadoActual);
	
	}
	
	                                                                     
/************************************************************************/