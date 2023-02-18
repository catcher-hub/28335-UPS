/*
 * Project_28335_InitTaskTimer.c
 *
 *  Created on: 2021��9��14��
 *      Author: YellowNew
 */


#include "Project_28335_TaskTimer.h"

float CpuTimer0Period,CpuTimer1Period;

// �βε�λKhz
void InitTaskTimer(long double TaskAFrq,long double TaskBFrq)
{


    CpuTimer0Period = 1000/TaskAFrq;
    CpuTimer1Period = 1000/TaskBFrq;

    InitCpuTimers();

// ������
    ConfigCpuTimer(&CpuTimer0, 150, CpuTimer0Period);
    ConfigCpuTimer(&CpuTimer1, 150, CpuTimer1Period);

// ������ʱ��
    CpuTimer0Regs.TCR.all = 0x4001; // Use write-only instruction to set TSS bit = 0
    CpuTimer1Regs.TCR.all = 0x4001; // Use write-only instruction to set TSS bit = 0
}
