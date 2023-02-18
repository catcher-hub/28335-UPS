/*
 * HX_Pid.h
 *
 *  Created on: 2021��9��14��
 *      Author: YellowNew
 */

#ifndef INCLUDE_HX_PID_H_
#define INCLUDE_HX_PID_H_
#include "main.h"

/* �ṹ������ */

//BoostPI
typedef struct
{
    float SetSpeed;
    float ActualSpeed;
    float err;
    float err_next;
    float err_last;
    float OutPut;
    float Kp,Ki,Kd;
    float incrementSpeed ;

    Uint16 Soft_Start;
    Uint16 IncrementRate;
    Uint16 SPwm_Start;

}PID_DUNC;

/* �궨�� */
#define Boost_Max 30
#define Boost_Min -30
#define Inverter_Min -0.05
#define Inverter_Max  0.05

/* �������� */
void PidInit_Boost(PID_DUNC *pid);
void PidRealize_Boost(PID_DUNC *pid);
void PidInit_Inverter(PID_DUNC *p);
void PidRealize_Inverter(PID_DUNC *p);

#endif /* INCLUDE_HX_PID_H_ */
