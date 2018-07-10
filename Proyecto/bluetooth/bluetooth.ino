#include <SoftwareSerial.h>
SoftwareSerial BT1(11, 10); // RX | TX //esto lo voy a borrar luego
int led = 4; //led Rojo de prueba de conexión
char cadena[20];
 
void setup() {
  // initialise serial communications at 9600 bps:
  //Serial.begin(9600);
  pinMode(led, OUTPUT);
  digitalWrite(led, LOW);
 
  BT1.begin(38400); //super importante, lo cambiaremos por serial cuando lo pasemos al nano, y conectamos en tx y rx directo
  Serial.begin(9600);
  
}
 
void loop() {
    
    if (BT1.available()>0){
          char caracter = BT1.read();
          switch(caracter){
            case '#':{
              //modo imprimir string
              int i=0;
              Serial.println("imprimir string");
              while(BT1.available()>0){
                cadena[i] = BT1.read();
                
                Serial.write(cadena[i]);
                i++;
              }
            }break;
            case '2':{
              //Modo abecedario
              Serial.println("modo abecedario");
            }break;

            case '3':{
              //Modo bandas de rock
              Serial.println("modo bandas de rock");
            }break;

            case '4':{
              //Incrementar retardo
              Serial.println("incrementar retardo");
            }break;

            case '5':{
              //Decrementar retardo
              Serial.println("decrementar retardo");
            }break;

            case '6':{
              //Incrementar espaciado
              Serial.println("incrementar espaciado");
            }break;

            case '7':{
              //Decrementar espaciado
              Serial.println("decrementar espaciado");
            }break;
            case '8':{
              Serial.println("modo ACDC");
            }break;
            case '*':{
              Serial.println("modo hora");
              int i=0;
               while(BT1.available()>0){
                cadena[i] = BT1.read();
                
                Serial.write(cadena[i]);
                i++;
              }
            }break;
          }
    }
         

   
}
