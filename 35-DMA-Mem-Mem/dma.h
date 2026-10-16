#ifndef DMA_H
#define DMA_H
/**
 *  @file   dma.h
 *
 * @author  Hans
 */

#include <stdint.h>

/**
 *  @brief  Indexes for channel strutures. Also used as channel identificator
 */
///@{
#define DMA_CH0                     (0)
#define DMA_CH1                     (1)
#define DMA_CH2                     (2)
#define DMA_CH3                     (3)
#define DMA_CH4                     (4)
#define DMA_CH5                     (5)
#define DMA_CH6                     (6)
#define DMA_CH7                     (7)
#define DMA_CH8                     (8)
#define DMA_CH9                     (9)
#define DMA_CH10                    (10)
#define DMA_CH11                    (11)

#define DMA_CH0ALT                  (16+0)
#define DMA_CH1ALT                  (16+1)
#define DMA_CH2ALT                  (16+2)
#define DMA_CH3ALT                  (16+3)
#define DMA_CH4ALT                  (16+4)
#define DMA_CH5ALT                  (16+5)
#define DMA_CH6ALT                  (16+6)
#define DMA_CH7ALT                  (16+7)
#define DMA_CH8ALT                  (16+8)
#define DMA_CH9ALT                  (16+9)
#define DMA_CH10ALT                 (16+10)
#define DMA_CH11ALT                 (16+11)
///@}


/**
 *  @brief  Data size
 */
///@{
#define DMA_SIZE_BYTE               0b00
#define DMA_SIZE_HALF               0b01
#define DMA_SIZE_WORD               0b10
#define DMA_SIZE_8                  0b00
#define DMA_SIZE_16                 0b01
#define DMA_SIZE_32                 0b10
///@}

///@{
#define DMA_STATE_IDLE              (0)
#define DMA_STATE_RDCHCTRLDATA      (1)
#define DMA_STATE_RDSRCENDPTR       (2)
#define DMA_STATE_RDDSTENDPTR       (3)
#define DMA_STATE_RDSRCDATA         (4)
#define DMA_STATE_WRDSTDATA         (5)
#define DMA_STATE_WAITREQCLR        (6)
#define DMA_STATE_WRCHCTRLDATA      (7)
#define DMA_STATE_STALLED           (8)
#define DMA_STATE_DONE              (9)
#define DMA_STATE_PERSCATTRANS      (10)
///@}

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

int  DMA_Init(void);
void DMA_DeInit(void);
void DMA_Reset(void);

int  DMA_SetupTransferMemToMem_8(unsigned ch, uint8_t *src, uint8_t *dst, unsigned num);
int  DMA_SetupTransferMemToMem_16(unsigned ch, uint16_t *src, uint16_t *dst, unsigned num);
int  DMA_SetupTransferMemToMem_32(unsigned ch, uint32_t *src, uint32_t *dst, unsigned num);

int  DMA_StartTransfer(unsigned ch);
int  DMA_StopTransfer(unsigned ch);


uint32_t DMA_GetDeviceState(void);
uint32_t DMA_GetTransferStatus(unsigned ch);
uint32_t DMA_GetTransferCount(unsigned ch);

int  DMA_ConfigureDescriptor(unsigned ch, unsigned size, unsigned inc, void *src, void *dst, unsigned number);

DMA_DESCRIPTOR_TypeDef *GetPrimaryDescriptor(unsigned ch);
DMA_DESCRIPTOR_TypeDef *GetAlternateDescriptor(unsigned ch);
DMA_DESCRIPTOR_TypeDef *GetDescriptor(unsigned ch);


extern uint32_t enabled_channels;
extern uint32_t done_channels;
extern uint32_t error_channels;
#endif /* DMA_H */
