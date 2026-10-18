/** **************************************************************************
 * @file    uart.c
 * @brief   LED HAL for EFM32GG STK
 * @version 1.0
 *
 * @note    Configure UART0 to work at 115200 bps, 8 bit, no parity, 1 stop bit
 *          no parity handshake.
 *
 *          With this configuration, it can communicate with the board controller,
 *          which relay data in and from a Virtual Communication Port created
 *          as a CDC inside the USB connection.
 *
 *          In Windows, it appears as COMx. In Linux, as /dev/ttyACMx.
 *
 * @note
 *          Device      RX                      TX
 *          UART0       PF7|PE1|PA4         PF6|PE0|PA3
 *          UART1       PF11|PB10|PE3       PF10|PB9|PE2
 *
 * @note    PB9, PB10,PE0,PE1,PE2,PE3 are on the breakout pads
 *
 *****************************************************************************/

#include <stdint.h>
/*
 * Including this file, it is possible to define which processor using command line
 * E.g. -DEFM32GG995F1024
 * The alternative is to include the processor specific file directly
 * #include "efm32gg995f1024.h"
 */

#include "em_device.h"
#include "clock_efm32gg2.h"
#include "uart3.h"
#include "fifo.h"

/// Define this (uncomment) to use UART1
#define USE_UART1 1


/**
 * @brief   Macros to enhance portability
 */
#define BIT(N)              (1U<<(N))
#define ENTER_ATOMIC()      __disable_irq()
#define EXIT_ATOMIC()       __enable_irq()

/**
 * @brief   Configuration
 */
/// FIFO size for input and output
#define INPUTFIFO_SIZE              100
#define OUTPUTFIFO_SIZE             100

/// Interrupt level
#define RXINTLEVEL 6
#define TXINTLEVEL 6

/// baudrate
const uint32_t BAUD = 115200;
const uint32_t OVERSAMPLING = 16;

/**
 *  @note   GPIO Port E is used for UART0 RX/TX on STK3700 board
 *          GPIO Port F used for enable transceiver for UART0 on the STK3700 board
 */
#define UNUSED  __attribute__((unused))
static UNUSED  GPIO_P_TypeDef * const GPIOA = &(GPIO->P[0]);    // GPIOE
static UNUSED  GPIO_P_TypeDef * const GPIOB = &(GPIO->P[1]);    // GPIOE
static UNUSED  GPIO_P_TypeDef * const GPIOC = &(GPIO->P[2]);    // GPIOE
static UNUSED  GPIO_P_TypeDef * const GPIOD = &(GPIO->P[3]);    // GPIOE
static UNUSED  GPIO_P_TypeDef * const GPIOE = &(GPIO->P[4]);    // GPIOE
static UNUSED  GPIO_P_TypeDef * const GPIOF = &(GPIO->P[5]);    // GPIOF



/**
 * @brief   Global variables
 *
 * @note    To avoid use of malloc, it uses a macro to define area
 */

///@{
FIFO_DECLARE_AREA(ibuffarea0,INPUTFIFO_SIZE);
FIFO_DECLARE_AREA(obuffarea0,OUTPUTFIFO_SIZE);
Fifo_TypeDef inputfifo_0 = 0;
Fifo_TypeDef outputfifo_0 = 0;


#ifdef USE_UART1
FIFO_DECLARE_AREA(ibuffarea1,INPUTFIFO_SIZE);
FIFO_DECLARE_AREA(obuffarea1,OUTPUTFIFO_SIZE);
Fifo_TypeDef inputfifo_1 = 0;
Fifo_TypeDef outputfifo_1 = 0;
#endif
///@}

/**
 * @brief   Set baudrate UART
 *
 * @note    Configured for 16 times oversampling
 */

void UART_SetBaudrate(USART_TypeDef *uart, uint32_t baud) {
uint32_t bauddiv;

    // Asynchronous with 16x oversampling
    uart->CTRL  = _UART_CTRL_RESETVALUE|UART_CTRL_OVS_X16;         // Set field

    bauddiv = (ClockGetPeripheralClockFrequency()*4)/(16*baud)-4;
    uart->CLKDIV = bauddiv<<_UART_CLKDIV_DIV_SHIFT;

}


/**
 * @brief   Turn OFF all transmissions for UART1
 */
static inline void UART_TurnOff(USART_TypeDef *uart) {

    uart->CMD  = UART_CMD_TXDIS|UART_CMD_RXDIS;

}


/**
 * @brief   Turn ON all transmissions for UART0
 */
static inline void UART_TurnOn(USART_TypeDef *uart) {

    uart->CMD  = UART_CMD_TXEN|UART_CMD_RXEN;
}


/**
 * @brief   Prepare clock reconfiguration
 */
static void UART0_Prepare(uint32_t u) {

    UART_TurnOff(UART0);
}


/**
 * @brief   Prepare clock reconfiguration
 */
#ifdef USE_UART1
static void UART1_Prepare(uint32_t u) {

    UART_TurnOff(UART1);
}
#endif


/**
 * @brief   Reconfigures UART when clock changes
 */
static void UART0_Reconfigure(uint32_t u) {

    UART_SetBaudrate(UART0, BAUD);
    UART_TurnOn(UART0);
}


/**
 * @brief   Reconfigures UART when clock changes
 */
#ifdef USE_UART1
static void UART1_Reconfigure(uint32_t u) {

    UART_SetBaudrate(UART1, BAUD);
    UART_TurnOn(UART1);
}
#endif
/**
 * @brief   Resets UART
 */

static void UART_Reset(USART_TypeDef *uart) {

    /* Make sure disabled first, before resetting other registers */
    uart->CMD = UART_CMD_RXDIS | UART_CMD_TXDIS | UART_CMD_MASTERDIS
                | UART_CMD_RXBLOCKDIS | UART_CMD_TXTRIDIS | UART_CMD_CLEARTX
                | UART_CMD_CLEARRX;

    uart->CTRL      = _UART_CTRL_RESETVALUE;
    uart->FRAME     = _UART_FRAME_RESETVALUE;
    uart->TRIGCTRL  = _UART_TRIGCTRL_RESETVALUE;
    uart->CLKDIV    = _UART_CLKDIV_RESETVALUE;
    uart->IEN       = _UART_IEN_RESETVALUE;
    uart->IFC       = _UART_IFC_MASK;
    uart->ROUTE     = _UART_ROUTE_RESETVALUE;
    uart->IRCTRL    = _UART_IRCTRL_RESETVALUE;
    uart->INPUT     = _UART_INPUT_RESETVALUE;

#ifdef USE_UART1
    switch( (uint32_t) uart) {
    case UART0_BASE:
#endif
        fifo_deinit(inputfifo_0);
        fifo_deinit(outputfifo_0);
#ifdef USE_UART1
        break;
    case UART1_BASE:
        fifo_deinit(inputfifo_1);
        fifo_deinit(outputfifo_1);

        break;
    }
#endif
}


/**
 * @brief   Initializes UART
 *
 * @note    Does not enable interrupts!!!!
 */

void UART_Init(USART_TypeDef *uart) {

    /* Enable Clock for GPIO and UART */
    CMU->HFPERCLKDIV |= CMU_HFPERCLKDIV_HFPERCLKEN;     // Enable HFPERCLK
    CMU->HFPERCLKEN0 |= CMU_HFPERCLKEN0_GPIO;           // Enable HFPERCLK for GPIO

#ifdef USE_UART1
    switch( (uint32_t) uart) {
    case UART0_BASE:
#endif
    // Configure PE0 (TX)
        GPIOE->MODEL &= ~_GPIO_P_MODEL_MODE0_MASK;          // Clear field
        GPIOE->MODEL |= GPIO_P_MODEL_MODE0_PUSHPULL;        // Set field
        GPIOE->DOUT  |= BIT(0);

        // Configure PE1 (RX)
        GPIOE->MODEL &= ~_GPIO_P_MODEL_MODE1_MASK;          // Clear field
        GPIOE->MODEL |= GPIO_P_MODEL_MODE1_INPUT;           // Set field
        GPIOE->DOUT  |= BIT(1);

        // Enable clock for UART0
        CMU->HFPERCLKEN0 |= CMU_HFPERCLKEN0_UART0;          // Enable HFPERCLK for UART0
#ifdef USE_UART1
        break;
    case UART1_BASE:
    // Configure PE2 (TX). It can be PF10 or PB9 too
        GPIOE->MODEL &= ~_GPIO_P_MODEL_MODE0_MASK;          // Clear field
        GPIOE->MODEL |= GPIO_P_MODEL_MODE0_PUSHPULL;        // Set field
        GPIOE->DOUT  |= BIT(2);

        // Configure PE3 (RX). It can be PF11 or PB10 too
        GPIOE->MODEL &= ~_GPIO_P_MODEL_MODE1_MASK;          // Clear field
        GPIOE->MODEL |= GPIO_P_MODEL_MODE1_INPUT;           // Set field
        GPIOE->DOUT  |= BIT(3);

        // Enable clock for UART01
        CMU->HFPERCLKEN0 |= CMU_HFPERCLKEN0_UART1;          // Enable HFPERCLK for UART0
        break;
    }
#endif
    /* Reset UART */
    UART_Reset(uart);

    // 8 bits, no parity, 1 stop bit
    uart->FRAME  &= ~( _UART_FRAME_STOPBITS_MASK
                      |_UART_FRAME_PARITY_MASK
                      |_UART_FRAME_DATABITS_MASK );                 // Clear field

    uart->FRAME  |=   UART_FRAME_STOPBITS_ONE
                    | UART_FRAME_PARITY_NONE
                    | UART_FRAME_DATABITS_EIGHT;                    // Set field

    // Baud rate
    UART_SetBaudrate(uart,BAUD);

#ifdef USE_UART1
    switch( (uint32_t) uart) {
    case UART0_BASE:
#endif
        // Configure PF7 (Enable Transceiver)
        GPIOF->MODEL &= ~_GPIO_P_MODEL_MODE7_MASK;          // Clear field
        GPIOF->MODEL |= GPIO_P_MODEL_MODE7_PUSHPULL;        // Set field
        GPIOF->DOUT  |= BIT(7);

        // Set which location to be used
        UART0->ROUTE = UART_ROUTE_LOCATION_LOC1 | UART_ROUTE_RXPEN | UART_ROUTE_TXPEN;

        // Initializes fifos
        inputfifo_0  = fifo_init(ibuffarea0,INPUTFIFO_SIZE);
        outputfifo_0 = fifo_init(obuffarea0,OUTPUTFIFO_SIZE);
#ifdef USE_UART1
        break;
    case UART1_BASE:
        // Only if it uses the same enable
        // Configure PF7 (Enable Transceiver)
        GPIOF->MODEL &= ~_GPIO_P_MODEL_MODE7_MASK;          // Clear field
        GPIOF->MODEL |= GPIO_P_MODEL_MODE7_PUSHPULL;        // Set field
        GPIOF->DOUT  |= BIT(7);

        // Set which location to be used
        UART1->ROUTE = UART_ROUTE_LOCATION_LOC1 | UART_ROUTE_RXPEN | UART_ROUTE_TXPEN;

        // Initializes fifos
        inputfifo_1  = fifo_init(ibuffarea1,INPUTFIFO_SIZE);
        break;
    }
#endif

    // Enable interrupts on UART
    uart->IFC = (uint32_t) -1;
    uart->IEN |= UART_IEN_TXC|UART_IEN_RXDATAV;
#ifdef USE_UART1
    // Enable interrupts on NVIC
    switch( (uint32_t) uart) {
    case UART0_BASE:
#endif
        NVIC_SetPriority(UART0_RX_IRQn,RXINTLEVEL);
        NVIC_SetPriority(UART0_TX_IRQn,TXINTLEVEL);
        NVIC_ClearPendingIRQ(UART0_RX_IRQn);
        NVIC_ClearPendingIRQ(UART0_TX_IRQn);
        NVIC_EnableIRQ(UART0_RX_IRQn);
        NVIC_EnableIRQ(UART0_TX_IRQn);
        // Disable and then enable RX and TX
        UART_TurnOff(UART0);
        UART_TurnOn(UART0);
        // Register functions to reconfigure UART in case of clock change
        ClockRegisterCallback(CLOCK_CHANGED_HFPERCLK, UART0_Prepare, UART0_Reconfigure);
#ifdef USE_UART1
        break;
    case UART1_BASE:
        NVIC_SetPriority(UART1_RX_IRQn,RXINTLEVEL);
        NVIC_SetPriority(UART1_TX_IRQn,TXINTLEVEL);
        NVIC_ClearPendingIRQ(UART1_RX_IRQn);
        NVIC_ClearPendingIRQ(UART1_TX_IRQn);
        NVIC_EnableIRQ(UART1_RX_IRQn);
        NVIC_EnableIRQ(UART1_TX_IRQn);
        // Disable and then enable RX and TX
        UART_TurnOff(UART1);
        UART_TurnOn(UART1);
        // Register functions to reconfigure UART in case of clock change
        ClockRegisterCallback(CLOCK_CHANGED_HFPERCLK, UART1_Prepare, UART1_Reconfigure);
        break;
    }
#endif
}


/**
 * @brief   UART0 Interrupt routine for receiving data
 *
 * @note    Receives and put it in fifo
 */

void UART0_RX_IRQHandler(void) {
uint8_t ch;

    if ( UART0->IF&(UART_IF_RXDATAV|UART_IF_RXFULL) ) {
        // Put in input fifo
        ch = UART0->RXDATA;
//        ENTER_ATOMIC();
        (void) fifo_insert(inputfifo_0,ch);
//        EXIT_ATOMIC();
    }
}


/**
 * @brief   UART1 Interrupt routine for receiving data
 *
 * @note    Receives and put it in fifo
 */
#ifdef USE_UART1
void UART1_RX_IRQHandler(void) {
uint8_t ch;

    if ( UART1->IF&(UART_IF_RXDATAV|UART_IF_RXFULL) ) {
        // Put in input fifo
        ch = UART1->RXDATA;
//        ENTER_ATOMIC();
          (void) fifo_insert(inputfifo_1,ch);
//        EXIT_ATOMIC();
    }
}
#endif
/**
 * @brief   UART0 Interrupt routine for transmitting data
 *
 * @note    If there is data to transmit, send it
 * @note    UART_SendChar generates this interrupt
 */

void UART0_TX_IRQHandler(void) {
uint8_t ch;

    // if data in output fifo and transmitter idle, send it
    if( UART0->IF&UART_IF_TXC ) {
        if( UART0->STATUS&UART_STATUS_TXBL ) {
            if( !fifo_empty(outputfifo_0) ) {
                // Get from output fifo
                ch = fifo_remove(outputfifo_0);
                UART0->TXDATA = ch;
            }
        }
       UART0->IFC = UART_IFC_TXC;
    }
}


/**
 * @brief   UART0 Interrupt routine for transmitting data
 *
 * @note    If there is data to transmit, send it
 * @note    UART_SendChar generates this interrupt
 */
#ifdef USE_UART1
void UART1_TX_IRQHandler(void) {
uint8_t ch;

    // if data in output fifo and transmitter idle, send it
    if( UART1->IF&UART_IF_TXC ) {
        if( UART1->STATUS&UART_STATUS_TXBL ) {
            if( !fifo_empty(outputfifo_1) ) {
                // Get from output fifo
                ch = fifo_remove(outputfifo_1);
                UART1->TXDATA = ch;
            }
        }
       UART1->IFC = UART_IFC_TXC;
    }
}
#endif

/**
 * @brief   Get status of UART
 *
 * @note    Could be inline in uart.h
 */

unsigned UART_GetStatus(USART_TypeDef *uart) {
uint32_t w;

    w = uart->STATUS;
    return w;
}

/**
 * @brief   Send a char
 *
 * @note    Generates an interrupt to send char
 */

void UART_SendChar(USART_TypeDef *uart, char ch) {

#ifdef USE_UART1
    switch( (uint32_t) uart) {
    case UART0_BASE:
#endif
        if ( fifo_empty(outputfifo_0) ) {
            while ( (UART0->STATUS&UART_STATUS_TXBL) == 0 ) {}
            UART0->TXDATA = ch;
        } else {
            ENTER_ATOMIC();
            fifo_insert(outputfifo_0,ch);
            EXIT_ATOMIC();
        }
#ifdef USE_UART1
        break;
    case UART1_BASE:
        if ( fifo_empty(outputfifo_1) ) {
            while ( (UART1->STATUS&UART_STATUS_TXBL) == 0 ) {}
            UART1->TXDATA = ch;
        } else {
            ENTER_ATOMIC();
            fifo_insert(outputfifo_1,ch);
            EXIT_ATOMIC();
        }
        break;
    }
#endif
}

/**
 * @brief   Send a string
 *
 * @note    Could be inline in uart.h
 */

void UART_SendString(USART_TypeDef *uart, char *s) {

    while(*s) UART_SendChar(uart, *s++);

}

/**
 * @brief   Get a char from UART without waiting
 *
 * @note    Does not block. Returns 0 when there is none
 */

unsigned UART_GetCharNoWait(USART_TypeDef *uart) {
int ch=0;

#ifdef USE_UART1
    switch( (uint32_t) uart) {
    case UART0_BASE:
#endif
        if( fifo_empty(inputfifo_0) )
            return 0;
        ENTER_ATOMIC();
        ch = fifo_remove(inputfifo_0);
        EXIT_ATOMIC();
#ifdef USE_UART1
        break;
    case UART1_BASE:
        if( fifo_empty(inputfifo_1) )
            return 0;
        ENTER_ATOMIC();
        ch = fifo_remove(inputfifo_1);
        EXIT_ATOMIC();
        break;
    }
#endif
    return ch;
}

/**
 * @brief   Get a char from UART
 *
 * @note    Does block!!!!!
 */

unsigned UART_GetChar(USART_TypeDef *uart) {
int ch=0;

#ifdef USE_UART1
    switch( (uint32_t) uart) {
    case UART0_BASE:
#endif
        while( fifo_empty(inputfifo_0) ) {}
        ENTER_ATOMIC();
        ch = fifo_remove(inputfifo_0);
        EXIT_ATOMIC();
#ifdef USE_UART1
        break;
    case UART1_BASE:
        while( fifo_empty(inputfifo_1) ) {}
        ENTER_ATOMIC();
        ch = fifo_remove(inputfifo_1);
        EXIT_ATOMIC();
        break;
    }
#endif
    return ch;
}

/**
 * @brief   Disable interrupts
 *
 */

void UART_EnableInterrupts(USART_TypeDef *uart, uint32_t m) {

    if( m&UART_TXINT ) uart->IEN |= UART_IEN_TXC;
    if( m&UART_RXINT ) uart->IEN |= UART_IEN_RXDATAV;
}

/**
 * @brief   Disable interrupts
 *
 */

void UART_DisableInterrupts(USART_TypeDef *uart, uint32_t m) {

    if( m&UART_TXINT ) uart->IEN &= ~UART_IEN_TXC;
    if( m&UART_RXINT ) uart->IEN &= ~UART_IEN_RXDATAV;
}

/**
 * @brief   Output char using polling
 *
 * @note    Does block!!!!!
 * @note    Trasmitter interrupts must be disabled
 */

void UART_PutCharPolling(USART_TypeDef *uart, char ch) {

    while ( (uart->STATUS&UART_STATUS_TXBL) == 0 ) {}
    uart->TXDATA = ch;

}


/**
 * @brief   Flush fifos
 *
 * @note    Does block!!!!!
 */

int UART_Flush(USART_TypeDef *uart) {
int cnt;
int ch;

#ifdef USE_UART1
    switch( (uint32_t) uart) {
    case UART0_BASE:
#endif
        // Clear input fifo
        fifo_clear(inputfifo_0);
#ifdef USE_UART1
        break;
    case UART1_BASE:
        // Clear input fifo
        fifo_clear(inputfifo_1);
        break;
    }
#endif

    // Clear output fifo
    cnt = 0;
    // Disable interrupts
    uart->IEN &= ~(UART_IEN_TXC|UART_IEN_RXDATAV);

#ifdef USE_UART1
    switch( (uint32_t) uart) {
    case UART0_BASE:
#endif
        while( ! fifo_empty(outputfifo_0) ) {
            // Wait until UART TX is free
            ch = fifo_remove(outputfifo_0);
            UART_PutCharPolling(UART0,ch);
            cnt++;
        }
#ifdef USE_UART1
        break;
    case UART1_BASE:
        while( ! fifo_empty(outputfifo_1) ) {
            // Wait until UART TX is free
            ch = fifo_remove(outputfifo_1);
            UART_PutCharPolling(UART1,ch);
            cnt++;
        }
        break;
    }
#endif
    // Reenable interrupts
    uart->IEN |= UART_IEN_TXC|UART_IEN_RXDATAV;
    return cnt;
}

