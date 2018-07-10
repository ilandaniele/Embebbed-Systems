#include "D:\Usuario\Escritorio\Sistemas Embebidos\Laboratorio 2\Actividad 4\adc_driver.h"
#include <avr/interrupt.h>
#include "Arduino.h"

volatile adc_cfg * cfg;
volatile int flag=0;
void adc_init(adc_cfg * aux){
	  
	cfg = aux;
	//hay dos modos, free running mode, que hace conversiones y no para de convertir, single conversion que convierte una sola vez y para.
	if(cfg->canal <= 14 && cfg->canal >= 0){ 
		
		cli(); //deshabilitamos las interrupciones globales
		
		/**
		El siguiente paso es configurar el voltaje de referencia usado por el ADC, este este caso es 5V. 
		El voltaje de referencia es configurado en el registro ADMUX con los bits REFS1 y REFS0. 
		En este caso es el valor por defecto y no hace falta modificar.
		En el registro ADMUX también podemos seleccionar en qué canal se va a hacer la conversión mediante los bits del MUX3 al MUX0. 
		En este caso es el valor por defecto y no hace falta modificar.
		El ADC está casi configurado, solo hace falta iniciarlo (por defecto está apagado para consumir menos) 
		y para ello se debe poner a 1 el bit ADEN del registro ADCSRA y luego poner a 1 el bit ADSC para comenzar la conversión en el mismo registro.
		En el registro ADCSRB los bits ADTS2, ADTS1 and ADTS0 determinan como una nueva conversión comienza, por defecto está en mode free running. 
		También debe ponerse a 1 el bit ADATE en del registro ADCSRA para que en modo free running comience la conversión.
		**/
		//Seteamos el canal por el cual trabajara el conversor, modificando los bits de MUX0, MUX1,MUX2,MUX3

		ADMUX = (ADMUX & 0xF0) | (cfg->canal & 0x0F); //haces un or poniendo en cero los 4 menos significativos y desp comibnandolo con cfg.canal numer
		//0x0F en binario es 0...00001111	
		//tengo que insertarle al canal en los bits de MUX3,2,1,0 el valor 0001, que indica el canal ADC1, el canal A1, donde esta conectado el sensor.
		
		ADMUX |= (1 << REFS0);		// use AVcc as the reference
		//el de referencia al admux, para indicarle cual es el alto y cual el bajo, debería estar en 0
		//este selecciona el voltaje de referencia para el ADC,
		//la peracion del 1 << argumento, desplaza un 1 hasta argumento, en este caso REFS0
		
		ADMUX &= ~(1<<ADLAR); //esto no debería estar en 0? si pero no se si lo pone en cero esto, se escribe 1 para hacer ajuste izquierdo de la conversion
		//si quiere ser derecho se lo deja en 0
		//If ADLAR is set, the result is left adjusted. If ADLAR is cleared (default), the result
		//is right adjusted.
		//hay que hacer clear para la 10 bit resolution
		
		ADCSRA |= ((1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0)); //Prescaler at 128 so we have an 125Khz clock source
		// EL ADC puede trabajar mas lento mas preciso, mas rapido menos preciso, aquí es preferible que sea lento y tengo mejor medida
		// con el prescaler modificas la velocidad del adc en trade off de la precisión, el prescaler al máximo para que sea lo mas lento posible.
		//Debe darse un reloj al ADC para establecer la velocidad de muestreo, 
		//pero esta tiene un máximo de 200kHz, este valor se establece mediante los prescaler. 
		//Estos son configurados por el registro ADCSRA y los bits relacionados 
		//que son ADPS2, ADPS1 and ADPS0 con valores de prescaler de 2 a 128. 
		//Como el microcontrolador funciona a 16 MHz se usará el prescaler 128 para que el reloj del ADC funcione a 125 KHz.
		
		
		//ADCSRB &= ~((1 << ADTS2) | (1 << ADTS1) | (1 << ADTS0)); //ADC in free-running mode, no es necesario, ya seteado en 0
		
		ADCSRA |= (1 << ADEN);      //Power up the ADC
		
		ADCSRA |= (1 << ADIE); 		//Para habilitar que cuando termine de hacer la conversion, genere una interrupcion
		//y ejecute el código de la interrupción correspondiente
		
		//ADCSRA |= (1 << ADATE);     //Signal source, in this case is the free-running
		
		
		
		ADCSRA |= (1 << ADSC);      //Asumimos que esta en single conversion mode, este bit en 1 indica que arranque con la conversion
		
		
		
		
		
		//---------------------------------------
		sei();
		
	}
	
}
void adc_loop(){
	//aquí llamo a la función callback del main que calcula el valor de Lux que le corresponde, segun el valor ADC obtenido
	if(flag==1){
		cfg->callback();
		flag=0;
	}
}

ISR(ADC_vect){
  //obtenemos el valor de resistencia medido de los registros  
  flag=1;
  int aux = ADCL;
  cfg->adc_value = (ADCH << 8) + aux; //aca estoy desplazando los 8 bits de adch a izquierda, para quedarme solo con los últimos dos, y los últimos 8 en cero,
//que luego se combinan con los bits de ADCL  
//CREO QUE EL PROBLEMA ESTA AQUI EN EL CORRIMIENTO.
  ADCSRA |= (1 << ADSC);      //Start converting, es para indicar que comience una nueva conversion, pero como esta en free running no la preciso.
  //y luego hago un OR con los 8 bits de adcl, y termina quedando, los 2 bits de ADCH + los 8 bits de ADCL
  
}
