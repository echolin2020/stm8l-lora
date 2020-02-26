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
#include "rtc.h"


/*******************************************************************************
****入口参数：无
****出口参数：无
****函数备注：非精确延时函数
****版权信息：蓝旗嵌入式系统
*******************************************************************************/
/*
void Delay(__IO uint16_t nCount)
{
    // Decrement nCount value 
    while (nCount != 0)
    {
        nCount--;
    }
}
*/
/*******************************************************************************
****入口参数：无
****出口参数：无
****函数备注：时钟配置函数函数
****版权信息：蓝旗嵌入式系统
*******************************************************************************/
void CLK_Config(void)
{
    CLK_DeInit();
    CLK_HSICmd(ENABLE);//开始内部高频RC 16M
    CLK_SYSCLKSourceSwitchCmd(ENABLE);//使能时钟切换
    CLK_SYSCLKDivConfig(CLK_SYSCLKDiv_2);//2分频，即8M
    CLK_SYSCLKSourceConfig(CLK_SYSCLKSource_HSI);//HSI为系统时钟
  while (CLK_GetSYSCLKSource() != CLK_SYSCLKSource_HSI)//等待时钟稳定
  {}

}
void RTC_Config(void)
{
        CLK_LSICmd(ENABLE);//使能LSI
        CLK_RTCClockConfig(CLK_RTCCLKSource_LSI, CLK_RTCCLKDiv_1);//38K
        while (CLK_GetFlagStatus(CLK_FLAG_LSIRDY) == RESET);
        
        RTC_WakeUpCmd(DISABLE);
        CLK_PeripheralClockConfig(CLK_Peripheral_RTC, ENABLE);//RTC时钟门控使能
        RTC_WakeUpClockConfig(RTC_WakeUpClock_RTCCLK_Div16);//38K/16=2375HZ t=0.421ms
        delay_ms(500);//延时
    
        RTC_ITConfig(RTC_IT_WUT, ENABLE);//唤醒定时器中断使能      
        RTC_SetWakeUpCounter(2375);//设置唤醒时间 如果counter = 2375；则counter*（FLSI/PREC1/PREC2） = 2375*（1/（38000/1/16）） = 1s
        RTC_WakeUpCmd(ENABLE);//RTC唤醒使能

//        ITC_SetSoftwarePriority(RTC_IRQn,ITC_PriorityLevel_3);//优先级
       
}
/*******************************************************************************
****入口参数：无
****出口参数：无
****函数备注：设置唤醒时间
****版权信息：蓝
*******************************************************************************/
void RTC_WakeUp(void)
{
/*
  
   
  
  Delay(0xffff);//延时 一定要加
    
  RTC_ITConfig(RTC_IT_WUT, ENABLE);//唤醒定时器中断使能
   
  RTC_SetWakeUpCounter(3000);//设置唤醒时间
  RTC_WakeUpCmd(ENABLE);//RTC唤醒使能

   Convert_MAX44009();    //采集数据BH1750光照数据
  */   
  //Create_js();//自动上传JS格式
}



/*--------------------------------------------------------------------------------------------------------
                   							                  0ooo
                   								ooo0     (   )
                								(   )     ) /
                								 \ (     (_/
                								  \_)
----------------------------------------------------------------------------------------------------------*/
