#include <avr/io.h>
#include "Arduino.h"
#include <inttypes.h>
typedef struct adc_cfg adc_cfg;
struct adc_cfg
{
    int canal;
    int val;
    void (*callback)();
};

void adc_init(adc_cfg *cfg);
void adc_loop();

