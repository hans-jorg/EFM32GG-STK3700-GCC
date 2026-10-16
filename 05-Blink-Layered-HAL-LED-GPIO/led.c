
/**
 * @file    gpio.c
 * @brief   GPIO HAL for EFM32GG
 * @version 1.0
 */
#include <stdint.h>
#include "gpio.h"
#include "led.h"

/**
 * @brief   Initializes system to enable LED control
 *
 * @note    LEDs are defined to easy access the corresponding pins in GPIO port
 */
void LED_Init(uint32_t leds) {

    // Configure Pins in GPIOE
    GPIO_Init(GPIOE,0,leds);

    // Turn Off LEDs
    GPIO_ClearPins(GPIOE,leds);

}

/**
 * @brief   Turns on LEDs specified by leds
 *
 * @note    LEDs are defined to easy access the corresponding pins in GPIO port
 */
void LED_On(uint32_t leds) {

    GPIO_SetPins(GPIOE,leds);
}

/**
 * @brief   Turns off LEDs specified by leds
 *
 * @note    LEDs are defined to easy access the corresponding pins in GPIO port
 */
void LED_Off(uint32_t leds) {

    GPIO_ClearPins(GPIOE,leds);
}

/**
 * @brief   Toggles the LEDs specified by leds
 *
 * @note    LEDs are defined to easy access the corresponding pins in GPIO port
 */
void LED_Toggle(uint32_t leds) {

    GPIO_TogglePins(GPIOE,leds);
}

void LED_Write(uint32_t off, uint32_t on) {

    GPIO_ClearAndSetPins(GPIOE,off,on);
}
