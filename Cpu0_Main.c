/**********************************************************************************************************************
 * \file Cpu0_Main.c
 * \copyright Copyright (C) Infineon Technologies AG 2019
 *
 * Use of this file is subject to the terms of use agreed between (i) you or the company in which ordinary course of
 * business you are acting and (ii) Infineon Technologies AG or its licensees. If and as long as no such terms of use
 * are agreed, use of this file is subject to following:
 *
 * Boost Software License - Version 1.0 - August 17th, 2003
 *
 * Permission is hereby granted, free of charge, to any person or organization obtaining a copy of the software and
 * accompanying documentation covered by this license (the "Software") to use, reproduce, display, distribute, execute,
 * and transmit the Software, and to prepare derivative works of the Software, and to permit third-parties to whom the
 * Software is furnished to do so, all subject to the following:
 *
 * The copyright notices in the Software and this entire statement, including the above license grant, this restriction
 * and the following disclaimer, must be included in all copies of the Software, in whole or in part, and all
 * derivative works of the Software, unless such copies or derivative works are solely in the form of
 * machine-executable object code generated by a source language processor.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE
 * WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE, TITLE AND NON-INFRINGEMENT. IN NO EVENT SHALL THE
 * COPYRIGHT HOLDERS OR ANYONE DISTRIBUTING THE SOFTWARE BE LIABLE FOR ANY DAMAGES OR OTHER LIABILITY, WHETHER IN
 * CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
 * IN THE SOFTWARE.
 *********************************************************************************************************************/
#include "Ifx_Types.h"
#include "IfxCpu.h"
#include "IfxScuWdt.h"

#include "IfxCcu6_reg.h"
#include "IfxVadc_reg.h"
#include "IfxGtm_reg.h"

// jskwon
// for debug
//#include <stdio.h>

// Port registers
#define PC1_BIT_LSB_IDX             11
#define PC2_BIT_LSB_IDX             19
#define PC3_BIT_LSB_IDX             27
#define PC4_BIT_LSB_IDX             3
#define PC5_BIT_LSB_IDX             11
#define PC6_BIT_LSB_IDX             19
#define PC7_BIT_LSB_IDX             27
#define PC0_BIT_LSB_IDX             3
#define P1_BIT_LSB_IDX              1
#define P2_BIT_LSB_IDX              2
#define P3_BIT_LSB_IDX              3
#define P4_BIT_LSB_IDX              4
#define P5_BIT_LSB_IDX              5
#define P6_BIT_LSB_IDX              6
#define P7_BIT_LSB_IDX              7

// Extra
#define PC8_BIT_LSB_IDX             3
#define PC9_BIT_LSB_IDX             11
#define PC10_BIT_LSB_IDX            19
#define PC11_BIT_LSB_IDX            27
#define PC12_BIT_LSB_IDX            3

// SCU registers
#define LCK_BIT_LSB_IDX             1
#define ENDINIT_BIT_LSB_IDX         0
#define EXIS0_BIT_LSB_IDX           4
#define EXIS1_BIT_LSB_IDX           20
#define FEN0_BIT_LSB_IDX            8
#define FEN1_BIT_LSB_IDX            24
#define EIEN0_BIT_LSB_IDX           11
#define EIEN1_BIT_LSB_IDX           27
#define INP0_BIT_LSB_IDX            12
#define INP1_BIT_LSB_IDX            28
#define IGP0_BIT_LSB_IDX            14
#define IGP1_BIT_LSB_IDX            30
#define REN0_BIT_LSB_IDX            9
#define REN1_BIT_LSB_IDX            25

// SRC registers
#define SRPN_BIT_LSB_IDX            0
#define TOS_BIT_LSB_IDX             11
#define SRE_BIT_LSB_IDX             10

// CCU60 registers
#define DISS_BIT_LSB_IDX            1
#define DISR_BIT_LSB_IDX            0
#define CTM_BIT_LSB_IDX             7
#define T12PRE_BIT_LSB_IDX          3
#define T12CLK_BIT_LSB_IDX          0
#define T12STR_BIT_LSB_IDX          6
#define T12RS_BIT_LSB_IDX           1
#define INPT12_BIT_LSB_IDX          10
#define ENT12PM_BIT_LSB_IDX         7
#define T12SSC_BIT_LSB_IDX          0

// VADC registers
#define DISS_BIT_LSB_IDX            1
#define DISR_BIT_LSB_IDX            0
#define ANONC_BIT_LSB_IDX           0
#define ASEN0_BIT_LSB_IDX           24
#define CSM0_BIT_LSB_IDX            3
#define PRIO0_BIT_LSB_IDX           0
#define CMS_BIT_LSB_IDX             8
#define FLUSH_BIT_LSB_IDX           10
#define TREV_BIT_LSB_IDX            9
#define ENGT_BIT_LSB_IDX            0
#define RESPOS_BIT_LSB_IDX          21
#define RESREG_BIT_LSB_IDX          16
#define ICLSEL_BIT_LSB_IDX          0
#define VF_BIT_LSB_IDX              31
#define RESULT_BIT_LSB_IDX          0

// GTM registers
#define DISS_BIT_LSB_IDX            1
#define DISR_BIT_LSB_IDX            0
#define SEL7_BIT_LSB_IDX            14
#define SEL9_BIT_LSB_IDX            18
#define SEL11_BIT_LSB_IDX           22
#define EN_FXCLK_BIT_LSB_IDX        22
#define FXCLK_SEL_BIT_LSB_IDX       0

// GTM - TOM0 registers
#define UPEN_CTRL1_BIT_LSB_IDX      18
#define HOST_TRIG_BIT_LSB_IDX       0
#define ENDIS_CTRL1_BIT_LSB_IDX     2
#define OUTEN_CTRL1_BIT_LSB_IDX     2
#define RSCNT0_CN1_BIT_LSB_IDX      18
#define FUPD_CTRL1_BIT_LSB_IDX      2
#define CLK_SRC_SR_BIT_LSB_IDX      12
#define SL_BIT_LSB_IDX              11

#define T13_CLK_FREQ_DIVIDER        0x6     // i.e. 50MHz / 8 = 1562500 Hz
#define T13_PRESCALER               0x0     // Prescaler disable
#define COUNTER_PER_SEC             100     // # of counters per sec
#define SLEEP_THRESH_TIME             5     // Sleep decision after n seconds
#define SLEEP_THRESH_DIST            30

#define SLEEP_THRESH_TIME_LEV1      500   // 5 sec
#define SLEEP_THRESH_TIME_LEV2     1000  // 10 sec
#define SLEEP_THRESH_TIME_LEV3     1500  // 15 sec
#define BUZZ_FREQ_THRESH_LEV1        50   // 5 sec
#define BUZZ_FREQ_THRESH_LEV2        25  // 10 sec
#define BUZZ_FREQ_DIV_LEV1          100
#define BUZZ_FREQ_DIV_LEV2           50

#define BUFF_SIZE                   100
IfxCpu_syncEvent g_cpuSyncEvent = 0;

void initLED(void);
void initButton(void);
void initERU2(void);
void initERU3(void);
void initCCU60(void);
void initRGBLED(void);
void initVADC(void);
void initOutputCommunicate(void);
void VADC_startConversion(void);
unsigned int VADC_readResult(void);
void initGTM(void);
void usonicTrigger(void);
void initCCU61(void);
void initUSonic(void);
void initBuzzer(void);
unsigned int Range_LPF(int);
void sendGPIOSignal(void);

unsigned int range;
unsigned int range_avg = 0;
unsigned int range_count = 0;
unsigned int range_buff[BUFF_SIZE] = {0,};
unsigned char range_valid_flag = 0;
unsigned char sleep_flag = 0;
unsigned int sleep_counter;
unsigned char a, b, c;
unsigned int T13_INTERRUPT_FREQ = (50000000/T13_CLK_FREQ_DIVIDER) / COUNTER_PER_SEC;
unsigned int sleep_level = 0;
unsigned int buzz_status_flag = 0;

__interrupt(0xFE) __vector_table(0)
void ERU3_ISR(void)
{
    if( (P14_IN.U & (0x1 << P1_BIT_LSB_IDX)) != 0 )     // rising edge of echo
    {
        //                    _________
        //        echo ______|
        //                   ^
        CCU61_TCTR4.U = 0x1 << T12RS_BIT_LSB_IDX;       // start CCU61 T12 counter
    }
    else                                            // falling edge of echo
    {
        //             ______
        //        echo       |______
        //                   ^

        CCU61_TCTR4.B.T12RR = 0x1;       // stop CCU61 T12 counter

        // (1 / t_freq) * counter * 1000000 / 58  =  centimeter
        range = ((CCU61_T12.B.T12CV * 1000000) / 48828) / 58;
        range = Range_LPF(range);
        range_valid_flag = 1;

        CCU61_TCTR4.B.T12RES = 0x1;       // reset CCU61 T12 counter
    }
}

__interrupt(0x0B) __vector_table(0)
void CCU60_T12_ISR(void)
{
    // end of 10us Trig
    // GPIO P02.6 --> LOW
    P02_OUT.U &= ~(0x1 << P6_BIT_LSB_IDX);
}

__interrupt(0x0C) __vector_table(0)
void CCU60_T13_ISR(void) // 100 Hz
{
    if (sleep_flag == 1)
    {
        sleep_counter++; // 100 counters for 1 sec since interrupt happens every 0.01s

        if (SLEEP_THRESH_TIME_LEV3 <= sleep_counter){
            sleep_level = 3;
        }else if (SLEEP_THRESH_TIME_LEV2 <= sleep_counter){
            sleep_level = 2;
        }else if (SLEEP_THRESH_TIME_LEV1 <= sleep_counter){
            sleep_level = 1;
        }

        if (sleep_level == 1){  // sleep level 1
            buzz_status_flag = sleep_counter % BUZZ_FREQ_DIV_LEV1;
            if ( buzz_status_flag <= BUZZ_FREQ_THRESH_LEV1 ){
                // buzz on
                P02_IOCR0.B.PC3 = 0x11;
                // light on
                P10_OUT.U |= 0x1 << P1_BIT_LSB_IDX;
            }
            else{
                // buzz off
                P02_IOCR0.B.PC3 = 0x0;
                // light off
                P10_OUT.U &= ~(0x1 << P1_BIT_LSB_IDX);
            }
        }
        else if (sleep_level == 2){ // sleep level 2
            buzz_status_flag = sleep_counter % BUZZ_FREQ_DIV_LEV2;
            if ( buzz_status_flag <= BUZZ_FREQ_THRESH_LEV2 ){
                // buzz on
                P02_IOCR0.B.PC3 = 0x11;
                // light on
                P10_OUT.U |= 0x1 << P1_BIT_LSB_IDX;
            }
            else{
                // buzz off
                P02_IOCR0.B.PC3 = 0x0;
                // light off
                P10_OUT.U &= ~(0x1 << P1_BIT_LSB_IDX);
            }
        }
        else if (sleep_level == 3){   // sleep level 3
            P02_IOCR0.B.PC3 = 0x11;
            // light on
            P10_OUT.U |= 0x1 << P1_BIT_LSB_IDX;
            P14_OUT.U |= 0x1 << PC0_BIT_LSB_IDX;
        }

    }
}

__interrupt(0xFF) __vector_table(0)
void ERU2_ISR(void) // 100 Hz
{
    // check button pushed
    a = P02_IN.U & (0x1 << P1_BIT_LSB_IDX); // check button status
    if(sleep_flag && (range >= 50)) // wake up condition satisfied
    {
        if(a==0) // button pushed
        {
            sleep_flag = 0;
            sleep_counter = 0;
            sleep_level = 0;
            P10_OUT.U &= ~(0x1 << P1_BIT_LSB_IDX); // Turn off LED
            P02_IOCR0.B.PC3 = 0x0;  // turn off buzzer

        }
    }
}

int core0_main(void)
{
    IfxCpu_enableInterrupts();

    /* !!WATCHDOG0 AND SAFETY WATCHDOG ARE DISABLED HERE!!
     * Enable the watchdogs and service them periodically if it is required
     */
    IfxScuWdt_disableCpuWatchdog(IfxScuWdt_getCpuWatchdogPassword());
    IfxScuWdt_disableSafetyWatchdog(IfxScuWdt_getSafetyWatchdogPassword());

    /* Wait for CPU sync event */
    IfxCpu_emitEvent(&g_cpuSyncEvent);
    IfxCpu_waitEvent(&g_cpuSyncEvent, 1);

    initERU2();
    initERU3();
    initCCU60();
    initCCU61();
    initLED();
    initRGBLED();
    //initVADC();
    initBuzzer();
    initGTM();
    initButton();
    initUSonic();
    initOutputCommunicate();

    GTM_TOM0_TGC0_GLB_CTRL.U |= 0x1 << HOST_TRIG_BIT_LSB_IDX;       // trigger update request signal
    GTM_TOM0_TGC1_GLB_CTRL.U |= 0x1 << HOST_TRIG_BIT_LSB_IDX;       // trigger update request signal

    while(1)
    {
        for(unsigned int i = 0; i < 100000000; i++);
        usonicTrigger();
        while( range_valid_flag == 0);

        if (range <= SLEEP_THRESH_DIST)
        {
            sleep_flag = 1;
        }


        if (sleep_counter > COUNTER_PER_SEC*SLEEP_THRESH_TIME)
        {
            P10_OUT.U |= 0x1 << P1_BIT_LSB_IDX;  // turn on P10.2 (LED D13 BLUE)
            P02_IOCR0.B.PC3 = 0x11; // turn on buzzer
        }

    }
    return (1);
}

void initLED(void)
{
    P10_IOCR0.U &= ~(0x1F << PC1_BIT_LSB_IDX);     // reset P10_IOCR0 PC1
    P10_IOCR0.U &= ~(0x1F << PC2_BIT_LSB_IDX);     // reset P10_IOCR0 PC2

    P10_IOCR0.U |= 0x10 << PC1_BIT_LSB_IDX;        // set P10.1 push-pull general output
    P10_IOCR0.U |= 0x10 << PC2_BIT_LSB_IDX;        // set P10.2 push-pull general output
}

void initButton(void)
{
    P02_IOCR0.U &= ~(0x1F << PC1_BIT_LSB_IDX);       // reset P02_IOCR0 PC1

    P02_IOCR0.U |= 0x02 << PC1_BIT_LSB_IDX;          // set P02.1 general input (pull-up connected)
}

void initERU3(void) // ultrasonic
{
    // ERU setting
    SCU_EICR1.U &= ~(0x7 << EXIS1_BIT_LSB_IDX);
    SCU_EICR1.U |= (0x1  << EXIS1_BIT_LSB_IDX);     // ERS3 - Input 1

    SCU_EICR1.U |= 0x1   << FEN1_BIT_LSB_IDX;       // falling edge
    SCU_EICR1.U |= 0x1   << REN1_BIT_LSB_IDX;       // rising edge

    SCU_EICR1.U |= 0x1   << EIEN1_BIT_LSB_IDX;

    SCU_EICR1.U &= ~(0x7 << INP1_BIT_LSB_IDX);
    SCU_EICR1.U |= 0x1 << INP1_BIT_LSB_IDX;

    SCU_IGCR0.U &= ~(0x3 << IGP1_BIT_LSB_IDX);
    SCU_IGCR0.U |= 0x1   << IGP1_BIT_LSB_IDX;

    // SRC Interrupt setting
    SRC_SCU_SCU_ERU1.U &= ~(0xFF << SRPN_BIT_LSB_IDX);
    SRC_SCU_SCU_ERU1.U |= 0xFE   << SRPN_BIT_LSB_IDX;

    SRC_SCU_SCU_ERU1.U &= ~(0x3  << TOS_BIT_LSB_IDX);

    SRC_SCU_SCU_ERU1.U |= 1      << SRE_BIT_LSB_IDX;
}


void initERU2(void) // button
{
    // ERU setting
    SCU_EICR1.U &= ~(0x7 << EXIS0_BIT_LSB_IDX);
    SCU_EICR1.U |= (0x1  << EXIS0_BIT_LSB_IDX);     // ERS2 - Input 2

    SCU_EICR1.U |= 0x1   << FEN0_BIT_LSB_IDX;       // falling edgeW
//    SCU_EICR1.U |= 0x1   << REN0_BIT_LSB_IDX;       // rising edge

    SCU_EICR1.U |= 0x1   << EIEN0_BIT_LSB_IDX;

    SCU_EICR1.U &= ~(0x7 << INP0_BIT_LSB_IDX);

    SCU_IGCR0.U &= ~(0x3 << IGP0_BIT_LSB_IDX);
    SCU_IGCR0.U |= 0x1   << IGP0_BIT_LSB_IDX;

    // SRC Interrupt setting
    SRC_SCU_SCU_ERU0.U &= ~(0xFF << SRPN_BIT_LSB_IDX);
    SRC_SCU_SCU_ERU0.U |= 0xFF   << SRPN_BIT_LSB_IDX;

    SRC_SCU_SCU_ERU0.U &= ~(0x3  << TOS_BIT_LSB_IDX);

    SRC_SCU_SCU_ERU0.U |= 1      << SRE_BIT_LSB_IDX;
}

void initCCU60(void)
{
    // Password Access to unlock SCU_WDTSCON0
    SCU_WDTCPU0_CON0.U = ((SCU_WDTCPU0_CON0.U ^ 0xFC) & ~(1 << LCK_BIT_LSB_IDX)) | (1 << ENDINIT_BIT_LSB_IDX);
    while((SCU_WDTCPU0_CON0.U & (1 << LCK_BIT_LSB_IDX)) != 0);    // wait until unlocked

    // Modify Access to clear ENDINIT
    SCU_WDTCPU0_CON0.U = ((SCU_WDTCPU0_CON0.U ^ 0xFC) | (1 << LCK_BIT_LSB_IDX)) & ~(1 << ENDINIT_BIT_LSB_IDX);
    while((SCU_WDTCPU0_CON0.U & (1 << LCK_BIT_LSB_IDX)) == 0);    // wait until locked

    CCU60_CLC.U &= ~(1 << DISR_BIT_LSB_IDX);    // enable CCU

    // Password Access to unlock SCU_WDTSCON0
    SCU_WDTCPU0_CON0.U = ((SCU_WDTCPU0_CON0.U ^ 0xFC) & ~(1 << LCK_BIT_LSB_IDX)) | (1 << ENDINIT_BIT_LSB_IDX);
    while((SCU_WDTCPU0_CON0.U & (1 << LCK_BIT_LSB_IDX)) != 0);    // wait until unlocked

    // Modify Access to set ENDINIT
    SCU_WDTCPU0_CON0.U = ((SCU_WDTCPU0_CON0.U ^ 0xFC) | (1 << LCK_BIT_LSB_IDX)) | (1 << ENDINIT_BIT_LSB_IDX);
    while((SCU_WDTCPU0_CON0.U & (1 << LCK_BIT_LSB_IDX)) == 0);    // wait until locked

    // CCU60 T12 configurations
    while((CCU60_CLC.U & (1 << DISS_BIT_LSB_IDX)) != 0);// wait until CCU60 module enabled


    CCU60_TCTR0.U &= ~(0x7 << T12CLK_BIT_LSB_IDX);      // f_T12 = f_CCU6 / prescaler
    CCU60_TCTR0.U |= 0x2 << T12CLK_BIT_LSB_IDX;         // f_CCU6 = 50 MHz, prescaler = 4
    //CCU60_TCTR0.U |= 0x1 << T12PRE_BIT_LSB_IDX;         // f_T12 --> 12.5 MHz


    CCU60_TCTR0.U &= ~(0x1 << CTM_BIT_LSB_IDX);         // T12 auto reset when period match (PM) occur


    CCU60_T12PR.U = 125 -1;                       // PM interrupt freq. = f_T12 / (T12PR + 1)
    CCU60_TCTR4.U |= 0x1 << T12STR_BIT_LSB_IDX;         // load T12PR from shadow register


    CCU60_TCTR2.B.T12SSC = 0x1;                     // Single Shot Control


    CCU60_T12.U = 0;                                // clear T12 counter register


    // CCU60 T12 PM interrupt setting
    CCU60_INP.U &= ~(0x3 << INPT12_BIT_LSB_IDX);        // service request output SR0 selected
    CCU60_IEN.U |= 0x1 << ENT12PM_BIT_LSB_IDX;          // enable T12 PM interrupt


    // SRC setting for CCU60
    SRC_CCU6_CCU60_SR0.U &= ~(0xFF << SRPN_BIT_LSB_IDX);
    SRC_CCU6_CCU60_SR0.U |= 0x0B << SRPN_BIT_LSB_IDX;   // set priority 0x0B

    SRC_CCU6_CCU60_SR0.U &= ~(0x3 << TOS_BIT_LSB_IDX);  // CPU0 service T12 PM interrupt

    SRC_CCU6_CCU60_SR0.U |= 0x1 << SRE_BIT_LSB_IDX;     // SR0 enabled

    // T13 configurations
    CCU60_TCTR0.B.T13CLK = T13_CLK_FREQ_DIVIDER;        // f_CCU6 = 1.562500 MHz, 1562500 counters per sec
    CCU60_TCTR0.B.T13PRE = T13_PRESCALER;               // Prescaler disable
    CCU60_T13PR.B.T13PV = T13_INTERRUPT_FREQ - 1;       // interrupt every 0.01s
    CCU60_TCTR4.B.T13STR = 0x1;
    CCU60_T13.B.T13CV = 0x0;
    CCU60_IEN.B.ENT13PM = 0x1;
    CCU60_INP.B.INPT13 = 0x1;       // SR1
    SRC_CCU6_CCU60_SR1.B.SRPN = 0x0C;
    SRC_CCU6_CCU60_SR1.B.TOS = 0x0;
    SRC_CCU6_CCU60_SR1.B.SRE = 0x1;
    CCU60_TCTR4.B.T13RS = 0x1;
}

void initCCU61(void)
{
    // Password Access to unlock SCU_WDTSCON0
    SCU_WDTCPU0_CON0.U = ((SCU_WDTCPU0_CON0.U ^ 0xFC) & ~(1 << LCK_BIT_LSB_IDX)) | (1 << ENDINIT_BIT_LSB_IDX);
    while((SCU_WDTCPU0_CON0.U & (1 << LCK_BIT_LSB_IDX)) != 0);    // wait until unlocked

    // Modify Access to clear ENDINIT
    SCU_WDTCPU0_CON0.U = ((SCU_WDTCPU0_CON0.U ^ 0xFC) | (1 << LCK_BIT_LSB_IDX)) & ~(1 << ENDINIT_BIT_LSB_IDX);
    while((SCU_WDTCPU0_CON0.U & (1 << LCK_BIT_LSB_IDX)) == 0);    // wait until locked

    CCU61_CLC.U &= ~(1 << DISR_BIT_LSB_IDX);    // enable CCU

    // Password Access to unlock SCU_WDTSCON0
    SCU_WDTCPU0_CON0.U = ((SCU_WDTCPU0_CON0.U ^ 0xFC) & ~(1 << LCK_BIT_LSB_IDX)) | (1 << ENDINIT_BIT_LSB_IDX);
    while((SCU_WDTCPU0_CON0.U & (1 << LCK_BIT_LSB_IDX)) != 0);    // wait until unlocked

    // Modify Access to set ENDINIT
    SCU_WDTCPU0_CON0.U = ((SCU_WDTCPU0_CON0.U ^ 0xFC) | (1 << LCK_BIT_LSB_IDX)) | (1 << ENDINIT_BIT_LSB_IDX);
    while((SCU_WDTCPU0_CON0.U & (1 << LCK_BIT_LSB_IDX)) == 0);    // wait until locked

    // CCU60 T12 configurations
    while((CCU61_CLC.U & (1 << DISS_BIT_LSB_IDX)) != 0);// wait until CCU60 module enabled


    CCU61_TCTR0.U &= ~(0x7 << T12CLK_BIT_LSB_IDX);      // f_T12 = f_CCU6 / prescaler = 12.5 MHz
//    CCU61_TCTR0.U |= 0x7 << T12CLK_BIT_LSB_IDX;         // f_CCU6 = 50 MHz, prescaler = 128
    CCU61_TCTR0.U |= 0x2 << T12CLK_BIT_LSB_IDX;         // f_CCU6 = 50 MHz, prescaler = 4

    CCU61_TCTR0.U |= 0x1 << T12PRE_BIT_LSB_IDX;         // f_T12 = f_CCU6 / 256 = 48,828 Hz


    CCU61_T12PR.U = 100000 -1;                       // PM interrupt freq. = f_T12 / (T12PR + 1)
    CCU61_TCTR4.U |= 0x1 << T12STR_BIT_LSB_IDX;         // load T12PR from shadow register

    CCU61_T12.U = 0;                                // clear T12 counter register
}

void initRGBLED(void)
{
    // reset Port IOCR register
    P02_IOCR4.U &= ~(0x1F << PC7_BIT_LSB_IDX);
    P10_IOCR4.U &= ~(0x1F << PC5_BIT_LSB_IDX);
    P10_IOCR0.U &= ~(0x1F << PC3_BIT_LSB_IDX);

    // set Port as general purpose output (push-pull)
    P02_IOCR4.U |= 0x10 << PC7_BIT_LSB_IDX;
    P10_IOCR4.U |= 0x10 << PC5_BIT_LSB_IDX;
    P10_IOCR0.U |= 0x10 << PC3_BIT_LSB_IDX;
}

void initVADC(void)
{
    // Password Access to unlock SCU_WDTSCON0
    SCU_WDTCPU0_CON0.U = ((SCU_WDTCPU0_CON0.U ^ 0xFC) & ~(1 << LCK_BIT_LSB_IDX)) | (1 << ENDINIT_BIT_LSB_IDX);
    while((SCU_WDTCPU0_CON0.U & (1 << LCK_BIT_LSB_IDX)) != 0);    // wait until unlocked

    // Modify Access to clear ENDINIT
    SCU_WDTCPU0_CON0.U = ((SCU_WDTCPU0_CON0.U ^ 0xFC) | (1 << LCK_BIT_LSB_IDX)) & ~(1 << ENDINIT_BIT_LSB_IDX);
    while((SCU_WDTCPU0_CON0.U & (1 << LCK_BIT_LSB_IDX)) == 0);    // wait until locked

    VADC_CLC.U &= ~(1 << DISR_BIT_LSB_IDX);    // enable VADC

    // Password Access to unlock SCU_WDTSCON0
    SCU_WDTCPU0_CON0.U = ((SCU_WDTCPU0_CON0.U ^ 0xFC) & ~(1 << LCK_BIT_LSB_IDX)) | (1 << ENDINIT_BIT_LSB_IDX);
    while((SCU_WDTCPU0_CON0.U & (1 << LCK_BIT_LSB_IDX)) != 0);    // wait until unlocked

    // Modify Access to set ENDINIT
    SCU_WDTCPU0_CON0.U = ((SCU_WDTCPU0_CON0.U ^ 0xFC) | (1 << LCK_BIT_LSB_IDX)) | (1 << ENDINIT_BIT_LSB_IDX);
    while((SCU_WDTCPU0_CON0.U & (1 << LCK_BIT_LSB_IDX)) == 0);


    // VADC configurations
    while((VADC_CLC.U & (1 << DISS_BIT_LSB_IDX)) != 0); // wait until VADC module enabled

    VADC_G4_ARBPR.U |= 0x3 << PRIO0_BIT_LSB_IDX;         // highest priority for Request Source 0
    VADC_G4_ARBPR.U &= ~(0x1 << CSM0_BIT_LSB_IDX);       // Wait-for-Start Mode
    VADC_G4_ARBPR.U |= 0x1 << ASEN0_BIT_LSB_IDX;         // Arbitration Source Input 0 Enable

    VADC_G4_QMR0.U &= ~(0x3 << ENGT_BIT_LSB_IDX);
    VADC_G4_QMR0.U |= 0x1 << ENGT_BIT_LSB_IDX;          // enable conversion request
    VADC_G4_QMR0.U |= 0x1 << FLUSH_BIT_LSB_IDX;         // clear ADC queue

    VADC_G4_ARBCFG.U |= 0x3 << ANONC_BIT_LSB_IDX;       // ADC normal operation

    VADC_G4_ICLASS0.U &= ~(0x7 << CMS_BIT_LSB_IDX);     // Class 0 Standard Conversion (12-bit)


    // VADC Group 4 Channel 7 configuration
    VADC_G4_CHCTR7.U |= 0x1 << RESPOS_BIT_LSB_IDX;      // result right-aligned
    VADC_G4_CHCTR7.U &= ~(0xF << RESREG_BIT_LSB_IDX);   // store result @ Result Register G4RES0
    VADC_G4_CHCTR7.U &= ~(0x3 << ICLSEL_BIT_LSB_IDX);   // Class 0

    VADC_G4_CHASS.U |= 0x1 << 7;
}

void VADC_startConversion(void)
{
    VADC_G4_QINR0.U |= 0x07;                // no. of Request Channel = 7

    VADC_G4_QMR0.U |= 0x1 << TREV_BIT_LSB_IDX;  // Generate Conversion Start Trigger Event
}

unsigned int VADC_readResult(void)
{
    unsigned int result;

    while( (VADC_G4_RES0.U & (0x1 << VF_BIT_LSB_IDX)) == 0 );   // wait until read available
    result = VADC_G4_RES0.U & (0xFFF << RESULT_BIT_LSB_IDX);    // read ADC value

    return result;
}

void initGTM(void)
{
    // Password Access to unlock SCU_WDTSCON0
    SCU_WDTCPU0_CON0.U = ((SCU_WDTCPU0_CON0.U ^ 0xFC) & ~(1 << LCK_BIT_LSB_IDX)) | (1 << ENDINIT_BIT_LSB_IDX);
    while((SCU_WDTCPU0_CON0.U & (1 << LCK_BIT_LSB_IDX)) != 0);    // wait until unlocked

    // Modify Access to clear ENDINIT
    SCU_WDTCPU0_CON0.U = ((SCU_WDTCPU0_CON0.U ^ 0xFC) | (1 << LCK_BIT_LSB_IDX)) & ~(1 << ENDINIT_BIT_LSB_IDX);
    while((SCU_WDTCPU0_CON0.U & (1 << LCK_BIT_LSB_IDX)) == 0);    // wait until locked

    GTM_CLC.U &= ~(1 << DISR_BIT_LSB_IDX);    // enable GTM

    // Password Access to unlock SCU_WDTSCON0
    SCU_WDTCPU0_CON0.U = ((SCU_WDTCPU0_CON0.U ^ 0xFC) & ~(1 << LCK_BIT_LSB_IDX)) | (1 << ENDINIT_BIT_LSB_IDX);
    while((SCU_WDTCPU0_CON0.U & (1 << LCK_BIT_LSB_IDX)) != 0);    // wait until unlocked

    // Modify Access to set ENDINIT
    SCU_WDTCPU0_CON0.U = ((SCU_WDTCPU0_CON0.U ^ 0xFC) | (1 << LCK_BIT_LSB_IDX)) | (1 << ENDINIT_BIT_LSB_IDX);
    while((SCU_WDTCPU0_CON0.U & (1 << LCK_BIT_LSB_IDX)) == 0);

    while((GTM_CLC.U & (1 << DISS_BIT_LSB_IDX)) != 0); // wait until GTM module enabled


    // GTM clock configuration
    GTM_CMU_FXCLK_CTRL.U &= ~(0xF << FXCLK_SEL_BIT_LSB_IDX);  // input clock of CMU_FXCLK --> CMU_GCLK_EN
    GTM_CMU_CLK_EN.U |= 0x2 << EN_FXCLK_BIT_LSB_IDX;        // enable all CMU_FXCLK

    // set TGC0 to enable GTM TOM0 channel 1
    GTM_TOM0_TGC0_GLB_CTRL.U |= 0x2 << UPEN_CTRL1_BIT_LSB_IDX;  // TOM channel 1 enable
    GTM_TOM0_TGC0_ENDIS_CTRL.U |= 0x2 << ENDIS_CTRL1_BIT_LSB_IDX;   // enable channel 1 on update trigger
    GTM_TOM0_TGC0_OUTEN_CTRL.U |= 0x2 << OUTEN_CTRL1_BIT_LSB_IDX;   // enable channel 1 output on update trigger


    // set TGC0 to enable GTM TOM0 channel 2, 3, 15
    GTM_TOM0_TGC0_GLB_CTRL.B.UPEN_CTRL2 |= 0x2;  // TOM0 channel 2 enable
    GTM_TOM0_TGC0_GLB_CTRL.B.UPEN_CTRL3 |= 0x2;  // TOM0 channel 3 enable
    GTM_TOM0_TGC1_GLB_CTRL.B.UPEN_CTRL7 |= 0x2;  // TOM0 channel 15 enable

    GTM_TOM0_TGC0_ENDIS_CTRL.B.ENDIS_CTRL2 |= 0x2;   // enable channel 2 on update trigger
    GTM_TOM0_TGC0_ENDIS_CTRL.B.ENDIS_CTRL3 |= 0x2;   // enable channel 3 on update trigger
    GTM_TOM0_TGC1_ENDIS_CTRL.B.ENDIS_CTRL7 |= 0x2;   // enable channel 15 on update trigger

    GTM_TOM0_TGC0_OUTEN_CTRL.B.OUTEN_CTRL2 |= 0x2;   // enable channel 2 output on update trigger
    GTM_TOM0_TGC0_OUTEN_CTRL.B.OUTEN_CTRL3 |= 0x2;   // enable channel 3 output on update trigger
    GTM_TOM0_TGC1_OUTEN_CTRL.B.OUTEN_CTRL7 |= 0x2;   // enable channel 15 output on update trigger


    // TOM 0_1
    GTM_TOM0_CH1_CTRL.U |= 0x1 << SL_BIT_LSB_IDX;                   // high signal level for duty cycle

    GTM_TOM0_CH1_CTRL.U &= ~(0x7 << CLK_SRC_SR_BIT_LSB_IDX);
    GTM_TOM0_CH1_CTRL.U |= 0x1 << CLK_SRC_SR_BIT_LSB_IDX;           // clock source --> CMU_FXCLK(1) = 6250 kHz

    GTM_TOM0_CH1_SR0.U = 12500 - 1;                                 // PWM freq. = 6250 kHz / 12500 = 250 kHz
    GTM_TOM0_CH1_SR1.U = 1250 - 1;                                  // duty cycle = 6250 / 12500 = 50 %

    // TOM 0_2
    GTM_TOM0_CH2_CTRL.U |= 0x1 << SL_BIT_LSB_IDX;                   // high signal level for duty cycle

    GTM_TOM0_CH2_CTRL.U &= ~(0x7 << CLK_SRC_SR_BIT_LSB_IDX);
    GTM_TOM0_CH2_CTRL.U |= 0x1 << CLK_SRC_SR_BIT_LSB_IDX;           // clock source --> CMU_FXCLK(1) = 6250 kHz

    GTM_TOM0_CH2_SR0.U = 12500 - 1;                                 // PWM freq. = 6250 kHz / 12500 = 250 kHz
    //GTM_TOM0_CH2_SR1.U = 12500 - 1;                               // duty cycle = 6250 / 12500 = 50 %

    // TOM 0_3
    GTM_TOM0_CH3_CTRL.U |= 0x1 << SL_BIT_LSB_IDX;                   // high signal level for duty cycle

    GTM_TOM0_CH3_CTRL.U &= ~(0x7 << CLK_SRC_SR_BIT_LSB_IDX);
    GTM_TOM0_CH3_CTRL.U |= 0x1 << CLK_SRC_SR_BIT_LSB_IDX;           // clock source --> CMU_FXCLK(1) = 6250 kHz

    GTM_TOM0_CH3_SR0.U = 12500 - 1;                                 // PWM freq. = 6250 kHz / 12500 = 250 kHz
    //GTM_TOM0_CH3_SR1.U = 125 - 1;                                 // duty cycle = 6250 / 12500 = 50 %

    // TOM 0_15
    GTM_TOM0_CH15_CTRL.B.SL |= 0x1;                                 // high signal level for duty cycle
    GTM_TOM0_CH15_CTRL.B.CLK_SRC_SR |= 0x1;                         // clock source --> CMU_FXCLK(1) = 6250 kHz

    GTM_TOM0_CH15_SR0.U = 12500 - 1;                                // PWM freq. = 6250 kHz / 12500 = 250 kHz
    //GTM_TOM0_CH15_SR1.U = 125 - 1;                                // duty cycle = 6250 / 12500 = 50 %


    // TOUT pin selection
    GTM_TOUTSEL6.U &= ~(0x3 << SEL7_BIT_LSB_IDX);                   // TOUT103  --> TOM0 channel 1
    GTM_TOUTSEL0.U &= ~(0x3 << SEL7_BIT_LSB_IDX);                   // TOUT7    --> TOM0 channel 15
    GTM_TOUTSEL6.U &= ~(0x3 << SEL11_BIT_LSB_IDX);                  // TOUT103  --> TOM0 channel 2
    GTM_TOUTSEL6.U &= ~(0x3 << SEL9_BIT_LSB_IDX);                   // TOUT105  --> TOM0 channel 3





    // set GTM TOM0 channel 11 - Buzzer
    GTM_TOM0_TGC1_GLB_CTRL.B.UPEN_CTRL3     |= 0x2;                   // TOM0 channel 11 enable
    GTM_TOM0_TGC1_ENDIS_CTRL.B.ENDIS_CTRL3  |= 0x2;                   // enable channel 11 on update trigger
    GTM_TOM0_TGC1_OUTEN_CTRL.B.OUTEN_CTRL3  |= 0x2;                   // enable channel 11 output on update trigger


    // TOM 0_11
    GTM_TOM0_CH11_CTRL.B.SL = 0x1;                                  // high signal level for duty cycle
    GTM_TOM0_CH11_CTRL.B.CLK_SRC_SR = 0x1;                          // clock source --> CMU_FXCLK(1) = 6250 kHz
    GTM_TOM0_CH11_SR0.B.SR0 = 12500 - 1;                            // PWM freq. = 6250 kHz / 12500 = 500 Hz
    GTM_TOM0_CH11_SR1.B.SR1 = 6250 - 1;                             // duty cycle = 6250 / 12500 = 50 %

    // TOUT pin selection
    GTM_TOUTSEL0.B.SEL3 = 0x0;                                      // TOUT3  --> TOM0 channel 11
}

void initBuzzer(void)
{
//    P02_IOCR0.B.PC3 = 0x11;
}

void initUSonic(void)
{
    P02_IOCR4.U &= ~(0x1F << PC6_BIT_LSB_IDX);       // reset P02_IOCR4 PC6
    P14_IOCR0.U &= ~(0x1F << 11);       // reset P00_IOCR4 PC4

    P14_IOCR0.U |= 0x01 << 11;        // set P00.4 general input (pull-down connected) [Echo]
    P02_IOCR4.U |= 0x10 << PC6_BIT_LSB_IDX;        // set P02.6 push-pull general output            [Trig]

    P02_OUT.U &= ~(0x1 << P6_BIT_LSB_IDX);
}

void usonicTrigger(void)
{
    // start of 10us Trigger Pulse
    // GPIO P02.6 --> HIGH
    P02_OUT.U |= 0x1 << P6_BIT_LSB_IDX;
    range_valid_flag = 0;
    CCU60_TCTR4.U = 0x1 << T12RS_BIT_LSB_IDX;           // T12 start counting
}

unsigned int Range_LPF(int range)
{
    if (range_count < BUFF_SIZE)
    {
        range_buff[range_count] = range;
        range_count += 1;
    }

    if (range_count==BUFF_SIZE)
    {
        range_avg = 0;
        for(unsigned int i = 0; i<(BUFF_SIZE-1); i++)
        {
            range_buff[i] = range_buff[i+1];
            range_avg += range_buff[i];
        }
        range_buff[BUFF_SIZE-1] = range;
        range_avg += range;
        range_avg /= BUFF_SIZE;
        range = range_avg;
    }
    return range;
}

void initInputCommunicate(void)
{
    P14_IOCR0.U &= ~(0x1F << PC0_BIT_LSB_IDX);      // reset PIN24 - P15 6
    P15_IOCR4.U &= ~(0x1F << PC6_BIT_LSB_IDX);      // reset PIN24 - P15 6
    P00_IOCR8.U &= ~(0x1F << PC8_BIT_LSB_IDX);      // reset PIN26 - P00 8
    P00_IOCR8.U &= ~(0x1F << PC9_BIT_LSB_IDX);      // reset PIN28 - P00 9
    P00_IOCR8.U &= ~(0x1F << PC10_BIT_LSB_IDX);     // reset PIN30 - P00 10
    P00_IOCR8.U &= ~(0x1F << PC11_BIT_LSB_IDX);     // reset PIN32 - P00 10
    P00_IOCR12.U &= ~(0x1F << PC12_BIT_LSB_IDX);        // reset PIN34 - P00 10
    P33_IOCR0.U &= ~(0x1F << PC2_BIT_LSB_IDX);      // reset PIN36 - P00 10
    P33_IOCR0.U &= ~(0x1F << PC1_BIT_LSB_IDX);      // reset PIN38 - P00 10
    P33_IOCR0.U &= ~(0x1F << PC0_BIT_LSB_IDX);      // reset PIN40 - P00 10

    P14_IOCR0.U |= (0x00 << PC0_BIT_LSB_IDX);       // set PIN24 - P15 6 IN
    P15_IOCR4.U |= (0x00 << PC6_BIT_LSB_IDX);       // reset PIN24 - P15 6 IN
    P00_IOCR8.U |= (0x00 << PC8_BIT_LSB_IDX);       // reset PIN26 - P00 8 IN
    P00_IOCR8.U |= (0x00 << PC9_BIT_LSB_IDX);       // reset PIN28 - P00 9 IN
    P00_IOCR8.U |= (0x00 << PC10_BIT_LSB_IDX);      // reset PIN30 - P00 10 IN
    P00_IOCR8.U |= (0x00 << PC11_BIT_LSB_IDX);      // reset PIN32 - P00 10 IN
    P00_IOCR12.U |= (0x00 << PC12_BIT_LSB_IDX);     // reset PIN34 - P00 10 IN
    P33_IOCR0.U |= (0x00 << PC2_BIT_LSB_IDX);       // reset PIN36 - P00 10 IN
    P33_IOCR0.U |= (0x00 << PC1_BIT_LSB_IDX);       // reset PIN38 - P00 10 IN
    P33_IOCR0.U |= (0x00 << PC0_BIT_LSB_IDX);       // reset PIN40 - P00 10 IN
}

void initOutputCommunicate(void)
{
    P14_IOCR0.U &= ~(0x1F  << PC0_BIT_LSB_IDX);     // reset PIN24 - P15 6
    P15_IOCR4.U &= ~(0x1F  << PC6_BIT_LSB_IDX);     // reset PIN24 - P15 6
    P00_IOCR8.U &= ~(0x1F << PC8_BIT_LSB_IDX);     // reset PIN26 - P00 8
    P00_IOCR8.U &= ~(0x1F << PC9_BIT_LSB_IDX);     // reset PIN28 - P00 9
    P00_IOCR8.U &= ~(0x1F << PC10_BIT_LSB_IDX);     // reset PIN30 - P00 10
    P00_IOCR8.U &= ~(0x1F << PC11_BIT_LSB_IDX);// reset PIN32 - P00 10
    P00_IOCR12.U &= ~(0x1F << PC12_BIT_LSB_IDX);// reset PIN34 - P00 10
    P33_IOCR0.U &= ~(0x1F << PC2_BIT_LSB_IDX);// reset PIN36 - P00 10
    P33_IOCR0.U &= ~(0x1F << PC1_BIT_LSB_IDX);// reset PIN38 - P00 10
    P33_IOCR0.U &= ~(0x1F << PC0_BIT_LSB_IDX);// reset PIN40 - P00 10

    P14_IOCR0.U |= (0x10 << PC0_BIT_LSB_IDX);     // set PIN24 - P15 6 IN
    P15_IOCR4.U |= (0x10 << PC6_BIT_LSB_IDX);     // reset PIN24 - P15 6 IN
    P00_IOCR8.U |= (0x10 << PC8_BIT_LSB_IDX);     // reset PIN26 - P00 8 IN
    P00_IOCR8.U |= (0x10 << PC9_BIT_LSB_IDX);     // reset PIN28 - P00 9 IN
    P00_IOCR8.U |= (0x10 << PC10_BIT_LSB_IDX);     // reset PIN30 - P00 10 IN
    P00_IOCR8.U |= (0x10 << PC11_BIT_LSB_IDX);// reset PIN32 - P00 10 IN
    P00_IOCR12.U |= (0x10 << PC12_BIT_LSB_IDX);// reset PIN34 - P00 10 IN
    P33_IOCR0.U |= (0x10 << PC2_BIT_LSB_IDX);// reset PIN36 - P00 10 IN
    P33_IOCR0.U |= (0x10 << PC1_BIT_LSB_IDX);// reset PIN38 - P00 10 IN
    P33_IOCR0.U |= (0x10 << PC0_BIT_LSB_IDX);// reset PIN40 - P00 10 IN
}

void sendGPIOSignal(void)
{
        // send GPIO signal
        P10_OUT.U |= signal[0] << PC1_BIT_LSB_IDX;
        P14_OUT.U |= (signal[1]  << PC0_BIT_LSB_IDX);     // reset PIN24 - P15 6
        P15_OUT.U |= (signal[2]  << PC6_BIT_LSB_IDX);     // reset PIN24 - P15 6
        P00_OUT.U |= (signal[3] << PC8_BIT_LSB_IDX);     // reset PIN26 - P00 8
        P00_OUT.U |= (signal[4] << PC9_BIT_LSB_IDX);     // reset PIN28 - P00 9
        P00_OUT.U |= (signal[5] << PC10_BIT_LSB_IDX);     // reset PIN30 - P00 10
        P00_OUT.U |= (signal[6] << PC11_BIT_LSB_IDX);// reset PIN32 - P00 10
        P00_OUT.U |= (signal[7] << PC12_BIT_LSB_IDX);// reset PIN34 - P00 10
}