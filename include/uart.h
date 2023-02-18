/*
 * uart.h
 *
 *  Created on: 2021.3.27
 *      Author: huang'xin
 */

#ifndef UART_H_
#define UART_H_

#include "DSP2833x_Device.h"     // DSP2833x 头锟侥硷�?
#include "DSP2833x_Examples.h"   // DSP2833x 锟斤拷锟斤拷锟斤拷锟酵凤拷募锟�

extern int negative_number;

void UARTa_Init(Uint32 baud);
void UARTa_SendByte(int a);
void UARTa_SendString(char * msg);
void Float2Str(char* str,float floa,char len1,char len2);
void Int_Printf(char str[],int Num,char len);
unsigned long num_pow(char m,char n);
void Float_Printf(char str[],float floa,char len1,char len2);
#endif /* UART_H_ */
