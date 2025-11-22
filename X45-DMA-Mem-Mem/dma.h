#ifndef DMA_H
#define DMA_H
/**
 * @file  dma.h
 */

 /**
  * Indexes for channel strutures. Also used as channel identificator
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
///@{
int  DMA_Init(void);

int  DMA_SetupTransferMemToMem_8(int ch, uint8_t *src, uint8_t *dst, unsigned num);
int  DMA_SetupTransferMemToMem_16(int ch, uint16_t *src, uint16_t *dst, unsigned num);
int  DMA_SetupTransferMemToMem_32(int ch, uint32_t *src, uint32_t *dst, unsigned num);

int  DMA_StartTransfer(int ch);
int  DMA_StopTransfer(int ch);


unsigned DMA_GetDeviceState(void);
unsigned DMA_GetTransferStatus(int ch);

int  DMA_ConfigureDescriptor(int ch, unsigned size, void *src, unsigned srcinc, void *dst, unsigned dstinc, unsigned number);



#endif /* DMA_H */
