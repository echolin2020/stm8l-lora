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

#include "SX1278.h"


void HardWare_Init(void);  
void SX1278_SEND(void);//SX1278���ͺ���


int main(void)
{
          GPIO_Init(GPIOE, GPIO_Pin_7, GPIO_Mode_Out_PP_Low_Fast);
          
          while(1){    
      HardWare_Init();
      u8  si4432_Send[24] = {"hello lora hello world"};//test
      //SPIReadOneByteFromAddress(REG_LR_IRQFLAGS); //��ȡ0x12�Ĵ������жϱ�־�Ĵ��� 
      Sx1278SendPacket(si4432_Send, 24);
      //SPIReadOneByteFromAddress(REG_LR_IRQFLAGS); //��ȡ0x12�Ĵ������жϱ�־�Ĵ���
      SPIWriteOneByteToAddress(REG_LR_IRQFLAGS, 0xff);//�������б�־λ�����е�DIOx�ڶ��ָ��͵�ƽ     
      SX1276LoRaSetOpMode(Sleep_mode);   
      GPIO_ToggleBits(GPIOE, GPIO_Pin_7);
      delay_ms(100);
      }
      //����˯��ģʽ
      

//          PWR_FastWakeUpCmd(ENABLE);  //���ٻ���ʹ��	
//          RTC_Config();//�����͹���
//        
//        
//        PWR_UltraLowPowerCmd(ENABLE);//���͹���
//        
//      
//        enableInterrupts(); //�������ж�  
//
//       
//	while(1)
//	{
//         halt();//����͹���
//         RTC_ClearITPendingBit(RTC_IT_WUT); 
//         RTC_WakeUpCmd(ENABLE);            
//	}
}




void HardWare_Init(void)
{     
       SPI_SX1278_Init();
       SX1278Reset();//��λ
       Sx1278LoRaInit();//LORA��ʼ��       
}




void SX1278_SEND(void)
{
      HardWare_Init();
      u8  si4432_Send[24] = {"���ϱ�����ӿƼ����޹�˾"};//test
      SPIReadOneByteFromAddress(REG_LR_IRQFLAGS); //��ȡ0x12�Ĵ������жϱ�־�Ĵ��� 
      Sx1278SendPacket(si4432_Send, 24);
      SPIReadOneByteFromAddress(REG_LR_IRQFLAGS); //��ȡ0x12�Ĵ������жϱ�־�Ĵ���
      SPIWriteOneByteToAddress(REG_LR_IRQFLAGS, 0xff);//�������б�־λ�����е�DIOx�ڶ��ָ��͵�ƽ     
      SX1276LoRaSetOpMode(Sleep_mode);                     //����˯��ģʽ

}

