/*
 * Project_28335_InitSystem.c
 *
 *  Created on: 2021��9��14��
 *      Author: YellowNew
 */

/* ͷ�ļ����� */
#include "Project_28335_InitSystem.h"

/* ������ */
void InitSystem(void)
{
// ��ʼ��ϵͳ��
	InitSysCtrl();

// ������е��жϺͳ�ʼ��PIE������
	DINT;
	InitPieCtrl();
	IER = 0x0000;
	IFR = 0x0000;
	InitPieVectTable();
#if  ROM
	{
		MemCopy(&RamfuncsLoadStart, &RamfuncsLoadEnd, &RamfuncsRunStart);
		InitFlash();
	}
#endif
}
