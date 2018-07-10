#include "keyboard_driver.h"
#include <LiquidCrystal.h>

// these constants won't change.  But you can change the size of
// your LCD using them:
const int numRows = 2;
const int numCols = 16;
LiquidCrystal lcd(8, 9, 4, 5, 6, 7);

// Init of functions ///

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

void setup() {

  Serial.begin(9600);
  
  lcd.begin(numCols,numRows);
  lcd.setCursor(0,0);
  lcd.print("Tecladriver");

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
}

void loop() {
  // put your main code here, to run repeatedly:
  keyboard_loop(); 
}


void flancoAscTeclaUp(){
  lcd.setCursor(0,1);
  lcd.print("Press UP    ");
}
void flancoDescTeclaUp(){
  lcd.setCursor(0,1);
  lcd.print("Solto UP    ");  
}
void flancoAscTeclaDown(){
  lcd.setCursor(0,1);
  lcd.print("Press DOWN    ");
}
void flancoDescTeclaDown(){
  lcd.setCursor(0,1);
  lcd.print("Solto DOWN    ");
}
void flancoAscTeclaLeft(){
  lcd.setCursor(0,1);
  lcd.print("Press LEFT    ");
}
void flancoDescTeclaLeft(){
  lcd.setCursor(0,1);
  lcd.print("Solto LEFT    ");
}
void flancoAscTeclaRight(){
  lcd.setCursor(0,1);
  lcd.print("Press RIGHT    ");
}
void flancoDescTeclaRight(){
  lcd.setCursor(0,1);
  lcd.print("Solto RIGHT    ");
}
void flancoAscTeclaSelect(){
  lcd.setCursor(0,1);
  lcd.print("Press SELECT    ");
}
void flancoDescTeclaSelect(){
  lcd.setCursor(0,1);
  lcd.print("Solto SELECT    ");
}
void flancoAscTeclaA2(){
  lcd.setCursor(0,1);
  lcd.print("Press A2    ");
}
void flancoDescTeclaA2(){
  lcd.setCursor(0,1);
  lcd.print("Solto A2    ");
}
void flancoAscTeclaA3(){
  lcd.setCursor(0,1);
  lcd.print("Press A3    ");
}
void flancoDescTeclaA3(){
  lcd.setCursor(0,1);
  lcd.print("Solto A3    ");
}
void flancoAscTeclaA4(){
  lcd.setCursor(0,1);
  lcd.print("Press A4    ");
}
void flancoDescTeclaA4(){
  lcd.setCursor(0,1);
  lcd.print("Solto A4    ");
}
void flancoAscTeclaA5(){
  lcd.setCursor(0,1);
  lcd.print("Press A5    ");    
}
void flancoDescTeclaA5(){
  lcd.setCursor(0,1);
  lcd.print("Solto A5    ");
}


