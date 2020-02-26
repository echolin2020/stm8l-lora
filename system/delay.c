/**
  ******************************************************************************
  * @file    delay.c
  * @author  Microcontroller Division
  * @version V1.2.0
  * @date    09/2010
  * @brief   delay functions
  ******************************************************************************
  * @copy
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  *
  * <h2><center>&copy; COPYRIGHT 2010 STMicroelectronics</center></h2>
  */

/* Includes ------------------------------------------------------------------*/


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
#include "delay.h"

/**********************************************************************************************************
*   函 数 名: delay_us
*   功能说明: 微秒延时程序
*   形    参：nCount要延时的微秒数
*   返 回 值: 无
**********************************************************************************************************/
void delay_us(void)
{
    asm("nop"); //一个asm("nop")函数经过示波器测试代表100ns
    asm("nop");
    asm("nop");
    asm("nop"); 
}

/**********************************************************************************************************
*   函 数 名: delay_ms
*   功能说明: 毫秒延时程序
*   形    参：nCount要延时的毫米数
*   返 回 值: 无
**********************************************************************************************************/
void delay_ms(u16 nCount)
{
//16MHz晶振，大概1.4ms
    unsigned int i;
    while(nCount--)  
    for(i=900;i>0;i--)
    delay_us();
}


/*--------------------------------------------------------------------------------------------------------
                   							                  0ooo
                   								ooo0     (   )
                								(   )     ) /
                								 \ (     (_/
                								  \_)
----------------------------------------------------------------------------------------------------------*/
