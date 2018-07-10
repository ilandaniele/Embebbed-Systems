
typedef struct{
	int canal;
	void (*callback)(); //con el parentesis indico que es una función
	//void (*callback)(int x);
	int adc_value;
}adc_cfg;

void adc_init(adc_cfg *cfg);
void adc_loop();