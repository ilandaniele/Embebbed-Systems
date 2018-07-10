#include "mraa.hpp"
#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <ctype.h>

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
#define TECLA_UP        10
#define TECLA_DOWN      11
#define TECLA_LEFT      12
#define TECLA_RIGHT     13
#define BOTON_A2        14

//si el entero a mandar por i2c es menor a 255 lo puedo meter en un solo byte espacio del arreglo

uint8_t msj[17];
typedef struct{
	uint8_t tipo;
	uint8_t tamanio;
	float payload[4] = {0.0,0.0,0.0,0.0};
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
   msj[1]= (uint8_t) tipoMsj; //ojo con el cero
   msj[2]= (uint8_t) ',';
   msj[3]= (uint8_t) 0;
   msj[4]= (uint8_t) ']';

}
void crearMensajeSocket(){
   int tipoMsj = (int) pkt.tipo; //ver si esto anda
   //printf("tipo del mensaje en crear Mensaje Socket: ");
   //printf("%d\n",tipoMsj);
   msj[0]= (uint8_t) '[';
   msj[1]= (uint8_t) tipoMsj;
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
        float valor = pkt.payload[0];

        int centivalor = valor*100;
        int parteDecimal = centivalor % 100;
        int parteEntera = centivalor / 100;

        msj[5] = (uint8_t) parteEntera;
        msj[6] = (uint8_t) parteDecimal;

      }
      break;
      case OBTENER_MIN:
      {
        float valor = pkt.payload[2];
        int centivalor = valor*100;
        int parteDecimal = centivalor % 100;
        int parteEntera = centivalor / 100;

        msj[5] = (uint8_t) parteEntera;
        msj[6] = (uint8_t) parteDecimal;

      }
      break;
      case OBTENER_MAX:
      {
        float valor = pkt.payload[1];
        int centivalor = valor*100;
        int parteDecimal = centivalor % 100;
        int parteEntera = centivalor / 100;

        msj[5] = (uint8_t) parteEntera;
        msj[6] = (uint8_t) parteDecimal;

      }
      break;
      case OBTENER_PROM:
      {

        float valor = pkt.payload[3];
        int centivalor = valor*100;
        int parteDecimal = centivalor % 100;
        int parteEntera = centivalor / 100;

        msj[5] = (uint8_t) parteEntera;
        msj[6] = (uint8_t) parteDecimal;

      }
      break;
      case RESPONDER_TODO:
      {
        float valor = pkt.payload[0];
        int centivalor = valor*100;
        int parteDecimal = centivalor % 100;
        int parteEntera = centivalor / 100;

        msj[5] = (uint8_t) parteEntera;
        msj[6] = (uint8_t) parteDecimal;
        msj[7] = (uint8_t) ',';

        valor = pkt.payload[1];
        centivalor = valor*100;
        parteDecimal = centivalor % 100;
        parteEntera = centivalor / 100;

        msj[8] = (uint8_t) parteEntera;
        msj[9] = (uint8_t) parteDecimal;
        msj[10] =(uint8_t) ',';

        valor = pkt.payload[2];
        centivalor = valor*100;
        parteDecimal = centivalor % 100;
        parteEntera = centivalor / 100;

        msj[11] = (uint8_t) parteEntera;
        msj[12] = (uint8_t) parteDecimal;
        msj[13] = (uint8_t) ',';


        float valorProm = pkt.payload[3];
        centivalor = valorProm*100;
        parteDecimal = centivalor % 100;
        parteEntera = centivalor / 100;

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
void descomprimirMensaje2(uint8_t * mensaje){
  if(checkearFormato(mensaje)){
    pkt.tipo = mensaje[1];
    pkt.tamanio = mensaje[3];
    int cantidadComas = 0;
    int i = 5;
    uint8_t parteEntera=0;
    uint8_t parteDecimal=0;
    if(length(mensaje)>5){
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
    }


  }else{
    printf("formato de mensaje erróneo");
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
			printf("formato de mensaje erróneo");
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
	int cantidadBytesDelPaqueteARecibir=0;

	//Armo la conexion del socket

    int sockfd, newsockfd, portno; //sockfd y newsockfd son descriptores de archivo, i.e. array subscripts into the file descriptor table.
	//estas dos variables almacenan los valores retornados por la system call del socket y aceptan dicha system call
	//portno guarda el numero de puerto en el que el servidor acepta conexiones

	portno = 2252;

	socklen_t clilen; //guarda el tamaño de la direccion del cliente, es precisado para aceptar la system call

	uint8_t buffer[5]; //El servidor lee caracteres de la conexion del socket dentro de este buffer

	int n; //es el valor de retorno para las llamadas de read() y write(). Contiene el número de caracteres escritos o leidos.

	struct sockaddr_in serv_addr, cli_addr;  //sockaddr_in es una estructura que contiene una direccion de internet
											//la variable serv_addr contendra la dirección del servidor
											//la variable cli_addr contendrá la dirección del cliente que se conecta al servidor

	 //creo el socket

	 sockfd = socket(AF_INET, SOCK_STREAM, 0); //esta función crea el socket, el primer parámetro es el dominio de direcciones del socket, el AF_INET
											   //es usado para dos hosts en internet, el segundo parametro es el tipo de socket
											   //el tercer parámetro es el protocolo a usar, en este caso con 0 el SO elije que protocolo conviene

	 if (sockfd < 0){
		printf("ERROR opening socket");
	 }
	 //La system call del socket retorna una entrada a la tabla del descriptor de archivo, un entero pequeño, este valor es usado para todas las
	 //subsecuentes referencias a este socket, si la llamada al socket falla, esta retorna -1

	 bzero((char *) &serv_addr, sizeof(serv_addr));
	 //The function bzero() sets all values in a buffer to zero.
	 //It takes two arguments, the first is a pointer to the buffer and the second is the size of the buffer.
	 //Thus, this line initializes serv_addr to zeros.

	 serv_addr.sin_family = AF_INET; //el parámetro sin_family de serv_addr es una variable de tipo short que contiene el codigo para la familia de direcciones
	 serv_addr.sin_port = htons(portno); //aqui seteamos al parámetro sin_port de la variable serv_addr, el puerto, en este caso
	 //con la funcion htons lo pasamos al formato de network byte order
	 serv_addr.sin_addr.s_addr = INADDR_ANY; //en el campo sin_addr.s_addr guardo la ip del host, en este caso la ip de la máquina donde estoy corriendo
	 //y dicha es INADDR_any

	 if (bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0){
			  printf("ERROR on binding");
	 } //con la system call bind() yo bindeo el socket a una dirección, en este caso la dirección del host actual y el numero de puerto donde correra el servidor
	 //en los argumentos de bind, por ejemplo serv_addr tengo guardado el puerto y demas datos importantes, etc.

	 listen(sockfd,5); //esta system call le permite al proceso escuchar por conexiones en el socket, el primer argumento es el descriptor de archivo
	 // y el segundo es el tamaño de la backlog queue, el número de conexiones que pueden esperar cuando el proceso esta utilizando una conexión

	 clilen = sizeof(cli_addr);

	 newsockfd = accept(sockfd, (struct sockaddr *) &cli_addr, &clilen); //The accept() system call causes the process to block until a client connects to the server.
	 //Thus, it wakes up the process when a connection from a client has been successfully established
	 //It returns a new file descriptor, and all communication on this connection should be done using the new file descriptor.


	 if (newsockfd < 0){
		  printf("ERROR on accept");
	 }



	 for (;;) {

		/**
		*Acá debería pedirle al socket recibir el mensaje para saber que tipo de mensaje enviarle al arduino
		**/
		bzero(buffer,5);
		n = read(newsockfd, buffer, 5);
		if (n < 0){
			perror("ERROR reading from socket");
		}

		//recibo el mensaje, obtengo el tipo y a partir de esto creo el nuevo mensaje
		descomprimirMensaje(buffer);

		crearMensaje((int)pkt.tipo);

		i2c->write(msj,5);
    	sleep(1);

		if((int)pkt.tipo == OBTENER_TODO){ //si el tipo de mensjae es de obtener algo
				cantidadBytesDelPaqueteARecibir = 17;
				i2c->read(rx_tx_buf, cantidadBytesDelPaqueteARecibir);
				sleep(1);
				uint8_t * mensaje = rx_tx_buf;
				descomprimirMensaje(mensaje);
				printear();
				fflush(stdout);

				/**
				* aca luego envío el mensaje al socket
				*/
				bzero(msj,17); //limpio el mensaje enviado
				crearMensajeSocket();

				n = write(newsockfd, msj, 17);
				if (n < 0){
					printf("ERROR writing to socket");
				}

		}


      // Forzar la salida de stdout
      fflush(stdout);
   }
	close(newsockfd);
    close(sockfd);
    return 0;
}

