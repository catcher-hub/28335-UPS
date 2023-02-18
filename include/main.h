/*
 * main.h
 *
 *  Created on: 2021.3.27
 *      Author: huang'xin
 */

#ifndef INCLUDE_MAIN_H_
#define INCLUDE_MAIN_H_

/* ͷ�ļ����� */
#include "DSP28x_Project.h"
#include "Project_28335_InitSystem.h"
#include "Project_28335_TaskTimer.h"
#include "Project_28335_InitEpwm.h"
#include "Project_28335_Adc.h"
#include "Project_28335_LED.h"
#include "uart.h"
#include "oled.h"
#include "Hx_Spwm.h"
#include "HX_Pid.h"



// ������ִ�е�Ƶ��(Khz)
#define TASKA_FRQ    6
#define TASKB_FRQ    0.00125

// ģʽ����
#define DEBUG_BOOST 1
#define DEBUG_INVERTER 0
#define SCOPE_FRE 1
#define Star_Flag 0

// DMAĿ���ַ�ռ��С
#define BUF_SIZE     922

// ��д����ģʽ��ROM/RAM
#define ROM    1
#define ADC_DMATRSIZE 30
#define multiple 20.0
#define Verf 2.0

/* �ṹ�� */
typedef struct
{
    Uint16 Yn_Adc_A0[ADC_DMATRSIZE];   // a0�Ĳ����� -> boost�����ѹ
    Uint16 Yn_Adc_B0[ADC_DMATRSIZE];   //   �Ĳ����� -> ��������ѹ��Чֵ
    float Voltage_A0;
    float Voltage_B0;
    Uint16 sumA0;
    Uint16 sumB0;
}ADC_VAB;


/* ȫ�ֱ���������  */
extern volatile Uint16 DMABuf[BUF_SIZE];

#endif
