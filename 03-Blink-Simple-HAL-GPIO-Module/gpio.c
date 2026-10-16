
/**
 * @file    gpio.c
 * @brief   GPIO HAL for EFM32GG
 * @version 1.0
 */

#include <stdint.h>
#include "gpio.h"

/**
 * @brief   Configure pins
 * @param   gpio: pointer to gpio registers
 * @param   pins: bit mask containing 1 in the position to be configured
 * @param   mode: GPIO_MODE_INPUT, GPIO_MODE_PUSHPULL, etc.
 */

void
GPIO_ConfigPins(GPIO_t gpio, uint32_t pins, uint32_t mode) {
const uint32_t mask = 0xF;  // Fields are 4 bits wide
int i;
uint32_t mbit;


    mbit = 0x0001; // Bit 0 set
    /* Configure low order bits/pins  0-7 */
    for(i=0;i<32;i+=4) {
        if( (pins&mbit) != 0 ) {
            gpio->MODEL &= ~(mask<<i);
            gpio->MODEL |= mode<<i;
        }
        mbit <<= 1;
    }
    /* Configure High order bits/pins 8-15 */
    for(i=0;i<32;i+=4) {
        if( (pins&mbit) != 0 ) {
            gpio->MODEH &= ~(mask<<i);
            gpio->MODEH |= mode<<i;
        }
        mbit <<= 1;
    }

}

/**
 * @brief   Configure pins for input or output
 * @param   gpio:    pointer to gpio registers
 * @param   inputs:  bit mask containing 1 in the position to be configured as input
 * @param   outputs: bit mask containing 1 in the position to be configured as output
 */

void
GPIO_Init(GPIO_t gpio, uint32_t inputs, uint32_t outputs) {


    /* Enable Clock for GPIO */
    CMU->HFPERCLKDIV |= CMU_HFPERCLKDIV_HFPERCLKEN;     // Enable HFPERCLK
    CMU->HFPERCLKEN0 |= CMU_HFPERCLKEN0_GPIO;           // Enable HFPERCKL for GPIO

    GPIO_ConfigPins(gpio,outputs,GPIO_MODE_PUSHPULL);
    GPIO_ConfigPins(gpio,inputs,GPIO_MODE_INPUT);       // Safer when both specified

}


/**
 * @brief   Set specified pins to zero
 * @param   gpio:   pointer to GPIO registers
 * @param   zeroes: bit mask specifing pins to be cleared
 * @param   ones:   bit mask specifing pins to be set
 *
 * @note    when a pin is specified in both (zeroes and ones) it is
 *          cleared and then set
 */

void
GPIO_ClearPins(GPIO_t gpio, uint32_t zeroes) {
#ifdef GPIO_ALTERNATE_OPERATIONS
    gpio->DOUTCLR = zeroes;
#else
    gpio->DOUT = (gpio->DOUT&~zeroes);
#endif
}

/**
 * @brief   Set specified pins to one
 * @param   gpio:   pointer to GPIO registers
 * @param   zeroes: bit mask specifing pins to be cleared
 * @param   ones:   bit mask specifing pins to be set
 *
 * @note    when a pin is specified in both (zeroes and ones) it is
 *          cleared and then set
 */

void
GPIO_SetPins(GPIO_t gpio, uint32_t ones) {
#ifdef GPIO_ALTERNATE_OPERATIONS
    gpio->DOUTSET = ones;
#else
    gpio->DOUT = (gpio->DOUT|ones);
#endif
}

/**
 * @brief   Set specified pins to zero, then others (or same) to 1
 * @param   gpio:   pointer to GPIO registers
 * @param   zeroes: bit mask specifing pins to be cleared
 * @param   ones:   bit mask specifing pins to be set
 *
 * @note    when a pin is specified in both (zeroes and ones) it is
 *          cleared and then set
 */

void
GPIO_ClearAndSetPins(GPIO_t gpio, uint32_t zeroes, uint32_t ones) {
#ifdef GPIO_ALTERNATE_OPERATIONS
    gpio->DOUTCLR = zeroes;
    gpio->DOUTSET = ones;
#else
    gpio->DOUT = (gpio->DOUT&~zeroes)|ones;
#endif
}

/**
 * @brief   Set specified pins to zero, then others (or same) to 1
 * @param   gpio:   pointer to GPIO registers
 * @param   zeroes: bit mask specifing pins to be cleared
 * @param   ones:   bit mask specifing pins to be set
 *
 * @note    when a pin is specified in both (zeroes and ones) it is
 *          cleared and then set
 */

void
GPIO_SetAndClearPins(GPIO_t gpio, uint32_t zeroes, uint32_t ones) {
#ifdef GPIO_ALTERNATE_OPERATIONS
    gpio->DOUTSET = ones;
    gpio->DOUTCLR = zeroes;
#else
    gpio->DOUT = (gpio->DOUT|ones)&~zeroes;
#endif
}

/**
 * @brief  Toggle specified pins
 * @param  gpio: GPIO port
 * @param  pins: bit mask specifing pins to be toggled
 */

void
GPIO_TogglePins(GPIO_t gpio, uint32_t pins) {
#ifdef GPIO_ALTERNATE_OPERATIONS
    gpio->DOUTTGL = pins;
#else
    gpio->DOUT ^= pins;
#endif
}

/**
 * @brief  Return pin status
 * @param  gpio: GPIO port
 */
uint32_t
GPIO_ReadPins(GPIO_t gpio) {
    return gpio->DIN;
}

/**
 * @brief  Write pins of specified GPIO port
 * @param  gpio: GPIO port
 * @param  pins: bit mask specifing new values for pins
 *
 * @note:  Beware. It affects all pins of the GPIO
 */
void
GPIO_WritePins(GPIO_t gpio, uint32_t pins) {
    gpio->DOUT = pins;
}
