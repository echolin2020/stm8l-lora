#include "init.h"
#include "gpio.h"
#include <stdarg.h>

//---------------------------------------------------------------------------
//  �������ƣ�USART3_Init
//  ����˵��������3�Ĳ�������
//						����3��DMA����
//						����3��DMA��������ж�
//---------------------------------------------------------------------------



void Usart3_Init(void)
{
  /*
	USART_InitTypeDef USART_InitStructure;
					  
	USART_InitStructure.USART_BaudRate  =  9600; 
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
  USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
 	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
  USART_Init(USART3, &USART_InitStructure);
	
	USART_ITConfig(USART3,USART_IT_RXNE,ENABLE);//ʹ�ܽ����ж�
	USART_ITConfig(USART3,USART_IT_TXE,DISABLE);//��ֹ�����ж�

	USART_Cmd(USART3, ENABLE);
//	USART_ClearFlag(USART3,USART_FLAG_TC);
//	Gpio_485_RX3_En();
  */
}

