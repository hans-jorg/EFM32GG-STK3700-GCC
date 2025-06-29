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


#define DA_VREF_VDD              (1<<4)
#define DA_VREF_2_5              (1<<5)
#define DA_VREF_1_5              (1<<6)
#define DA_ENABLE_CH0            (1<<7)
#define DA_ENABLE_CH1            (1<<8)
#define DA_SINGLE_ENDED_OUTPUT   (0)
#define DA_DIFFERENTIAL_OUTPUT   (1<<9)
#define DA_ENABLE_CH0_ALT        (1<<10)
#define DA_ENABLE_CH1_ALT        (1<<11)

#define DA_STATUS_CH0_READY       DAC_STATUS_CH0DV
#define DA_STATUS_CH1_READY       DAC_STATUS_CH1DV


int DA_Init(unsigned config, unsigned samplerate);
int DA_ReconfigureClock(unsigned samplerate);
int DA_EnableOutputs(unsigned bm);
unsigned DA_Status(void);
int DA_SetOutput(int ch, unsigned v);
int DA_SetCombOutput( unsigned vch0, unsigned vch1);
int DA_SetDifferentialOutput(int v);
int DA_SetSineOuput(void);
int DA_ClearSineOuput(void);
int DA_SetCallback( void(*)(int) );
int DA_DisableIRQ(void);
int DA_EnableIRQ(void);
int DA_DisableChannel(int ch);
int DA_EnableChannel(int ch);
unsigned DA_Status(void);

#endif // DACONVERTER_H
