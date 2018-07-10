// Arduino timer CTC interrupt example
// www.engblaze.com


/****************/
/*
 * PREGUNTAS:
 * //PREGUNTAR //aca le tengo que indicar que es el A2, cual es el número?
 * tengo que ponerle un timer adecuado a cada pcintx?
 * como hago para indicarle cuando largar la interrupcion al pulsador?
 */
 
// avr-libc library includes
#include <avr/io.h>
#include <avr/interrupt.h>
// include the library code:
#include <LiquidCrystal.h>
#define F_CPU 16000000ul
#include "Arduino.h"
 
//aca le tengo que indicar que es el A2, cual es el número?
 volatile int minutos = 0;
 volatile int segundos = 0;
 volatile int centisegundos = 0;
 volatile int modo=0;
 volatile int flag=0;
 volatile int buttonState=0;
 volatile int lastButtonState=0;
 volatile uint8_t portchistory = 0xFF;


 // these constants won't change.  But you can change the size of
// your LCD using them:
const int numRows = 2;
const int numCols = 8;

// initialize the library with the numbers of the interface pins
LiquidCrystal lcd(8, 9, 4, 5, 6, 7);


void setup()
{
    pinMode(A2, INPUT);

    //LCD
     pinMode(10, OUTPUT);
    // set up the LCD's number of columns and rows: 
    lcd.begin(numCols,numRows);
    analogWrite(10, 200); //Controla intensidad backlight
    lcd.setCursor(6, 0);
    lcd.print("Timer");
    
  /***********************************************************************************/
    cli();          // disable global interrupts
    //configuro pin change interrupts
    DDRC &= ~(1 << DDC2);         // Clear the PC2 pin
    // PC2 (PCINT10 pin) is now an input

    PORTC |= (1 << PORTC2);        // turn On the Pull-up
    // PC2 is now an input with pull-up enabled

    PCICR |= (1 << PCIE1);     // set PCIE1 to enable PCMSK1 scan
    PCMSK1 |= (1 << PCINT10);   // set PCINT10 to trigger an interrupt on state change 
    //duda con esto, cuando larga las interrupciones? cuanto? solo una vez? como no hay loop de esto
    /********************************************************************************/
    // initialize Timer0
    
    TCCR0A = 0;     // set entire TCCR1A register to 0
    TCCR0B = 0;     // same for TCCR1B
    TCNT0 = 0;      //agREGADO ULTIMO
    // set compare match register to desired timer count:
    OCR0A = 156; //duda aquí, no se si va entero. SI VA ENTERO, pierdo centisegundos pero igual es muy preciso
    //con esto hago que el cronometro tenga overflow cada 1 centisegundo, y ahí se envía una interrupcion, en el momento que hace el overflow
    //y se resetea el timer, luego de esto se crea la interrupción, que sigue lo especificado abajo, y entonces pasa a cambiar lo que hay en el lcd.
    // turn on CTC mode:
    TCCR0A |= (1 << WGM01); //cambiado el timer a 01 
    // Set CS10 and CS12 bits for 1024 prescaler:
    TCCR0B |= (1 << CS10);
    TCCR0B |= (1 << CS12);
    // enable timer compare interrupt:
    TIMSK0 |= (1 << OCIE0A);
    // enable global interrupts:
    sei();
}
 
void loop()
{
  
  if(buttonState == 1){
    modo++;
    modo = modo % 3;
    buttonState=0;
    if(modo == 0){
      //quiere decir que cambiaron al modo 0 y arranco una sola vez el timer, no todas las veces.
        TCCR0B |= (1 << CS10);
        TCCR0B |= (1 << CS12);
    }
  }
  
  if(modo==0){
    //con esto seteamos en 1 los bits de los registros en el timer, con el prescaler 1024 para que arranque la cuenta
    //actualizo el display
    //le indico que se prenda el led con los minutos
    if(flag==1){
      flag=0;
      lcd.setCursor(1, 1); 
      lcd.print(minutos);
      lcd.setCursor(2, 1); 
      lcd.print(":");
      //le indico que se prenda el led con los segundos
      if(segundos<10) {lcd.setCursor(3,1);lcd.print("0");lcd.setCursor(4,1);lcd.print(segundos);}
      else {lcd.setCursor(3, 1); lcd.print(segundos);}
      lcd.setCursor(5, 1); lcd.print(":");
      //le indico que se prenda el led con los centisegundos
      lcd.setCursor(6, 1); lcd.print(centisegundos);     
    } 
  }
  if(modo==1){
              //debo hacer un stop
              //le indico al clock que la corte, ver como hago esto tambien
              
              //se limpia los registros del timer para que frene y no cuente mas
              TCCR0B &= ~ (1 << CS10);
              TCCR0B &= ~ (1 << CS11);
              TCCR0B &= ~ (1 << CS12);
  }
  if(modo==2){
        //borro todo, resetea todo a cero.
        minutos=0;
        segundos=0;
        centisegundos=0;
        //le indico que se prenda el led con los minutos
        lcd.setCursor(0, 1); lcd.print(minutos);lcd.setCursor(1, 1); lcd.print(minutos);
        //le indico que se prenda el led con los segundos
        lcd.setCursor(3, 1); lcd.print(segundos);lcd.setCursor(4, 1); lcd.print(segundos);
        //le indico que se prenda el led con los centisegundos
        lcd.setCursor(6, 1); lcd.print(centisegundos);lcd.setCursor(7, 1); lcd.print(centisegundos);
  }
}
 
ISR(TIMER0_COMPA_vect)
{   
    flag=1;
    centisegundos++;
    if(centisegundos == 100){
        centisegundos=0;
        segundos++;
        if(segundos == 60){
          minutos++;
          segundos=0;
          centisegundos=0;
        }
    }    
}

//el pcint1_vect es por la interrupcion para el grupo
ISR (PCINT1_vect)
{


    uint8_t changedbits;
    changedbits = PINC ^ portchistory;
    portchistory = PINC; 
    if(changedbits & (1 << PINC2))
    {
        //PCINT10 changed 
        if( ((PINC & (1 << PINC2))) ){
          //LOW TO HIGH
          buttonState=1;
        }
    }
}

