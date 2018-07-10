
#include "Arduino.h"
// include the library code:
#include <LiquidCrystal.h>
 // these constants won't change.  But you can change the size of
  // your LCD using them:
  const int numRows = 2;
  const int numCols = 16;
  float R=0;
  float Lux;
  // initialize the library with the numbers of the interface pins
  LiquidCrystal lcd(8, 9, 4, 5, 6, 7);

float mapfloat(float x, float in_min, float in_max, float out_min, float out_max){
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

float obtenerLux(float R){ //esta fncion de callback especifica para el LDR nos convierte el valor
  
  float Vi= mapfloat(R,0,1023,0,5);
  float RLDR = (10*5)/Vi - 10;
  Lux=0;
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
void setup() {
  // put your setup code here, to run once:
  pinMode(A1,INPUT);
  pinMode(10, OUTPUT);
  lcd.begin(numCols,numRows);
  analogWrite(10, 200); //Controla intensidad backlight
  lcd.setCursor(0, 0);
  lcd.print("Actividad 3");
 }

void loop() {
  R = analogRead(A1);
  Lux = obtenerLux(R);
  lcd.setCursor(0,1);
  lcd.print("Lux:");
  lcd.setCursor(6,1);
  lcd.print(Lux);

}


