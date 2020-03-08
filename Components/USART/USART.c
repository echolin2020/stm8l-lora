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

    |------------------------|
    |  TXD - PC4(UART1->RX)  |
    |  RXD - PC5(UART1->TX)  |
    |------------------------|

*******************************************************************************/

/* 包含系统头文件 */
#include <stdarg.h>

/* 包含自定义头文件 */
#include "usart.h"
/* 自定义新类型 */

/* 自定义宏 */

uart usart1;      
/* 全局变量定义 */
/*******************************************************************************
 * 名称: Uart_Init
 * 功能: UART1初始化操作
 * 形参: 无
 * 返回: 无
 * 说明: 无 
 ******************************************************************************/
void Usart_Init(void)
{
/*  
初始化IO  
TxD  配置成输出上拉高速模式 
RxD  配置成输入上拉无中断模式   
  */
   CLK_PeripheralClockConfig (CLK_Peripheral_USART1,ENABLE);//开启USART时钟    

      USART_DeInit(USART1); //重新将USART1设置为缺省值的USART1
   
   
       GPIO_Init(USART_GPIO_PORT, TXD_GPIO_PINS, GPIO_Mode_Out_PP_High_Fast); 
       GPIO_Init(USART_GPIO_PORT, RXD_GPIO_PINS, GPIO_Mode_In_FL_No_IT);
   
/*        
	SYSCFG_REMAPDeInit();//引脚复用功能
        SYSCFG_REMAPPinConfig(REMAP_Pin_USART1TxRxPortC,ENABLE);//将USART Tx--Rx（）映射到PC5-PC6
  */      
        
	/*
	 * 将UART1配置为：
	 * 波特率 = 115200
	 * 数据位 = 8
	 * 1位停止位
	 * 无校验位
	 * 使能接收和发送
	 * 使能接收中断
	 */
       
      
 
    USART_Init(USART1,(uint32_t)115200,USART_WordLength_8b,USART_StopBits_1,USART_Parity_No,USART_Mode_Tx | USART_Mode_Rx);//设置USART参数115200，8N1，接收/发送
   
    USART_ClearITPendingBit(USART1, USART_IT_RXNE);
    // USART_ITConfig(USART1, USART_IT_OR, ENABLE);      
   // USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
   USART_ITConfig(USART1, USART_IT_IDLE, DISABLE);
  //  USART_ITConfig (USART1,USART_IT_TC,ENABLE);//使能接收中断,中断向量号为28
    USART_Cmd (USART1,ENABLE);//使能USART
    
}
//初始化串口
void Usart1_clear_init (void)
{
    memset(usart1.rx_data,0,sizeof(usart1.rx_data));
    memset(usart1.tx_data,0,sizeof(usart1.tx_data));
}
/*******************************************************************************
 * 名称: UART1_SendByte
 * 功能: UART1发送一个字节
 * 形参: data -> 要发送的字节
 * 返回: 无
 * 说明: 无 
 ******************************************************************************/

void USART_SendByte(u8 data)
{
	 USART_SendData8(USART1,(unsigned char)data);
	
	// 等待传输结束 
	while (USART_GetFlagStatus (USART1,USART_FLAG_TXE) == RESET);
}

/*******************************************************************************
****入口参数：需要发送的字符串
****出口参数：无
****函数备注：USART发送函数
****版权信息：蓝旗嵌入式系统
*******************************************************************************/

void USART_SendStr(unsigned char *Str) 
{
        while(*Str!=0)//不为结束
        {
            USART_SendData8(USART1,*Str);     //发送数据 
            while(!USART_GetFlagStatus (USART1,USART_FLAG_TXE));//等待发送完毕
            Str++;//下一个数据
        }
}


/*******************************************************************************
 * 名称: UART1_SendString
 * 功能: UART1发送len个字符
 * 形参: data -> 指向要发送的字符串
 *       len -> 要发送的字节数
 * 返回: 无
 * 说明: 无 
 ******************************************************************************/

void USART_SendString(u8* Data,u16 len)
{
	u16 i=0;
	for(; i < len; i++)
	{
           // 等待传输结束 
	    while (USART_GetFlagStatus (USART1,USART_FLAG_TXE) == RESET);
            USART_SendData8(USART1,Data[i]);	// 循环调用发送一个字符函数 
         }
        delay_ms(5);    
       while (USART_GetFlagStatus (USART1,USART_FLAG_TC) == RESET);
}

/*******************************************************************************
 * 名称: UART1_ReceiveByte
 * 功能: UART1接收一个字符
 * 形参: 无
 * 返回: 接收到的字符
 * 说明: 无 
 ******************************************************************************/
/*u8 USART_ReceiveByte(void)
{
	u8 USART_RX_BUF; 
	
	// 等待接收完成 
	while (USART_GetFlagStatus(USART1, USART_FLAG_RXNE) == RESET);
	
	USART_RX_BUF = USART_ReceiveData8(USART1);
	
	return  USART_RX_BUF;
}
*/
/********************************************************
 * 函数名：itoa
 * 描述  ：将整形数据转换成字符串
 * 输入  ：-radix =10 表示10进制，其他结果为0
 *         -value 要转换的整形数
 *         -buf 转换后的字符串
 *         -radix = 10
 * 输出  ：无
 * 返回  ：无
 * 调用  ：被USART1_printf()调用
 *******************************************************/
static char *itoa(int value, char *string, int radix)
{
    int     i, d;
    int     flag = 0;
    char    *ptr = string;

    /* This implementation only works for decimal numbers. */
    if (radix != 10)
    {
        *ptr = 0;
        return string;
    }

    if (!value)
    {
        *ptr++ = 0x30;
        *ptr = 0;
        return string;
    }

    /* if this is a negative value insert the minus sign. */
    if (value < 0)
    {
        *ptr++ = '-';

        /* Make the value positive. */
        value *= -1;
    }

    for (i = 10000; i > 0; i /= 10)
    {
        d = value / i;

        if (d || flag)
        {
            *ptr++ = (char)(d + 0x30);
            value -= (d * i);
            flag = 1;
        }
    }

    /* Null terminate the string. */
    *ptr = 0;

    return string;

} /* NCL_Itoa */

/************************************************************************
 * 函数名：USART1_printf
 * 描述  ：格式化输出，类似于C库中的printf，但这里没有用到C库
 * 输入  ：-USARTx 串口通道，这里只用到了串口1，即USART1
 *		     -Data   要发送到串口的内容的指针
 *			   -...    其他参数
 * 输出  ：无
 * 返回  ：无 
 * 调用  ：外部调用
 *         典型应用USART1_printf( "\r\n this is a demo \r\n" );
 *            		 USART1_printf(  "\r\n %d \r\n", i );
 *            		 USART1_printf(  "\r\n %s \r\n", j );
 ***************************************************************************/
/*
void USART_printf( uint8_t *Data,...)
{
  const char *s;
  int d;   
   float f;
         int   dec,sign;
      
  char buf[16];
  va_list ap;
  va_start(ap, Data);

  while ( *Data != 0)     // 判断是否到达字符串结束符
  {				                          
      if ( *Data == 0x5c )  //'\'
      {									  
              switch ( *++Data )
              {
                      case 'r':							          //回车符
                              USART_SendData8(USART1, 0x0d);
                              Data ++;
                              break;

                      case 'n':							          //换行符
                              USART_SendData8(USART1, 0x0a);	
                              Data ++;
                              break;
                      
                      default:
                              Data ++;
                          break;
              }			 
      }
      else if ( *Data == '%')
      {					//
	switch ( *++Data )
	  {				
	    case 's':						//字符串
		s = va_arg(ap, const char *);
          for ( ; *s; s++) 
	  {
	    USART_SendData8(USART1, *s);
	    while (USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET);
          }
	  Data++;
          break;

          case 'd':	//十进制
          d = va_arg(ap, int);
          itoa(d, buf, 10);
          for (s = buf; *s; s++) 
	{
	  USART_SendData8(USART1, *s);
	  while (USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET);
        }
	Data++;
        
        break;
        
          case 'f':	//十进制
            
          f  = (float)va_arg(ap,double);
     //buf=ecvt(f,16,&dec,&sign);
       //   gcvt(f,16, buf);
         
            sprintf(buf, "%f", f);
            
          for (s = buf; *s; s++) 
	{
	  USART_SendData8(USART1, *s);
	  while (USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET);
        }
	Data++;
        
        break;
         case 'l':	//十进制
          d = va_arg(ap, long);
          itoa(d, buf, 10);
          for (s = buf; *s; s++) 
	{
	  USART_SendData8(USART1, *s);
	  while (USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET);
        }
	Data++;
        break;
	default:
	Data++;
	break;
	}		 
     }  //end of else if 
    else USART_SendData8(USART1, *Data++);
    while (USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET);
  }
}
*/
/*--------------------------------------------------------------------------------------------------------
                   									   0ooo
                   								ooo0     (   )
                								(   )     ) /
                								 \ (     (_/
                								  \_)
----------------------------------------------------------------------------------------------------------*/
