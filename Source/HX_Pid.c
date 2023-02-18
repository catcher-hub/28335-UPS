/*
 * HX_Pid.c
 *
 *  Created on: 2021��9��14��
 *      Author: YellowNew
 */


#include "HX_Pid.h"

/* ������ */
void PidInit_Boost(PID_DUNC *pid)
{
    pid->ActualSpeed = 1.61;  // ʵ��ֵ������ǰ������ֵ      12V���0.845      1A���0.45
    pid->SetSpeed = 1.61;     // �趨ֵ �趨ֵ 63V -> 1.61V
    pid->err = 0.0;          // ����ʽpid�����Ϊ�趨ֵ-ʵ��ֵ
    pid->err_last = 0.0;     // ��һ������� ��e(k-1)
    pid->err_next = 0.0;     // ����������� ��e(k-2)
    pid->OutPut = 2265;      // PWM���ڿ�����ʵ�����ֵ
    pid->Kp = 0.9000;        // Kp
    pid->Ki = 0.4500;        // Ki
    pid->Kd = 0.0000;        // Kd ͨ���£�����ʽpidֻ��kp��ki���㹻�ˣ�����ʽpid�ȵ�ki,���kp
    pid->incrementSpeed = 0.0;

    pid->Soft_Start = 500;
    pid->IncrementRate= 130;    //�Ŵ�ϵ�� �ѵ�ѹ������ֵ��ռ�ձȵıȽ�ֵ��ϵ������
    pid->SPwm_Start = 800;
}

void PidRealize_Boost(PID_DUNC *pid)
{
    pid->incrementSpeed = 0;

    pid->err=pid->SetSpeed-pid->ActualSpeed;
    pid->incrementSpeed=pid->Kp*(pid->err-pid->err_last)+pid->Ki*pid->err;
    pid->incrementSpeed*=pid->IncrementRate;

    pid->OutPut = (pid->OutPut <= 5000) ? (pid->OutPut >= 20)? pid->OutPut : 20 :5000;

    if(pid->OutPut>=20 && pid->OutPut<=5000 )
    {
        pid->incrementSpeed = (pid->incrementSpeed<=Boost_Max) ? (pid->incrementSpeed>=Boost_Min) ? pid->incrementSpeed : Boost_Min : Boost_Max ;
        pid->OutPut+=pid->incrementSpeed;
    }

    pid->err_last=pid->err;
    pid->err_next=pid->err_last;
}

void PidInit_Inverter(PID_DUNC *p)
{
    p->incrementSpeed = 0.0;
    p->ActualSpeed=0.9324;  //ʵ��ֵ Ҳ���ǵ�ǰ������ֵ
    p->SetSpeed=0.9324;     //�趨ֵ
    p->err = 0.0;         //��� SetSpeed-ActualSpeed
    p->err_last = 0.0;    //��һ������� ��e(k-1)
    p->err_next = 0.0;    //�����������     e(k-2)
    p->OutPut=0.675;       //ʵ�����
    p->Kp = 0.245;         // 0.275 0.58
    p->Ki = 0.00232;//0.0025;     // 0.025 0.000845
    p->Kd = 0;
}

//void PidRealize_Inverter(PID_DUNC *p)
//{
//
//    p->incrementSpeed=0;
//
//    p->err=p->SetSpeed-p->ActualSpeed;    // ����ʽPID
//
//    p->incrementSpeed=p->Kp*(p->err-p->err_last)+p->Ki*p->err;
//
//    if((p->OutPut)>=0.3&&(p->OutPut)<=0.9 || p->OutPut<0.3&&p->incrementSpeed>0 || p->OutPut>0.9&&p->incrementSpeed<0)   // ��ֹռ�ձȹ����С�������عܣ����Ʊ仯�ٶ�
//    {
//
//        p->incrementSpeed = (p->incrementSpeed < Inverter_Max) ? (p->incrementSpeed > Inverter_Min) ? p->incrementSpeed : Inverter_Min : Inverter_Max;
//
//        p->OutPut+=p->incrementSpeed;
//    }
//
//       p->err_last=p->err;
//       p->err_next=p->err_last;
//}

void PidRealize_Inverter(PID_DUNC *p)
{
    p->incrementSpeed=0;

    p->err=p->SetSpeed-p->ActualSpeed;    // ����ʽPID

//    incrementSpeed=p->Ki*p->err+p->Kd*(p->err-2*p->err_next+p->err_last);
    p->incrementSpeed=p->Kp*(p->err-p->err_last)+p->Ki*p->err+p->Kd*(p->err-2*p->err_last+p->err_next);

    if((p->OutPut)>=0.3&&(p->OutPut)<=0.9 || p->OutPut<0.3&&p->incrementSpeed>0 || p->OutPut>0.9&&p->incrementSpeed<0)   // ��ֹռ�ձȹ����С�������عܣ����Ʊ仯�ٶ�
    {
        if(p->incrementSpeed>=0.006)
        {
            p->incrementSpeed=0.006;
        }
        if(p->incrementSpeed<=-0.006)
        {
            p->incrementSpeed=-0.006;
        }

        p->OutPut+=p->incrementSpeed;
    }

    p->err_last=p->err;
    p->err_next=p->err_last;
}
