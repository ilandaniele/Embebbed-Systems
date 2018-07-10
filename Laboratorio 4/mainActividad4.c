#include <stdio.h>
#include <system.h>
#include <sys/alt_irq.h>
#include <altera_avalon_pio_regs.h>
#include <altera_avalon_timer_regs.h>

//Variable de conteo
volatile unsigned int counter=-1;
volatile int new_second = 0;

static void timerISR(void* context)
{
	//Incrementa el contador en fcn de la base temporal del timer
	counter++;
    new_second = 1;

	//Muestra el contador por los leds
	IOWR_ALTERA_AVALON_PIO_DATA(PIO_0_BASE, counter);

	//Limpia el flag de interrupción (evita la ejecución múltiple de la ISR ante la misma IRQ)
	IOWR_ALTERA_AVALON_TIMER_STATUS(TIMER_0_BASE, 0);
}

int main()
{
  //Inicializo el arreglo de valores
  int data[8]={0,0,0,0,0,0,0,0};
  int old_value = IORD_ALTERA_AVALON_PIO_DATA(SWITCHES_BASE);
  int new_value = 0;
  int index = 0;
  int indexMin = 0;
  int delete_value = 0;
  int not_find = 0;
  
  //Inicializa la CARGA DE DATOS EN LOS PIO'S DE ENTRADA DEL MODULO C
	IOWR_ALTERA_AVALON_PIO_DATA(PIO_0_EXTERNAL_CONNECTION_BASE, data[0]);
	IOWR_ALTERA_AVALON_PIO_DATA(PIO_1_EXTERNAL_CONNECTION_BASE, data[1]);
	IOWR_ALTERA_AVALON_PIO_DATA(PIO_2_EXTERNAL_CONNECTION_BASE, data[2]);
	IOWR_ALTERA_AVALON_PIO_DATA(PIO_3_EXTERNAL_CONNECTION_BASE, data[3]);
	IOWR_ALTERA_AVALON_PIO_DATA(PIO_4_EXTERNAL_CONNECTION_BASE, data[4]);
	IOWR_ALTERA_AVALON_PIO_DATA(PIO_5_EXTERNAL_CONNECTION_BASE, data[5]);
	IOWR_ALTERA_AVALON_PIO_DATA(PIO_6_EXTERNAL_CONNECTION_BASE, data[6]);
	IOWR_ALTERA_AVALON_PIO_DATA(PIO_7_EXTERNAL_CONNECTION_BASE, data[7]);
	
  //Inicializa el Timer escribiendo su registro de control.
  IOWR_ALTERA_AVALON_TIMER_CONTROL (TIMER_0_BASE,
              ALTERA_AVALON_TIMER_CONTROL_ITO_MSK  |
              ALTERA_AVALON_TIMER_CONTROL_CONT_MSK |
              ALTERA_AVALON_TIMER_CONTROL_START_MSK);


  //Registra la ISR (callback) del Timer
  int regStatus = alt_ic_isr_register(TIMER_0_IRQ_INTERRUPT_CONTROLLER_ID,
		  	  	  	  TIMER_0_IRQ,
		  	  	  	  &timerISR, 0x0, 0x0);
  
  while (1)
  {
    //MANEJO DEL ARREGLO DATA
	new_value = IORD_ALTERA_AVALON_PIO_DATA(SWITCHES_BASE);
	if(new_value != old_value)
	{
		if(index < 8) //llenamos el arreglo data
		{		
			data[index] = new_value;
			index++;			
		}
		else//el arreglo data esta lleno, hay que buscar el index del menor valor
		{
			delete_value = IORD_ALTERA_AVALON_PIO_DATA(PIO_15_EXTERNAL_CONNECTION_BASE);
			while(not_find)
			{
				if(data[indexMin]==delete_value)
				{
					data[indexMin] = new_value;
					not_find = 0;
					indexMin = 0;
				}
				else
				{
					indexMin++;
				}
			}
		}
		//CARGAR DATOS EN LOS PIO'S DE ENTRADA DEL MODULO C
		IOWR_ALTERA_AVALON_PIO_DATA(PIO_0_EXTERNAL_CONNECTION_BASE, data[0]);
		IOWR_ALTERA_AVALON_PIO_DATA(PIO_1_EXTERNAL_CONNECTION_BASE, data[1]);
		IOWR_ALTERA_AVALON_PIO_DATA(PIO_2_EXTERNAL_CONNECTION_BASE, data[2]);
		IOWR_ALTERA_AVALON_PIO_DATA(PIO_3_EXTERNAL_CONNECTION_BASE, data[3]);
		IOWR_ALTERA_AVALON_PIO_DATA(PIO_4_EXTERNAL_CONNECTION_BASE, data[4]);
		IOWR_ALTERA_AVALON_PIO_DATA(PIO_5_EXTERNAL_CONNECTION_BASE, data[5]);
		IOWR_ALTERA_AVALON_PIO_DATA(PIO_6_EXTERNAL_CONNECTION_BASE, data[6]);
		IOWR_ALTERA_AVALON_PIO_DATA(PIO_7_EXTERNAL_CONNECTION_BASE, data[7]);
		
		old_value = new_value;
	}
	
	
	
	//CARGAR DATOS EN EL PIO DE SALIDA QUE VISUALIZA LOS DISPLAY
	if (new_second)
    {
		switch(counter){
			case 0: IOWR_ALTERA_AVALON_PIO_DATA(LEDS_BASE, IORD_ALTERA_AVALON_PIO_DATA(PIO_8_EXTERNAL_CONNECTION_BASE));
			break;
			case 1:	IOWR_ALTERA_AVALON_PIO_DATA(SWITCHES_BASE, IORD_ALTERA_AVALON_PIO_DATA(PIO_9_EXTERNAL_CONNECTION_BASE));
			break;
			case 2:	IOWR_ALTERA_AVALON_PIO_DATA(SWITCHES_BASE, IORD_ALTERA_AVALON_PIO_DATA(PIO_10_EXTERNAL_CONNECTION_BASE));
			break;
			case 3:	IOWR_ALTERA_AVALON_PIO_DATA(SWITCHES_BASE, IORD_ALTERA_AVALON_PIO_DATA(PIO_11_EXTERNAL_CONNECTION_BASE));
			break;
			case 4:	IOWR_ALTERA_AVALON_PIO_DATA(SWITCHES_BASE, IORD_ALTERA_AVALON_PIO_DATA(PIO_12_EXTERNAL_CONNECTION_BASE));
			break;
			case 5:	IOWR_ALTERA_AVALON_PIO_DATA(SWITCHES_BASE, IORD_ALTERA_AVALON_PIO_DATA(PIO_13_EXTERNAL_CONNECTION_BASE));
			break;
			case 6:	IOWR_ALTERA_AVALON_PIO_DATA(SWITCHES_BASE, IORD_ALTERA_AVALON_PIO_DATA(PIO_14_EXTERNAL_CONNECTION_BASE));
			break;
			case 7: IOWR_ALTERA_AVALON_PIO_DATA(SWITCHES_BASE, IORD_ALTERA_AVALON_PIO_DATA(PIO_15_EXTERNAL_CONNECTION_BASE));
					counter=-1;
			break;
		}			
		new_second = 0;
    }
  }
  return 0;
}