#include "D:\Usuario\Escritorio\Sistemas Embebidos\Laboratorio 2\Actividad 4\teclado_driver.cpp"

// include the library code:
#include <LiquidCrystal.h>
#include "Arduino.h"

// these constants won't change.  But you can change the size of
// your LCD using them:
const int numRows = 2;
const int numCols = 16;

// initialize the library with the numbers of the interface pins
LiquidCrystal lcd(8, 9, 4, 5, 6, 7);

void mi_handler_down_tecla_0(){ //esta fncion de callback especifica para el LDR nos convierte el valor  
   lcd.setCursor(0, 1);
   
   lcd.print("DOWN KEY UP                ");
}

void mi_handler_down_tecla_1(){ //esta fncion de callback especifica para el LDR nos convierte el valor  
   lcd.setCursor(0, 1);
   
   lcd.print("DOWN KEY DOWN");
}

void mi_handler_down_tecla_2(){ //esta fncion de callback especifica para el LDR nos convierte el valor  
   lcd.setCursor(0, 1);
 
   lcd.print("DOWN KEY LEFT                ");
}

void mi_handler_down_tecla_3(){ //esta fncion de callback especifica para el LDR nos convierte el valor  
   lcd.setCursor(0, 1);
   lcd.print("DOWN KEY RIGHT                ");
}

void mi_handler_down_tecla_4(){ //esta fncion de callback especifica para el LDR nos convierte el valor  
   lcd.setCursor(0, 1);
   lcd.print("DOWN KEY SELECT               ");
}

void mi_handler_up_tecla_0(){ //esta fncion de callback especifica para el LDR nos convierte el valor  
   
   lcd.setCursor(0, 1);
   lcd.print("UP KEY UP                ");
}

void mi_handler_up_tecla_1(){ //esta fncion de callback especifica para el LDR nos convierte el valor  
    
   lcd.setCursor(0, 1);
   lcd.print("UP KEY DOWN                ");
}

void mi_handler_up_tecla_2(){ //esta fncion de callback especifica para el LDR nos convierte el valor  
   lcd.setCursor(0, 1);
   lcd.print("UP KEY LEFT                ");
}

void mi_handler_up_tecla_3(){ //esta fncion de callback especifica para el LDR nos convierte el valor  
   lcd.setCursor(0, 1);
   lcd.print("UP KEY RIGHT                ");
}

void mi_handler_up_tecla_4(){ //esta fncion de callback especifica para el LDR nos convierte el valor  

   lcd.setCursor(0, 1);
   lcd.print("UP KEY SELECT              ");
}

void mi_handler_down_tecla_5(){ //esta fncion de callback especifica para el LDR nos convierte el valor  
   
   lcd.setCursor(0, 1);
   lcd.print("DOWN A2                ");
}

void mi_handler_down_tecla_6(){ //esta fncion de callback especifica para el LDR nos convierte el valor  
    
   lcd.setCursor(0, 1);
   lcd.print("DOWN A3                ");
}

void mi_handler_down_tecla_7(){ //esta fncion de callback especifica para el LDR nos convierte el valor  
   lcd.setCursor(0, 1);
   lcd.print("DOWN A4                ");
}

void mi_handler_down_tecla_8(){ //esta fncion de callback especifica para el LDR nos convierte el valor  
   lcd.setCursor(0, 1);
   lcd.print("DOWN A5                ");
}

void mi_handler_up_tecla_5(){ //esta fncion de callback especifica para el LDR nos convierte el valor  
   
   lcd.setCursor(0, 1);
   lcd.print("UP A2                ");
}

void mi_handler_up_tecla_6(){ //esta fncion de callback especifica para el LDR nos convierte el valor  
   lcd.setCursor(0, 1);
   lcd.print("UP A3                ");
}

void mi_handler_up_tecla_7(){ //esta fncion de callback especifica para el LDR nos convierte el valor  
   lcd.setCursor(0, 1);
   lcd.print("UP A4                ");
}

void mi_handler_up_tecla_8(){ //esta fncion de callback especifica para el LDR nos convierte el valor  
   lcd.setCursor(0, 1);
   lcd.print("UP A5                ");
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
    teclado_init();
    init_handlers_teclas();

    //INICIALIZAR EL LED
    pinMode(10, OUTPUT);
    // set up the LCD's number of columns and rows: 
    lcd.begin(numCols,numRows);
    analogWrite(10, 200); //Controla intensidad backlight
    lcd.setCursor(0, 0);
    lcd.print("ACTIVIDAD 4");    
}

void loop(){  
  teclado_loop(); 
}



