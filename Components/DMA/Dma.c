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
#include "DMA.h"

//DMA1的各通道配置
//这里的传输形式是固定的,这点要根据不同的情况来修改
//从存储器->外设模式/8位数据宽度/存储器增量模式
//DMA_CHx:DMA通道CHx
//cpar:外设地址
//cmar:存储器地址
//cndtr:数据传输量 
void MYDMA_Config(DMA_Channel_TypeDef* DMA_CHx,u32 RxBuffer,u16 cndtr)  
{         
    CLK_PeripheralClockConfig(CLK_Peripheral_DMA1, ENABLE);    //打开时钟，很重要  
    /* Deinitialize DMA channels */  
    DMA_GlobalDeInit();  
   
  //  DMA_DeInit(DMA1_Channel1);  
    DMA_DeInit(DMA_CHx);  //将DMA的通道1寄存器重设为缺省值
      
    /* DMA channel Rx of USART Configuration */  
    //该函数主要要配置好接受的数组，以及USART的数据寄存器地址，数组大小，以及DMA模式  
    DMA_Init(DMA_CHx, 
             (uint16_t)RxBuffer, 
             (uint16_t)0x5231,   
             cndtr,
             DMA_DIR_PeripheralToMemory, 
             DMA_Mode_Normal,   //工作在正常缓存模式
             DMA_MemoryIncMode_Inc,
             DMA_Priority_Low, //DMA通道 x拥有中优先级 
             DMA_MemoryDataSize_Byte);  //数据宽度为8位
   
    /* DMA channel Tx of USART Configuration */  
    //该函数主要配置发送数组，以及USART的数据寄存器地址，数组大小，以及DMA模式  
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


//开启一次DMA传输
void MYDMA_Enable(DMA_Channel_TypeDef*DMA_CHx)
{ 
	DMA_Cmd(DMA_CHx, ENABLE);       
}
//关闭一次DMA传输
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


