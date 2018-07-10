
/**
 * Ejemplo de skecth Processing para el desarrollo del Laboratorio.
 * Sistemas Embebidos - 1º Cuatrimestre de 2016
 *
 * Este skecth implementa un programa en el host que tiene la capacidad de
 * graficar funciones que evolucionan en el tiempo y además permite la
 * presentación de datos simples mediante Labels, y la captura de eventos
 * del usuario mediante la implementación de botones simples.
 *
 * La aplicación divide la ventana en 2 regiones, una de dibujado y otra 
 * donde se ubican los botones y etiquetas de información.
 *
 * Autor: Sebastián Escarza
 * Sistemas Embebidos - 1ºC de 2016.
 */

  
import processing.net.*; 

public class paquete{
  byte tipo;
  byte tamanio;
  float payload[] = {0,0,0,0};
  
  public paquete(){
    
  }

  public byte getTipo(){
    return tipo;
  }
  public void setTipo(byte t){
   tipo=t;
  }
  public byte getTamanio(){
    return tamanio;
  }
  public void setTamanio(byte tam){
    tamanio=tam;
  }
  
  public float[] getPayload(){
    return payload;
  }
  
  public void setPayload(float valor1, float valor2, float valor3, float valor4){
    payload[0]=valor1;
    payload[1]=valor2;
    payload[2]=valor3;
    payload[3]=valor4;
  }
}
int flagBoton = -1;
int Solicitar = 1;
//variable global para el paquete
paquete pkt;

//Variable global para mi cliente
Client myClient; 

//Variable global para mi buffer
byte[] byteBuffer = new byte[17]; //ojo con los tipos bytes, decia que aceptaba de -127 a 127
byte[] msj = new byte[5];
byte caracterFin = ']'; //caracter interesante de fin para que corte la funcion de read 0101 1101  93  5D  ]

// Declaraciones para graficar funciones...
float valorLux, valorLuxMin;
int cantValues;
ScrollingFcnPlot f1,f2,f3,f4;

//Botones...
RectButton rectBtn1, rectBtn2, rectBtn3, rectBtn4, rectBtn5;
PFont myFont;

//Etiquetas textuales...
boolean alert = false;
boolean prealert = false;
Label lbl1, lbl2, lbl3, lbl4, lbl5, lbl6, lbl7, lbl8, lbl9, lbl10, lbl11;
Label yVal1,yVal2,yVal3;

//Ventana y viewports...
int pgFunctionViewportWidth = 730;
int pgControlViewportWidth = 310;
int pgViewportsHeight = 430;

void settings () {

    size(pgFunctionViewportWidth+pgControlViewportWidth, pgViewportsHeight);
}

void setup() {

  //Se define el tamaño de la ventana de la aplicación... 
  
  pkt = new paquete();
  //Se inicializan los arreglos para almacenar las funciones...
  cantValues = pgFunctionViewportWidth;
  f1 = new ScrollingFcnPlot(cantValues,color(100,0,0),0,101);
  f2 = new ScrollingFcnPlot(cantValues,color(0,100,0),0,101);
  f3 = new ScrollingFcnPlot(cantValues,color(0,0,100),0,101);
  f4 = new ScrollingFcnPlot(cantValues,color(255),0,101);
  
  //Se inicializan los botones...
  
  
  rectBtn1 = new RectButton(pgFunctionViewportWidth+160,70,140,40,color(102),color(50),color(255),"UP");
  rectBtn2 = new RectButton(pgFunctionViewportWidth+10,130,140,40,color(102),color(50),color(255),"DOWN");
  rectBtn3 = new RectButton(pgFunctionViewportWidth+160,130,140,40,color(102),color(50),color(255),"LEFT");
  rectBtn4 = new RectButton(pgFunctionViewportWidth+10,190,140,40,color(102),color(50),color(255),"RIGHT");
  rectBtn5 = new RectButton(pgFunctionViewportWidth+160,190,140,40,color(102),color(50),color(255),"BOTON");
  
  
  
  //Se inicializan los labels...
  lbl1 = new Label(pgFunctionViewportWidth+10,250,color(255),"Status: ");
  lbl2 = new Label(pgFunctionViewportWidth+140,250,color(255),"-");
  lbl3 = new Label(pgFunctionViewportWidth+10,270,color(100,0,0),"Luminosidad Actual: ");
  lbl4 = new Label(pgFunctionViewportWidth+220,270,color(255),"-");
  lbl5 = new Label(pgFunctionViewportWidth+10,290,color(0,100,0),"Luminosidad Máxima: ");
  lbl6 = new Label(pgFunctionViewportWidth+220,290,color(255),"-");
  lbl7 = new Label(pgFunctionViewportWidth+10,310,color(0,0,100),"Luminosidad Mínima: ");
  lbl8 = new Label(pgFunctionViewportWidth+220,310,color(255),"-");
  lbl9 = new Label(pgFunctionViewportWidth+10,330,color(255),"Luminosidad Promedio: ");
  lbl10 = new Label(pgFunctionViewportWidth+220,330,color(255),"-");
  lbl11 = new Label(pgFunctionViewportWidth+10,400,color(255,0,0),"ALERTA!!!");
  
  yVal1 = new Label(10,5,color(255),"101");
  yVal2 = new Label(10,(pgViewportsHeight-20)/2,color(255),"50");
  yVal3 = new Label(10,pgViewportsHeight-25,color(255),"0");
  
  //Inicializa el font de la GUI...
  myFont = createFont("FFScala", 14);
  textFont(myFont);
  
  //Inicializo mi cliente
  myClient = new Client(this, "172.16.30.5", 2252); 
  if(myClient != null){
    println("Conexion establecida");
  }

 
   
}

void crearMensaje(int tipoMsj){
  //Preguntar si aquí no puedo mandar directamente un entero por byte
  
  int tamanio = 0;
   msj[0]= (byte) '[';
   msj[1]= (byte) tipoMsj; //ojo con el cero
   msj[2]= (byte) ',';
   msj[3]= (byte) tamanio;
   msj[4]= (byte) ']';
   
}

void descomprimirMensaje(byte mensaje[]){ //uint_8 mensaje[32]
  //estas leyendo byte por byte, entonces 1 posicion del arreglo es un byte.
  int i=0;
  int esTamanio = 0;
  int esPayload = 0;
  int j=0;
  int k=0;
  int fin=0;
  int contador=0;
  byte parteEntera = 0, parteDecimal = 0;

  while(fin == 0){
   
    

        if(mensaje[i] == '[' && i==0){//lei el primer caracter
              pkt.setTipo(mensaje[i+1]);
              
        }              
    
    
        if(esPayload == 1){
                            for(j=0; j < pkt.tamanio; j++){
                              
                                  if(mensaje[i] == ','){
                                        contador++;
                                      }else{
                                          if(mensaje[i] == ']'){
                                              fin=1;
                                          }else{

                                                  if(k==0){ //estamos leyendo parte entera
                                                        parteEntera = mensaje[i];
                                                        k++;
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
    if(i > 16 && fin != 1){
      //hay código basura
      println("formato de mensaje erróneo\n");
      fin=1;
    }
  
  }

 }


void draw() {
  
  
  if (myClient.active() == true) {
        if (myClient.available() > 0) {
          background(0);
          int byteCount = myClient.readBytes(byteBuffer);
          if(byteCount == 17){
            println("Recibi tamanio adecuado");
          }else{
            print("no recibi tamanio adecuado: ");
            println(byteCount);
          }
        descomprimirMensaje(byteBuffer);
          myClient.clear();
          Solicitar = 1;
        }
  }else{
    println("Client is not active."); 
  }
  //Se actualizan las funciones de ejemplo (f1: coseno(x), f2: función que depende de la posición Y del mouse)
  
  f1.updateValue(pkt.payload[0]);
  f2.updateValue(pkt.payload[1]);
  f3.updateValue(pkt.payload[2]);
  f4.updateValue(pkt.payload[3]);
  
  
  //Rendering de la interface...
  background(125);
  stroke(0);
  noFill();
 
  //Dibuja las funciones...
  f1.displayIntoRect(30,10,pgFunctionViewportWidth-10,pgViewportsHeight-10);
  f2.displayIntoRect(30,10,pgFunctionViewportWidth-10,pgViewportsHeight-10);
  f3.displayIntoRect(30,10,pgFunctionViewportWidth-10,pgViewportsHeight-10);
  f4.displayIntoRect(30,10,pgFunctionViewportWidth-10,pgViewportsHeight-10);
  
  //Procesa eventos de MouseOver...
  rectBtn1.update();
  rectBtn2.update();
  rectBtn3.update();
  rectBtn4.update();
  rectBtn5.update();
 
 
  //actualizo las etiquetas con los valores de LUX
  lbl4.caption = Float.toString(pkt.payload[0]);
  lbl6.caption = Float.toString(pkt.payload[1]);
  lbl8.caption = Float.toString(pkt.payload[2]);
  lbl10.caption = Float.toString(pkt.payload[3]);
  
  
  //Procesa las entradas (botones)
  if(mousePressed) {
    if(rectBtn1.pressed()) {
      rectBtn1.currentcolor = color(0,100,0);
      lbl2.caption = "UP";
      prealert = true;
      flagBoton=10;
      
    }else{ 
      if(rectBtn2.pressed()) {
        rectBtn2.currentcolor = color(0,100,0);
        lbl2.caption = "DOWN";
        prealert = true;
        flagBoton=11;
        
      }else{
        if(rectBtn3.pressed()){
          rectBtn3.currentcolor = color(0,100,0);
          lbl2.caption = "LEFT";
          prealert = true;
          flagBoton=12;
          
        }else{
          if(rectBtn4.pressed()){
            rectBtn4.currentcolor = color(0,100,0);
            lbl2.caption = "RIGHT";
            prealert = true;
            flagBoton=13;
            
          }else{
            if(rectBtn5.pressed()){
              rectBtn5.currentcolor = color(0,100,0);
              lbl2.caption = "BOTON A2";
              prealert = true;
              flagBoton=14;
               
            }
          }
        }
      }  
    }
  }else{ 
    lbl2.caption = "-";
  }
  if(Solicitar == 1){
     crearMensaje(4); //creo un mensaje con el tipo obtenerTodo;
     myClient.write(msj);
     Solicitar = 0;
  }

 
  //Dibuja el eje X y el recuadro de los gráficos...
  stroke(0);
  rect(30,10,pgFunctionViewportWidth-40,pgViewportsHeight-20);
  
  //Se dibujan los botones...
  rectBtn1.display();
  rectBtn2.display();
  rectBtn3.display();
  rectBtn4.display();
  rectBtn5.display();
  
  //Se dibuja texto adicional...(labels, etc)
  lbl1.display();
  lbl2.display();
  lbl3.display();
  lbl4.display();
  lbl5.display();
  lbl6.display();
  lbl7.display();
  lbl8.display();
  lbl9.display();
  lbl10.display();

  if (alert) lbl11.display();
  
  yVal1.display();
  yVal2.display();
  yVal3.display();  
  
  
}

void mouseReleased()
{
  //Si se pulsó algún botón y se completa el click, se hace el toggle sobre la etiqueta de alerta...
  if(prealert)
  {
    alert=!alert;
    prealert = false;
  }
  if(flagBoton != -1){
    crearMensaje(flagBoton);
    myClient.write(msj);
    flagBoton=-1;
  }
}