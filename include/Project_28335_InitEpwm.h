/*
 * Project_28335_InitEpwm.h
 *
 *  Created on: 2021��9��14��
 *      Author: YellowNew
 */

#ifndef INCLUDE_PROJECT_28335_INITEPWM_H_
#define INCLUDE_PROJECT_28335_INITEPWM_H_

/* ͷ�ļ����� */
#include "main.h"

/*�궨��*/
#define PWM_CH 4

void InitEpwm(void);
void  Config_Epwm23();;
void Config_EPwm4(void);
interrupt void EPWM2_INT(void);

#endif /* INCLUDE_PROJECT_28335_INITEPWM_H_ */
