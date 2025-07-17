/**
 * @file    dma.c
 *
 * @brief   <Short description>
 *
 * @note    DMA source (SOURCESEL)
 *
 * |   Value    | Mode      | Description------------------------------------------------|
 * |------------|-----------|------------------------------------------------------------|
 * |  0b000000  |  NONE     | No source selected-----------------------------------------|
 * |  0b001000  |  ADC0     | Analog to Digital Converter 0------------------------------|
 * |  0b001010  |  DAC0     | Digital to Analog Converter 0------------------------------|
 * |  0b001100  |  USART0   | Universal Synchronous/Asynchronous Receiver/Transmitter 0--|
 * |  0b001101  |  USART1   | Universal Synchronous/Asynchronous Receiver/Transmitter 1--|
 * |  0b001110  |  USART2   | Universal Synchronous/Asynchronous Receiver/Transmitter 2--|
 * |  0b010000  |  LEUART0  | Low Energy UART 0------------------------------------------|
 * |  0b010001  |  LEUART1  | Low Energy UART 1----------------------------- ------------|
 * |  0b010100  |  I2C0     | I2C 0------------------------------------------   ---------|
 * |  0b010101  |  I2C1     | I2C 1---------------------------------------------   ------|
 * |  0b011000  |  TIMER0   | Timer 0--------------------------------------------- ------|
 * |  0b011001  |  TIMER1   | Timer 1--------------------------------------------- ------|
 * |  0b011010  |  TIMER2   | Timer 2--------------------------------------------- ------|
 * |  0b011011  |  TIMER3   | Timer 3--------------------------------------------- ------|
 * |  0b101100  |  UART0    | Universal Asynchronous Receiver/Transmitter 0-------- -----|
 * |  0b101101  |  UART1    | Universal Asynchronous Receiver/Transmitter 1--------------|
 * |  0b110000  |  MSC      | ---------------------------------------------------        |
 * |  0b110001  |  AES      | Advanced Encryption Standard Accelerator-------------------|
 * |  0b110010  |  LESENSE  | Low Energy Sensor Interface--------------------------------|
 * |  0b110011  |  EBI      | External Bus Interface-------------------------------------|
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


/**
 *
 */
#define DMA_NUMPRIMARYCHANNELS                 (12)
#define DMA_NUMALTERNATECHANNELS               (12)


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

typedef union  {
    uint32_t        word;
    struct {
        unsigned    cycle_ctr:3;            ///! bits 2-0
        unsigned    next_useburst:1;        ///! bits 3-3
        unsigned    n_minus_1:10;           ///! bits 13-4
        unsigned    R_power:4;              ///! bits 17-14
        unsigned    src_prot_ctrl:3;        ///! bits 20-18
        unsigned    dst_prot_ctrl:3;        ///! bits 23-21
        unsigned    src_size:2;             ///! bits 25-24
        unsigned    src_inc:2;              ///! bits 27-26
        unsigned    dst_size:2;             ///! bits 29-28
        unsigned    dst_inc:2;              ///! bits 31-30
    } control;
} DMA_ControlTypeDef;


/**
 *  @brief  Field position
 */
///@{
#define  POS_cycle_ctr                          (0)
#define  POS_next_useburst                      (3)
#define  POS_n_minus_1                          (4)
#define  POS_R_power                            (14)
#define  POS_src_prot_ctrl                      (18)
#define  POS_dst_prot_ctrl                      (21)
#define  POS_src_size                           (24)
#define  POS_src_inc                            (26)
#define  POS_dst_size                           (28)
#define  POS_dst_inc                            (30)
///@}

/**
 *  @brief  Layout of DMA descriptor
 *
 *  @note   Se 8.4.3.3 Control data configuration
 */

typedef union {
    uint32_t  w[4];
    struct {
        uint32_t            src_data_end_ptr;
        uint32_t            dest_data_end_ptr;
        uint32_t            control;
        uint32_t            unused;
    } desc;
} DMA_DescriptorTypeDef;

/**
 * @brief Area for descriptors
 *
 * @note  The number of channels should be configurable
 */
///@{
#if DMA_NUMALTERNATECHANNELS > 0
static DMA_DescriptorTypeDef  DMA_DescriptorTable[16+DMA_NUMALTERNATECHANNELS]
                 __attribute__((aligned (256)));
#else
static DMA_DescriptorTypeDef  DMA_DescriptorTable[DMA_NUMPRIMARYCHANNELS]
                 __attribute__((aligned (256)));
#endif

///@}
/**
 *
 */
int DMA_Init(void) {

    // Enable clock for DMA
    CMU->HFCORECLKEN0 |= CMU_HFCORECLKEN0_DMA;

    DMA->CTRLBASE = (uint32_t) DMA_DescriptorTable;
}


/**
 * @brief  Enable Channel with number n

 * @note   Long description of function
 *
 */

void DMA_EnableChannel(int ch) {

    DMA->CHENS = (1<<ch);
    return;
}


/**
 * @brief  Disable Channel with number n

 * @note   Long description of function
 *
 */

void DMA_DisableChannel(int ch) {

    DMA->CHENC = (1<<ch);
    return;
}


/**
 * @brief  Disable Channel with number n

 * @note   Long description of function
 *
 */

void DMA_SelectAlternateDescriptor(int ch) {

    DMA->CHENC = (1<<ch);
    return;
}


/**
 * @brief  Disable Channel with number n

 * @note   Long description of function
 *
 */

void DMA_DeselectAlternateDescriptor(int ch) {

    DMA->CHALTC = (1<<ch);
    return;
}



/**
 * @brief  Configure Channel

 * @note   Long description of function
 *
 */

void DMA_ConfigureChannel(int ch, uint32_t source, unsigned signal) {

    DMA->CH[ch].CTRL = (source<<_DMA_CH_CTRL_SOURCESEL_SHIFT)|(signal<<_DMA_CH_CTRL_SIGSEL_SHIFT);



    return;
}


/**
 * @brief  Configure Descriptor
 *
 */

void DMA_ConfigureDescriptor(int ch,
                    void        *src,
                    unsigned    srcinc,
                    void        *dst,
                    unsigned    dstinc,
                    unsigned    number ) {
uint32_t control;

    DMA_DescriptorTypeDef *p = &DMA_DescriptorTable[ch];

    //control =   (src)


    return;
}

/**
 * @brief  Configure Primary Descriptor
 *
 */
void DMA_ConfigurePrimaryDescriptor(int ch,
                    void        *src,
                    unsigned    srcinc,
                    void        *dst,
                    unsigned    dstinc,
                    unsigned    number ) {

    DMA_ConfigureDescriptor(ch,src,srcinc,dst,dstinc,number);

    return;
}

/**
 * @brief  Configure Alternate Descriptor
 *
 */
void DMA_ConfigureAlternateDescriptor(int ch,
                    void        *src,
                    unsigned    srcinc,
                    void        *dst,
                    unsigned    dstinc,
                    unsigned    number ) {

    DMA_ConfigureDescriptor(ch+16,src,srcinc,dst,dstinc,number);

    switch(ch) {
    case 0|1|3:
        ch = 1;
    case 4|6:
        ch = 2;
    default:
        ch = 3;
    }

    return;
}
