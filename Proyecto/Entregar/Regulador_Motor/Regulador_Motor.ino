float velocidad = 0;

void setup(){
  pinMode(9, OUTPUT);
  Serial.begin(9600);
}
void cambiarVelocidad(int entrada){
  switch(entrada){
    case 0:{
      velocidad = 102/4;
    }break;
    case 1:{
      velocidad = 204/4;
    }break;
    case 2:{  
      velocidad = 306/4;
    }break;
    case 3:{
      velocidad = 408/4;
    }
    break;
    case 4:{
      velocidad =  510/4;
    }break;
    case 5: {
      velocidad = 612/4;
    }
    break;
    case 6: {
      velocidad = 714/4;
    }
    break;
    case 7: {
      velocidad = 816/4;
    }
    break;
    case 8: {
      velocidad = 918/4;
    }
    break;
    case 9: {
      velocidad = 1020/4;
    }
    break;
  }

}
void loop() {
    // put your main code here, to run repeatedly:
  if(Serial.available()>0){
    char a = Serial.read();
    if(a != 's'){
    int entrada = (int)a - 48;
    entrada = entrada % 10;
    cambiarVelocidad(entrada);
    Serial.print("Cambiamos la velocidad a: ");
    Serial.println(velocidad);
    }else{
      Serial.println("Detuvimos el motor");
      velocidad=0;  
    }
    
    analogWrite(9, velocidad);
  }
}
