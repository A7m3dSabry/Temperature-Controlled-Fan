/*************************** Section: Includes *********************************/
#include "MCAL/ADC/adc.h"
#include "HAL/LCD/lcd.h"
#include "HAL/MOTOR/motor.h"
#include "HAL/LM35_SENSOR/lm35_sensor.h"


/*************************** Section: Local Variables Declarations *************/

/* Structure to dynamic configure the ADC module */
ADC_Config_t ADC_Configration = {ADC_INTERNAL_VREF, ADC_F_CPU_8};

DCMotor_t dcMotor = {PORTB_ID,PIN_0,PIN_1,PIN_3};
/* global variable to store the temperature value */
uint8_t g_tempValue;
/*************************** Section: Helper Methods Prototypes  ***************/
/*************************** Section: Methods Implementations ******************/




void APP_displayTemp() {
    if (g_tempValue >= 100) {
        HAL_LCD_moveCursor(1, 10);
        HAL_LCD_intToString(g_tempValue);
    } else {
        HAL_LCD_moveCursor(1, 10);
        HAL_LCD_intToString(g_tempValue);
        HAL_LCD_displayCharacter(' ');
    }
}


int main(void) {
    /* Initializing different modules */
    HAL_ADC_init(&ADC_Configration);
    HAL_LCD_init();
    HAL_DCMotor_Init(&dcMotor);

    /* Displaying outputs on the LCD */
    HAL_LCD_displayStringRowColumn(0, 3, "Fan is    ");
    HAL_LCD_displayStringRowColumn(1, 3, "Temp =    C");

    /* Calculate the temperature value and store it in a global variable */
     HAL_LM35_getTemperature(&g_tempValue);

    while (1) {
        HAL_LM35_getTemperature(&g_tempValue);
        APP_displayTemp();

        if (g_tempValue >= 120) {
            /* Temperature >= 120 -> Fan is ON  with 100% of its speed */
            HAL_LCD_displayStringRowColumn(0, 10, "ON ");
            HAL_DCMotor_Rotate(&dcMotor, DCMotor_CW, 100);
        } else if (g_tempValue >= 90) {
            /* Temperature >= 90 -> Fan is ON  with 75% of its speed */
            HAL_LCD_displayStringRowColumn(0, 10, "ON ");
            HAL_DCMotor_Rotate(&dcMotor, DCMotor_CW, 75);
        } else if (g_tempValue >= 60) {
            /* Temperature >= 60 -> Fan is ON  with 50% of its speed */
            HAL_LCD_displayStringRowColumn(0, 10, "ON ");
            HAL_DCMotor_Rotate(&dcMotor, DCMotor_CW, 50);
        } else if (g_tempValue >= 30) {
            /* Temperature >= 30 -> Fan is ON  with 25% of its speed */
            HAL_LCD_displayStringRowColumn(0, 10, "ON ");
            HAL_DCMotor_Rotate(&dcMotor, DCMotor_CW, 25);
        } else {
            /* Temperature >= 60 -> Fan is OFF */
            HAL_LCD_displayStringRowColumn(0, 10, "OFF ");
            HAL_DCMotor_Rotate(&dcMotor, DCMotor_STOP, 0);
        }

    }
}
