

/**
 * @file    daconverter.c
 * @brief   DAC HAL for EFM32GG
 * @version 1.0
 */
#include <stdint.h>
#include "em_device.h"
#include "clock_efm32gg_ext.h"
#include "daconverter.h"

/**
 * Configuration parameters
 */
#define MODE            (0)
#define BIASPROG        (7)
#define MAXSAMPLERATE   (500000UL)

/**
 * @brief   Pointer to GPIO registers
 */
//static GPIO_P_TypeDef * const GPIOE = &(GPIO->P[4]);  // GPIOE

/**
 *  @brief  Pointer to call backfunction
 */
static void (*DAC_Callback)(int ch) = 0;

/**
 *  @brief IRQ Handler for DAC0
 */
void DAC0_IRQHandler(void) {

    int ch = 0;
    if( DAC0->IF&DAC_IF_CH0) ch |= 1;
    if( DAC0->IF&DAC_IF_CH0) ch |= 2;

    if( DAC_Callback ) DAC_Callback(ch);
    // Clear interrupt.
    DAC0->IFC = DAC_IFC_CH0|DAC_IFC_CH1;
}


/**
 *  @brief      DAC_DisableChannels
 *
 *  @param     unsingmed mapped as a bitvector. Bit 0 corresponds to CH0 and bit 1, to CH1
 */
int DAC_DisableChannels(unsigned bm) {

    if( (bm&DAC_CH0)!=0 ) {
        DAC0->CH0CTRL &= ~DAC_CH0CTRL_EN;
    }
    if( (bm&DAC_CH1)!=0 ) {
        DAC0->CH0CTRL &= ~DAC_CH1CTRL_EN;
    }
    return 0;
}


/**
 *  @brief      DAC_EnableChannels
 *
 *  @param     unsingmed mapped as a bitvector. Bit 0 corresponds to CH0 and bit 1, to CH1
 */
int DAC_EnableChannels(unsigned bm) {

    if( (bm&DAC_CH0)!=0 ) {
        DAC0->CH0CTRL |= DAC_CH0CTRL_EN;
    }
    if( (bm&DAC_CH0)!=0 ) {
        DAC0->CH0CTRL |= DAC_CH0CTRL_EN;
    }
    return 0;
}

/**
 *  @brief      DAC_Callback
 *
 *  @param      config:
 *  @param      samplerate: desired sample rate
 *
 *  @returns    0=OK, other value in case of error
 *
 */
int DAC_Init(unsigned config, unsigned samplerate, int ch0config, int ch1config) {
unsigned daclkfreq;

    /* Enable Clock for GPIO */
    CMU->HFPERCLKDIV |= CMU_HFPERCLKDIV_HFPERCLKEN;     // Enable HFPERCLK
    CMU->HFPERCLKEN0 |= CMU_HFPERCLKEN0_DAC0;           // Enable HFPERCKL for DAC

    // Just in case, disable channels
    DAC_DisableChannels(DAC_CH0|DAC_CH1);

    // configure clock prescaler. It modifies the CTRL register
    int rc = DAC_ConfigureClock(samplerate);
    if( rc < 0 )
        return -2;


    unsigned chs = 0;
    if( ch0config == DAC_CHN_NOTUSED )
        chs |= DAC_CH0;
    if( ch1config == DAC_CHN_NOTUSED )
        chs |= DAC_CH1;

    // Differential output needs both channels
    if( config&DAC_DIFFERENTIAL_OUTPUT ) {
        if( chs != (DAC_CH0|DAC_CH1) )
            return -3;
    }

    uint32_t dacctrl = _DAC_CTRL_RESETVALUE;
    uint32_t calctrl = _DAC_CAL_RESETVALUE;

    if( config&DAC_VREF_VDD ) {
        dacctrl = (dacctrl&~(_DAC_CTRL_REFSEL_MASK))
                    | (_DAC_CTRL_REFSEL_VDD);
    } else if( config&DAC_VREF_2V5 ) {
        dacctrl = (dacctrl&~(_DAC_CTRL_REFSEL_MASK))
                    | (_DAC_CTRL_REFSEL_2V5);
    } else if( config&DAC_VREF_1V25 ) {
        dacctrl = (dacctrl&~(_DAC_CTRL_REFSEL_MASK))
                    | (_DAC_CTRL_REFSEL_1V25);
    }
    dacctrl &= ~(  _DAC_CTRL_CONVMODE_MASK
                  |_DAC_CTRL_DIFF_MASK
                  |_DAC_CTRL_OUTMODE_MASK);

    if( config&DAC_SINGLE_ENDED_OUTPUT )
        dacctrl &= ~(DAC_CTRL_DIFF);
    if( config&DAC_DIFFERENTIAL_OUTPUT )
        dacctrl |= DAC_CTRL_DIFF;

    // Set conversion mode to continuous
    dacctrl |= DAC_CTRL_CONVMODE_CONTINUOUS;

    // Set output mode
    dacctrl |= DAC_CTRL_OUTMODE_PIN;

    // Adjust calibration
    if( (config&DAC_VREF_VDD) != 0 ) {
        DAC0->CAL = DEVINFO->DAC0CAL2;
    } else if( (config&DAC_VREF_2V5) != 0 ) {
        DAC0->CAL = DEVINFO->DAC0CAL1;
    } else if( (config&DAC_VREF_1V25) != 0 ) {
        DAC0->CAL = DEVINFO->DAC0CAL0;
    }

    // set control register
    DAC0->CTRL = dacctrl;

    // Configure channels
/*    if( ch0config != DAC_CHN_NOTUSED ) {*/
/*        DAC0->CH0CTRL |= DAC_CH0CTRL_EN;*/
/*        if( (ch0config&DAC_CHN_USEALT) != 0 )*/
/*            DAC0->OPA0MUX = (DAC0->OPA0MUX&~(_DAC_OPA0MUX_OUTMODE_MASK))*/
/*                            |DAC_OPA0MUX_OUTMODE_ALL;*/
/*        else*/
/*            DAC0->OPA0MUX = (DAC0->OPA0MUX&~(_DAC_OPA0MUX_OUTMODE_MASK))*/
/*                            |DAC_OPA0MUX_OUTMODE_MAIN;*/
/*    }*/
/*    if( ch1config != DAC_CHN_NOTUSED ) {*/
/*        DAC0->CH1CTRL |= DAC_CH1CTRL_EN;*/
/*        if( (ch1config&DAC_CHN_USEALT) != 0 )*/
/*            DAC0->OPA1MUX = (DAC0->OPA1MUX&~(_DAC_OPA1MUX_OUTMODE_MASK))*/
/*                            |DAC_OPA1MUX_OUTMODE_ALL;*/
/*        else*/
/*            DAC0->OPA1MUX = (DAC0->OPA1MUX&~(_DAC_OPA1MUX_OUTMODE_MASK))*/
/*                            |DAC_OPA1MUX_OUTMODE_MAIN;*/
/*    }*/

    DAC_EnableChannels(chs);
    return 0;
}


/**
 *  @brief      DAC_ReconfigureClock
 *
 *  @note       The CHxDV bit is 1 when data is written and 0
 *              when it is converted.
 *
 *  @note       The meaning of the returned value are 0 when busy
 *              and 1 when ready, ie, data can be written.
 */
unsigned DAC_Status(void) {
uint32_t w = DAC0->STATUS;

    return w^(DAC_STATUS_CH0DV|DAC_STATUS_CH1DV);

}


/**
 *  @brief      DAC_ReconfigureClock
 *
 *  @param      samplerate
 *
 *  @returns    0=OK, other value in case of error
 *
 *  @note       Sample rate is limited to 500 Ksps, that
 *              means a clock of 1 MHz for the DAC
 */
int DAC_ConfigureClock(unsigned samplerate) {

   if( samplerate > MAXSAMPLERATE )
        return -1;

    uint32_t daclkfreq = 2*samplerate;

    uint32_t hfperclk = ClockGetPeripheralClockFrequency();

    uint32_t div = (hfperclk-daclkfreq-1)/daclkfreq;

    uint32_t presc = 0;
    while( (1<<presc) < div ) presc++;

    if( presc > 7 )
        return -2;

    DAC0->CTRL = ((DAC0->CTRL)&~(_DAC_CTRL_PRESC_MASK))
                |((presc)<_DAC_CTRL_PRESC_SHIFT);

    return 0;
}


/**
 *  @brief      DAC_EnableOutputs
 *
 *  @param      Bit mask specifying which output is to be enabled
 *
 *  @returns
 *
 */



/**
 *  @brief      DAC_SetOutput
 *
 *  @param      ch: channel number 0 or 1
 *  @param      v:  12-bit value

 *  @returns
 *
 */
int DAC_SetOutput(int ch, unsigned v) {

    v &= 0xFFF; // only 12 bits
    if( ch == 0 )
        DAC0->CH0DATA = v;
    if( ch == 1 )
        DAC0->CH1DATA = v;
    return 0;
}

/**
 *  @brief      DAC_SetOutput
 *
 *  @param      vch0: 12-bit value for channel number 0
 *  @param      vch1: 12-bit value for channel number 1

 *  @returns    always 0
 *
 */
int DAC_SetCombOutput( unsigned vch0, unsigned vch1) {

    vch0 &= 0xFFF;
    vch1 &= 0xFFF;

    DAC0->COMBDATA = (vch0<<16)|vch1;
    return 0;
}
/**
 *  @brief      DAC_SetDifferentialOutput
 *
 *  @param      v: value of the desired output
 *                 in range -2047 to +2048
 *
 *  @returns    always 0
 *
 */
int DAC_SetDifferentialOutput(int v) {

    v &= 0xFFF; // only 11 bits plus sign
    DAC0->CH0DATA = v;

    return 0;
}


/**
 *  @brief      DAC_SetSineOuput
 *
 *  @returns    always 0
 *
 */
int DAC_SetSineOuput(void) {

    DAC0->CTRL |= DAC_CTRL_SINEMODE;
    return 0;
}

/**
 *  @brief      DAC_ClearSineOuput
 *
 *  @returns    always 0
 *
 */
int DAC_ClearSineOuput(void) {

    DAC0->CTRL &= ~DAC_CTRL_SINEMODE;
    return 0;
}

/**
 *  @brief      DAC_SetCallback
 *
 *  @param      func: function to be called in case of interrupt
 *
 *  @returns    always 0
 *
 */
int DAC_SetCallback( void(*func)(int)) {

    DAC_Callback = func;
    return 0;
}


/**
 *  @brief      DAC_DisableIRQ
 *
 *  @returns    always 0
 *
 */
int DAC_DisableIRQ(void) {

    NVIC_DisableIRQ(DAC0_IRQn);
    return 0;
}


/**
 *  @brief      DAC_EnableIRQ
 *
 *  @returns    always 0
 *
 */
int DAC_EnableIRQ(void) {

    DAC0->IEN |= (DAC_IEN_CH0|DAC_IEN_CH1);
    NVIC_ClearPendingIRQ(DAC0_IRQn);
    NVIC_EnableIRQ(DAC0_IRQn);
    return 0;
}


/**
 *  @brief      DAC_DisableChannel
 *
 *  @param      ch: channel number 0 or 1
 *
 *  @returns    Always 0
 *
 */
int DAC_DisableChannel(int ch) {

    if( ch == 0 ) DAC0->CH0CTRL &= ~(DAC_CH0CTRL_EN);
    if( ch == 1 ) DAC0->CH1CTRL &= ~(DAC_CH1CTRL_EN);

    return 0;
}


/**
 *  @brief      DAC_EnableChannel
 *
 *  @param      ch: channel number 0 or 1
 *
 *  @returns    Always 0
 *
 */
int DAC_EnableChannel(int ch) {

    if( ch == 0 ) DAC0->CH0CTRL |= DAC_CH0CTRL_EN;
    if( ch == 1 ) DAC0->CH1CTRL |= DAC_CH1CTRL_EN;

    return 0;
}
