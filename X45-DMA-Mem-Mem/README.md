43  DMA-Direct Memory Access
============================

Introduction
------------

Direct Memory Access (DMA) is a mechanism for transferring data between a peripheral and memory
without demanding the processor to handle the transfer.

The DMA controller in the EFM32GG is an licensed implementation of the PL230 uDMA developed by ARM.

The DMA controller in the EFM32GG features:

* 12 channels
* Transfer from Memory (RAM/EBI/Flash) to peripheral
* Transfer from Peripheral to memory (RAM/EBI)
* Transfer from Memory (RAM/EBI/Flash) to memory (RAM/EBI)
* Use one descriptor (primary) or two descriptors (primary/alternate)
* Transfer size: 8, 16 or 32 bits
* Transfer modes: basic, ping-pong, scatter-gatter (WTF)
* Programmable transfer length
* Looped transfers (Channels 0 and 1)
* 2D transfers (Channel 0)
* Interrupt upon transfer completion

The DMA controller is configured by:
* Setting registers of the DMA peripheral
* Setting descriptor in system memory
* Setting channel configuration


There are 12 DMA channels in the EFM32GG. There are two descriptors for each channel: a primary one
and an alternate one, that is used in some operating modes.

Transfers can occurs between a peripheral to an memory area or vice-versa. It is also possible
to make memory to memory transfers. Transfers can occur as 8, 16 and 32 bits.

> Naturally the destination must be writable.

To configure a DMA transfer, one must specify:

1. The source. It can be a memory area or a memory mapped peripheral register.
2. The increment of the source address at each transfer. It can be null when accessing a peripheral register.
3. The destination. It can be a memory area or a memory mapped peripheral register.
4. The increment of the destination address at each transfer. It can be null when writing to a peripheral register.
5. The size of each transfer. It must be a power of 2.
6. The number of items to be transferred.
7. The requested trigger. A DMA transfer is started when a a certain condition is fulfilled. For memory to memory transfers, it is not needed.

Two sizes are specified. One is the size of the full transfer and the other the arbitration interval.

### Include files

These files are included in the efm32gg990f1024.h device file.


* efm32gg_dma_descriptor.h: DMA (in RAM) descriptor register and bit field definitions
* efm32gg_dma_ch.h: DMA Channel register and bit field definitions
* efm32gg_dma.h: DMA registers and bit field definitions
* efm32gg_dmactrl.h: DMA CTRL bit fields

### Example

From Reference Manual

1. Configure the channel select for using USART1 with DMA channel 0
    Write SOURCESEL=0b001101 and SIGSEL=XX to DMA_CHCTRL0
2. Configure the primary channel descriptor for DMA channel 0
    a. Write XX (read address of USART1) to src_data_end_ptr
    b. Write 0x20003420 + 40 to dst_data_end_ptr c
    c. Write these values to channel_cfg for channel 0:
        i. dst_inc=b01 (destination halfword address increment)
        ii. dst_size=b01 (halfword transfer size)
        iii. src_inc=b11 (no address increment for source)
        iv. src_size=01 (halfword transfer size)
        v. dst_prot_ctrl=000 (no cache/buffer/privilege)
        vi. src_prot_ctrl=000 (no cache/buffer/privilege)
        vii.R_power=b0000 (arbitrate after each DMA transfer)
        viii.n_minus_1=d20 (transfer 21 halfwords)
        ix. next_useburst=b0 (not applicable)
        x. cycle_ctrl=b001 (basic operating mode)
3. Enable the DMA
    Write EN=1 to DMA_CONFIG
4. Disable the single requests for channel 0 (i.e., do not react to data available, wait for buffer full)
    Write DMA_CHUSEBURSTS[0]=1
5. Enable buffer-full requests for channel 0
    Write DMA_CHREQMASKC[0]=1
6. Use the primary data structure for channel 0
    Write DMA_CHALTC[0]=1
7. Enable channel 0
    Write DMA_CHENS[0]=1

### Operations mode


The transfer is specified by two parameters:

* R: the arbitration interval $$2^R$$ , i.e., how many data elements are transferred until an
    arbitration occurs. A request signal is needed to restart the transfer, except in the **auto-request**
     operation mode

* N: the number of data elements to be transferred.

There are many operation modes:

* Basic DMA Transfer
* Auto-requeste Transfer
* Ping-Pong
* Memory scatter
* Peripheral scatter


The Basic and the Auto-request operation modes are similar. The Basic mode transfers $$2^R$$ items
at every request until the specified quantity is transferred and a request is needed to restart
the transfer operation.  The Auto-requested demands only one request to transfer the entire data.
There are still arbitration after $$2^R$$ transfers but an explicit request is not needed to restart
the transfer operation.



### Channel control data structure

There must be a contiguous area that both the DMA controller and the host processor can access.

>> This area must start at an address multiple of 256.This means the last eight bits of the address
must be zero.

>> From RM: After the controller completes the N transfers it sets the **cycle_ctrl** field to b000,
to indicate that the channel_cfg data is invalid. This prevents it from repeating the same DMA
transfer.

>> From RM: The controller does not support a dst_size value that is different to the src_size value.

The controller uses the lower 8 address bits to enable it to access all of the elements
in the structure and therefore the base address must be at 0xXXXXXX00. It address must be written to
the DMA_CTRLBASE register.

Its size must lie between 16 bytes, when using only Channel 0, to 384 when using all primary and
alternate channels. For example, when using Channels 0 to 3, only 64 bytes are needed.


| Offset  | Description                                      |
|---------|--------------------------------------------------|
|      +0 | Primary Channel 0                                |
|     +10 | Primary Channel 1                                |
|     +20 | Primary Channel 2                                |
|     +30 | Primary Channel 3                                |
|     +40 | Primary Channel 4                                |
|     +50 | Primary Channel 5                                |
|     +60 | Primary Channel 6                                |
|     +70 | Primary Channel 7                                |
|     +80 | Primary Channel 8                                |
|     +90 | Primary Channel 9                                |
|     +A0 | Primary Channel 10                               |
|     +B0 | Primary Channel 11                               |
|     +C0 | Not used                                         |
|     +D0 | Not used                                         |
|     +E0 | Not used                                         |
|     +F0 | Not used                                         |
|    +100 | Primary Channel 0                                |
|    +110 | Primary Channel 1                                |
|    +120 | Primary Channel 2                                |
|    +130 | Primary Channel 3                                |
|    +140 | Primary Channel 4                                |
|    +150 | Primary Channel 5                                |
|    +160 | Primary Channel 6                                |
|    +170 | Primary Channel 7                                |
|    +180 | Primary Channel 8                                |
|    +190 | Primary Channel 9                                |
|    +1A0 | Primary Channel 10                               |
|    +1B0 | Primary Channel 11                               |
|    +1C0 | Not used                                         |

Each channel structure is 16 bytes long and has the following format.
It is described in the *efm32gg_dmactrl.h* and *efm32gg_dma_descriptorsrl.h* header files.


| Offset  |   Description                                    |
|---------|--------------------------------------------------|
|     +0  | Source End Pointer (*SRCEND*)                    |
|     +4  | Destination End Pointer (*DSTEND*)               |
|     +8  | Control (*CTRL*)                                 |
|     +C  | DMA does not use it, but free to use (*USER*)    |

The pointer fields are not written by the controller.

The control word has the following format

| Bit field | Description                                    |
|-----------|------------------------------------------------|
|  31-30    | Destination address increment (*dst\_inc*)     |
|  29-28    | Destination data size (*dst\_size*)            |
|  27-26    | Source address increment (*src\_inc*)          |
|  25-24    | Source size (*src\_size*)                      |
|  23-22    | Not used. Must be 0b00                         |
|  21-21    | Destination HPROT (*dst\_prot\_ctrl*)          |
|  20-19    | Not used. Must be 0b00                         |
|  18-18    | Source HPROT (*src\_prot\_ctrl*)               |
|  17-14    | M. Arbitrate after 2^M DMA Transfers (max 1024)|
|  13- 4    | Number of transfers minus one (*n_minus_1*)    |
|   3- 3    |  (*next\_userburst*)                           |
|   2- 0    | Operating mode (*cycle_ctrl*)                  |

> *dst\_size* and *src\_size* must be identical.

The increments are defined by respective source or destination filter.

| Value    | Byte (0b00)   | Halfword(0x01)|   Word (0b10) |
|----------|---------------|---------------|---------------|
|   0b00   | byte          | reserved      | reserved      |
|   0b01   | halfword      | halfword      | reserved      |
|   0b10   | word          | word          | word          |
|   0b11   | no increment  | no increment  | no increment  |

| Mode  | Description                                      |
|-------|--------------------------------------------------|
| ob000 | Stop                                             |
| 0b001 | Basic                                            |
| 0b010 | Auto-request                                     |
| 0b011 | Ping-Pong                                        |
| 0b100 | Memory scatter/gather. Primary.                  |
| 0b101 | Memory scatter/gather. Alternate.                |
| 0b110 | Peripheral scatter/gather. Primary               |
| 0b111 | Peripheral scatter/gather. Alternate.            |


### Source and destination


| src_inc  | dst_inc | Source Address          | Destination Address                |
|----------|---------|-------------------------|------------------------------------|
|  0b00    |  0b00   | SRCEND - n_minus_1      | DSTEND - n_minus_1                 |
|  0b01    |  0b01   | SRCEND - (n_minus_1<<1) | DSTEND - (n_minus_1<<1)            |
|  0b10    |  0b10   | SRCEND - (n_minus_1<<2) | DSTEND - (n_minus_1<<2)            |
|  0b11    |  0b11   | SRCEND                  | DSTEND                             |


### Software support from Silicion Labs

Silicon Labs delivers a set of header files, where the registers of the DMA device is
defined thru symbols.

* **efm32gg990f1024.h** : DMA_CHAN_COUNT and DMA_PRESENT.
* **efm32gg_dma.h** : DMA registers and symbols for the corresponding fields
* **efm32gg_dma_ch.h** : Channel control structure
* **efm32gg_dma_descriptor.h** : DMA descriptor
* **efm32gg_dmactrl.h** : Symbols the control element of the DMA descriptor
* **efm32gg_dmareq.h** : DMA Req bit fields

Annex A - DMA Registers
=======================


| Register              |R/W  | Description                                             |
|-----------------------|-----|---------------------------------------------------------|
| DMA_STATUS            |R    | DMA Status Registers                                    |
| DMA_CONFIG            |R    | DMA Configuration Register                              |
| DMA_CTRLBASE          |RW   | Channel Control Data Base Pointer Register              |
| DMA_ALTCTRLBASE       |R    | Channel Alternate Control Data Base Pointer Register    |
| DMA_CHWAITSTATUS      |R    | Channel Wait on Request Status Register                 |
| DMA_CHSWREQ           |W1   | Channel Software Request Register                       |
| DMA_CHUSEBURSTSR      |RW1H | Channel Useburst Set Register                           |
| DMA_CHUSEBURSTC       |W1   | Channel Useburst Clear Register                         |
| DMA_CHREQMASKSR       |W1   | Channel Request Mask Set Register                       |
| DMA_CHREQMASKC        |W1   | Channel Request Mask Clear Register                     |
| DMA_CHENSR            |RW1  | Channel Enable Set Register                             |
| DMA_CHENC             |W1   | Channel Enable Clear Register                           |
| DMA_CHALTSR           |W1   | Channel Alternate Set Register                          |
| DMA_CHALTC            |W1   | Channel Alternate Clear Register                        |
| DMA_CHPRIS            |RW1  | Channel Priority Set Register                           |
| DMA_CHPRIC            |W1   | Channel Priority Clear Register                         |
| DMA_ERRORC            |RW   | Bus Error Clear Register                                |
| DMA_CHREQSTATUS       |R    | Channel Request Status                                  |
| DMA_CHSREQSTATUS      |R    | Channel Single Request Status                           |
| DMA_IF                |R    | Interrupt Flag Register                                 |
| DMA_IFS               |W1   | Interrupt Flag Set Register                             |
| DMA_IFC               |W1   | Interrupt Flag Clear Register                           |
| DMA_IEN               |R    | WInterrupt Enable register                              |
| DMA_CTRL              |RW   | DMA Control Register                                    |
| DMA_RDS               |RW   | DMA Retain Descriptor State                             |
| DMA_LOOP0             |RWH  | Channel 0 Loop Register                                 |
| DMA_LOOP1             |RW   | Channel 1 Loop Register                                 |
| DMA_RECT0             |RWH  | Channel 0 Rectangle Register                            |
| DMA\_CH0\_CTRL        |RW   | Channel Control Register #0                             |
| DMA\_CH1\_CTRL        |RW   | Channel Control Register #1                             |
| DMA\_CH2\_CTRL        |RW   | Channel Control Register #2                             |
| DMA\_CH3\_CTRL        |RW   | Channel Control Register #3                             |
| DMA\_CH4\_CTRL        |RW   | Channel Control Register #4                             |
| DMA\_CH5\_CTRL        |RW   | Channel Control Register #5                             |
| DMA\_CH6\_CTRL        |RW   | Channel Control Register #6                             |
| DMA\_CH7\_CTRL        |RW   | Channel Control Register #7                             |
| DMA\_CH8\_CTRL        |RW   | Channel Control Register #8                             |
| DMA\_CH9\_CTRL        |RW   | Channel Control Register #9                             |
| DMA\_CH10\_CTRL       |RW   | Channel Control Register #10                            |
| DMA\_CH11\_CTRL       |RW   | Channel Control Register #11                            |


References
----------

1. [EFM32GG Reference Manual](https://www.silabs.com/documents/public/reference-manuals/EFM32GG-RM.pdf)
2. [AN0013 - Direct Memory Access](https://www.silabs.com/documents/public/application-notes/AN0013.pdf)
3. [PrimeCell DMA Controller (PL230) Technical Reference Manual ](https://developer.arm.com/documentation/ddi0417/a/?lang=en)
4. [EFM32 Series 0: DMA (ARM PrimeCell µDMA PL230)](https://www.silabs.com/documents/public/training/mcu/efm32-series-0-dma.pdf)
