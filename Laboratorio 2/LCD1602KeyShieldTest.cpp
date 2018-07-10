/*
 LCD1602 Keypad Shield 1.0 Test Sketch - LiquidCrystal Library
 
 Este sketch demuestra el uso del LCD1602 Key Shield 1.0.
 
 Para ello se toman los pulsos de los botones mediante la entrada
 analógica AD0 y, mediante la librería LiquidCrystal de Arduino 1.0,
 se muestra en el display la tecla pulsada. 
 
 La librería LiquidCrystal
 permite controlar cualquier display LCD compatible con el 
 controlador Hitachi HD44780.
 
 La configuración de la Librería se realiza en base al esquemático
 del shield.
 
 Este ejemplo está basado en un ejemplo provisto con la documentación del
 LCD1602 Keypad Shield, el cuál ha sido adaptado en base a los ejemplos 
 de la librería LiquidCrystal de Arduino.
 
 http://www.arduino.cc/en/Tutorial/LiquidCrystal
 http://arduino.cc/en/Reference/LiquidCrystal
 
 Adaptación por Sebastián Escarza.
 */

#define F_CPU 16000000ul
#include "Arduino.h"
// include the library code:
#include <LiquidCrystal.h>

// these constants won't change.  But you can change the size of
// your LCD using them:
const int numRows = 2;
const int numCols = 16;

// initialize the library with the numbers of the interface pins
LiquidCrystal lcd(8, 9, 4, 5, 6, 7);

//Key message
char msgs[5][17] = {" Right Key:  OK ", 
                    " Up Key:     OK ", 
                    " Down Key:   OK ", 
                    " Left Key:   OK ", 
                    " Select Key: OK "};
int adc_key_val[5] ={30, 150, 360, 535, 760 };
int NUM_KEYS = 5;
int adc_key_in;
int key=-1;
int oldkey=-1;

void setup() {
  pinMode(10, OUTPUT);
  // set up the LCD's number of columns and rows: 
  lcd.begin(numCols,numRows);

  analogWrite(10, 100); //Controla intensidad backlight
  lcd.setCursor(0, 0);
  lcd.print("Key Pad Test");
  lcd.setCursor(0, 1);
  lcd.print("Sist.Emb. 2017");
  delay(2000);
  lcd.setCursor(0, 1);
  lcd.print("Press any key...");
}


// Convert ADC value to key number
int get_key(unsigned int input)
{
    int k;
    for (k = 0; k < NUM_KEYS; k++)
        if (input < adc_key_val[k])
            return k;

    if (k >= NUM_KEYS)
        k = -1;     // No valid key pressed

    return k;
}



void loop()
{
    adc_key_in = analogRead(0);     // read the value from the sensor  

    key = get_key(adc_key_in);	  // convert into key press

    if (key != oldkey)		  // if keypress is detected
    {
        delay(50);		          // wait for debounce time
        adc_key_in = analogRead(0);   // read the value from the sensor  
        key = get_key(adc_key_in);    // convert into key press
        if (key != oldkey)				
        {			
            oldkey = key;
            if (key >=0)
            {
                lcd.setCursor(0, 1);      //line=1, x=0
                lcd.print(msgs[key]);
            }
        }
    }
}


