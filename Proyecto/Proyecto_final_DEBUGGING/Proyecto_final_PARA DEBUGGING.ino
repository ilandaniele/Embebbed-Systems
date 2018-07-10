#include <SoftwareSerial.h>
SoftwareSerial BT1(11, 10); // RX | TX //esto lo voy a borrar luego

//Tamaño de la palabra / Size of the word
String palabra = "EMBEBIDOS 2017";

const int sensor=A6;  // sensor de posición
int modo = 0;
int contadorDeVueltas = 0;
int estado=0;
int valorSensor=1024;
const byte cantCaracteres = 37; //la voy a terminar eliminando después
const byte caracter[cantCaracteres][5] = { //agregarle para tener los 8 leds
{B00000000, B00000000, B00000000, B00000000, B00000000}, //_
{B11111111, B10001000, B10001000, B10001000, B11111111}, //A
{B10000001, B11111111, B10010001, B10010001, B01101110}, //B                    
{B00111100, B01000010, B10000001, B10000001, B01000010}, //C
{B10000001, B11111111, B10000001, B10000001, B01111110}, //D
{B11111111, B10010001, B10010001, B10010001, B10010001}, //E
{B11111111, B10010000, B10010000, B10000000, B10000000}, //F
{B11111111, B10000001, B10001001, B10001001, B10001111}, //G
{B11111111, B00001000, B00001000, B00001000, B11111111}, //H
{B10000001, B10000001, B11111111, B10000001, B10000001}, //I
{B11000110, B10000001, B10000001, B10000001, B11111111}, //J
{B11111111, B00010000, B00100100, B01000010, B10000001}, //K
{B11111111, B00000001, B00000001, B00000001, B00000001}, //L
{B11111111, B00100000, B00010000, B00100000, B11111111}, //M
{B11111111, B00100000, B00010000, B00001000, B11111111}, //N
{B01111110, B10000001, B10000001, B10000001, B01111110}, //O
{B11111111, B10010000, B10010000, B10010000, B01100000}, //P
{B11111110, B10000010, B10000110, B10000010, B11111111}, //Q
{B11111111, B10011000, B10010100, B10010010, B10010001}, //R
{B01100010, B10010001, B10011001, B10001001, B01000110}, //S
{B11000000, B10000000, B11111111, B10000000, B11000000}, //T
{B11111110, B00000001, B00000001, B00000001, B11111111}, //U
{B11111100, B00000010, B00000001, B00000010, B11111100}, //V
{B11111111, B00000110, B00000001, B00000110, B11111111}, //W
{B10000001, B01000110, B00011100, B01100010, B10000001}, //X
{B11100000, B00010000, B00001111, B00010000, B11100000}, //Y
{B11000001, B10000111, B10011001, B11100001, B11000011}, //Z
{B01111110, B10000011, B10001001, B11000001, B01111110}, //0
{B00100001, B01000001, B11111111, B00000001, B00000001}, //1
{B11011111, B10010001, B10010001, B10010001, B11110111}, //2
{B01000010, B10010001, B10010001, B10010010, B01101100}, //3
{B00010000, B00110000, B01010000, B11111111, B00010000}, //4
{B11110011, B10010001, B10010001, B10010001, B11011111}, //5
{B11111111, B10001001, B10001001, B10001001, B10001111}, //6
{B10000000, B10010000, B11111111, B00010000, B00000000}, //7
{B01101110, B10010001, B10010001, B10010001, B01101110}, //8
{B01100000, B10010000, B10010000, B10010000, B11111111}  //9
};

//LENGUA DE LOS ROLLING STONE
const byte rollingStone[89] = 
{
   B00000000,
   B10000001, B11111111, B10010001, B10011000, B01100101, B00000011, //R 
   B00111100, B01000010, B10000001, B01000010, B00111100, //O 
   B10000001, B11111111, B10000001, B00000001, B00000011, //L
   B10000001, B11111111, B10000001, B00000001, B00000011, //L
   B00000000, B10000001, B11111111, B10000001, B10000001, //I 
   B11111111, B00100000, B00010000, B00001000, B11111111, B10000001, //N 
   B00111100, B01000010, B10001001, B01001110, B00001000, //G 
   /**
    * LENGUA LOGO
    */
   B00000000, B00101110, B00110001, B01000101, B10100101, B10101101, 
   B01100001, B10010010, B10000100, B01010000, B00100000,
   B00000000,

   B01100010, B10010001, B10011001, B10001001, B01000110, //S 
   B11000000, B10000001, B11111111, B10000001, B11000000, //T
   B00111100, B01000010, B10000001, B01000010, B00111100, //O
   B11111111, B00100000, B00010000, B00001000, B11111111, B10000001, //N 
   B10000001, B11111111, B10010001, B10010001, B10111001, B11000011, //E
   B01100010, B10010001, B10011001, B10001001, B01000110, //S 
   B00000000
};

const byte AC_DC[33] =
{
   B00000000,
   B00000010, B00111101, B01010010, B10010000, B01010010, B00111101, B00000010, //A (7 columnas)
   B00011000, B00100100, B01000010, B10000001, B01000010, B00100100, //C(6 columnas)
   B00000001, B00001010, B00011110, B00111100, B01111100, B01101000, B10000000, //rayo (7 columnas)
   B10000001, B11111111, B10000001, B01000010, B00111100, //D (5 columnas)
   B00011000, B00100100, B01000010, B10000001, B01000010, B00100100, //C(6 columnas)  
   B00000000    
};

int espaciado;
int retardo;

void setup()
{
  // Configuro los puertos de entrada y salida / Setting the ports of the leds to OUTPUT and sensor to INPUT}
  for(int w=2; w<10; w++){
    pinMode(w, OUTPUT);
    digitalWrite(w,LOW);
  }

  //Datos BLUETOOTH
  BT1.begin(38400); //super importante, lo cambiaremos por serial cuando lo pasemos al nano, y conectamos en tx y rx directo
  Serial.begin(9600);
  Serial.flush();
  
  //Espacio entre cada letra, en si termina siendo un retardo agregado, mas grande para tener mas distancia entre cada letra
  espaciado = 5;
  
  // definiendo el retardo entre que voy a printear cada columna
  retardo = 2;

  contadorDeVueltas = 0;
  
  /**
   * MODO 0 --> EDITOR DE TEXTO
   * MODO 1 --> ABECEDARIO COMPLETO
   * MODO 2 --> ROLLING STONE & AC%DC
   */
  modo = 0; 
}

//***** Para limpiar la columna 
void borrar()
{
  for(int z=2;z<10;z++)
    {
      digitalWrite(z,LOW);
    } 
}

void imprimirColumna(int nroColumna, int indice){
  byte columna;
  if(modo == 0 || modo == 1) columna = caracter[indice][nroColumna];
  if(modo == 2) columna = rollingStone[nroColumna];
  if(modo == 3) columna = AC_DC[nroColumna];
  byte valor = 0;
  for (int y=0; y<8; y++)
  { 
    valor = columna & 1;  //valor de encendido de LED's
    columna = columna >> 1 ; // Shift bits
    digitalWrite(9-y,valor);
  }
  delay(retardo);
  borrar();
}


void imprimir(int indice,int cantColumnas)
{ 
  for (int y=0; y<cantColumnas; y++)
    imprimirColumna(y,indice);  
  // printing the space between the letters
  borrar();
  delay(espaciado);
}


void loop()
{
  
    if (BT1.available())
    {
      decodificarMensaje();
    }  // fin del if  
    
    /*
     * MODO ABECEDARIO--esta parte hace que se reinicien las letras del abecedario
     */
     
    if(modo == 1)
    {
      if(palabra[0]=='[')
        palabra="0";
      if(palabra[0]==':')
        palabra="A";
    }
    
    /**
     * sensa el valor del infrarojo
     */
     
    valorSensor = analogRead(sensor);  //verifico si ya pasó por el sensor de posición / check if already cross the position sensor
    if(valorSensor < 900){ 
      estado = 1;
    }
      
    /*
     * paso por el emisor de infrarojo
     */
    if(estado==1)  //si ya pasó// if has just crossed
    {   
      
      if(modo==0 || modo==1){
        if (modo == 1)
        {
          contadorDeVueltas++;
          if(contadorDeVueltas == 10) 
          {
            palabra[0]++; //si dio 10 vueltas cambio de letra
            contadorDeVueltas = 0;
          }
        }
        Serial.print("voy a imprimir en el for la palabra: ");
        for(int x=0;x<palabra.length();x++)
        {
          char caracter = palabra.charAt(x);  //Obtengo cada letra y la almaceno en la variable
          Serial.print(caracter);
          int caracterLimpio=(caracter-'0');     
          //Serial.print(caracterLimpio);     
            if (caracterLimpio>0)  //Esto es para evitar que se envien caracteres no deseados
            {                                                                                                                                                                                                                                                          
                 int indice = calcularIndiceDeCaracter(caracter);
                 imprimir(indice,5);//5 son las cantidad de columnas
            }
         } //fin del for 
         Serial.println("");
      }
      
      if(modo==2 || modo==3)//ROLLING STONE & AC%DC
      {
        modo = 2;
        imprimir(24,89);//24 de X's(msj de error) y 89 nroColumnas 
        delay(espaciado);
        modo = 3;
        imprimir(25,33);//25 de Y's(msj de error) y 33 nroColumnas
      }
     estado=0;
  } //fin del if estado
  
} // fin del loop

void decodificarMensaje()
{
  Serial.flush(); //Limpio el buffer
  Serial.println("entre a decodificar mensaje: ");
  char msj = BT1.read();  //Lee el primer caracter del MSJ

  switch(msj){
            case '#':{
              //modo editor de texto
              modo = 0;
              palabra="";
              char letra;
              Serial.print("editor de texto: ");
              while(BT1.available()>0){
                //delay(5);
                letra = BT1.read();                
                palabra = palabra+letra;
                Serial.print(letra);
              }
              Serial.println("");
            }break;
            
            case '2':{
              //Modo abecedario
              modo = 1;
              palabra="A";
              Serial.println("modo abecedario");
            }break;

            case '3':{
              //Modo banda
              modo = 2;
              Serial.println("modo bandas");
            }break;

            case '4':{
              //Incrementar retardo
              retardo++;
              //Serial.println("incrementar retardo");
            }break;

            case '5':{
              //Decrementar retardo
              if(retardo>0) retardo--;
              //Serial.println("decrementar retardo");
            }break;

            case '6':{
              //Incrementar espaciado
              espaciado++;
              //Serial.println("incrementar espaciado");
            }break;

            case '7':{
              //Decrementar espaciado
              if(espaciado>0) espaciado--;
              //Serial.println("decrementar espaciado");
            }break;
          }
}
 
int calcularIndiceDeCaracter(char caracter)
{
        switch (caracter)
                {
                 case ' ':
                    return 0;
                    break;  
                 case 'A':
                    return 1;
                    break;
                  case 'B':
                    return 2;
                    break;
                  case 'C':
                    return 3;
                    break;
                  case 'D':
                    return 4;
                    break;
                  case 'E':
                    return 5;
                    break;   
                  case 'F':
                    return 6;
                    break;             
                  case 'G':
                    return 7;
                    break;              
                  case 'H':    
                    return 8;
                    break;              
                  case 'I':      
                    return 9;
                    break;              
                  case 'J':
                    return 10;
                    break;              
                  case 'K':
                    return 11;
                    break;     
                  case 'L':
                    return 12;
                    break;   
                  case 'M':
                    return 13;
                    break;   
                  case 'N':
                    return 14;
                    break;   
                  case 'O':
                    return 15;
                    break;   
                  case 'P':
                    return 16;
                    break;  
                  case 'Q':
                    return 17;
                    break;                   
                  case 'R':
                    return 18;
                    break;   
                  case 'S':
                    return 19;
                    break;  
                  case 'T':
                    return 20;
                    break;  
                  case 'U':
                    return 21;
                    break;  
                  case 'V':
                    return 22;
                    break;
                  case 'W':
                    return 23;
                    break;
                  case 'X':
                    return 24;
                    break;      
                  case 'Y':
                    return 25;
                    break;
                  case 'Z':
                    return 26;
                    break;  
                  case '0':
                    return 27;
                    break;
                  case '1':
                    return 28;
                    break;
                  case '2':
                    return 29;
                    break;
                  case '3':
                    return 30;
                    break;
                  case '4':
                    return 31;
                    break;   
                  case '5':
                    return 32;
                    break;             
                  case '6':
                    return 33;
                    break;              
                  case '7':    
                    return 34;
                    break;              
                  case '8':      
                    return 35;
                    break;              
                  case '9':
                    return 36;
                    break;   
       }  //fin del switch    
}




