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
uint64_t lim = tick+delay;

    while(tick<lim) {}

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
 ****************************************************************************/

#define AREASIZE                (100)
#define MEMCPY_CHANNEL            (4)

uint8_t area1[AREASIZE+2]; // 2 guard positions, one before and other after
uint8_t area2[AREASIZE+2]; // 2 guard positions, one before and other after


void Check(unsigned ch) {
DMA_DESCRIPTOR_TypeDef *pDesc = GetPrimaryDescriptor(ch);
unsigned pch = ch&0xF;

    printf("STATUS                  = %08x\n",DMA->STATUS);
    printf("ERRORC                  = %08x\n",DMA->ERRORC);
    printf("CONFIG                  = %08x\n",DMA->CONFIG);
    printf("CHENS                   = %08x\n",DMA->CHENS);
    printf("IF                      = %08x\n",DMA->IF);
    printf("IFC                     = %08x\n",DMA->IFC);
    printf("CHREQMASKS              = %08x\n",DMA->CHREQMASKS);
    printf("CHREQSTATUS             = %08x\n",DMA->CHREQSTATUS);
    printf("CHWAITSTATUS            = %08x\n",DMA->CHWAITSTATUS);

    printf("CHx_CTRL                = %08X\n",DMA->CH[ch&0xF].CTRL);
    printf("Description Source      = %08x\n",pDesc->SRCEND);
    printf("Description Destination = %08x\n",pDesc->DSTEND);
    printf("Description User        = %08x\n",pDesc->USER);
    printf("Description Control     = %08x\n",pDesc->CTRL);

    printf("enabled_channels        = %08x\n",enabled_channels);
    printf("done_channels           = %08x\n",done_channels);
    printf("error_channels          = %08x\n",error_channels);

}


int main(void) {
char line[100];
int tryn = 0;
int rc;

    /* Configure LEDs */
    LED_Init(LED1|LED2);

    // Set clock source to external crystal: 48 MHz
    //(void) SystemCoreClockSet(CLOCK_HFXO,1,1);

    /* Turn on LEDs */
    LED_Write(0,LED1|LED2);

    /* Configure SysTick */
    SysTick_Config(SystemCoreClock/TickDivisor);

    rc = DMA_Init();
    printf("rc = %d\n",rc);

    __enable_irq();

    // TODO: Test varying the values in the source area

    printf("area 1 = %p\n",area1);
    printf("area 2 = %p\n",area2);

    while (1) {
        for(uint8_t c  = 'A';c<='Z';c++) {
            printf("Trying to transfer an area full of %c....\n",c);
            fill_constant(area1,c,AREASIZE+2);// Initialize source area
            memset(area2,0,AREASIZE+2);       // Clear destination area
            rc = DMA_SetupTransferMemToMem_8(MEMCPY_CHANNEL,area1+1,area2+1,AREASIZE);
            printf("rc = %d\n",rc);
            Check(MEMCPY_CHANNEL);
            rc = DMA_StartTransfer(MEMCPY_CHANNEL);

            int count;
            do {
               // DMA->CHENS = (1U<<MEMCPY_CHANNEL);
               // DMA->CHSWREQ = (1U<<MEMCPY_CHANNEL);
                Check(MEMCPY_CHANNEL);
                count = DMA_GetTransferCount(MEMCPY_CHANNEL);
                printf("Count: %d\n",count);
            } while ( count != 0 );
            rc = test_constant(area2+1,c,AREASIZE);
            if( rc == 1 ) {
                if ( (area2[0]!=0) || (area2[AREASIZE+1]!=0) ) {
                    printf("    Guard positions modified\n");
                } else {
                    printf("    Transfer OK\n");
                }
            } else {
                printf("    Transfer Error\n");
            }
            printf("rc = %d\n",rc);
            puts("Press ENTER to continue");
            fgets(line,10,stdin);
        } // for(c)
    }; // loop forever

}
