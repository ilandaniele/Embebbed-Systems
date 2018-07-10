#include "adc_driver.h"
#include <avr/interrupt.h>

#define TECLA_UP      0
#define TECLA_DOWN    1
#define TECLA_LEFT    2
#define TECLA_RIGHT   3
#define TECLA_SELECT  4
#define BOTON_A2      5
#define BOTON_A3      6
#define BOTON_A4      7
#define BOTON_A5      8 


void keyboard_driver_init();
void keyboard_loop();
void callback_keyboard();
void key_down_callback(void (*handler)(), int tecla);
void key_up_callback(void (*handler)(), int tecla);
