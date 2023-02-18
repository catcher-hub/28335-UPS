/*
 *  FileName:      main.c
 *  Describe:      2019�����B��--���಻��ϵ�Դ
 *  Creator:       YellowNew
 *  Encoding:      GBK
 *  Date:          2021.9.14
 */

/* ͷ�ļ����� */
#include "main.h"

/* ������������  */
// ��֧���ڵ�
void A0(void);
void B0(void);

// A��֧
void A1(void);								// ǰ��boost�ͺ������ЧֵADC�����˲�
void A2(void);								// ��˫��ѹ����PID

// B��֧
void B1(void);								// �ⶨOled��ʾ

/* ȫ�ֱ������� */
void (*Alpha_State_Ptr)(void);		        // ����״ָ̬�룬AB��֧��
void (*A_Task_Ptr)(void);				    // A�����֧����ָ��
void (*B_Task_Ptr)(void);				    // B�����֧����ָ��
interrupt void EPWM2_INT(void);


SPWM_VRB m_spwm;
ADC_VAB m_adc;
PID_DUNC m_PidBoost;
PID_DUNC m_PidInverter;

char Str[10];
//Uint16 Key_flg = 0;
int Key_flag = 0;

/* ������ */
int main(void)
    {
//TODO: ��ʼ��ϵͳ
 	InitSystem();

//TODO: ��ʼ��Ƭ������
	InitTaskTimer(TASKA_FRQ,TASKB_FRQ);
	InitEpwm();
	InitAdcToDMA();
 	UARTa_Init(115200);
    IICA_Init();
    OLED_Init();

//TODO: ������ʼ��
	Init_Spwm(&m_spwm);
	PidInit_Boost(&m_PidBoost);
	PidInit_Inverter(&m_PidInverter);
	setupLEDGPIO();

//TODO: ʹ��GPIO22�������ڼ��
	EALLOW;
	GpioCtrlRegs.GPAMUX2.bit.GPIO20 = 0;    // GPIO pin
	GpioCtrlRegs.GPADIR.bit.GPIO20 = 1;     // Output pin

    GpioCtrlRegs.GPAMUX2.bit.GPIO21 = 0;    // GPIO pin
    GpioCtrlRegs.GPADIR.bit.GPIO21 = 1;     // Output pin
	EDIS;

//TODO: OLED ��ʾ��ʼ��
    OLED_ColorTurn(0);
    OLED_ShowString(32,0,"YellowNew",16,1);
    OLED_ShowString(2,24,"Uac_out:",16,1);
    OLED_ShowString(2,48,"Udc_out:",16,1);

//TODO: ��ʼ������ָ��
	Alpha_State_Ptr = &A0;
	A_Task_Ptr = &A1;
	B_Task_Ptr = &B1;

	while(1)
	{
		// ״̬�����
		//====================================
		(*Alpha_State_Ptr)();	// ����ת (A0,B0,...)
		//====================================
	}
}

//===================================
 /*
  * ״̬������ڵ������붨����
  */
//====================================
void A0(void)
{
	// A����ͬ��ʱ��ѭ���ȴ�
	if(CpuTimer0Regs.TCR.bit.TIF == 1)
	{
		CpuTimer0Regs.TCR.bit.TIF = 1;	// clear flag

		//-----------------------------------------------------------
		(*A_Task_Ptr)();		// ��ת��A�����֧ (A1,A2,A3,...)
		//-----------------------------------------------------------
	}

	Alpha_State_Ptr = &B0;		// ��ȥB0
}

void B0(void)
{
	// A����ͬ��ʱ��ѭ���ȴ�
	if(CpuTimer1Regs.TCR.bit.TIF == 1)
	{
		CpuTimer1Regs.TCR.bit.TIF = 1;				// clear flag

		//-----------------------------------------------------------
		(*B_Task_Ptr)();		//��ת��B�����֧ (B1,B2,B3,...)
		//-----------------------------------------------------------

	}

	Alpha_State_Ptr = &A0;		// ��ȥA0
}

//-----------------------------------------------------------
 /*
  * A�����֧
  * A1���������ݴ���
  * A2��PID�ջ�����
  */
//-----------------------------------------------------------
void A1(void)
{
    register Uint16 i,j,t;
    register Uint16 *pYn_Adc_A0;
    register Uint16 *pYn_Adc_B0;
    register volatile Uint16 *P_DMABuf;
    pYn_Adc_B0 = m_adc.Yn_Adc_B0;
    pYn_Adc_A0 = m_adc.Yn_Adc_A0;
    P_DMABuf = DMABuf;

    while(DmaRegs.CH1.CONTROL.bit.RUNSTS);

//TODO: ��DMA�����������ó�������������һ��ת����
//TODO: ȡA0������
        for(i=0;i<(2*ADC_DMATRSIZE-1);i+=2)
        {
                *pYn_Adc_A0++ = *(P_DMABuf+i);
        }
//TODO: ȡB0������
        for(i=1;i<=(2*ADC_DMATRSIZE-1);i+=2)
        {
                *pYn_Adc_B0++ = *(P_DMABuf+i);
        }
//TODO: ��������DMA
    StartDMACH1();
//TODO: �����˲�
    pYn_Adc_A0 = m_adc.Yn_Adc_A0;
    pYn_Adc_B0 = m_adc.Yn_Adc_B0;

        for(i=0;i<ADC_DMATRSIZE-1;i++)  // ����FORѭ�� ��ɲ�������Ĵ�С��������
        for(j=0;j<ADC_DMATRSIZE-1-i;j++)
        {
            if(*(pYn_Adc_A0+j)>*(pYn_Adc_A0+j+1))
            {
                t=*(pYn_Adc_A0+j);
                *(pYn_Adc_A0+j)=*(pYn_Adc_A0);
                *(pYn_Adc_A0+j+1)=t;
            }

            if(*(pYn_Adc_B0+j)>*(pYn_Adc_B0+j+1))
            {
                t=*(pYn_Adc_B0+j);
                *(pYn_Adc_B0+j)=*(pYn_Adc_B0);
                *(pYn_Adc_B0+j+1)=t;
            }
        }
        m_adc.sumA0=0,m_adc.sumB0=0;
        for(i=0;i<10;i++)   //ȡ���м��ʮ����
        {
            m_adc.sumA0+=*(pYn_Adc_A0+i+10);
            m_adc.sumB0+=*(pYn_Adc_B0+i+10);
        }
        m_adc.Voltage_A0=(m_adc.sumA0/10.0*3.0/4096.0);    //���չ�ʽת����ѹ
        m_adc.Voltage_B0=(m_adc.sumB0/10.0*3.0/4096.0);    //���չ�ʽת����ѹ

#if SCOPE_FRE
            //TODO: ����Ƶ���Ƿ���ȷ
       GpioDataRegs.GPADAT.bit.GPIO20 = ~GpioDataRegs.GPADAT.bit.GPIO20;
#endif

	A_Task_Ptr = &A2;	// ��ȥA2
}

void A2(void)
{
//  Float_Printf(Str,m_spwm.M,1,3);
//  UARTa_SendString((char *)",");
//  Float_Printf(Str,m_PidInverter.ActualSpeed,1,3);
//  UARTa_SendString((char *)"\n");

//TODO: Boost �ջ�����
if (m_PidBoost.Soft_Start > Star_Flag)
    {m_PidBoost.Soft_Start--;}
else if (m_PidBoost.Soft_Start <= Star_Flag)
    {m_PidBoost.Soft_Start = 0;}

if(m_PidBoost.Soft_Start == Star_Flag)
{
    toggleLED();
    m_PidBoost.ActualSpeed = m_adc.Voltage_A0;
    PidRealize_Boost(&m_PidBoost);
    EPwm4Regs.CMPA.half.CMPA=m_PidBoost.OutPut;
}

# if DEBUG_BOOST

    Float_Printf(Str,m_PidBoost.SetSpeed,1,3);
    UARTa_SendString((char *)",");
    Float_Printf(Str,m_PidBoost.ActualSpeed,1,3);
    UARTa_SendString((char *)"\n");

# endif

if(m_PidBoost.Soft_Start == Star_Flag)
{
    if(m_spwm.Pid_Flag == 1)
    {
        if(m_adc.Voltage_B0 > 0.8 || Key_flag == 1)
        {
        if(m_adc.Voltage_B0 > 0.8) Key_flag = 1;
        m_PidInverter.ActualSpeed = m_adc.Voltage_B0;
        PidRealize_Inverter(&m_PidInverter);
        m_spwm.M = m_PidInverter.OutPut;
        Change_M(&m_spwm);
        m_spwm.Change_Flag =1;
        m_spwm.Pid_Flag = 0;

        }



    }
}

# if DEBUG_INVERTER

    Float_Printf(Str,m_PidInverter.SetSpeed,1,3);
    UARTa_SendString((char *)",");
    Float_Printf(Str,m_PidInverter.ActualSpeed,1,3);
    UARTa_SendString((char *)"\n");

# endif


	A_Task_Ptr = &A1; 	// ��ȥA1
}

//-----------------------------------------------------------
 /*
  * B�����֧
  * B1��
  */
//--------j---------------------------------------------------
void B1(void)
{
    float temp,temp1;
    temp = m_adc.Voltage_B0*32.137;
    temp1 = m_adc.Voltage_A0*39.44;

    Float2Str(Str,temp,2,2);
    OLED_ShowString(72,32,(unsigned char *)Str,8,1);
    Float2Str(Str,temp1,2,2);
    OLED_ShowString(72,56,(unsigned char *)Str,8,1);

//TODO: ����Ƶ���Ƿ���ȷ
# if SCOPE_FRE
    GpioDataRegs.GPADAT.bit.GPIO21 = ~GpioDataRegs.GPADAT.bit.GPIO21;
# endif
    B_Task_Ptr = &B1;
}

#pragma CODE_SECTION(EPWM2_INT,"ramfuncs");

interrupt void EPWM2_INT(void)
{



//
//   if(m_PidBoost.SPwm_Start == 0)
//   {
    Uint16 CCR_Date=m_spwm.CMPA_data[m_spwm.Counter++];

    if(m_spwm.Counter>=200)
    {
        m_spwm.Counter=0;
        m_spwm.Pid_Flag=1;
    }

    EPwm2Regs.CMPA.half.CMPA=CCR_Date;
    EPwm2Regs.CMPB=CCR_Date;

    EPwm3Regs.CMPA.half.CMPA=CCR_Date;
    EPwm3Regs.CMPB=CCR_Date;

//   }
   // Clear INT flag for this timer
   EPwm2Regs.ETCLR.bit.INT = 1;
   // Acknowledge this interrupt to receive more interrupts from group 3
   PieCtrlRegs.PIEACK.all = PIEACK_GROUP3;

}


