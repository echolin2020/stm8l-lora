#ifndef  __USART_H__
#define  __USART_H__

/* 包含系统头文件 */
#include "stm8l15x.h"
#include "stm8l15x_gpio.h"

#include "delay.h"
#include "LightSensor.h"
#include <string.h>
#include <stdarg.h>

//#include <stdlib.h>
#include <stdio.h>

/*定义串口*/
#define USART_GPIO_PORT  GPIOC  
#define TXD_GPIO_PINS  GPIO_Pin_3  
#define RXD_GPIO_PINS  GPIO_Pin_2
#define CS_GPIO_PINS   GPIO_Pin_4

#define Gpio_485_RX3_En()	do{GPIO_ResetBits(USART_GPIO_PORT,CS_GPIO_PINS);}while(0)
#define Gpio_485_TX3_En()	do{GPIO_SetBits(USART_GPIO_PORT,CS_GPIO_PINS);}while(0)

#define LIGHT     "3"  //光照



/* 自定义常量宏和表达式宏 */
#define countof(a)   (sizeof(a) / sizeof(*(a)))
#define RxBufferSize 50
#define TxBufferSize 120
typedef struct tag_uart{

	unsigned char rx_data[RxBufferSize];
	unsigned char tx_data[TxBufferSize];
        unsigned char rx_idx_start;

}uart;

extern uart usart1;


/* 声明给外部使用的变量 */

/* 声明给外部使用的函数 */

/*******************************************************************************
 * 名称: Uart_Init
 * 功能: UART1初始化操作
 * 形参: 无
 * 返回: 无
 * 说明: 无 
 ******************************************************************************/
void Usart_Init(void);

/*******************************************************************************
 * 名称: Uart_DMA_Init
 * 功能: UART1初始化操作
 * 形参: 无
 * 返回: 无
 * 说明: 无 
 ******************************************************************************/

//自动上传


//初始化串口
void Usart1_clear_init (void);
/*******************************************************************************
 * 名称: UART1_SendByte
 * 功能: UART1发送一个字节
 * 形参: data -> 要发送的字节
 * 返回: 无
 * 说明: 无 
 ******************************************************************************/
void USART_SendByte(u8 data);

/*******************************************************************************
 * 名称: UART1_SendString
 * 功能: UART1发送len个字符
 * 形参: data -> 指向要发送的字符串
 *       len -> 要发送的字节数
 * 返回: 无
 * 说明: 无 
 ******************************************************************************/
void USART_SendString(u8* Data,u16 len);
void USART_SendStr(unsigned char *Str);
/*******************************************************************************
 * 名称: UART1_ReceiveByte
 * 功能: UART1接收一个字符
 * 形参: 无
 * 返回: 接收到的字符
 * 说明: 无 
 ******************************************************************************/
//u8 USART_ReceiveByte(void);

/*******************************************************************************
 * 名称: fputc
 * 功能: 重定向c库函数printf到UART1
 * 形参: 无
 * 返回: 要打印的字符
 * 说明: 由printf调用 
 ******************************************************************************/
 //#ifdef _IAR_
//int fputc(int ch, FILE *f);
//#endif
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
static char *itoa(int value, char *string, int radix);

/************************************************************************
 * 函数名：USART1_printf
 * 描述  ：格式化输出，类似于C库中的printf，但这里没有用到C库
 * 输入  ：-USARTx 串口通道，这里只用到了串口1，即USART1
 *		     -Data   要发送到串口的内容的指针
 *			   -...    其他参数
 * 输出  ：无
 * 返回  ：无 
 * 调用  ：外部调用
 *         典型应用	 USART1_printf( "\r\n this is a demo \r\n" );
 *            		 USART1_printf(  "\r\n %d \r\n", i );
 *            		 USART1_printf(  "\r\n %s \r\n", j );
 ***************************************************************************/
void USART_printf( uint8_t *Data,...);



#endif 


