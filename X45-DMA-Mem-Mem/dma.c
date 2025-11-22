/**
 * @file    dma.c
 *
 * @brief   Controls the DMA device (ARM )PL230 uDMA)
 *
 * @brief   A uniform channel number is used. Primary channels are numbered 0 to 15 and
 *          alternate channels 16 to 31. Both are limited by the actual number of channels
 *          available. A smaller number of channels can be configured reducing the demand of RAM
 *          DMA_NUMPRIMARYCHANNELS:   Must be at most 12 for the EFM32GG
 *          DMA_NUMALTERNATECHANNELS: Can be set to zero if no alternate channels are used.
 *                                    Otherwise limited to 12 for the EFM32GG
 *
 * @note    DMA source (SOURCESEL)
 *
 * |   Value    | Mode      | Description                                                         |
 * |------------|---------- |                                                                     |
 * |  0b000000  |  NONE     | No source selected                                                  |
 * |  0b001000  |  ADC0     | Analog to Digital Converter 0                                       |
 * |  0b001010  |  DAC0     | Digital to Analog Converter 0                                       |
 * |  0b001100  |  USART0   | Universal Synchronous/Asynchronous Receiver/Transmitter 0--         |
 * |  0b001101  |  USART1   | Universal Synchronous/Asynchronous Receiver/Transmitter 1--         |
 * |  0b001110  |  USART2   | Universal Synchronous/Asynchronous Receiver/Transmitter 2--         |
 * |  0b010000  |  LEUART0  | Low Energy UART 0                                                   |
 * |  0b010001  |  LEUART1  | Low Energy UART 1                                                   |
 * |  0b010100  |  I2C0     | I2C 0                                                               |
 * |  0b010101  |  I2C1     | I2C 1                                                               |
 * |  0b011000  |  TIMER0   | Timer 0                                                             |
 * |  0b011001  |  TIMER1   | Timer 1                                                             |
 * |  0b011010  |  TIMER2   | Timer 2                                                             |
 * |  0b011011  |  TIMER3   | Timer 3                                                             |
 * |  0b101100  |  UART0    | Universal Asynchronous Receiver/Transmitter 0                       |
 * |  0b101101  |  UART1    | Universal Asynchronous Receiver/Transmitter 1                       |
 * |  0b110000  |  MSC      | Memory System Controller                                            |
 * |  0b110001  |  AES      | Advanced Encryption Standard Accelerator                            |
 * |  0b110010  |  LESENSE  | Low Energy Sensor Interface                                         |
 * |  0b110011  |  EBI      | External Bus Interface                                              |
 *
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
 * @author  <author>
 *
 * @version <version>
 *
 * @date    XX/XX/2020
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
#define DMA_NUMPRIMARYCHANNELS                 (12)
#define DMA_NUMALTERNATECHANNELS               (12)


#define CHMASK                                 ((1<<DMA_CHAN_COUNT)-1)
/**
 *  @brief  Source Selection Encoding
 */
///@{
#define DMA_SRC_NONE                           _DMA_CH_CTRL_SOURCESEL_NONE
#define DMA_SRC_ADC0                           _DMA_CH_CTRL_SOURCESEL_ADC0
#define DMA_SRC_DAC0                           _DMA_CH_CTRL_SOURCESEL_DAC0
#define DMA_SRC_USART0                         _DMA_CH_CTRL_SOURCESEL_USART0
#define DMA_SRC_USART1                         _DMA_CH_CTRL_SOURCESEL_USART1
#define DMA_SRC_USART2                         _DMA_CH_CTRL_SOURCESEL_USART2
#define DMA_SRC_LEUART0                        _DMA_CH_CTRL_SOURCESEL_LEUART0
#define DMA_SRC_LEUART1                        _DMA_CH_CTRL_SOURCESEL_LEUART1
#define DMA_SRC_I2C0                           _DMA_CH_CTRL_SOURCESEL_I2C0
#define DMA_SRC_I2C1                           _DMA_CH_CTRL_SOURCESEL_I2C1
#define DMA_SRC_TIMER0                         _DMA_CH_CTRL_SOURCESEL_TIMER0
#define DMA_SRC_TIMER1                         _DMA_CH_CTRL_SOURCESEL_TIMER1
#define DMA_SRC_TIMER2                         _DMA_CH_CTRL_SOURCESEL_TIMER2
#define DMA_SRC_TIMER3                         _DMA_CH_CTRL_SOURCESEL_TIMER3
#define DMA_SRC_UART0                          _DMA_CH_CTRL_SOURCESEL_UART0
#define DMA_SRC_UART1                          _DMA_CH_CTRL_SOURCESEL_UART1
#define DMA_SRC_MSC                            _DMA_CH_CTRL_SOURCESEL_MSC
#define DMA_SRC_AES                            _DMA_CH_CTRL_SOURCESEL_AES
#define DMA_SRC_LESENSE                        _DMA_CH_CTRL_SOURCESEL_LESENSE
#define DMA_SRC_EBI                            _DMA_CH_CTRL_SOURCESEL_EBI
///@}



/**
 *  @brief  Source Selection Encoding
 */
///@{
#define  DMA_SIG_SRC_OFF                       (0U)
#define  DMA_SIG_SRC_ADC0SINGLE                _DMA_CH_CTRL_SIGSEL_ADC0SINGLE
#define  DMA_SIG_SRC_ADC0SCAN                  _DMA_CH_CTRL_SIGSEL_ADC0SCAN

#define  DMA_SIG_SRC_DAC0CH0                   _DMA_CH_CTRL_SIGSEL_DAC0CH0
#define  DMA_SIG_SRC_DAC0CH1                   _DMA_CH_CTRL_SIGSEL_DAC0CH1

#define  DMA_SIG_SRC_USART0RXDATAV             _DMA_CH_CTRL_SIGSEL_USART0RXDATAV
#define  DMA_SIG_SRC_USART0TXBL                _DMA_CH_CTRL_SIGSEL_USART0TXBL
#define  DMA_SIG_SRC_USART0TXEMPTY             _DMA_CH_CTRL_SIGSEL_USART0TXEMPTY

#define  DMA_SIG_SRC_USART1RXDATAV             _DMA_CH_CTRL_SIGSEL_USART1RXDATAV
#define  DMA_SIG_SRC_USART1TXBL                _DMA_CH_CTRL_SIGSEL_USART1TXBL
#define  DMA_SIG_SRC_USART1TXEMPTY             _DMA_CH_CTRL_SIGSEL_USART1TXEMPTY
#define  DMA_SIG_SRC_USART1RXDATAVRIGHT        _DMA_CH_CTRL_SIGSEL_USART1RXDATAVRIGHT
#define  DMA_SIG_SRC_USART1TXBLRIGHT           _DMA_CH_CTRL_SIGSEL_USART1TXBLRIGHT

#define  DMA_SIG_SRC_USART2RXDATAV             _DMA_CH_CTRL_SIGSEL_USART2RXDATAV
#define  DMA_SIG_SRC_USART2TXBL                _DMA_CH_CTRL_SIGSEL_USART2TXBL
#define  DMA_SIG_SRC_USART2TXEMPTY             _DMA_CH_CTRL_SIGSEL_USART2TXEMPTY
#define  DMA_SIG_SRC_USART2RXDATAVRIGHT        _DMA_CH_CTRL_SIGSEL_USART2RXDATAVRIGHT
#define  DMA_SIG_SRC_USART2TXBLRIGHT           _DMA_CH_CTRL_SIGSEL_USART2TXBLRIGHT

#define  DMA_SIG_SRC_LEUART0RXDATAV            _DMA_CH_CTRL_SIGSEL_LEUART0RXDATAV
#define  DMA_SIG_SRC_LEUART0TXBL               _DMA_CH_CTRL_SIGSEL_LEUART0TXBL
#define  DMA_SIG_SRC_LEUART0TXEMPTY            _DMA_CH_CTRL_SIGSEL_LEUART0TXEMPTY

#define  DMA_SIG_SRC_LEUART1RXDATAV            _DMA_CH_CTRL_SIGSEL_LEUART1RXDATAV
#define  DMA_SIG_SRC_LEUART1TXBL               _DMA_CH_CTRL_SIGSEL_LEUART1TXBL
#define  DMA_SIG_SRC_LEUART1TXEMPTY            _DMA_CH_CTRL_SIGSEL_LEUART1TXEMPTY

#define  DMA_SIG_SRC_I2C0RXDATAV               _DMA_CH_CTRL_SIGSEL_I2C0RXDATAV
#define  DMA_SIG_SRC_I2C0TXBL                  _DMA_CH_CTRL_SIGSEL_I2C0TXBL
#define  DMA_SIG_SRC_I2C1RXDATAV               _DMA_CH_CTRL_SIGSEL_I2C1RXDATAV
#define  DMA_SIG_SRC_I2C1TXBL                  _DMA_CH_CTRL_SIGSEL_I2C1TXBL

#define  DMA_SIG_SRC_TIMER0UFOF                _DMA_CH_CTRL_SIGSEL_TIMER0UFOF
#define  DMA_SIG_SRC_TIMER0CC0                 _DMA_CH_CTRL_SIGSEL_TIMER0CC0
#define  DMA_SIG_SRC_TIMER0CC1                 _DMA_CH_CTRL_SIGSEL_TIMER0CC1
#define  DMA_SIG_SRC_TIMER0CC2                 _DMA_CH_CTRL_SIGSEL_TIMER0CC2

#define  DMA_SIG_SRC_TIMER1UFOF                _DMA_CH_CTRL_SIGSEL_TIMER1UFOF
#define  DMA_SIG_SRC_TIMER1CC0                 _DMA_CH_CTRL_SIGSEL_TIMER1CC0
#define  DMA_SIG_SRC_TIMER1CC1                 _DMA_CH_CTRL_SIGSEL_TIMER1CC1
#define  DMA_SIG_SRC_TIMER1CC2                 _DMA_CH_CTRL_SIGSEL_TIMER1CC2

#define  DMA_SIG_SRC_TIMER2UFOF                _DMA_CH_CTRL_SIGSEL_TIMER2UFOF
#define  DMA_SIG_SRC_TIMER2CC0                 _DMA_CH_CTRL_SIGSEL_TIMER2CC0
#define  DMA_SIG_SRC_TIMER2CC1                 _DMA_CH_CTRL_SIGSEL_TIMER2CC1
#define  DMA_SIG_SRC_TIMER2CC2                 _DMA_CH_CTRL_SIGSEL_TIMER2CC2

#define  DMA_SIG_SRC_TIMER3UFOF                _DMA_CH_CTRL_SIGSEL_TIMER3UFOF
#define  DMA_SIG_SRC_TIMER3CC0                 _DMA_CH_CTRL_SIGSEL_TIMER3CC0
#define  DMA_SIG_SRC_TIMER3CC1                 _DMA_CH_CTRL_SIGSEL_TIMER3CC1
#define  DMA_SIG_SRC_TIMER3CC2                 _DMA_CH_CTRL_SIGSEL_TIMER3CC2

#define  DMA_SIG_SRC_UART0RXDATAV              _DMA_CH_CTRL_SIGSEL_UART0RXDATAV
#define  DMA_SIG_SRC_UART0TXBL                 _DMA_CH_CTRL_SIGSEL_UART0TXBL
#define  DMA_SIG_SRC_UART0TXEMPTY              _DMA_CH_CTRL_SIGSEL_UART0TXEMPTY

#define  DMA_SIG_SRC_UART1RXDATAV              _DMA_CH_CTRL_SIGSEL_UART1RXDATAV
#define  DMA_SIG_SRC_UART1TXBL                 _DMA_CH_CTRL_SIGSEL_UART1TXBL
#define  DMA_SIG_SRC_UART1TXEMPTY              _DMA_CH_CTRL_SIGSEL_UART1TXEMPTY

#define  DMA_SIG_SRC_MSCWDATA                  _DMA_CH_CTRL_SIGSEL_MSCWDATA

#define  DMA_SIG_SRC_AESDATAWR                 _DMA_CH_CTRL_SIGSEL_AESDATAWR
#define  DMA_SIG_SRC_AESXORDATAWR              _DMA_CH_CTRL_SIGSEL_AESXORDATAWR
#define  DMA_SIG_SRC_AESDATARD                 _DMA_CH_CTRL_SIGSEL_AESDATARD
#define  DMA_SIG_SRC_AESKEYWR                  _DMA_CH_CTRL_SIGSEL_AESKEYWR

#define  DMA_SIG_SRC_LESENSEBUFDATAV           _DMA_CH_CTRL_SIGSEL_LESENSEBUFDATAV

#define  DMA_SIG_SRC_EBIPXL0EMPTY              _DMA_CH_CTRL_SIGSEL_EBIPXL0EMPTY
#define  DMA_SIG_SRC_EBI_EBIPXL1EMPTY          _DMA_CH_CTRL_SIGSEL_EBIPXL1EMPTY
#define  DMA_SIG_SRC_EBI_EBIPXLFULL            _DMA_CH_CTRL_SIGSEL_EBIPXLFULL
#define  DMA_SIG_SRC_EBI_EBIDDEMPTY            _DMA_CH_CTRL_SIGSEL_EBIDDEMPTY
///@}


/**
 *  @brief  DMA descriptor
 *
 *  @note   Layout described in efm32gg_dma_descriptor.h
 *  @note   Field in ctrl described int efm32gg_dmactrl.h
 */

/**
 * @brief Area for DMA Descriptors
 *
 * @note  The number of channels is configurable
 *
 * @note  DMA_NUMPRIMARYCHANNELS must be less or equal to DMA_CHAN_COUNT
 *
 * @note  DMA_NUMALTERNATECHANNELS must be less or equal to DMA_CHAN_COUNT
 */
///@{
#if DMA_NUMALTERNATECHANNELS > 0
static DMA_DESCRIPTOR_TypeDef
    DMA_DescriptorTable[16+DMA_NUMALTERNATECHANNELS] __attribute__((aligned (256)));
#else
static DMA_DESCRIPTOR_TypeDef
    DMA_DescriptorTable[DMA_NUMPRIMARYCHANNELS]      __attribute__((aligned (256)));
#endif

static inline DMA_DESCRIPTOR_TypeDef *GetPrimaryDescriptor(int ch) {

    return &DMA_DescriptorTable[ch&0xF];
#if 0
    // After initialization,
    return (DMA_DESCRIPTOR_TypeDef *) DMA->DMA_CTRLBASE + (ch&0xF)
#endif
}

static inline DMA_DESCRIPTOR_TypeDef *GetAlternateDescriptor(int ch) {

    return &DMA_DescriptorTable[ch&0xF+0x10];
#if 0
    // After initialization,
    return (DMA_DESCRIPTOR_TypeDef *) DMA->DMA_ALTCTRLBASE + (ch&0xF)
#endif
}


///@}


/**
 *  @brief  Handling of channel numbers
 *
 *  @note   In order to avoid separate functions for primary channels and alternative channels
 *          a number scheme is used.
 *  @note   Channel numbers from 0 to 15 refer to primary channels
 *          Channel numbers from 16 to 31 refer to alternate channels.
 *
 *  @note   This is exactly then index number used for a channel in the Descriptor Table
 *
 */
///@{
static inline int GetPrimaryChannelNumber(int ch) { return ch&0xF; }
static inline int GetAlternateChannelNumber(int ch) { return (ch&0xF)|0x10; }
static inline int IsAlternateChannel(int ch) { return ch&0x10; }
static inline int IsPrimaryChannel(int ch)   { return !(ch&0x10); }
static inline unsigned GenChannelMask(int ch) { return (1<<(ch&0xF)); }
///@}


/**
 *  @brief  Managing error signaling
 *
 *  @note   The program running on the host processor must always keep a record of which channels
 *          have recently asserted their dma_done[ ] outputs. It must compare the disabled channels
 *          list from step 1 (p. 61) , with the record of the channels that
 *          have recently set their dma_done[ ] outputs. The channel with no record of dma_done[C]
 *          being set is the channel that the ERROR occurred on.
 */
static uint32_t enabled_channels = 0;

/**
 * @brief  Pointer to callback routine
 *
 */

static void (*DMA_CallbackFunction)(uint32_t errors, uint32_t done) = 0;

/**
 * @brief  Interrupt routine for DMA
 *
 */

void DMA_IRQHandler(void) {

    uint32_t errors = 0;
    uint32_t iflags = DMA->IF;
    if( (iflags&DMA_IF_ERR) != 0 ) {
        errors = ~(DMA->CHENS)&enabled_channels;
    }
    uint32_t done = iflags&CHMASK;

    if( DMA_CallbackFunction ) DMA_CallbackFunction(errors,done);

}




/**
 * @brief  Short description of function
 *
 * @note   Long description of function
 *
 * @param  Description of parameter
 *
 * @return Description of return parameters
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
    NVIC_EnableIRQ(DMA_IRQn);

    return;
}


/**
 * @brief  DMA_Reset
 *
 * @param  Reset the DMA device to default
 *
 */

void DMA_Reset(void) {

    // Disable device
    DMA->CONFIG = _DMA_CONFIG_RESETVALUE;

    // Clear interrupts
    DMA_DisableIRQ();
    DMA->IEN = _DMA_IEN_RESETVALUE;
    DMA->IFC = _DMA_IFC_MASK;

    // Clear  Enable and Select rregisters
    DMA->CHENC  = _DMA_CHENC_MASK;
    DMA->CHALTC = _DMA_CHALTC_MASK;
    DMA->CHPRIC = _DMA_CHPRIC_MASK;

    // Clear Burst and Request registers
    DMA->CHUSEBURSTC = _DMA_CHUSEBURSTC_MASK;
    DMA->CHREQMASKC  = _DMA_CHREQMASKC_MASK;

    // Clear Error register
    DMA->ERRORC = DMA_ERRORC_ERRORC;

    // Clear channel contgrol registers
    for(int ch=0;ch<DMA_CHAN_COUNT;ch++) {
        DMA->CH[ch].CTRL = _DMA_CH_CTRL_RESETVALUE;
    }

    // Recording which channels are enabled
    enabled_channels = 0;

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
}


/**
 * @brief  Enable Channel with number n

 * @note   Long description of function
 *
 */

void DMA_EnableChannel(unsigned chmask) {

    DMA->CHENS = chmask;
    enabled_channels |= chmask;
    return;
}


/**
 * @brief  Disable Channels corresponding to 1 in the chmask
 *
 * @note   According 8.2.4, *he program running
 *         on the host processor must always keep a record of which channels have recently asserted
 *         their dma_done[ ] outputs.*
 *
 */

void DMA_DisableChannels(unsigned chmask) {

    DMA->CHENC = chmask;
    enabled_channels &= ~(chmask);
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
 * @note   It can be
 *
 */

unsigned DMA_GetDeviceState(void) {

    return (DMA->STATUS&_DMA_STATUS_STATE_MASK)>>_DMA_STATUS_STATE_SHIFT;

}


/**
 * @brief  Get Transfer Status

 * @note   Get the status of the transfer in a specified channel
 *
 * @note   The Cycle field is set to 0 by the DMA, after completing the transfer
 *
 *
 */

unsigned DMA_GetTransferStatus(int ch) {
DMA_DESCRIPTOR_TypeDef *pDesc = &DMA_DescriptorTable[ch];

    return (pDesc->CTRL&_DMA_CTRL_CYCLE_CTRL_MASK)>>_DMA_CTRL_CYCLE_CTRL_SHIFT;

}


/**
 * @brief  Configure Channel for Memory to Memory transfer for 8-bit data

 * @note   Long description of function
 *
 */

int DMA_SetupTransferMemToMem_8(int ch, uint8_t *src, uint8_t *dst, unsigned num) {

    int rc = DMA_ConfigureDescriptor(ch, 8, src, 1, dst, 1, num);

    return rc;
}


/**
 * @brief  Configure Channel for Memory to Memory transfer for 16-bit data

 * @note   Long description of function
 *
 */
int DMA_SetupTransferMemToMem_16(int ch, uint16_t *src, uint16_t *dst, unsigned num) {

    int rc = DMA_ConfigureDescriptor(ch, 16, src, 2, dst, 2, num);

    return rc;
}



/**
 * @brief  Configure Channel for Memory to Memory transfer for 32-bit data

 * @note   Long description of function
 *
 */
int DMA_SetupTransferMemToMem_32(int ch, uint32_t *src, uint32_t *dst, unsigned num) {

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
    DMA->CHSWREQ = (1<<pch);
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

    // Enable channel if it not yet enabled
    if ( IsPrimaryChannel(ch) ) {
        DMA->CHENC = (1<<ch);
    } else {
        int ach = GetPrimaryChannelNumber(ch);
        DMA->CHALTC = (1<<ach);
    }

    return 0;
}

/**
 * @brief  General routine to configure descriptor
 *
 */

int  DMA_ConfigureDescriptor(int ch, unsigned size, void *src, unsigned srcinc, void *dst, unsigned dstinc, unsigned number) {
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
