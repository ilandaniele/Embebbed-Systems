
#include "D:\Usuario\Escritorio\Sistemas Embebidos\Laboratorio 3\Actividad 2\teclado_driver2.cpp"
#include <LiquidCrystal.h>
#include "Arduino.h"
#include <Wire.h>

#define OBTENER_LUX      0
#define OBTENER_MAX      1
#define OBTENER_MIN      2
#define OBTENER_PROM     3
#define OBTENER_TODO     4
#define RESPONDER_LUX    5
#define RESPONDER_MAX    6
#define RESPONDER_MIN    7
#define RESPONDER_PROM   8 
#define RESPONDER_TODO   9 

typedef struct{
  uint8_t tipo;
  uint8_t tamanio;
  float payload[4] = {0,0,0,0};
} paquete;

uint8_t msj[18];
  
adc_cfg adc2;
paquete pkt;
int flagImprimir=0;
int midiendo_lux=0;
//variables de sensado de Lux
float valores[200] = {0};
int puntero = 0;
float MAX = 0;
float MIN = 2000;
int huboMax=1;
int huboMin=1;
int backlight = 204; 
volatile int modo = 0;
volatile int contador =0;
volatile int contador2=0;
volatile int flagApretaronBotonOterminoTimer = 0; 
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
   // lcd.print("DOWN KEY UP                ");
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
 //lcd.print("DOWN KEY DOWN               ");
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
  
  aumentarModo();
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
}

void mi_handler_up_tecla_4(){ //esta fncion de callback especifica para el LDR nos convierte el valor  
  // lcd.setCursor(0, 1);
   //lcd.print("UP KEY SELECT");
}

void mi_handler_down_tecla_5(){ //esta fncion de callback especifica para el LDR nos convierte el valor  
   //lcd.setCursor(0, 1);
   //lcd.print("DOWN A2                ");
   /* Reinicio los valores de luminosidad maxima y minima, y los arreglos de 200 muestras*/
  
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
  
  float Vi= mapfloat(adc2.adc_value,0,1023,0,5);
  float RLDR = (10*5)/Vi - 10;
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

    //todo para inicializar I2C
    Wire.begin(8);                // join i2c bus with address #8
    Wire.onRequest(requestEvent); // register event
    Wire.onReceive(receiveEvent); // register event
     
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
    
    TCCR0A = 0;     
    TCCR0B = 0;     
    TCNT0 = 0;      
    OCR0A = 156; 
    TCCR0A |= (1 << WGM01); 
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
  }
 
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

void crearMensaje(){
   int tipoMsj = (int) pkt.tipo; //ver si esto anda
   Serial.print("tipo del mensaje: ");
   Serial.println(tipoMsj);
   msj[0]= (uint8_t) '[';
   msj[1]= (uint8_t) tipoMsj + 5;
   msj[2]= (uint8_t) ',';
   msj[4]= (uint8_t) ',';

   //Seteo tamanio y caracter de terminacion
   if(tipoMsj == OBTENER_TODO){
    msj[3]= (uint8_t) 12;
    msj[16]= (uint8_t) ']';
   
   }else{
    msj[3]= (uint8_t) 3;
    msj[7]= (uint8_t) ']';
   }
   
   
   switch(tipoMsj){
      case OBTENER_LUX:
      {
        float valor = valores[puntero-1];
        
        int centivalor = valor*100;
        int parteDecimal = centivalor % 100;
        int parteEntera = centivalor / 100;
   
        msj[5] = (uint8_t) parteEntera;
        msj[6] = (uint8_t) parteDecimal;
     
      }
      break;
      case OBTENER_MIN:
      {
        float valor = MIN;
        int centivalor = valor*100;
        int parteDecimal = centivalor % 100;
        int parteEntera = centivalor / 100;
   
        msj[5] = (uint8_t) parteEntera;
        msj[6] = (uint8_t) parteDecimal;
       
      }
      break;
      case OBTENER_MAX:
      {
        float valor = MAX;
        int centivalor = valor*100;
        int parteDecimal = centivalor % 100;
        int parteEntera = centivalor / 100;
   
        msj[5] = (uint8_t) parteEntera;
        msj[6] = (uint8_t) parteDecimal;
        
      }
      break;
      case OBTENER_PROM:
      {
        float valorProm = 0;
        for(int i=0;i<200;i++){
              valorProm= valorProm + valores[i];   
        }
        valorProm = valorProm/200; 
        
        float valor = valorProm;
        int centivalor = valor*100;
        int parteDecimal = centivalor % 100;
        int parteEntera = centivalor / 100;
   
        msj[5] = (uint8_t) parteEntera;
        msj[6] = (uint8_t) parteDecimal;
        
      }
      break;
      case OBTENER_TODO:
      {
        float valor = valores[puntero-1];
        int centivalor = valor*100;
        int parteDecimal = centivalor % 100;
        int parteEntera = centivalor / 100;
        Serial.print("valor de Lux: ");
        Serial.println(valor);
        msj[5] = (uint8_t) parteEntera;
        msj[6] = (uint8_t) parteDecimal;
        msj[7] = (uint8_t) ',';

        valor = MAX;
        centivalor = valor*100;
        parteDecimal = centivalor % 100;
        parteEntera = centivalor / 100;
        Serial.print("valor de maximo: ");
        Serial.println(valor);
        msj[8] = (uint8_t) parteEntera;
        msj[9] = (uint8_t) parteDecimal;
        msj[10] =(uint8_t) ',';
  
        valor = MIN;
        centivalor = valor*100;
        parteDecimal = centivalor % 100;
        parteEntera = centivalor / 100;
        Serial.print("valor de minimo: ");
        Serial.println(valor);
        msj[11] = (uint8_t) parteEntera;
        msj[12] = (uint8_t) parteDecimal;
        msj[13] = (uint8_t) ',';
        
        float valorProm = 0;
        for(int i=0;i<200;i++){
              valorProm= valorProm + valores[i];   
        }
        valorProm = valorProm/200;
        centivalor = valorProm*100;
        parteDecimal = centivalor % 100;
        parteEntera = centivalor / 100;
        Serial.print("valor promedio: ");
        Serial.println(valorProm);
        msj[14] = (uint8_t) parteEntera; 
        msj[15] = (uint8_t) parteDecimal;
        msj[16] = (uint8_t) ']';
      }
      break;
   }
   
}

/*
int checkearFormato(uint8_t * mensaje){
  int formatoCorrecto = 1;
  int i=0;
 //duda con el length, si tengo comoponentes vacias del arreglo al final tambien las cuenta ?
 
  while(formatoCorrecto == 1 && i < length(mensaje)){
    if(length(mensaje) != 8 || length(mensaje) != 17 || length(mensaje) != 5){
      formatoCorrecto=0;
    }
    if(i==0 && mensaje[i] != '['){
      //es el principio de mensaje y no es el simbolo de comienzo
        formatoCorrecto=0; 
    }
    if(i== length(mensaje)-1 && mensaje[i] != ']'){
      //es el final del mensaje y no es el simbolo de fin
      formatoCorrecto=0;
    }
    if( (mensaje[i] == '[' || mensaje[i] == ']') && i!=length(mensaje)-1 && mensaje[i+1] != '/'){
      //es un simbolo de fin o comienzo y no le sigue el simbolo de escape
      formatoCorrecto=0;
    }
    if(i==1 && mensaje[i] < 0 && mensaje[i] > 15){
        //checkeo que el tipo sea un entero entre 0 y 15
        formatoCorrecto=0;
    }
    if(mensaje[i] == ',' && i != 2 && i != 4 && i != 7 && i != 10 && i != 13){
      //checkeo que las comas esten todas bien
      formatoCorrecto=0;
    }
    if((i==2 && mensaje[i] != ',') || (i==4 && mensaje[i] != ',') || (i==7 && mensaje[i] != ',') || (i==10 && mensaje[i] != ',') ||(i==13 && mensaje[i] != ',')){
      formatoCorrecto=0;
    }
    i++;
  }
  return formatoCorrecto;
}
int descomprimirMensaje2(uint8_t * mensaje){
  if(checkearFormato(mensaje)){
    pkt.tipo = mensaje[1];
    pkt.tamanio = mensaje[3];
    int cantidadComas = 0;
    int i = 5;
    uint8_t parteEntera=0;
    uint8_t parteDecimal=0;
    while(i < length(mensaje) {
      if(mensaje[i] == ','){
        cantidadComas++;
      }else{
        if(mensaje[i] != ']'){
          if(parteEntera==0){
            parteEntera = mensaje[i];
          }else{
            parteDecimal = mensaje[i];
            float valor = parteEntera + (float) parteDecimal/100;
            pkt.payload[cantidadComas] = valor;
            parteEntera=0;
            parteDecimal=0;
          }
        }
      }
      
     i++;
    }
    return 1; //guarda con esto
   
  }else{
    printf("formato de mensaje erróneo");
    return 0;
  }
}
*/
void descomprimirMensaje(uint8_t * mensaje){ 
  //estas leyendo byte por byte, entonces 1 posicion del arreglo es un byte.
  int i=0;
  int esTamanio = 0;
  int esPayload = 0;
  int j=0;
  int k=0;
  int fin=0;
  int contador=0;
  uint8_t parteEntera, parteDecimal;
  
  while(fin == 0){
        if(mensaje[i] == '[' && i==0){//lei el primer caracter
              pkt.tipo = mensaje[i+1]; 
        }             
         if(i > 18){
        //hay código basura
            printf("formato de mensaje erróneo");
            fin=1;
           }   
        if(esPayload == 1){
                            for(j=0; j < pkt.tamanio; j++){
                                      if(mensaje[i] == ','){
                                        contador++;
                                      }else{
                                          if(mensaje[i] == ']'){
                                              fin=1;
                                          }else{
                                                  /*
                                                   * if(parteEntera == 0){
                                                   *    parteEntera = mensaje[i];
                                                   * }else{
                                                   *    parteDecimal = mensaje[i];
                                                        float valor = parteEntera + (float) parteDecimal/100;
                                                        pkt.payload[contador] = valor;
                                                        parteEntera=0;
                                                        parteDecimal=0;
                                                   * }
                                                   */
                                                  if(k==0){ //estamos leyendo parte entera
                                                        parteEntera = mensaje[i]; //aca podría deshacerme de la K, haciendo la parteEntera 0 al final 
                                                        k++;                    //y checkeando en un if si es cero la parteEntera, ver que onda para checkear con uint8-t
                                                                                //cuando es un cero lo que quiero comparar con el uint8_t
                                                  }else{  //k=1, estamos leyendo parte decimal
                                                        parteDecimal = mensaje[i];
                                                        float valor = parteEntera + (float) parteDecimal/100;
                                                        pkt.payload[contador] = valor; //transforma los 4 uint8_t a un solo float
                                                        k=0;                                          
                                                  }
                                          }
                                      }
                                      i++;
                            }
      
        }else{
                if(esTamanio == 1){
                              if(mensaje[i] == ','){ //es la segunda coma
                                esPayload=1;
                                esTamanio=0;
                              }else{
                                pkt.tamanio = mensaje[i]; 
                              }               
                }
               
                if(mensaje[i] == ',' && i==2){ //es la primera coma
                  esTamanio=1;
                }
               
                if(mensaje[i] == ']'){ //esto lo agregue para hacerla portable y que tambien la use arduino
                              if(i>0 &&  mensaje[i-1] != '/'){
                                fin = 1;
                                esPayload=0;
                              }
                }
                
                i++;
           }
  }
  
 }

void receiveEvent(int howMany) {
  uint8_t mensaje[5];       
  int i=0;
  if(Wire.available()==5){
  while (Wire.available() > 0) //queriendo decir que todavia hay algo en el wire
    {
      mensaje[i]= Wire.read(); 
      i++;
    }
    /*
    if(descomprimirMensaje2(mensaje) == 1){
      Serial.println("Mensaje recibido en buen formato");
    }else{
      Serial.println("Emensaje recibido en formato incorrecto");
    }*/
    descomprimirMensaje(mensaje);
    

  }else{
    Serial.println("Error, recibimos una cantidad de bytes en el receiveEvent que no es correcta, no son 5");
  }
  
}


void requestEvent() {
  crearMensaje();
 
  
  if((int) pkt.tipo == OBTENER_TODO){
          int cantidadEnviados = Wire.write(msj,17);
          
  }else{
          Wire.write(msj,8); 
  }
   
}

