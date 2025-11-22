/** ***************************************************************************
 * @file    main.c
 * @brief   Simple Flash Demo for EFM32GG_STK3700
 * @version 1.0
******************************************************************************/

#include <stdint.h>
#include <stdio.h>
#include <string.h>
/*
 * Including this file, it is possible to define which processor using command line
 * E.g. -DEFM32GG995F1024
 * The alternative is to include the processor specific file directly
 * #include "efm32gg995f1024.h"
 */
#include "em_device.h"
#include "clock_efm32gg_ext.h"
#include "dma.h"
#include "led.h"


//void UART0_TX_IRQHandler(void);
#define ENTER_ATOMIC() __disable_irq()
#define EXIT_ATOMIC()  __enable_irq()

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


void Delay(int delay) {
uint64_t l = tick+delay;

    while(tick<l) {}

}




static void fill_increasing(uint8_t *a, uint8_t c, size_t size) {
uint8_t *lim = a+size;

    while( a < lim )  *a++ = c++;

}

static int test_increasing(uint8_t *a, uint8_t c, size_t size) {
uint8_t *lim = a+size;

    while( (a < lim) && (*a==c) ) {
        a++;
        c++;
    }

    return a == lim;
}

static int fill_constant(uint8_t *a, uint8_t c, size_t size) {
uint8_t *lim = a+size;

    while( a < lim )  *a++ = c;

}

static int test_constant(uint8_t *a, uint8_t c, size_t size) {
uint8_t *lim = a+size;

    while( (a < lim) && (*a==c) ) a++;

    return a == lim;
}

/**************************************************************************//**
 * @brief  Main function
 *
 * @note   Using external crystal oscillator
 *         HFCLK = HFXO
 *         HFCORECLK = HFCLK
 *         HFPERCLK  = HFCLK
 ****************************************************************************/

int main(void) {
char line[100];
int tryn = 0;
#define AREASIZE (100)
char area1[AREASIZE+2];
char area2[AREASIZE+2];
    /* Configure LEDs */
    LED_Init(LED1|LED2);

    // Set clock source to external crystal: 48 MHz
    (void) SystemCoreClockSet(CLOCK_HFXO,1,1);

    /* Turn on LEDs */
    LED_Write(0,LED1|LED2);

    /* Configure SysTick */
    SysTick_Config(SystemCoreClock/TickDivisor);

    DMA_Init();

    __enable_irq();

    // TODO: Fazer test incrementing the values in the source
    int     ch = 4;
    uint8_t c  = 1;
    do {
        printf("Trying an area full of %d....\n",c);
        fill_constant(area1,c,AREASIZE+2);       // Initialize source area
        memset(area2,0,AREASIZE+2);       // Clear destination area
        DMA_SetupTransferMemToMem_8(ch,area1+1,area2+1,AREASIZE);
        DMA_StartTransfer(ch);
        while ( DMA_GetTransferStatus(ch) ) {}
        int rc = test_constant(area2+1,c,AREASIZE);
        if( (rc == 1) && (area2[0]==0) && (area2[AREASIZE-1]==0) ) {
            printf("    Transfer OK\n");
        } else {
            printf("    Transfer Error\n");
        }
        c++;
    } while (c < 2);

}
