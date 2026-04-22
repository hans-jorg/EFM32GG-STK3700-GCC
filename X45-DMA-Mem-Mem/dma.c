/**
 * @file    dma.c
 *
 * @brief   Controls the DMA device (ARM )PL230 uDMA)
 *
 * @note    A uniform channel number is used. Primary channels are numbered 0 
 *          to 15 and alternate channels 16 to 31. Both are limited by the 
 *          actual number of channels available.
 *
 * @note    The EFM32GG uses a PL230 uDMA controller, developed and licensed 
 *          by ARM. It support 12 primary channels and 12 alternate channels.
 *
 *          A smaller number of channels can be configured reducing the demand 
 *          of RAM by setting the following compilation symbols
 *          DMA_NUMPRIMARYCHANNELS:   Must be at most 12 for the EFM32GG
 *          DMA_NUMALTERNATECHANNELS: Can be set to zero if no alternate 
 *                                    channels are used.
 *                                    Otherwise limited to 12 for the EFM32GG
 *
 * @note    DMA source (SOURCESEL)
 *
 * |   Value    | Mode      | Description                                    |
 * |------------|---------- |------------------------------------------------|
 * |  0b000000  |  NONE     | No source selected                             |
 * |  0b001000  |  ADC0     | Analog to Digital Converter                    |
 * |  0b001010  |  DAC0     | Digital to Analog Converter                    |
 * |  0b001100  |  USART0   | USART 0--                                      |
 * |  0b001101  |  USART1   | USART 1--                                      |
 * |  0b001110  |  USART2   | USART 2--                                      |
 * |  0b010000  |  LEUART0  | Low Energy UART                                |
 * |  0b010001  |  LEUART1  | Low Energy UART                                |
 * |  0b010100  |  I2C0     | I2C                                            |
 * |  0b010101  |  I2C1     | I2C                                            |
 * |  0b011000  |  TIMER0   | Timer                                          |
 * |  0b011001  |  TIMER1   | Timer 1                                        |
 * |  0b011010  |  TIMER2   | Timer                                          |
 * |  0b011011  |  TIMER3   | Timer                                          |
 * |  0b101100  |  UART0    | UART 0                                         |
 * |  0b101101  |  UART1    | UART 1                                         |
 * |  0b110000  |  MSC      | Memory System                                  |
 * |  0b110001  |  AES      | Advanced Encryption Standard Accelerator       |
 * |  0b110010  |  LESENSE  | Low Energy Sensor Interface                    |
 * |  0b110011  |  EBI      | External Bus Interface                         |
 *
 * @note  Signal Selection (SIGSEL)
 *
 *  | SOURCESEL | VALUE  | Register                        |
 *  |-----------|--------|---------------------------------|
 *  | NONE      | 0bxxxx | OFF                             |
 *  | ADC0      | 0b0000 | ADC0SINGLE                      |
 *  | ADC0      | 0b0001 | ADC0SCAN                        |
 *  | DAC0      | 0b0000 | DAC0CH0                         |
 *  | DAC0      | 0b0001 | DAC0CH1                         |
 *  | USART0    | 0b0000 | USART0RXDATAV                   |
 *  | USART0    | 0b0001 | USART0TXBL                      |
 *  | USART0    | 0b0010 | USART0TXEMPTY                   |
 *  | USART1    | 0b0000 | USART1RXDATAV                   |
 *  | USART1    | 0b0001 | USART1TXBL                      |
 *  | USART1    | 0b0010 | USART1TXEMPTY                   |
 *  | USART1    | 0b0011 | USART1RXDATAVRIGHT              |
 *  | USART1    | 0b0100 | USART1TXBLRIGHT                 |
 *  | USART2    | 0b0000 | USART2RXDATAV                   |
 *  | USART2    | 0b0001 | USART2TXBL                      |
 *  | USART2    | 0b0010 | USART2TXEMPTY                   |
 *  | USART2    | 0b0011 | USART2RXDATAVRIGHT              |
 *  | USART2    | 0b0100 | USART2TXBLRIGHT                 |
 *  | LEUART0   | 0b0000 | LEUART0RXDATAV                  |
 *  | LEUART0   | 0b0001 | LEUART0TXBL                     |
 *  | LEUART0   | 0b0010 | LEUART0TXEMPTY                  |
 *  | LEUART1   | 0b0000 | LEUART1RXDATAV                  |
 *  | LEUART1   | 0b0001 | LEUART1TXBL                     |
 *  | LEUART1   | 0b0010 | LEUART1TXEMPTY                  |
 *  | I2C0      | 0b0000 | I2C0RXDATAV                     |
 *  | I2C0      | 00b000 | I2C0TXBL                        |
 *  | I2C1      | 0b0000 | I2C1RXDATAV                     |
 *  | I2C1      | 0b0001 | I2C1TXBL                        |
 *  | TIMER0    | 0b0000 | TIMER0UFOF                      |
 *  | TIMER0    | 0b0001 | TIMER0CC0                       |
 *  | TIMER0    | 0b0010 | TIMER0CC1                       |
 *  | TIMER0    | 0b0011 | TIMER0CC2                       |
 *  | TIMER1    | 0b0000 | TIMER1UFOF                      |
 *  | TIMER1    | 0b0001 | TIMER1CC0                       |
 *  | TIMER1    | 0b0010 | TIMER1CC1                       |
 *  | TIMER1    | 0b0011 | TIMER1CC2                       |
 *  | TIMER2    | 0b0000 | TIMER2UFOF                      |
 *  | TIMER2    | 0b0001 | TIMER2CC0                       |
 *  | TIMER2    | 0b0010 | TIMER2CC1                       |
 *  | TIMER2    | 0b0011 | TIMER2CC2                       |
 *  | TIMER3    | 0b0000 | TIMER3UFOF                      |
 *  | TIMER3    | 0b0001 | TIMER3CC0                       |
 *  | TIMER3    | 0b0010 | TIMER3CC1                       |
 *  | TIMER3    | 0b0011 | TIMER3CC2                       |
 *  | UART0     | 0b0000 | UART0RXDATAV                    |
 *  | UART0     | 0b0001 | UART0TXBL                       |
 *  | UART0     | 0b0010 | UART0TXEMPTY                    |
 *  | UART1     | 0b0000 | UART1RXDATAV                    |
 *  | UART1     | 0b0001 | UART1TXBL                       |
 *  | UART1     | 0b0010 | UART1TXEMPTY                    |
 *  | MSC       | 0b0000 | MSCWDATA                        |
 *  | AES       | 0b0000 | AESDATAWR                       |
 *  | AES       | 0b0001 | AESXORDATAWR                    |
 *  | AES       | 0b0010 | AESDATARD                       |
 *  | AES       | 0b0011 | AESKEYWR                        |
 *  | LESENSE   | 0b0000 | LESENSEBUFDATAV                 |
 *  | EBI       | 0b0000 | EBIPXL0EMPTY                    |
 *  | EBI       | 0b0001 | EBIPXL1EMPTY                    |
 *  | EBI       | 0b0010 | EBIPXLFULL                      |
 *  | EBI       | 0b0011 | EBIDDEMPTY                      |
 *
 *
 *
 *
 * @author  Hans
 *
 * @version 1.0
 *
 * @date    16/04/2026
 */

#include "em_device.h"
#include "dma.h"


/**
 *  @brief   Number of channels
 *
 *  @note    Can be set to lower numbers to reduce demand of RAM
 *  @note    The number of alternate channels can be set to 0 to reduce demand of RAM
 *  @note    At most, 12 as defined by DMA_CHAN_COUNT for the EFM32GG
 */
#ifndef DMA_NUMPRIMARYCHANNELS
#define DMA_NUMPRIMARYCHANNELS                 (12)
#endif

#ifndef DMA_NUMALTERNATECHANNELS
#define DMA_NUMALTERNATECHANNELS               (0)
#endif

/**
 *  @brief  DMA Interrupt Priority
 */
#define DMA_PRIO            (4)


/**
 *  @brief  This is a bit mask with the bit set for an existing channel 
 *
 *  @note   It uses DMA_CHAN_COUNT defined in the device header file
 *
 *  @note   This format is used for registers named CH*, IF*
 */
#define CHMASK                                 ((1<<DMA_CHAN_COUNT)-1)

/**
 *  @brief  Managing error signaling
 *
 *  @note   See 8.4.2.4 Error signaling in the Reference Manual
 *
 *  @note   The program running on the host processor must always keep a 
 *          record of which channels have recently asserted their dma_done[ ]
 *          outputs. It must compare the disabled channels list from step 1
 *          (p. 61) , with the record of the channels that
 *          have recently set their dma_done[ ] outputs. The channel with no 
 *          record of dma_done[C] being set is the channel that the ERROR 
 *          occurred on.
 *
 *  @note   These are bit-mapped with channel 0 in the bit 0 (LSB)
 *
 *  @note   The bits in *enabled channels* are set in the StartTransfer and
 *          cleared in the IRQ processing
 *
 *  @note   The bits in *done channels* are set in the IRQ processing and 
 *          cleared in the StartTransfer
 *
 *  @note   The bits in *error_channels* are set in the IRQ processing and
 *          
 */
static uint32_t enabled_channels = 0;
static uint32_t done_channels    = 0;
static uint32_t error_channels   = 0;

/**
 * @brief  Pointer to callback routine
 *
 */

static void (*DMA_CallbackFunction)(uint32_t errors, uint32_t done) = 0;

/*
 *  @brief  Clear Channel state variables 
 */
static inline void ClearChannelState(uint32_t chmask) {

    enabled_channels &= ~(chmask);
    error_channels   &= ~(chmask);
    done_channels    &= ~(chmask);
}

/*
 *  @brief  Clear Channel state variables 
 */
static inline void ClearChannelStateAll(void) {

    enabled_channels = 0;
    error_channels   = 0;
    done_channels    = 0;
}

/*
 *  @brief  Set channel state to start DMA 
 */
static inline void SetChannelState(uint32_t chmask) {

    enabled_channels |= chmask;
    error_channels   &= ~(chmask);
    done_channels    &= ~(chmask);
}

/**
 * @brief Area for DMA Descriptors
 *
 * @note  The number of channels is configurable
 *
 * @note  DMA_NUMPRIMARYCHANNELS must be less or equal to DMA_CHAN_COUNT
 * @note  DMA_NUMALTERNATECHANNELS must be less or equal to DMA_CHAN_COUNT
 *
 * @note  DMA_DESCRIPTOR_TypeDef is defined in efm32gg_dma_descriptor.h
 *        
 */
///@{
#if DMA_NUMALTERNATECHANNELS > 0
static __attribute__((aligned (256)))  DMA_DESCRIPTOR_TypeDef
    DMA_DescriptorTable[16+DMA_NUMALTERNATECHANNELS];
#else
static __attribute__((aligned (256)))  DMA_DESCRIPTOR_TypeDef
    DMA_DescriptorTable[DMA_NUMPRIMARYCHANNELS];
#endif

static inline DMA_DESCRIPTOR_TypeDef *GetPrimaryDescriptor(int ch) {

    return &DMA_DescriptorTable[ch&0xF];
#if 0
    // After initialization,
    return (DMA_DESCRIPTOR_TypeDef *) DMA->DMA_CTRLBASE + (ch&0xF)
#endif
}

static inline DMA_DESCRIPTOR_TypeDef *GetAlternateDescriptor(int ch) {

    return &DMA_DescriptorTable[(ch&0xF)+16];
#if 0
    // After initialization,
    return (DMA_DESCRIPTOR_TypeDef *) DMA->DMA_ALTCTRLBASE + (ch&0xF)
#endif
}
///@}


/**
 *  @brief  Handling of channel numbers
 *
 *  @note   In order to avoid separate functions for primary channels and 
 *          alternative channels a number scheme is used.
 *  @note   Channel numbers from 0 to 15 refer to primary channels
 *          Channel numbers from 16 to 31 refer to alternate channels.
 *
 *  @note   This is exactly then index number used for a channel in the Descriptor Table
 *
 */
///@{
static inline int GetPrimaryChannelNumber(int ch) { return ch&0xF; }
static inline int GetAlternateChannelNumber(int ch) { return ((ch&0xF))|0x10; }
static inline int IsAlternateChannel(int ch) { return ch&0x10; }
static inline int IsPrimaryChannel(int ch)   { return !(ch&0x10); }
static inline unsigned GenChannelMask(int ch) { return (1<<(ch&0xF)); }
///@}




/**
 *  @brief  Interrupt routine for DMA
 *
 * From RM: 
 * >> If the controller detects an ERROR response on the AHB-Lite master 
 * >> interface, it:
 * >>         * disables the channel that corresponds to the ERROR
 * >>         * sets dma_err HIGH.
 * >> After the host processor detects that dma_err is HIGH, it must check which 
 * >> channel was active when the ERROR occurred. It can do this by:
 * >> 1. Reading the DMA_CHENS register to create a list of disabled channels.
 * >>    When a channel asserts dma_done[ ] then the controller disables the 
 * >>    channel. The program running on the host processor must always keep a 
 * >>    record of which channels have recently asserted their 
 * >>    dma_done[ ] outputs.
 * >> 2. It must compare the disabled channels list from step 1 (p. 61) , with 
 * >>    the record of the channels that have recently set their 
 * >>    dma_done[ ] outputs. The channel with no record of dma_done[C] being
 * >>    set is the channel that the ERROR occurred on.
 * >> 
 */

void DMA_IRQHandler(void) {


    uint32_t intflags = DMA->IF;            // Get all done channels
    uint32_t done = intflags&CHMASK;        // Only the done flags
    uint32_t nowenabled = DMA->CHENS;       // Get all channels that are active NOW
    
    if( (intflags&DMA_IF_ERR) != 0 ) {
        // Clear Error register
        DMA->ERRORC = DMA_ERRORC_ERRORC;
        // What else?
        // What happens to the ongoing operations?
    }
    
    /*
     * When a channel is enabled but it is not enabled any longer and
     * it is not done
     */
    error_channels = (enabled_channels&~nowenabled)&~done;
    
    /*
     * Store done information
     */
    done_channels |= done;
    
    /*
     * Clear interrupt and update enabled channels info
     */
    DMA->IFC = done;            // reset all done flags
    enabled_channels &= ~done;  // mirror it

    /*
     * Callback function
     * Should there be one for each channel
     */
    if( DMA_CallbackFunction )
        DMA_CallbackFunction(error_channels,done_channels);

}




/**
 * @brief  Disable IRQ for the  DMA device
 *
 */

void DMA_DisableIRQ(void) {

    NVIC_DisableIRQ(DMA_IRQn);
    NVIC_ClearPendingIRQ(DMA_IRQn);

    return;
}


/**
 * @brief  Enable Interrupt from DMA
 *
 */

void DMA_EnableIRQ(void) {

    NVIC_ClearPendingIRQ(DMA_IRQn);
    NVIC_SetPriority(DMA_IRQn, DMA_PRIO);
    NVIC_EnableIRQ(DMA_IRQn);

    return;
}


/**
 *  @brief  DMA_DeInit
 *
 *  @note   Reset the DMA device to default and restore the DMA to reset state
 *
 *  @note  It does disable DMA_CLK!!!!!
 */

void DMA_DeInit(void) {

    DMA_Reset();
    
    CMU->HFCORECLKEN0 &= ~CMU_HFCORECLKEN0_DMA;
    CMU->HFPERCLKDIV  &= ~CMU_HFPERCLKDIV_HFPERCLKEN;

}


/**
 *  @brief  DMA_Reset
 *
 *  @note   Reset the DMA device to default
 *
 *  @note  It does not disable DMA_CLK!!!!!
 */

void DMA_Reset(void) {

    // Disable device
    DMA->CONFIG = _DMA_CONFIG_RESETVALUE;

    // Clear interrupts
    DMA_DisableIRQ();
    DMA->IEN = _DMA_IEN_RESETVALUE;
    DMA->IFC = _DMA_IFC_MASK;

    // Clear  Enable and Select registers
    DMA->CHENC  = _DMA_CHENC_MASK;
    DMA->CHALTC = _DMA_CHALTC_MASK;
    DMA->CHPRIC = _DMA_CHPRIC_MASK;

    // Clear Burst and Request registers
    DMA->CHUSEBURSTC = _DMA_CHUSEBURSTC_MASK;
    DMA->CHREQMASKC  = _DMA_CHREQMASKC_MASK;

    // Clear Error register
    DMA->ERRORC = DMA_ERRORC_ERRORC;

    // Clear channel control registers
    for(int ch=0;ch<DMA_CHAN_COUNT;ch++) {
        DMA->CH[ch].CTRL = _DMA_CH_CTRL_RESETVALUE;
    }

    // State channel variable are used to detect error
    ClearChannelStateAll();

    return;
}

/**
 * @brief DMA_Init
 *
 * @note  Initialize the DMA device
 */
int DMA_Init(void) {

    // Enable clock for DMA
    CMU->HFPERCLKDIV  |= CMU_HFPERCLKDIV_HFPERCLKEN;
    CMU->HFCORECLKEN0 |= CMU_HFCORECLKEN0_DMA;

    // Set device to default
    DMA_Reset();

    // Disable DMA IRQ
    DMA_DisableIRQ();

    // Initialize DMA Descriptor. Most important is the CTRL word, specifically
    // the cycle field

#if DMA_NUMALTERNATECHANNELS > 0
const int indexmax = 16+DMA_NUMALTERNATECHANNELS;
#else
const int indexmax = DMA_NUMPRIMARYCHANNELS;
#endif

    for(int index=0; index<indexmax;index++) {
        DMA_DescriptorTable[index].SRCEND = 0;
        DMA_DescriptorTable[index].DSTEND = 0;
        DMA_DescriptorTable[index].CTRL   = 0;
        DMA_DescriptorTable[index].USER   = 0;
    }

    DMA->CTRLBASE = (uint32_t) DMA_DescriptorTable;

    // Enable device
    DMA->CONFIG |= DMA_CONFIG_EN;
    
    // Clear channel state variables (already done in DMA_Reset) 
    ClearChannelStateAll();
}

/**
 * @brief  Enable Channel with number *ch*
 *
 * @note   Enable the channel effectivally starting the DMA transfer
 */

void DMA_EnableChannel(unsigned ch) {
uint32_t chmask = (1UL<<ch);

    DMA->CHENS = chmask;
    SetChannelState(chmask);
    
    return;
}  

/**
 * @brief  Enable Channels according a bit mask
 *
 * @note   Long description of function
 *
 */

void DMA_EnableChannels(unsigned chmask) {

    DMA->CHENS = chmask;
    SetChannelState(chmask);
    
    return;
}

/**
 * @brief  Disable Channel with number *ch*
 *
 * @note   According 8.2.4, *he program running on the host processor 
 *         must always keep a record of which channels have recently asserted
 *         their dma_done[ ] outputs.*
 */

void DMA_DisableChannel(unsigned ch) {
uint32_t chmask = (1UL<<ch);

    DMA->CHENC = chmask;
    ClearChannelState(chmask);
    
    return;
}

/**
 * @brief  Disable Channels corresponding to 1s in the chmask
 *
 * @note   According 8.2.4, *he program running
 *         on the host processor must always keep a record of which channels
 *         have recently asserted their dma_done[ ] outputs.*
 */

void DMA_DisableChannels(unsigned chmask) {

    DMA->CHENC = chmask;
    ClearChannelState(chmask);
    
    return;
}


/**
 * @brief  Select Alternate Channel with number n

 * @note   Long description of function
 *
 */

void DMA_SelectAlternateDescriptor(int ch) {

    int pch = GetPrimaryChannelNumber(ch);

    DMA->CHENC = (1<<pch);
    return;
}

/**
 * @brief  Deselect Alternate Channel with number n

 * @note   Long description of function
 *
 */

void DMA_DeselectAlternateDescriptor(int ch) {

    int pch = GetPrimaryChannelNumber(ch);

    DMA->CHALTC = (1<<pch);
    return;
}



/**
 * @brief  Get State of DMA device

 * @note   Get the state of the DMA device.
 *
 */

uint32_t DMA_GetDeviceState(void) {

    return (DMA->STATUS&_DMA_STATUS_STATE_MASK)>>_DMA_STATUS_STATE_SHIFT;

}


/**
 * @brief  Get Transfer Status
 *
 * @note   Get the status of the transfer in a specified channel
 *
 * @note   The Cycle field is set to 0 by the DMA, after completing the transfer
 *
 *
 */

uint32_t DMA_GetTransferStatus(int ch) {
DMA_DESCRIPTOR_TypeDef *pDesc = &DMA_DescriptorTable[ch];

    return (pDesc->CTRL&_DMA_CTRL_CYCLE_CTRL_MASK)>>_DMA_CTRL_CYCLE_CTRL_SHIFT;

}


/**
 * @brief  Configure Channel for Memory to Memory transfer for 8-bit data

 * @note   Long description of function
 *
 */

int DMA_SetupTransferMemToMem_8(int ch, uint8_t *src, uint8_t *dst, uint32_t num) {

    int rc = DMA_ConfigureDescriptor(ch, 8, src, 1, dst, 1, num);

    return rc;
}


/**
 * @brief  Configure Channel for Memory to Memory transfer for 16-bit data

 * @note   Long description of function
 *
 */
int DMA_SetupTransferMemToMem_16(int ch, uint16_t *src, uint16_t *dst, uint32_t num) {

    int rc = DMA_ConfigureDescriptor(ch, 16, src, 2, dst, 2, num);

    return rc;
}



/**
 * @brief  Configure Channel for Memory to Memory transfer for 32-bit data

 * @note   Long description of function
 *
 */
int DMA_SetupTransferMemToMem_32(int ch, uint32_t *src, uint32_t *dst, uint32_t num) {

    int rc = DMA_ConfigureDescriptor(ch, 32, src, 4, dst, 4, num);

    return rc;
}

/**
 * @brief  Configure Descriptor
 *
 *
 */
int DMA_StartTransfer(int ch) {
DMA_DESCRIPTOR_TypeDef *pDesc = &DMA_DescriptorTable[ch];

// For now only auto
const uint32_t cycle = DMA_CTRL_CYCLE_CTRL_AUTO;

    int pch = GetPrimaryChannelNumber(ch);
    uint32_t chmask = (1<<pch);

    if( (pDesc->CTRL&_DMA_CTRL_CYCLE_CTRL_MASK) != DMA_CTRL_CYCLE_CTRL_INVALID )
        return -1; // Already started

    pDesc->CTRL = (pDesc->CTRL&~_DMA_CTRL_CYCLE_CTRL_MASK)|cycle;

    // Enable channel if it not yet enabled

    if ( IsPrimaryChannel(ch) ) {
        DMA->CHENS = chmask;
    } else {
        DMA->CHALTS = chmask;
    }
    
    // Only for Auto
    DMA->CHSWREQ = chmask;
    
    // Clear state variables
    ClearChannelState(chmask);
    
    return 0;
}
/**
 * @brief  Stop transfer
 *
 */
int DMA_StopTransfer(int ch) {
DMA_DESCRIPTOR_TypeDef *pDesc = &DMA_DescriptorTable[ch];
const uint32_t cycle = DMA_CTRL_CYCLE_CTRL_AUTO;



    if( (pDesc->CTRL&_DMA_CTRL_CYCLE_CTRL_MASK) != DMA_CTRL_CYCLE_CTRL_INVALID ) {
        pDesc->CTRL = (pDesc->CTRL&~_DMA_CTRL_CYCLE_CTRL_MASK)|DMA_CTRL_CYCLE_CTRL_INVALID;
    }

    uint32_t chmask = (1UL<<ch);
    // Enable channel if it not yet enabled
    if ( IsPrimaryChannel(ch) ) {
        chmask = (1UL<<ch);
        DMA->CHENC = chmask;
    } else {
        int ach = GetPrimaryChannelNumber(ch);
        chmask = (1UL<<ch);
        DMA->CHALTC = chmask;
    }

    // Clear state variables
    ClearChannelState(chmask);

    return 0;
}

/**
 * @brief  General routine to configure descriptor
 *
 */

int  DMA_ConfigureDescriptor(int ch, uint32_t size, void *src, uint32_t srcinc, void *dst, uint32_t dstinc, uint32_t number) {
uint32_t control;
uint32_t sc;


    int ich = GetPrimaryChannelNumber(ch);

    // Get size code, that is used in the control element. Size in bytes is 1<<s;
    if( size < 3 ) {
        sc = size;
    } else if ( size == 8 )
        sc = 0x0;
    else if ( size == 16 )
        sc = 0x1;
    else if ( size == 32 )
        sc = 0x2;
    else
        return -1; // Invalid size

    unsigned nm1 = number - 1;

    // Test if it is too big
    if( nm1 > (_DMA_CTRL_N_MINUS_1_MASK>>_DMA_CTRL_N_MINUS_1_SHIFT) ) {
        return -2;
    }

    unsigned rpower = _DMA_CTRL_R_POWER_4; // Default!!!!

    // Calculate end addresses
    uint8_t *srcend = (uint8_t *)src + nm1*size;
    uint8_t *dstend = (uint8_t *)dst + nm1*size;

    uint32_t ctrl =  ((dstinc<<_DMA_CTRL_DST_INC_SHIFT)&_DMA_CTRL_DST_INC_MASK)
                    |((size<<_DMA_CTRL_DST_SIZE_SHIFT)&_DMA_CTRL_DST_SIZE_MASK)
                    |((srcinc<<_DMA_CTRL_SRC_INC_SHIFT)&_DMA_CTRL_SRC_INC_MASK)
                    |((rpower<<_DMA_CTRL_R_POWER_SHIFT)&_DMA_CTRL_R_POWER_MASK)
                    |((nm1<<_DMA_CTRL_N_MINUS_1_SHIFT)&_DMA_CTRL_N_MINUS_1_MASK);

    // Configure DMA Descriptor
    DMA_DESCRIPTOR_TypeDef *pDesc = &DMA_DescriptorTable[ich];
    pDesc->SRCEND = srcend;
    pDesc->DSTEND = dstend;
    pDesc->CTRL   = ctrl;

    // For now, only Memory to Memory
    // TODO: Other sources
    DMA->CH[ich].CTRL = DMA_CH_CTRL_SOURCESEL_NONE; // DMA_CH_CTRL_SIGSEL_source


    return 0;
}
