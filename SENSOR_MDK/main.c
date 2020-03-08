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
#include "stm8l15x.h"
#include "stm8l15x_it.h"
 #include "stm8l15x_conf.h"
//Sys
#include "delay.h"
#include "rtc.h"
//Comp

#include "usart.h"
#include "sprintf.h"
#include "stm8l15x_exti.h"
#include "SX1278.h"
#include "main.h"





int main(void)
{
    disableInterrupts();
    Usart_Init();
    GPIO_Init(GPIOE, GPIO_Pin_7, GPIO_Mode_Out_PP_Low_Fast);
    GPIO_Init(GPIOC, GPIO_Pin_7, GPIO_Mode_Out_PP_Low_Fast);
    GPIO_Init(GPIOC, GPIO_Pin_1, GPIO_Mode_In_FL_IT);
    GPIO_Init(GPIOB, GPIO_Pin_2, GPIO_Mode_In_FL_IT);
    EXTI_SetPinSensitivity(EXTI_Pin_1, EXTI_Trigger_Falling);
    ITC_SetSoftwarePriority(EXTI1_IRQn, ITC_PriorityLevel_1);
    EXTI_SetPinSensitivity(EXTI_Pin_2, EXTI_Trigger_Rising);
    ITC_SetSoftwarePriority(EXTI2_IRQn, ITC_PriorityLevel_2);
    PWR_FastWakeUpCmd(ENABLE);  //���ٻ���ʹ��	
    //RTC_Config();//�����͹���
    HardWare_Init();
    PWR_UltraLowPowerCmd(ENABLE);//���͹���
    //enableInterrupts(); //�������ж�  
    while(1)
	{
          //disableInterrupts();
         Sx1278InteruptHandler(); 
         enableInterrupts(); //�������ж� 
         SPIWriteOneByteToAddress(REG_LR_IRQFLAGS, 0xff);
         halt();//����͹���          
	}
}


void HardWare_Init(void)
{     
       SPI_SX1278_Init();
       SX1278Reset();//��λ
       Sx1278LoRaInit();//LORA��ʼ��       
}


void SX1278_SEND(void)
{     GPIO_SetBits(GPIOE, GPIO_Pin_7);
      HardWare_Init();
      u8  si4432_Send[128] = {"hello world hello lora hello world hello lorahello world hello lora hello world hello"};//test
      Sx1278SendPacket(si4432_Send, 128);
}

void SX1278_SENDECHO(void)
{     GPIO_SetBits(GPIOE, GPIO_Pin_7);
      HardWare_Init();
      u8  si4433_Send[128] = {"echo hello world hello lora hello world hello lorahello world hello lora hello world"};//test
      Sx1278SendPacket(si4433_Send, 128);
}

