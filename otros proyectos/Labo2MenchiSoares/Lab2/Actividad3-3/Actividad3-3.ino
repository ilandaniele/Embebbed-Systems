#include <LiquidCrystal.h>
// These constants, define values needed for the LDR readings and ADC
#define LDR_PIN                   1
#define MAX_ADC_READING           1023
#define ADC_REF_VOLTAGE           5.0
#define REF_RESISTANCE            1000  // measure this for best results
#define LUX_CALC_SCALAR           5333349  //Found values in real life => 12518931
#define LUX_CALC_EXPONENT         -1.437 //Found values in real life => -1.405

// these constants won't change.  But you can change the size of
// your LCD using them:
const int numRows = 2;
const int numCols = 16;
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);


/**************************************************************************/
/*
 Arduino setup function (automatically called at startup)
 */
/**************************************************************************/
void setup(void)
{
    lcd.begin(numCols,numRows);
    lcd.setCursor(0,0);
    lcd.print("Ambient light");
    
}


/**************************************************************************/



/**************************************************************************/
void loop(void)
{
    
    int   ldrRawData;
    float resistorVoltage, ldrVoltage;
    float ldrResistance;
    float ldrLux;
    
    // Perform the analog to digital conversion
    ldrRawData = analogRead(LDR_PIN);
    
    // RESISTOR VOLTAGE_CONVERSION
    // Convert the raw digital data back to the voltage that was measured on the analog pin
    resistorVoltage = (float)ldrRawData / MAX_ADC_READING * ADC_REF_VOLTAGE;
    
    // voltage across the LDR is the 5V supply minus the 5k resistor voltage
    ldrVoltage = ADC_REF_VOLTAGE - resistorVoltage;
    
    // LDR_RESISTANCE_CONVERSION
    // resistance that the LDR would have for that voltage
    ldrResistance = ldrVoltage/resistorVoltage * REF_RESISTANCE;
    
    // LDR_LUX
    // Change the code below to the proper conversion from ldrResistance to
    // ldrLux
    ldrLux = LUX_CALC_SCALAR * pow(ldrResistance, LUX_CALC_EXPONENT);
    
    // print out the results
    lcd.setCursor(0,1);
    lcd.print(ldrLux);
    if (ldrLux >= 100 && ldrLux < 1000) lcd.print(" ");
    else if (ldrLux >= 10 && ldrLux < 100) lcd.print("  ");
    else if (ldrLux >= 0 && ldrLux < 10) lcd.print("   ");
    lcd.setCursor(13,1);
    lcd.print("Lux");
    
    delay(50);
}

