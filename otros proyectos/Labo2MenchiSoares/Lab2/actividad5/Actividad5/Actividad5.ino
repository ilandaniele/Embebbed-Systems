#include "keyboard_driver.h"
#include <LiquidCrystal.h>

// These constants, define values needed for the LDR readings and ADC
#define LDR_PIN                   1
#define MAX_ADC_READING           1023
#define ADC_REF_VOLTAGE           5.0
#define REF_RESISTANCE            1000  // measure this for best results
#define LUX_CALC_SCALAR           5333349  //Found values in real life => 12518931
#define LUX_CALC_EXPONENT         -1.437 //Found values in real life => -1.405 

#define LUMINOSIDAD_ACTUAL			0
#define LUMINOSIDAD_MAXIMA_MINIMA	1
#define LUMINOSIDAD_PROMEDIO		2
#define AJUSTE_DIMMER				3

// these constants won't change.  But you can change the size of
// your LCD using them:
const int numRows = 2;
const int numCols = 16;
int dimmer = 80;
int index = 0, modo = 0;
int medium[200];
float ldrLux;
float minimum = 10, maximum = 100;
int leerTeclado=1;
adc_cfg adc_ldr;
volatile int segundos=0, centesimas=0;
int pression[9] = {0,0,0,0,0,0,0,0,0};
LiquidCrystal lcd(8, 9, 4, 5, 6, 7);

void callbackLdr();

void flancoAscTeclaUp();
void flancoDescTeclaUp();
void flancoAscTeclaDown();
void flancoDescTeclaDown();
void flancoAscTeclaLeft();
void flancoDescTeclaLeft();
void flancoAscTeclaRight();
void flancoDescTeclaRight();
void flancoAscTeclaSelect();
void flancoDescTeclaSelect();
void flancoAscTeclaA2();
void flancoDescTeclaA2();
void flancoAscTeclaA3();
void flancoDescTeclaA3();
void flancoAscTeclaA4();
void flancoDescTeclaA4();
void flancoAscTeclaA5();
void flancoDescTeclaA5();

void menus(int modo);


void setup() {
  lcd.begin(numCols,numRows);
  analogWrite(10, dimmer*2.55);
  Serial.begin(9600);
  
  /*
  adc_ldr.callback = callbackLdr;
      adc_ldr.canal = LDR_PIN;
      adc_init(&adc_ldr);
  */
      
  // Asociamos handlers de las teclas
  key_down_callback(&flancoAscTeclaUp, TECLA_UP);
  key_down_callback(&flancoAscTeclaRight, TECLA_RIGHT);
  key_down_callback(&flancoAscTeclaDown, TECLA_DOWN);
  key_down_callback(&flancoAscTeclaLeft, TECLA_LEFT);
  key_down_callback(&flancoAscTeclaSelect, TECLA_SELECT);
  key_down_callback(&flancoAscTeclaA2, BOTON_A2);
  key_down_callback(&flancoAscTeclaA3, BOTON_A3);
  key_down_callback(&flancoAscTeclaA4, BOTON_A4);
  key_down_callback(&flancoAscTeclaA5, BOTON_A5);  

  key_up_callback(&flancoDescTeclaUp, TECLA_UP);
  key_up_callback(&flancoDescTeclaRight, TECLA_RIGHT);
  key_up_callback(&flancoDescTeclaDown, TECLA_DOWN);
  key_up_callback(&flancoDescTeclaLeft, TECLA_LEFT);
  key_up_callback(&flancoDescTeclaSelect, TECLA_SELECT);
  key_up_callback(&flancoDescTeclaA2, BOTON_A2);
  key_up_callback(&flancoDescTeclaA3, BOTON_A3);
  key_up_callback(&flancoDescTeclaA4, BOTON_A4);
  key_up_callback(&flancoDescTeclaA5, BOTON_A5);  

  
  keyboard_driver_init();

// initialize Timer0
  cli();
  TCCR0A = 0;     // set entire TCCR1A register to 0
  TCCR0B = 0;     // same for TCCR1B
  TCNT0  = 0;
  
  // set compare match register to desired timer count:
  OCR0A = 156; //duda aquí, no se si va entero. SI VA ENTERO, pierdo centisegundos pero igual es muy preciso
  //con esto hago que el cronometro tenga overflow cada 1 centisegundo, y ahí se envía una interrupcion, en el momento que hace el overflow
  //y se resetea el timer, luego de esto se crea la interrupción, que sigue lo especificado abajo, y entonces pasa a cambiar lo que hay en el lcd.
  // turn on CTC mode:
  TCCR0A |= (1 << WGM01);
  // Set CS00 and CS02 bits for 1024 prescaler:
  TCCR0B |= (1 << CS02) | (1 << CS00);;
  // enable timer compare interrupt:
  TIMSK0 |= (1 << OCIE0A);
  // enable global interrupts:
  sei();
  
}

void loop() {
  //Serial.println(minimum);
  if(leerTeclado==1){
    keyboard_driver_init();   
    keyboard_loop();
  }
   else{    
      adc_ldr.callback = callbackLdr;
      adc_ldr.canal = LDR_PIN;
      adc_init(&adc_ldr);
      adc_loop();             
      medium[index]=ldrLux;
      index=(index+1)%200;      
      leerTeclado=1;      
    }  
	menus(modo);
}

void menus(int modo) {

	float valorMedio = 0;

	switch (modo) {
        case LUMINOSIDAD_ACTUAL:
          lcd.setCursor(0,0);
    			lcd.print("Luminosidad act.");
    			// print out the results
    			lcd.setCursor(0,1);
    			lcd.print(ldrLux);
    			if (ldrLux >= 100 && ldrLux < 1000) lcd.print("      ");
    			else if (ldrLux >= 10 && ldrLux < 100) lcd.print("       ");
    			else if (ldrLux >= 0 && ldrLux < 10) lcd.print("        ");
    			lcd.setCursor(13,1);
    			lcd.print("Lux");
         
          break;
        case LUMINOSIDAD_MAXIMA_MINIMA:
        	lcd.setCursor(0,0);
        	lcd.print("Min ");
        	lcd.print(minimum);
          lcd.print("           ");
        	lcd.setCursor(0,1);
        	lcd.print("Max ");
        	lcd.print(maximum);
          lcd.print("          ");

        	break;
        case LUMINOSIDAD_PROMEDIO:
        	lcd.setCursor(0,0);
        	lcd.print("Medium");
          lcd.print("          ");

        	for (int i = 0; i < index; ++i)
        	{
            valorMedio += medium[i];
        	}
        	valorMedio /= index;

        	lcd.setCursor(0,1);
        	lcd.print(valorMedio);
          lcd.print("           ");

        	break;
        case AJUSTE_DIMMER:
        	lcd.setCursor(0,0);
        	lcd.print("Dimmer");
          lcd.print("          ");
          lcd.setCursor(0,1);
          lcd.print(dimmer);          
          lcd.print("          ");
        	analogWrite(10, dimmer*2.55);
          
        	break;   
        default:
            break;
    }
}

void callbackLdr() {
	  int   ldrRawData;
  	float resistorVoltage, ldrVoltage;
  	float ldrResistance;
  	// Perform the analog to digital conversion  
  	ldrRawData = adc_ldr.val;//analogRead(LDR_PIN);
  
  	// RESISTOR VOLTAGE_CONVERSION
  	// Convert the raw digital data back to the voltage that was measured on the analog pin
  	resistorVoltage = (float)ldrRawData / MAX_ADC_READING * ADC_REF_VOLTAGE;

  	// voltage across the LDR is the 5V supply minus the 5k resistor voltage
  	ldrVoltage = ADC_REF_VOLTAGE - resistorVoltage;
  
  	// LDR_RESISTANCE_CONVERSION
  	// resistance that the LDR would have for that voltage  
  	ldrResistance = ldrVoltage/resistorVoltage * REF_RESISTANCE;
  
  	// LDR_LUX
  	// Change the code below to the proper conversion from ldrResistance to
  	// ldrLux
  	ldrLux = LUX_CALC_SCALAR * pow(ldrResistance, LUX_CALC_EXPONENT);
    Serial.println(ldrLux);
  	if (ldrLux > maximum) maximum = ldrLux;

  	if (ldrLux < minimum && ldrLux > 0) minimum = ldrLux;
   
}

ISR(TIMER0_COMPA_vect)
{
    centesimas++;
    if(centesimas%6==0) {
      leerTeclado=0;
    }
    if(centesimas == 100){
        centesimas=0;
        segundos++;
        if(segundos == 3){
            //Resetear modo
            modo=0; //Volver a LA            
        }
    }
}

void flancoAscTeclaUp(){
	pression[TECLA_UP] = 1;
}
void flancoDescTeclaUp(){
  if (pression[TECLA_UP] == 1) {
  	if(modo==3){
    if(dimmer<100) dimmer+=20;    
    }
  	segundos=0; centesimas=0; //Reset contador de tiempos desde que se presiono un boton
  	pression[TECLA_UP] = 0;
  }
}
void flancoAscTeclaDown(){
  pression[TECLA_DOWN] = 1;
}
void flancoDescTeclaDown(){
  if (pression[TECLA_DOWN] == 1) {
  	if(modo==3){
    if(dimmer>20) dimmer-=20;    
    }
  	segundos=0; centesimas=0; //Reset contador de tiempos desde que se presiono un boton
  	pression[TECLA_DOWN] = 0;
  }
}
void flancoAscTeclaLeft(){
  pression[TECLA_LEFT] = 1;
}
void flancoDescTeclaLeft(){
  
  if (pression[TECLA_LEFT] == 1) {
    if(modo==0) modo=3;
    else modo--;
    segundos=0; centesimas=0; //Reset contador de tiempos desde que se presiono un boton
    pression[TECLA_LEFT]=0;
  }
}
void flancoAscTeclaRight(){
  pression[TECLA_RIGHT] = 1;
}
void flancoDescTeclaRight(){
  if (pression[TECLA_RIGHT] == 1) {
  	if(modo==3) modo=0;
  	else modo++;
  	segundos=0; centesimas=0; //Reset contador de tiempos desde que se presiono un boton
  	pression[TECLA_RIGHT] = 0;
  }
}
void flancoAscTeclaSelect(){
  
}
void flancoDescTeclaSelect(){
  
}
void flancoAscTeclaA2(){
  pression[BOTON_A2] = 1;
}
void flancoDescTeclaA2(){
  if (pression[BOTON_A2] == 1) {
  	minimum=10;
  	maximum=100;
  	for(int i=0; i<200; i++)
    	medium[i]=0;
  	index = 0;
 	segundos=0; centesimas=0; //Reset contador de tiempos desde que se presiono un boton
  	pression[BOTON_A2] = 0;
  }
}
void flancoAscTeclaA3(){
  
}
void flancoDescTeclaA3(){
  
}
void flancoAscTeclaA4(){
  
}
void flancoDescTeclaA4(){
  
}
void flancoAscTeclaA5(){
      
}
void flancoDescTeclaA5(){
  
}
