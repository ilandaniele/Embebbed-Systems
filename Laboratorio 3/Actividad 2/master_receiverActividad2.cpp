#include "mraa.hpp"
#include <iostream>
using namespace std;

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

//si el entero a mandar por i2c es menor a 255 lo puedo meter en un solo byte espacio del arreglo

uint8_t msj[6];
typedef struct{
	uint8_t tipo;
	uint8_t tamanio;
	float payload[4] = {0,0,0,0};
} paquete;

paquete pkt;
void printear(){

		int tipo = (int) pkt.tipo;

		if(tipo == RESPONDER_LUX ){
			printf("Tipo de paquete: RESPONDER_LUX \n");
			printf("Valor actual: ");
			printf("%.2f\n",pkt.payload[0]);
		}
		if(tipo == RESPONDER_MAX ){
			printf("Tipo de paquete: RESPONDER_MAX \n");
			printf("Valor maximo: ");
			printf("%.2f\n",pkt.payload[0]);
		}
		if(tipo == RESPONDER_MIN ){
			printf("Tipo de paquete: RESPONDER_MIN \n");
			printf("Valor minimo: ");
			printf("%.2f\n",pkt.payload[0]);
		}
		if(tipo == RESPONDER_PROM ){
			printf("Tipo de paquete: RESPONDER_PROM \n");
			printf("Valor promedio: ");
			printf("%.2f\n",pkt.payload[0]);
		}
		if(tipo == RESPONDER_TODO ){
			printf("Tipo de paquete: RESPONDER_TODO \n");
			printf("Valor actual: ");
			printf("%.2f\n",pkt.payload[0]);
			printf("Valor maximo: ");
			printf("%.2f\n", pkt.payload[1]);
			printf("Valor minimo: ");
			printf("%.2f\n",pkt.payload[2]);
			printf("Valor promedio: ");
			printf("%.2f\n",pkt.payload[3]);
		}
	}

void crearMensaje(int tipoMsj){
	
   msj[0]= (uint8_t) '[';
   msj[1]= (uint8_t) tipoMsj; 
   msj[2]= (uint8_t) ',';
   msj[3]= (uint8_t) 0;
   msj[4]= (uint8_t) ']';

}
/*
int checkearFormato(uint8_t * mensaje){
  int formatoCorrecto = 1;
  int i=0;
 //duda con el length, si tengo comoponentes vacias del arreglo al final tambien las cuenta ?
  length = sizeof(mensaje)/sizeof(uint8_t);
  printf("length -> %d\n",length);
  while(formatoCorrecto == 1 && i < length){
    if(length != 8 || length != 17 || length != 5){
      formatoCorrecto=0;
    }
    if(i==0 && mensaje[i] != '['){
      //es el principio de mensaje y no es el simbolo de comienzo
        formatoCorrecto=0;
    }
    if(i== length-1 && mensaje[i] != ']'){
      //es el final del mensaje y no es el simbolo de fin
      formatoCorrecto=0;
    }
    if( (mensaje[i] == '[' || mensaje[i] == ']') && i!=length-1 && mensaje[i+1] != '/'){
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


void descomprimirMensaje2(uint8_t * mensaje){
  if(checkearFormato(mensaje)){
    pkt.tipo = mensaje[1];
    pkt.tamanio = mensaje[3];
    int cantidadComas = 0;
    int i = 5;
    uint8_t parteEntera=0;
    uint8_t parteDecimal=0;
    while(i < length ){
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


  }else{
    printf("formato de mensaje erroneo");
  }
}
*/

void descomprimirMensaje(uint8_t * mensaje){ //uint_8 mensaje[32]
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


        if(esPayload == 1){
                      			for(j=0; j < pkt.tamanio; j++){printf("valor de i-> %d ",i);
                      						if(mensaje[i] == ','){

                              				}else{
                              					  if(mensaje[i] == ']'){
                              						    fin=1;
                              						  printf("entre en corchete final \n ");
                              					  }else{

                                                  if(k==0){ //estamos leyendo parte entera
                                                        parteEntera = mensaje[i];
                                                        k++;
                                                        printf("entre en parte entera %d \n ",parteEntera);
                                                  }else{  //k=1, estamos leyendo parte decimal
                                                        parteDecimal = mensaje[i];
                                                        float valor = parteEntera + (float) parteDecimal/100;
                                                        pkt.payload[contador] = valor; //transforma los 4 uint8_t a un solo float
                                                        k=0;
                                                        contador++;
                                                        printf("entre en parte decimal %d \n ",parteDecimal);
                                                        printf("contador -> %d \n",contador);
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
    	if(i > 16){
			//hay codigo basura
			printf("formato de mensaje erroneo");
			fin=1;
    	}
	}

 }

int main() {
  mraa::I2c* i2c;
  i2c = new mraa::I2c(0);
  i2c->address(8);

  uint8_t rx_tx_buf[17];
  int tipoMsj;
  int cantidadBytesDelPaquete=0;
  
    for (;;) {
    	
    		printf("Introduzca un numero entre [0,15]: \n");
    		scanf("%d",&tipoMsj);
    		crearMensaje(tipoMsj);
    	  
    	  if(tipoMsj == OBTENER_TODO){
    	        cantidadBytesDelPaquete=17;
    	  }else{
    	        cantidadBytesDelPaquete=8;
    	  }
    	i2c->write(msj,5);
		sleep(1);
    	i2c->read(rx_tx_buf, cantidadBytesDelPaquete);
		sleep(1);
    	

    	uint8_t * mensaje = rx_tx_buf;

		  descomprimirMensaje(mensaje);

		  printear();

    	// Forzar la salida de stdout
    	fflush(stdout);
    }

    return 0;
}
