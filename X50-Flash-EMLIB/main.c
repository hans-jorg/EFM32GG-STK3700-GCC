/**
 *  @file main.c
 *  @brief This is a test of the NAND flash in the STK3700
 *
 */
#include <stdio.h>
#include <stdint.h>

#include "em_device.h"
#include "em_chip.h"
#include "em_cmu.h"
#include "nandflash.h"

#define NANDDMACH  (2)

const int TickDivisor = 1000; // milliseconds

volatile uint64_t tick = 0;

void SysTick_Handler (void) {
static int counter = 0;

    tick++;

    if( counter == 0 ) {
        counter = TickDivisor;
        // Process every second
    }
    counter--;
}


void Delay(int delay) {
uint64_t l = tick+delay;

    while(tick<l) {}

}

/**
 *  @brief  Main function
 */
int main(void) {
//char line[200];

    // Chip errata
    CHIP_Init();


     // Set clock source to external crystal: 48 MHz

     /* Configure SysTick */
     SysTick_Config(SystemCoreClock/TickDivisor);

     /* Configure UART */


     __enable_irq();

    int rc = NANDFLASH_Init(NANDDMACH);
    //printf("rc = %04X\n",rc);

    int status = NANDFLASH_ReadStatus();
    //printf("status = %04X\n",status);

    int signature = NANDFLASH_ReadSignature();
    //printf("Signatgure = %04X\n",signature);

	// Shut up error messages
	(void) rc;
	(void) status;
	(void) signature;
    
	// Stop
    while(1) {}
}
