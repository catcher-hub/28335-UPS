/*
 * oled.c
 *
 *  Created on: 2021��5��5��
 *      Author: ����
 *
 *  ********************************************************************************************
 *  @version           V1.0
 *  @date               2021-xx-xx
 *  @brief               oled��ʾ�����ӿ� SCL ->33 SDA->32
 *  *******************************************************************************************
 */

#include "oledfont.h"
#include "oled.h"



unsigned char OLED_GRAM[128][8];//OLED���Դ�������


//����OLED��ʾ
void OLED_Display_On(void)
{
    OLED_WR_Byte(0X8D,OLED_CMD);  /* ���ó��ã����¸�������ʹ�ã� */
    OLED_WR_Byte(0X14,OLED_CMD);  /* 0x14 ʹ�ܳ��ã� 0x10 �ǹر� */
    OLED_WR_Byte(0XAF,OLED_CMD);   /* ��OLED��� */
}
//�ر�OLED��ʾ
void OLED_Display_Off(void)
{
    OLED_WR_Byte(0X8D,OLED_CMD);   /* ���ó��ã����¸�������ʹ�ã�*/
    OLED_WR_Byte(0X10,OLED_CMD);  /* 0x14 ʹ�ܳ��ã�0x10 �ǹر� */
    OLED_WR_Byte(0XAE,OLED_CMD);   /* �ر�OLED��� */
}

//m^n����
Uint32 oled_pow(unsigned char m,unsigned char n)
{
    Uint32 result=1;
    while(n--)result*=m;
    return result;
}
/*
 *��ʾ����
 *x,y :�������
 *num:Ҫ��ʾ������
 *len :���ֵ�λ��
 *sizey:����Ĵ�С
 *mode:0������ʾ��1������ʾ
 */
void OLED_ShowNum(unsigned char x,unsigned char y,Uint32 num,unsigned char len,unsigned char size1,unsigned char mode)
{
    unsigned char t,temp,m=0;
    if(size1==8)m=2;
    for(t=0;t<len;t++)
    {
        temp=(num/oled_pow(10,len-t-1))%10;
            if(temp==0)
            {
                OLED_ShowChar(x+(size1/2+m)*t,y,'0',size1,mode);
      }
            else
            {
              OLED_ShowChar(x+(size1/2+m)*t,y,temp+'0',size1,mode);
            }
  }
    OLED_Refresh();
}

/*
 * ��ָ����λ����ʾһ���ַ������������ַ�
 * x:0~127
 * y:0~63
 * chr��Ҫ��ʾ���ַ�
 * sizey:ѡ������ 6��8 6��12 8��16  12��24
 * mode:0������ʾ��1������ʾ
 */
void OLED_ShowChar(unsigned char x,unsigned char y,unsigned char chr,unsigned char size1,unsigned char mode)
{
    unsigned char i,m,temp,size2,chr1;
    unsigned char x0=x,y0=y;
    if(size1==8)size2=6;
    else size2=(size1/8+((size1%8)?1:0))*(size1/2);  //�õ�һ���ַ���Ӧ������ռ�õ��ֽ���
    chr1=chr-' ';  //����ƫ�ƺ��ֵ
    for(i=0;i<size2;i++)
    {
        if(size1==8)
              {temp=asc2_0806[chr1][i];} //����0806����
//        else if(size1==12)
//        {temp=asc2_1206[chr1][i];} //����1206����
        else if(size1==16)
        {temp=asc2_1608[chr1][i];} //����1608����
//        else if(size1==24)
//        {temp=asc2_2412[chr1][i];} //����2412����
        else return;
        for(m=0;m<8;m++)
        {
            if(temp&0x01)OLED_DrawPoint(x,y,mode);
            else OLED_DrawPoint(x,y,!mode);
            temp>>=1;
            y++;
        }
        x++;
        if((size1!=8)&&((x-x0)==size1/2))
        {x=x0;y0=y0+8;}
        y=y0;
  }
}


/*
 * ��ʾ�ַ���
 * x,y:�������
 * size1:�����С
 * *chr:�ַ�����ʼ��ַ
 * mode:0������ʾ��1������ʾ
 */
void OLED_ShowString(unsigned char x,unsigned char  y,unsigned char  *chr,unsigned char  size1,unsigned char mode)
{
    while((*chr>=' ')&&(*chr<='~'))//�ж��ǲ��ǷǷ��ַ�
        {
            OLED_ShowChar(x,y,*chr,size1,mode);
            if(size1==8)x+=6;
            else x+=size1/2;
            chr++;
      }
    OLED_Refresh();
}

/*
 *���Ժ���
 *i:0 ������ʾ��1��ɫ��ʾ
 */
void OLED_ColorTurn(unsigned char i)
{
    if(i==0)
        {
            OLED_WR_Byte(0xA6,OLED_CMD);
        }
    if(i==1)
        {
            OLED_WR_Byte(0xA7,OLED_CMD);
        }
}
/*
*********************************************************************************************************
*        �� �� ��: OLED_SetContrast
*        ����˵��: ���öԱȶ�
*        ��    ��:  ��
*        �� �� ֵ: ��
*********************************************************************************************************
*/
void OLED_SetContrast(unsigned char ucValue)
{
    OLED_WR_Byte(0x81,OLED_CMD);        /* ���öԱȶ�����(˫�ֽ��������1���ֽ��������2���ֽ��ǶԱȶȲ���0-255 */
    OLED_WR_Byte(ucValue,OLED_CMD);     /* ���öԱȶȲ���,ȱʡCF */
}

/*
 * ��Ļ��ת180��
 * i:0������ʾ��1��ת��ʾ
 */
void OLED_DisplayTurn(unsigned char i)
{
    if(i==0)
        {
            OLED_WR_Byte(0xC8,OLED_CMD); /* C0 ������ɨ��,��COM0��COM63;  C8 : ����ɨ��, �� COM63�� COM0 */
            OLED_WR_Byte(0xA1,OLED_CMD); /* A0 ���е�ַ0ӳ�䵽SEG0; A1 ���е�ַ127ӳ�䵽SEG0 */
        }
    if(i==1)
        {
            OLED_WR_Byte(0xC0,OLED_CMD); /* C0 ������ɨ��,��COM0��COM63;  C8 : ����ɨ��, �� COM63�� COM0 */
            OLED_WR_Byte(0xA0,OLED_CMD);/* A0 ���е�ַ0ӳ�䵽SEG0; A1 ���е�ַ127ӳ�䵽SEG0 */
        }
}

void OLED_Init(void)
{

    OLED_WR_Byte(0xAE,OLED_CMD);/*�ر�OLED�����ʾ�����ߣ�*/
    OLED_WR_Byte(0x00,OLED_CMD);/*�����е�ַ��4bit*/
    OLED_WR_Byte(0x10,OLED_CMD);/*�����е�ַ��4bit*/
    OLED_WR_Byte(0x40,OLED_CMD);/*������ʼ�е�ַ����5bit 0-63���� Ӳ�����*/
    OLED_WR_Byte(0x81,OLED_CMD);/* ���öԱȶ�����(˫�ֽ��������1���ֽ��������2���ֽ��ǶԱȶȲ���0-255 */
    OLED_WR_Byte(0xCF,OLED_CMD);/* ���öԱȶȲ���,ȱʡCF */
    OLED_WR_Byte(0xA1,OLED_CMD);/* A0 ���е�ַ0ӳ�䵽SEG0; A1 ���е�ַ127ӳ�䵽SEG0 */
    OLED_WR_Byte(0xC8,OLED_CMD);/* C0 ������ɨ��,��COM0��COM63;  C8 : ����ɨ��, �� COM63�� COM0 */
    OLED_WR_Byte(0xA6,OLED_CMD); /* A6 : ����������ʾģʽ; A7 : ����Ϊ����ģʽ */
    OLED_WR_Byte(0xA8,OLED_CMD); /* ����COM·�� */
    OLED_WR_Byte(0x3f,OLED_CMD); /* 1 ->��63+1��· */
    OLED_WR_Byte(0xD3,OLED_CMD); /* ������ʾƫ�ƣ�˫�ֽ����*/
    OLED_WR_Byte(0x00,OLED_CMD); /* ��ƫ�� */
    OLED_WR_Byte(0xd5,OLED_CMD);/* ������ʾʱ�ӷ�Ƶϵ��/��Ƶ�� */
    OLED_WR_Byte(0x80,OLED_CMD);/* ���÷�Ƶϵ��,��4bit�Ƿ�Ƶϵ������4bit����Ƶ�� */
    OLED_WR_Byte(0xD9,OLED_CMD); /* ����Ԥ������� */
    OLED_WR_Byte(0xF1,OLED_CMD); /* [3:0],PHASE 1; [7:4],PHASE 2; */
    OLED_WR_Byte(0xDA,OLED_CMD); /* ����COM��Ӳ�����߷�ʽ */
    OLED_WR_Byte(0x12,OLED_CMD);
    OLED_WR_Byte(0xDB,OLED_CMD); /* ���� vcomh ��ѹ���� */
    OLED_WR_Byte(0x40,OLED_CMD); /* [6:4] 000 = 0.65 x VCC; 0.77 x VCC (RESET); 0.83 x VCC  */
    OLED_WR_Byte(0x20,OLED_CMD); /*������Ѱַģʽ������0x20�������0x02Ϊȷ��ҳѰַģʽ*/
    OLED_WR_Byte(0x02,OLED_CMD);
    OLED_WR_Byte(0x8D,OLED_CMD);/* ���ó��ã����¸�������ʹ�ã� */
    OLED_WR_Byte(0x14,OLED_CMD);/* 0x14 ʹ�ܳ��ã� 0x10 �ǹر� */
    OLED_WR_Byte(0xA4,OLED_CMD);
    OLED_WR_Byte(0xA6,OLED_CMD);
    OLED_WR_Byte(0x2E,OLED_CMD);/*���ù���*/
    OLED_Clear();//����
    OLED_WR_Byte(0xAF,OLED_CMD); /* ��OLED��� */
}
/*
 * ����һ���ֽ�
 * mode������/���� 0Ϊ���ݣ�1��ʾ����
 */
void OLED_WR_Byte(unsigned char dat,unsigned char mode)
{
    IIC_Start();
    IIC_Send_Byte(0x78);//���ʹӻ���ַ
    IIC_Wait_Ack();
    if(mode){IIC_Send_Byte(0x40);}//0x40��д���� Co=0 D/C=1
  else{IIC_Send_Byte(0x00);}//0x00д����Co=0 D/C=0
    IIC_Wait_Ack();
    IIC_Send_Byte(dat);
    IIC_Wait_Ack();
    IIC_Stop();
}

/*
 * ��������Դ�
 */
void OLED_Clear(void)
{
    unsigned char i,n;
        for(i=0;i<8;i++)
        {
           for(n=0;n<128;n++)
                {
                 OLED_GRAM[n][i]=0;//�����������
                }
      }
        OLED_Refresh();//������ʾ
}
//����
//x:0~127
//y:0~63
//t:1 ������ʾ 0,������ʲô���ݶ�������
void OLED_DrawPoint(unsigned char x,unsigned char y,unsigned char t)
{
    unsigned char i,m,n;
    i=y/8;//ȷ������һҳ
    m=y%8;//ȷ������һҳ����һλ
    n=0x01<<m;//ȷ������һҳ��ʾ��1����ƫ��mλ����
    if(x>127||y>63)return;//������Χ��
    if(t){OLED_GRAM[x][i]|=n;}
    else
    {
        OLED_GRAM[x][i]=~OLED_GRAM[x][i];//��iҳ�ĵ�x�еİ�λ����ȥ��Ҳ����0  1����
        OLED_GRAM[x][i]|=n;//ȡ���󽫴�λ�õ�������1
        OLED_GRAM[x][i]=~OLED_GRAM[x][i];//�ڽ���ȥ��
        /*��������д������þ���������ʲô���ݶ�������*/
    }
}


//������ʾ��OLED
void OLED_Refresh(void)
{
    unsigned char i,n;
    for(i=0;i<8;i++)
    {
        OLED_WR_Byte(0xb0+i,OLED_CMD);//����ҳ��ַ��0~7��
        OLED_WR_Byte(0x00,OLED_CMD);/*�����е�ַ��4bit*/
        OLED_WR_Byte(0x10,OLED_CMD);/*�����е�ַ��4bit*/
        IIC_Start();
        IIC_Send_Byte(0x78);//�ӻ���ַ
        IIC_Wait_Ack();
        IIC_Send_Byte(0x40);//д����
        IIC_Wait_Ack();
        for(n=0;n<128;n++)
        {
            IIC_Send_Byte(OLED_GRAM[n][i]);
            IIC_Wait_Ack();
        }
        IIC_Stop();
  }
}


