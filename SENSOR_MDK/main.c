/******************************************************************************************
河南兵峰电子科技有限公司                                             
本代码为河南兵峰电子科技有限公司的Sx1278模块测试代码,根据Sx1278官方代码修改，            
为购买了本公司Sx1278系列模块的客户测试模块基本性能，客户可以根据自己的需要开发适用于自身 
系统的软件，未经许可，不得用于其他用途。
本测试程序为lora模式发送程序，模块上电后将按一定频率发送数据包给接收端，接收端接收到数据包后，通过串口将数据发送给上位机，同时板载LED灯闪烁1次。
CPU: STM8L151F3P6      开发平台IARForStm V1.4
File:main.c   
版本：V1.0
建立日期：2017年11月01日   修改日期：2017.12.17
作者：王港申
联系邮箱：dan772474868@qq.com 联系QQ：772474868
Copyright (C)  SoSharp  河南兵峰电子科技有限公司                                                              
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
void SX1278_SEND(void);//SX1278发送函数


int main(void)
{
          GPIO_Init(GPIOE, GPIO_Pin_7, GPIO_Mode_Out_PP_Low_Fast);
          
          while(1){    
      HardWare_Init();
      u8  si4432_Send[24] = {"hello lora hello world"};//test
      //SPIReadOneByteFromAddress(REG_LR_IRQFLAGS); //读取0x12寄存器，中断标志寄存器 
      Sx1278SendPacket(si4432_Send, 24);
      //SPIReadOneByteFromAddress(REG_LR_IRQFLAGS); //读取0x12寄存器，中断标志寄存器
      SPIWriteOneByteToAddress(REG_LR_IRQFLAGS, 0xff);//清零所有标志位，所有的DIOx口都恢复低电平     
      SX1276LoRaSetOpMode(Sleep_mode);   
      GPIO_ToggleBits(GPIOE, GPIO_Pin_7);
      delay_ms(100);
      }
      //设置睡眠模式
      

//          PWR_FastWakeUpCmd(ENABLE);  //快速唤醒使能	
//          RTC_Config();//启动低功耗
//        
//        
//        PWR_UltraLowPowerCmd(ENABLE);//超低功耗
//        
//      
//        enableInterrupts(); //开启总中断  
//
//       
//	while(1)
//	{
//         halt();//进入低功耗
//         RTC_ClearITPendingBit(RTC_IT_WUT); 
//         RTC_WakeUpCmd(ENABLE);            
//	}
}




void HardWare_Init(void)
{     
       SPI_SX1278_Init();
       SX1278Reset();//复位
       Sx1278LoRaInit();//LORA初始化       
}




void SX1278_SEND(void)
{
      HardWare_Init();
      u8  si4432_Send[24] = {"河南兵峰电子科技有限公司"};//test
      SPIReadOneByteFromAddress(REG_LR_IRQFLAGS); //读取0x12寄存器，中断标志寄存器 
      Sx1278SendPacket(si4432_Send, 24);
      SPIReadOneByteFromAddress(REG_LR_IRQFLAGS); //读取0x12寄存器，中断标志寄存器
      SPIWriteOneByteToAddress(REG_LR_IRQFLAGS, 0xff);//清零所有标志位，所有的DIOx口都恢复低电平     
      SX1276LoRaSetOpMode(Sleep_mode);                     //设置睡眠模式

}

