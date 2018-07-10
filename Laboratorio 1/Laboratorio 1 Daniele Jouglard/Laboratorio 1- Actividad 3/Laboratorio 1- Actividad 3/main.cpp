//ACTIVIDAD 3
#include <avr/io.h>
#include <util/delay.h>

// Variables:
int buttonState;       // the current reading from the input pin
int lastButtonState = 0;   // the previous reading from the input pin
const int debounceDelay = 50;    // the debounce time; increase if the output flickers
int estadoActual = 6;
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
							PORTB &= ~(1<<PB5);
							estadoActual++;
						}break;
						
						case 6:{
							//apagado
							//debo prender el led
							estadoActual=1;
							PORTB |= (1<<PB5);
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
					_delay_ms(2000);
					PORTB &= ~(1<<PB5); //Apaga el Led
					_delay_ms(2000);
					PORTB |= (1<<PB5);	//Enciende el Led
				}break;
				case 3:{
					//titilando a 1Hz, 1 segundo
					_delay_ms(1000);
					PORTB &= ~(1<<PB5); //Apaga el Led
					_delay_ms(1000);
					PORTB |= (1<<PB5);	//Enciende el Led
				}break;
				case 4:{
					//titilando a 2Hz, 0.5 segundos
					_delay_ms(500);
					PORTB &= ~(1<<PB5); //Apaga el Led
					_delay_ms(500);
					PORTB |= (1<<PB5);	//Enciende el Led
				}break;
				case 5:{
					//medio segundo encendido y un segundo apagado
					_delay_ms(500);
					PORTB &= ~(1<<PB5); //Apaga el Led
					_delay_ms(1000);
					PORTB |= (1<<PB5);	//Enciende el Led
				}break;
			}
		}
	}
}