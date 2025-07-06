/** ***************************************************************************
 * @file    main.c
 * @brief   Simple UART Demo for EFM32GG_STK3700
 * @version 1.0
******************************************************************************/

#include <stdint.h>
#include <stdio.h>
/*
 * Including this file, it is possible to define which processor using command line
 * E.g. -DEFM32GG995F1024
 * The alternative is to include the processor specific file directly
 * #include "efm32gg995f1024.h"
 */
#include "em_device.h"
#include "clock_efm32gg_ext.h"

#include "led.h"
#include "uart2.h"


/*************************************************************************//**
 * @brief  Sys Tick Handler
 */
const int TickDivisor = 1000; // milliseconds

volatile uint64_t tick = 0;

void SysTick_Handler (void) {
static int counter = 0;

    tick++;

    if( counter == 0 ) {
        counter = TickDivisor;
        // Process every second
        LED_Toggle(LED1);
    }
    counter--;
}


void Delay(unsigned delay) {
uint64_t l = tick+delay;

    while(tick<l) {}

}

/**
 *  @brief ConfigureClock
 */
#define SPEED_RC_DEFAULT            0
#define SPEED_RC_HIGUEST            1
#define SPEED_RC_1MHZ               2
#define SPEED_HIGHEST               3
#define SPEED_X_1MHZ                4

#define SPEED_DEFAULT               SPEED_RC_DEFAULT

void ConfigureClock(int speed) {

    // Set clock source to external crystal: 48 MHz
    switch(speed) {
    case SPEED_HIGHEST:
        (void) SystemCoreClockSet(CLOCK_HFXO,1,1);
        break;
    case SPEED_RC_HIGUEST:
        (void) SystemCoreClockSet(CLOCK_HFRCO_28MHZ,1,1);
        break;
    case SPEED_RC_DEFAULT:
        (void) SystemCoreClockSet(CLOCK_HFRCO_1MHZ,1,1);
        break;
    case SPEED_RC_1MHZ:
        (void) SystemCoreClockSet(CLOCK_HFRCO_1MHZ,1,1);
        break;
    case SPEED_X_1MHZ:
        (void) SystemCoreClockSet(CLOCK_HFXO,48,48);
        break;
    }
    
    /*
     * There is a lot of device with dependencies on the clock frequency
     * Every time, it is change, they must be reconfigured
     * Do it here
     */
     
     
    /* Configure SysTick */
    SysTick_Config(SystemCoreClock/TickDivisor);
    
    // Reconfigure UART because the Peripheral Clock has changed
    UART_ReconfigureClock();
}
/**************************************************************************//**
 * @brief  Main function
 *
 * @note   Using external crystal oscillator
 *         HFCLK = HFXO
 *         HFCORECLK = HFCLK
 *         HFPERCLK  = HFCLK
 *
 */

int main(void) {
char line[100];
int tryn = 0;

    /* Configure LEDs */
    LED_Init(LED1|LED2);

    ConfigureClock(SPEED_HIGHEST);

    /* Turn on LEDs */
    LED_Write(0,LED1|LED2);
    
    // UART module uses interrupt
    __enable_irq();
    
    
    /* Print message using UART HAL */
    UART_SendString("\n\rUsando UART_SendString\n\r");

    /* Print message using Newlib */
    printf("\n\rUsando printf\n\r");
    
    
#define TEST 3

#if TEST == 1
    char s[] = { [0 ... 197]='*','\n','\r' }; // Only gcc!!!
    puts(s);

    while (1) {
        puts("\n\r");
        printf("Try %d\n",tryn++);
        printf(s);

        Delay(100);
    }
    
#elif TEST == 2

    char s[] = { [0 ... 197]='*','\n','\r' }; // Only gcc!!!
    puts(s);

    while (1) {
        puts("\n\r");
        printf("Try %d\n",tryn++);
        for(int ch=47;ch<127;ch++)
            putchar(ch);
            
        Delay(100);
    }
    
#elif TEST == 3

    char s2[] = "abcdefghijklmnopqrstuvwxyz";
    char *p1 = s2;
    char *p2 = &(s2[25]);
    while (1) {
        puts("\n\r");
        printf("Try %d\n",tryn++);
        
        puts(p1);
        puts(p2);
        //printf(p1);
        //printf(p2);
        printf("%s\n\r",p1);
        printf("%s\n\r",p2);
        if( *p1 )
            p1++;
        else
            p1 = s2;
        if( *p2 )
            p2++;
        else
            p2 = s2;
            
        Delay(1000);
    }
    
#else

    UART_SendString("\n\rHello using UART_SendString\n");
    printf("\n\rHello using printf\n");

    line[0] = 0;
    while (1) {
        printf("Try %d\n",tryn++);
        printf("\nYour name: ");
        //fgets(line,99,stdin);
        printf("Hello %s\n",line);

        Delay(100);
    }
#endif

}
