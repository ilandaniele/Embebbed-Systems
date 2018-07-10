
//#include "D:\Usuario\Escritorio\Sistemas Embebidos\Laboratorio 2\Actividad 5\prueba drivers de teclado\teclado_driver4nuevo.cpp"
#include "D:\Usuario\Escritorio\Sistemas Embebidos\Laboratorio 2\Actividad 5\teclado_driver2probando.cpp"
#include <LiquidCrystal.h>
#include "Arduino.h"

adc_cfg adc2;

int flagImprimir=0;
int midiendo_lux=0;
//variables de sensado de Lux
float valores[200] = {0};
int puntero = 0;
float MAX = 0;
float MIN = 2000;
int huboMax=1;
int huboMin=1;
int backlight = 204; //no se si hay que definirla como volatile
volatile int modo = 0;
volatile int contador =0;
volatile int contador2=0;
volatile int flagApretaronBotonOterminoTimer = 0; //sirve como semaforo para poder imprimir una unica vez las cosas, en este caso los cambios de estado
volatile int flagADC=0;

// initialize the library with the numbers of the interface pins
LiquidCrystal lcd(8, 9, 4, 5, 6, 7);


void aumentarModo(){
  if(modo < 3){
    modo++;
  }else{
    modo = 0;
  }
}
void disminuirModo(){
  if(modo > 0){
    modo--;
  }else{
    modo = 3;
  }
}
void mi_handler_down_tecla_0(){ //esta fncion de callback especifica para el LDR nos convierte el valor  
   // DOWN KEY UP 
      lcd.setCursor(0, 1);
       if(modo == 3){
          if(backlight < 255){
             backlight = backlight + 51;
          }
          analogWrite(10, backlight);
          lcd.print("backlight =               ");
          lcd.setCursor(12,1);
          lcd.print(backlight);
          contador2=0;
       }
   
   
}

void mi_handler_down_tecla_1(){ //esta fncion de callback especifica para el LDR nos convierte el valor  
 //DOWN KEY DOWN
  lcd.setCursor(0, 1);
   
   if(modo == 3){   
      if(backlight>0){
        backlight = backlight - 51;
      }
      analogWrite(10, backlight);
      lcd.print("backlight =               ");
      lcd.setCursor(12,1);
      lcd.print(backlight);
      contador2=0;
   }
}

void mi_handler_down_tecla_2(){ //esta fncion de callback especifica para el LDR nos convierte el valor  
  //lcd.setCursor(0, 0);
  //lcd.print("DOWN KEY LEFT                ");
  
  disminuirModo();
  flagApretaronBotonOterminoTimer=1;
  contador2=0;
  lcd.clear();
  if(modo == 1){
    huboMax=1;
    huboMin=1;
  }
   
}

void mi_handler_down_tecla_3(){ //esta fncion de callback especifica para el LDR nos convierte el valor  
 // lcd.setCursor(0, 0);
  //lcd.print("DOWN KEY RIGHT                ");
  //Serial.println("entre derecha callback");
  aumentarModo();
    //Serial.println("Modo: ");
  //Serial.println(modo);
  flagApretaronBotonOterminoTimer=1;
  contador2=0;
  lcd.clear();
  if(modo == 1){
    huboMax=1;
    huboMin=1;
  }
   
}

void mi_handler_down_tecla_4(){ //esta fncion de callback especifica para el LDR nos convierte el valor  
   //lcd.setCursor(0, 1);
  // lcd.print("DOWN KEY SELECT");
}

void mi_handler_up_tecla_0(){ //esta fncion de callback especifica para el LDR nos convierte el valor  
   //lcd.print("UP KEY UP                ");
   
     
}

void mi_handler_up_tecla_1(){ //esta fncion de callback especifica para el LDR nos convierte el valor  
   //lcd.setCursor(0, 1);
   //lcd.print("UP KEY DOWN                ");
}

void mi_handler_up_tecla_2(){ //esta fncion de callback especifica para el LDR nos convierte el valor  
   //lcd.setCursor(0, 1);
   //lcd.print("UP KEY LEFT                ");


}

void mi_handler_up_tecla_3(){ //esta fncion de callback especifica para el LDR nos convierte el valor  
  // lcd.setCursor(0, 1);
   //lcd.print("UP KEY RIGHT                ");
   //Serial.println("entre a up right desde la callback");
}

void mi_handler_up_tecla_4(){ //esta fncion de callback especifica para el LDR nos convierte el valor  
  // lcd.setCursor(0, 1);
   //lcd.print("UP KEY SELECT");
}

void mi_handler_down_tecla_5(){ //esta fncion de callback especifica para el LDR nos convierte el valor  
   //lcd.setCursor(0, 1);
   //lcd.print("DOWN A2                ");
   /* Reinicio los valores de luminosidad maxima y minima, y los arreglos de 200 muestras*/
   
   //Serial.println("apretaron A2");
   MIN=2000;
   MAX=0;
   contador2=0;
   analogWrite(10, 204);
   for(int i=0; i<200; i++){
    valores[i]=0;
   }
  
}

void mi_handler_down_tecla_6(){ //esta fncion de callback especifica para el LDR nos convierte el valor  
   //lcd.setCursor(0, 1);
  // lcd.print("DOWN A3                ");
    
}

void mi_handler_down_tecla_7(){ //esta fncion de callback especifica para el LDR nos convierte el valor  
   //lcd.setCursor(0, 1);
   //lcd.print("DOWN A4                ");
    
}

void mi_handler_down_tecla_8(){ //esta fncion de callback especifica para el LDR nos convierte el valor  
   //lcd.setCursor(0, 1);
   //lcd.print("DOWN A5                ");
    
}

void mi_handler_up_tecla_5(){ //esta fncion de callback especifica para el LDR nos convierte el valor  
  // lcd.setCursor(0, 1);
   //lcd.print("UP A2                ");
}

void mi_handler_up_tecla_6(){ //esta fncion de callback especifica para el LDR nos convierte el valor  
   //lcd.setCursor(0, 1);
   //lcd.print("UP A3                ");
}

void mi_handler_up_tecla_7(){ //esta fncion de callback especifica para el LDR nos convierte el valor  
   //lcd.setCursor(0, 1);
   //lcd.print("UP A4                ");
}

void mi_handler_up_tecla_8(){ //esta fncion de callback especifica para el LDR nos convierte el valor  
   //lcd.setCursor(0, 1);
   //lcd.print("UP A5                ");
}
float mapfloat(float x, float in_min, float in_max, float out_min, float out_max){
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

float calcularLux(){
  /**
  Connect LDR with a 10 Kohms resistor as shown:


           Vin: 5V---*
                  |
                  \
                  0  LDR(16K - 2M Ohms)
                  /
                  |
                  |
        analog 1----*
                  |
                  /
                  \  
                  /   R1 (10K ohms)  
                  \
                  |
                  *----> Ground
  **/
  float Vi= mapfloat(adc2.adc_value,0,1023,0,5);
  float RLDR = (10*5)/Vi - 10;
  //valor de Resitencia LDR
  //float RLDR = (10 * (5 - Vout))/Vout;
  float Lux = 0;
  //mapear valor de Resistencia LDR a valor de LUX
  if(RLDR > 92) Lux = 0;
  else if (RLDR >= 41) Lux = 1; 
  else if (RLDR >= 24) Lux = mapfloat(RLDR,24,41,3,1);
  else if (RLDR >= 16) Lux = mapfloat(RLDR,16,24,6,3);
  else if (RLDR >= 10) Lux = mapfloat(RLDR,10,16,10,6);
  else if (RLDR >= 7) Lux = mapfloat(RLDR,7,10,15,10);
  else if (RLDR >= 5) Lux = mapfloat(RLDR,5,7,35,15);
  else if (RLDR >= 3) Lux = mapfloat(RLDR,3,5,80,35);
  else if (RLDR >= 1) Lux = mapfloat(RLDR,1,3,100,80);
  else Lux = 101;
  
  return Lux;
}
void mi_callbackLux(){
  //Serial.println(adc2.adc_value,"estoy en la callback del main, el valor del adc medido es: %f\n");
  float Lux = calcularLux();
  midiendo_lux = 0;
  flagImprimir=1;
  valores[puntero] = Lux;  
  if(puntero < 199) {
    puntero++;
  }else{  
    puntero=0; 
  }  
  if(Lux > MAX){
    MAX = Lux;
    huboMax = 1;
  }  
  if(Lux < MIN){ 
    MIN = Lux; 
    huboMin = 1;
  }  
  
  
  // Arrancar timer
  //con esto seteamos en 1 los bits de los registros en el timer, con el prescaler 1024 para que arranque la cuenta
  TCCR0B |= (1 << CS10);
  TCCR0B |= (1 << CS12);
  teclado_init();
  
}
void init_handlers_teclas(){
   //DOWN'S
   //LCD
   key_down_callback(mi_handler_down_tecla_0,0);
   key_down_callback(mi_handler_down_tecla_1,1);
   key_down_callback(mi_handler_down_tecla_2,2);
   key_down_callback(mi_handler_down_tecla_3,3);
   key_down_callback(mi_handler_down_tecla_4,4);
   //PINES
   key_down_callback(mi_handler_down_tecla_5,5);
   key_down_callback(mi_handler_down_tecla_6,6);
   key_down_callback(mi_handler_down_tecla_7,7);
   key_down_callback(mi_handler_down_tecla_8,8);
   //UP'S
   //LCD
   key_up_callback(mi_handler_up_tecla_0,0);
   key_up_callback(mi_handler_up_tecla_1,1);
   key_up_callback(mi_handler_up_tecla_2,2); 
   key_up_callback(mi_handler_up_tecla_3,3); 
   key_up_callback(mi_handler_up_tecla_4,4);
   //PINES 
   key_up_callback(mi_handler_up_tecla_5,5);
   key_up_callback(mi_handler_up_tecla_6,6);
   key_up_callback(mi_handler_up_tecla_7,7); 
   key_up_callback(mi_handler_up_tecla_8,8); 

}

void setup() {
    Serial.begin(9600);
    init_handlers_teclas();

    //INICIALIZAR EL LED
    pinMode(10, OUTPUT);
    lcd.begin(16,2);
    analogWrite(10, backlight); //Controla intensidad backlight, la seteamos al 80% al principio
    lcd.setCursor(0, 0);
    lcd.print("Lab 2");
    lcd.setCursor(0,1);
    lcd.print("S.E 2do 2017");    
    delay(3000);
    teclado_init();
    cli();
    //inicializo el driver de ADC para LUX
    adc2.canal=1;
    adc2.callback = mi_callbackLux;
    
     // initialize Timer2
    
    TCCR0A = 0;     
    TCCR0B = 0;     
    TCNT0 = 0;
    OCR0A = 156; //duda aquí, no se si va entero. SI VA ENTERO, pierdo centisegundos pero igual es muy preciso
    //con esto hago que el cronometro tenga overflow cada 1 centisegundo, y ahí se envía una interrupcion, en el momento que hace el overflow
    //y se resetea el timer, luego de esto se crea la interrupción, que sigue lo especificado abajo, y entonces pasa a cambiar lo que hay en el lcd.
    // turn on CTC mode:
    TCCR0A |= (1 << WGM01); //cambiado el timer a 21, que se corresponde al timer 2
    TCCR0B |= (1 << CS10);
    TCCR0B |= (1 << CS12);
    TIMSK0 |= (1 << OCIE0A);
    
    sei();
}
void imprimirModoLCD(){
  lcd.clear();
  lcd.setCursor(0, 0);    
  if(modo == 0){
    lcd.print("MODO LA       ");
  }else{
    if(modo == 1){
       lcd.print("MODO LM         ");
    }else{
      if(modo == 2){
         lcd.print("MODO LP          ");
      }else{
        if(modo == 3){
           lcd.print("MODO AD           ");
           lcd.setCursor(0,1);
           lcd.print("backlight =               ");
           lcd.setCursor(12,1);
           lcd.print(backlight);
        }
      }
    }
  }
  
}
void loop(){
  if (midiendo_lux == 1){  
    adc_loop();
  }else{
    teclado_loop();
    
  }
     
  if(flagADC==1){
    flagADC=0;
    adc_init(&adc2);
    midiendo_lux = 1;
    //Serial.println("tengo que medir lux, estoy en el if de flag ADC==1 \n");
  }
  //delay(1000);
  if(flagApretaronBotonOterminoTimer == 1){ //imprimo el modo en el que se encuentra
    imprimirModoLCD();
    flagApretaronBotonOterminoTimer=0;
  }
  if(flagImprimir == 1){
        flagImprimir=0;
       if(modo == 0){
          //codigo luminosidad actual imprimir
          lcd.setCursor(0,1);
          lcd.print("val actual           ");
          lcd.setCursor(11,1);
          lcd.print(valores[puntero-1]);
          }else{
            if(modo == 1){
               if(huboMin){
                huboMin=0;
                lcd.setCursor(8,0);
                lcd.print("min ");
                lcd.setCursor(12,0);
                lcd.print(MIN);
               }
               if(huboMax){
                huboMax=0;
                lcd.setCursor(8,1);
                lcd.print("max                ");
                lcd.setCursor(12,1);
                lcd.print(MAX);
               }
            }else{
              if(modo == 2){
                //codigo luminosidad LP imprimir
                float valorProm = 0;
                for(int i=0;i<200;i++){
                   valorProm= valorProm + valores[i];   
                }
                valorProm = valorProm/200; 
                lcd.setCursor(0,1);
                lcd.print("valor prom:            ");
                lcd.setCursor(12,1);
                lcd.print(valorProm);
              }
            }
           }       
  }
}

ISR(TIMER0_COMPA_vect)
{     
    //tengo que cada 300 centisegundos debo volver al modo LA
    //entonces, el timer tira overflow cada 0,01segundos o 1 centisegundos, luego 300/1 = 300, luego cuando el contador llega a 50 quiere decir
    //que llegue a los 3 segundos y debe cambiar el modo 
    contador++;
    contador2++;
   
    if (contador % 6 == 0){
      //llegue a 0.06 segundos, entonces ahí reinicio el adc
      //frenar timer
      //se limpia los registros del timer para que frene y no cuente mas
      TCCR0B &= ~ (1 << CS10);
      TCCR0B &= ~ (1 << CS11);
      TCCR0B &= ~ (1 << CS12);
      //debo obtener un valor del ADC.        
      flagADC=1;
      
    }
    if(contador2 == 300){
      modo = 0; //lo seteo al modo LA
      contador2=0;
      flagApretaronBotonOterminoTimer = 1;
    }    
}

