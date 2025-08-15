#ifndef DACONVERTER_H
#define DACONVERTER_H
/**
 * @file    daconverter.h
 * @brief   DA HAL for EFM32GG STK3200
 * @version 1.0
 */
#include <stdint.h>

/**
 * @brief   create a bit mask with bit N set
 */
#ifndef BIT
#define BIT(N) (1U<<(N))
#endif

// General configuration
#define DAC_VREF_VDD              (1<<4)
#define DAC_VREF_2V5              (1<<5)
#define DAC_VREF_1V25             (1<<6)
#define DAC_SINGLE_ENDED_OUTPUT   (0)
#define DAC_DIFFERENTIAL_OUTPUT   (1<<9)


// Channel config
#define DAC_CHN_NOTUSED           (-1)
/*#define DAC_CHN_USEALT            (1<<8)*/
#define DAC_CHN_LOC_0             (0)
#define DAC_CHN_LOC_1             (1)
#define DAC_CHN_LOC_2             (2)
#define DAC_CHN_LOC_3             (3)
#define DAC_CHN_LOC_4             (4)
#define DAC_CHN_LOC_5             (5)
#define DAC_CHN_LOC_6             (6)
#define DAC_CHN_LOC_7             (7)

// Status
#define DAC_STATUS_CH0_READY       DAC_STATUS_CH0DV
#define DAC_STATUS_CH1_READY       DAC_STATUS_CH1DV

// Enable Channel
#define DAC_CH0                   (1<<0)
#define DAC_CH1                   (1<<1)


int DAC_Init(unsigned config, unsigned samplerate, int ch0config, int ch1config);
int DAC_ConfigureClock(unsigned samplerate);
int DAC_EnableChannels(unsigned bm);
int DAC_DisableChannels(unsigned bm);
unsigned DAC_Status(void);
int DAC_SetOutput(int ch, unsigned v);
int DAC_SetCombOutput( unsigned vch0, unsigned vch1);
int DAC_SetDifferentialOutput(int v);
int DAC_SetSineOuputMode(void);
int DAC_ClearSineOuputMode(void);
int DAC_SetCallback( void(*)(int) );
int DAC_DisableIRQ(void);
int DAC_EnableIRQ(void);
int DAC_DisableChannel(int ch);
int DAC_EnableChannel(int ch);
unsigned DAC_Status(void);

#endif // DACONVERTER_H
