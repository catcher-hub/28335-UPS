/*
 * Project_28335_Adc.c
 *
 *  Created on: 2021��9��14��
 *      Author: YellowNew
 */

/* ͷ�ļ����� */
#include "Project_28335_Adc.h"

#pragma DATA_SECTION(DMABuf,"DMARAML4");

volatile Uint16 DMABuf[BUF_SIZE];

volatile Uint16 *DMADest;
volatile Uint16 *DMASource;


void InitAdcToDMA(void)
{
    Uint16 i;

    EALLOW;
    SysCtrlRegs.HISPCP.all = ADC_MODCLK;
    EDIS;

    InitAdc();
    AdcRegs.ADCTRL1.bit.ACQ_PS = ADC_SHCLK;                 // 4��ʱ�Ӳ�����
    AdcRegs.ADCTRL3.bit.ADCCLKPS = ADC_CKPS;                // ����Ƶ
    AdcRegs.ADCTRL1.bit.SEQ_CASC = 0;                       // ˫������ģʽ
    AdcRegs.ADCTRL3.bit.SMODE_SEL = 1;                      // ͬ������ģʽ
    AdcRegs.ADCCHSELSEQ1.bit.CONV00 = 0x00;                 // ͬ������A0��B0
    AdcRegs.ADCMAXCONV.all = 0x0000;                        // ���ͨ����2

    AdcRegs.ADCTRL2.bit.EPWM_SOCA_SEQ1 = 1;                 // Enable SOCA from ePWM to start SEQ1
    AdcRegs.ADCTRL2.bit.INT_ENA_SEQ1 = 1;                   // Enable SEQ1 interrupt (every EOS)
    AdcRegs.ADCTRL2.bit.SOC_SEQ1 = 1;                       // ��ʼ



// Initialize DMA
    DMAInitialize();

    for (i=0; i<BUF_SIZE; i++)
    {
        DMABuf[i] = 0;
    }

// Configure DMA Channel
    DMADest   = &DMABuf[0];                            // Point DMA destination to the beginning of the array
    DMASource = &AdcMirror.ADCRESULT0;
    DMACH1AddrConfig(DMADest,DMASource);
    DMACH1BurstConfig(1,1,1);                           // ͬ������ͨ��A0��A1��ֵ��һ֡����ֵ��
    DMACH1TransferConfig((ADC_DMATRSIZE-1),1,1);        // ��ADC_DMATRSIZE֡����,����0~(2ADC_DMATRSIZE-1)
    DMACH1WrapConfig(0,0,0xFFFF,0);                     // ÿ8�η��ص�����Ĵ���0�ĵ�ַ��
    DMACH1ModeConfig                                    // �ֶ�����ģʽ��һ��ֻ����2ADC_DMATRSIZE������
        (DMA_SEQ1INT,
        PERINT_ENABLE,
        ONESHOT_DISABLE,
        CONT_DISABLE,
        SYNC_DISABLE,
        SYNC_SRC,
        OVRFLOW_DISABLE,
        SIXTEEN_BIT,
        CHINT_END,
        CHINT_DISABLE);

    StartDMACH1();
}
