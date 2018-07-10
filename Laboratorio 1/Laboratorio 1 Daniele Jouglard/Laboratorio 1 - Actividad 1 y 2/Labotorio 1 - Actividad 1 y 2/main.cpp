/*
 PULSADOR
 Migración a código AVR del ejemplo Button de Arduino:
 http://www.arduino.cc/en/Tutorial/Button
 */
 
#include <avr/io.h>
#include <util/delay.h>


// variables
int buttonState = 0;         // variable para leer el estado del pulsador

int main()
{
	//Inicializar el pin del pulsador como entrada
	DDRD &= ~(1<<DDD2);
	//Inicializar el pin del led como salida
	DDRB |= (1<<DDB5);

	while (1)
	{
		//Leer el estado del pulsador
		buttonState = PIND & (1<<PD2);
		
		if(buttonState) { //Si el pulsador está apretado
			//Encender el led
			PORTB |= (1<<PB5);
		} else { 
			//Apagar el led
			PORTB &= ~(1<<PB5);
		}

	}

}

/************************************************************************/
 /*
ACTIVIDAD 2: FLANCO
 Código AVR de ejemplo para detectar un flanco
 Se utiliza el hardware del ejemplo Button de Arduino
  http://www.arduino.cc/en/Tutorial/Button
 */
 /*

#include <avr/io.h>
#include <util/delay.h>

// Variables:
int buttonState;       // the current reading from the input pin
int lastButtonState = 0;   // the previous reading from the input pin

int main()
{
	//Inicializar el pin del pulsador como entrada
	DDRD &= ~(1<<DDD2);
	//Inicializar el pin del led como salida
	DDRB |= (1<<DDB5);

	while (1)
	{
		//Leer el estado del pulsador
		buttonState = PIND & (1<<PD2);

		//Si cambió el estado del pulsador...
		if (buttonState != lastButtonState)
		{
			//Si no está presionado el pulsador (se detectó un keyup)...
			if (!buttonState)
			{					
				PORTB ^= (1<<PB5); //Se modifica el estado del led (toggle vía XOR)
			}

			//Si hubo cambios válidos actualizo el valor de lectura anterior
			lastButtonState = buttonState;
		}
	}
}
*/
/* 
 Código AVR de ejemplo para realizar debounce por Software
 Si bien la estrategia de debounce difiere, el código está inspirado en el ejemplo Debounce 
 de Arduino, y el hardware es el utilizado en dicho ejemplo.
  http://www.arduino.cc/en/Tutorial/Debounce
 */
/* 
#include <avr/io.h>
#include <util/delay.h>

// Variables:
int buttonState;       // the current reading from the input pin
int lastButtonState = 0;   // the previous reading from the input pin
int debounceDelay = 50;    // the debounce time; increase if the output flickers

int main()
{
	//Inicializar el pin del pulsador como entrada
	DDRD &= ~(1<<DDD2);
	//Inicializar el pin del led como salida
	DDRB |= (1<<DDB5);

	while (1)
	{
		//Leer el estado del pulsador
		buttonState = PIND & (1<<PD2);

		//Si cambió el estado del pulsador...
		if (buttonState != lastButtonState)
		{
			//Se espera un tiempo para evitar leer el ruido del rebote del pulsador
			_delay_ms(debounceDelay);

			//Se vuelve a leer el estado del pulsador
			buttonState = PIND & (1<<PD2);

			//Si el cambio se mantiene, se interpreta como evento de keydown/keyup válido
			if (buttonState != lastButtonState)
			{
				//El pulso es válido, se implementa la lógica del sistema

				//Si no está presionado el pulsador (se detectó un keyup)...
				if (!buttonState)
				{					
					PORTB ^= (1<<PB5); //Se modifica el estado del led (toggle vía XOR)
				}

				//Si hubo cambios válidos actualizo el valor de lectura anterior
				lastButtonState = buttonState;
			}
		}
	}
}
*/