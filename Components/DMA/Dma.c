/******************************************************************************************
���ϱ�����ӿƼ����޹�˾                                             
������Ϊ���ϱ�����ӿƼ����޹�˾��Sx1278ģ����Դ���,����Sx1278�ٷ������޸ģ�            
Ϊ�����˱���˾Sx1278ϵ��ģ��Ŀͻ�����ģ��������ܣ��ͻ����Ը����Լ�����Ҫ�������������� 
ϵͳ�������δ����ɣ���������������;��
�����Գ���Ϊloraģʽ���ͳ���ģ���ϵ�󽫰�һ��Ƶ�ʷ������ݰ������նˣ����ն˽��յ����ݰ���ͨ�����ڽ����ݷ��͸���λ����ͬʱ����LED����˸1�Ρ�
CPU: STM8L151F3P6      ����ƽ̨IARForStm V1.4
File:main.c   
�汾��V1.0
�������ڣ�2017��11��01��   �޸����ڣ�2017.12.17
���ߣ�������
��ϵ���䣺dan772474868@qq.com ��ϵQQ��772474868
Copyright (C)  SoSharp  ���ϱ�����ӿƼ����޹�˾                                                              
All rights send                                                                    
******************************************************************************************/
#include "DMA.h"

//DMA1�ĸ�ͨ������
//����Ĵ�����ʽ�ǹ̶���,���Ҫ���ݲ�ͬ��������޸�
//�Ӵ洢��->����ģʽ/8λ���ݿ��/�洢������ģʽ
//DMA_CHx:DMAͨ��CHx
//cpar:�����ַ
//cmar:�洢����ַ
//cndtr:���ݴ����� 
void MYDMA_Config(DMA_Channel_TypeDef* DMA_CHx,u32 RxBuffer,u16 cndtr)  
{         
    CLK_PeripheralClockConfig(CLK_Peripheral_DMA1, ENABLE);    //��ʱ�ӣ�����Ҫ  
    /* Deinitialize DMA channels */  
    DMA_GlobalDeInit();  
   
  //  DMA_DeInit(DMA1_Channel1);  
    DMA_DeInit(DMA_CHx);  //��DMA��ͨ��1�Ĵ�������Ϊȱʡֵ
      
    /* DMA channel Rx of USART Configuration */  
    //�ú�����ҪҪ���úý��ܵ����飬�Լ�USART�����ݼĴ�����ַ�������С���Լ�DMAģʽ  
    DMA_Init(DMA_CHx, 
             (uint16_t)RxBuffer, 
             (uint16_t)0x5231,   
             cndtr,
             DMA_DIR_PeripheralToMemory, 
             DMA_Mode_Normal,   //��������������ģʽ
             DMA_MemoryIncMode_Inc,
             DMA_Priority_Low, //DMAͨ�� xӵ�������ȼ� 
             DMA_MemoryDataSize_Byte);  //���ݿ��Ϊ8λ
   
    /* DMA channel Tx of USART Configuration */  
    //�ú�����Ҫ���÷������飬�Լ�USART�����ݼĴ�����ַ�������С���Լ�DMAģʽ  
   // DMA_Init(DMA1_Channel1, (uint16_t)TxBuffer, (uint16_t)0x5231, \  
   //          60, DMA_DIR_MemoryToPeripheral, DMA_Mode_Normal, \  
   //          DMA_MemoryIncMode_Inc, DMA_Priority_High, DMA_MemoryDataSize_Byte);  
      
    /* Enable the USART Tx/Rx DMA requests */  
   // USART_DMACmd(USART1, USART_DMAReq_TX, ENABLE);  
    USART_DMACmd(USART1, USART_DMAReq_RX, ENABLE);  
   
    /* Global DMA Enable */  
    DMA_GlobalCmd(ENABLE);  
   
    /* Enable the USART Tx DMA channel */  
   // DMA_Cmd(DMA1_Channel1, ENABLE);  
    /* Enable the USART Rx DMA channel */  
   // DMA_Cmd(DMA1_Channel2, ENABLE); 
    MYDMA_Enable(DMA1_Channel2);   
}   


//����һ��DMA����
void MYDMA_Enable(DMA_Channel_TypeDef*DMA_CHx)
{ 
	DMA_Cmd(DMA_CHx, ENABLE);       
}
//�ر�һ��DMA����
void MYDMA_Disable(DMA_Channel_TypeDef*DMA_CHx)
{
	DMA_Cmd(DMA_CHx, DISABLE); 
}

/*--------------------------------------------------------------------------------------------------------
                   							                  0ooo
                   								ooo0     (   )
                								(   )     ) /
                								 \ (     (_/
                								  \_)
----------------------------------------------------------------------------------------------------------*/


