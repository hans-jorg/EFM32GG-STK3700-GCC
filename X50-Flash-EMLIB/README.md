50  Using the EMLIB
===================

Introduction
------------

Silicon Labs provides a set of support software, including the Simplicity IDE.
Among them:

* EMLIB:
* EMDRV:
* BSP:


>> One problem is the low quality of the documentation. It is mostly generated
from comments in the source code. There is no consolidated document. Neither 
there is no PDF file for this documentation.

They are included in the GeckoSDK and can be used as a:

* Code example. 
* Library for fast development.

They are provided as a Zlib License, that means:

1 - They are not liable for any errors.
2 - Origin must be disclosed and acknowledged.
3 - Altered versions must be plainly marked as such.
4 - The license note can not be removed.

These are reasonable terms. The main problem is the coupling to a specific microcontroller family in many small details and sometimes a more complex implementation than it is not needed for most applications. In the end, the
implementation should cover all applications.

The EMLIB library
-----------------

As stated in the Silabs documentation, "EMLIB is a low-level peripheral support library that provides a unified API for all EFM32, EZR32 and EFR32 MCUs and SoCs from Silicon Laboratories."

Its components are listed in the table below.

| Module     | Description                                            |
|------------|--------------------------------------------------------|
|  ACMP      | Analog comparator (ACMP) Peripheral API.               |
|  ADC       | Analog to Digital Converter (ADC) Peripheral API.      |
|  AES       | Advanced Encryption Standard Accelerator (AES) API     |
|  ASSERT    | Error checking module.                                 |
|  BURTC     | Backup Real Time Counter (BURTC) Peripheral API.       |
|  BUS       | BUS register and RAM bit/field read/write API.         |
|  CHIP      | Chip errata workarounds initialization API.            |
|  CMU       | Clock management unit (CMU) Peripheral API.            |
|  CORE      | Core interrupt handling API.                           |
|  DAC       | Digital to Analog Converter (DAC) Peripheral API.      |
|  DBG       | Debug (DBG) Peripheral API.                            |
|  DMA       | Direct Memory Access (DMA) Peripheral API.             |
|  EBI       | External Bus Interface (EBI) Peripheral API.           |
|  EMU       | Energy Management Unit (EMU) Peripheral API.           |
|  GPIO      | General Purpose Input/Output (GPIO) API.               |
|  I2C       | Inter-integrated Circuit (I2C) Peripheral API.         |
|  INT       | Safe nesting of interrupt disable/enable API.          |
|  LCD       | Liquid Crystal Display (LCD) Peripheral API.           |
|  LESENSE   | Low Energy Sensor (LESENSE) Peripheral API.            |
|  LETIMER   | Low Energy Timer (LETIMER) Peripheral API.             |
|  LEUART    | Low Energy UART (LEUART) Peripheral API.               |
|  MPU       | Memory Protection Unit (MPU) Peripheral API.           |
|  MSC       | Memory System Controller API.                          |
|  OPAMP     | Operational Amplifier (OPAMP) peripheral API.          |
|  PCNT      | Pulse Counter (PCNT) Peripheral API.                   |
|  PRS       | Peripheral Reflex System (PRS) Peripheral API.         |
|  RAMFUNC   | RAM code support.                                      |
|  RMU       | Reset Management Unit (RMU) Peripheral API.            |
|  RTC       | Real Time Counter (RTC) Peripheral API.                |
|  SYSTEM    | System API.                                            |
|  TIMER     | Timer/Counter (TIMER) Peripheral API.                  |
|  USART     | USART Peripheral API.                                  |
|  VCMP      | Voltage Comparator (VCMP) Peripheral API.              |
|  VERSION   | Version API.                                           |
|  WDOG      | Watchdog (WDOG) Peripheral API.                        |

The EMLIB is in the *platforms/emlib* folder in two subfolder *inc* and *src*.

     em_acmp[.ch]          em_cryotimer[.ch]  em_gpcrc[.ch]    em_leuart[.ch] 
     em_rtcc[.ch]          em_adc[.ch]        em_crypto[.ch]   em_gpio[.ch]
     em_msc[.ch]           em_se[.ch]         em_aes[.ch]      em_csen[.ch]
     em_i2c[.ch]           em_opamp[.ch]      em_system[.ch]   em_burtc[.ch]
     em_dac[.ch]           em_iadc[.ch]       em_pcnt[.ch]     em_timer[.ch]
     em_can[.ch]           em_dbg[.ch]        em_idac[.ch]     em_pdm[.ch]
     em_usart[.ch]         em_cmu[.ch]        em_dma[.ch]      em_lcd[.ch]
     em_prs[.ch]           em_vcmp[.ch]       em_cmu_fpga[.ch] em_ebi[.ch]
     em_ldma[.ch]          em_qspi[.ch]       em_vdac[.ch]     em_emu[.ch]
     em_cmu_fpga_npu[.ch]  em_lesense[.ch]    em_rmu[.ch]      em_wdog[.ch]
     em_core[.ch]          em_eusart[.ch]     em_letimer[.ch]  em_rtc[.ch]

###USART API

The *em_usart.\[ch\]* support synchronous, asynchronous, I2S, SPI, I2C and IrDA communication. It support the Peripheral Reflex System (PRS)
It consists of the following routines:

USART_BaudrateAsyncSet
USART_BaudrateCalc
USART_BaudrateGet
USART_BaudrateSyncSet
USART_Enable
USART_InitAsync
USART_InitSync
USARTn_InitIrDA
USART_InitI2s
USART_InitPrsTrigger
USART_Reset
USART_Rx
USART_RxDouble
USART_RxDoubleExt
USART_RxExt
USART_SpiTransfer
USART_Tx
USART_TxDouble
USART_TxDoubleExt
USART_TxExt

The initialization is done using data in USART_InitAsync_TypeDef, USART_InitSync, USART_InitIrDA, USART_InitI2s



Common component
-----------------

there is a COMMON component, used by EMLIB, that includes general purpose utilities and cross-compiler support.  

The common component is in the *platforms/common* folder and it contains the following files in two subfolders *inc* and *src*.

| Routine                           | Description                            |
|-----------------------------------|----------------------------------------|
| sl_assert.c                       | Internal Assert Handling               |
| sl_slist.c                        | Handling of single linked lists        |
| sl_stdio.c                        | Basic C standard I/O                   |
| sl_syscalls.c                     | POSIX place (weak) holder functions    |
| sli_cmsis_os2_ext_task_register.c | RTOS (uc/os and FreeRTOS) interface    |
| sl_status.c                       | Strings for status messages            |
| sl_string.c                       | String handling                        |

The basic C standard I/O libraryin *sl_stdio* contains

sl_putchar  Calls sl_iostream_write (where ?)
sl_getchar  Calls sl_iostream_getchar (where ?)




The EMDRV library
-----------------

The Board Support Package (BSP)
-------------------------------

Hardware Drivers
----------------

In the folder *hardware/kit/common/drivers* one can find support for many
devices including the nandflash and segmentlcd among others.

The NAND Flash support is implemented in the *nandflash.\[ch\]* files. It uses
the *dmactrl.\[ch]* that can be found in this folder.

NANDFLASH_AddressValid
NANDFLASH_CopyPage
NANDFLASH_DeviceInfo
NANDFLASH_EccCorrect
NANDFLASH_EraseBlock
NANDFLASH_Init
NANDFLASH_MarkBadBlock
NANDFLASH_ReadPage
NANDFLASH_ReadSpare
NANDFLASH_WritePage

Development tools
-----------------

Simplicity Studio v5
Simplicity Studio v6

References
----------

1. [EFM32 Gecko Software Documentation](https://siliconlabs.github.io/Gecko_SDK_Doc/efm32g/html/index.html)
2. [](https://docs.silabs.com/dev-tools)
3. [](https://docs.silabs.com/mcu/5.4/efm32gg/)
