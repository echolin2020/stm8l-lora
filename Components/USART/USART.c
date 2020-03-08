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

    |------------------------|
    |  TXD - PC4(UART1->RX)  |
    |  RXD - PC5(UART1->TX)  |
    |------------------------|

*******************************************************************************/

/* ����ϵͳͷ�ļ� */
#include <stdarg.h>

/* �����Զ���ͷ�ļ� */
#include "usart.h"
/* �Զ��������� */

/* �Զ���� */

uart usart1;      
/* ȫ�ֱ������� */
/*******************************************************************************
 * ����: Uart_Init
 * ����: UART1��ʼ������
 * �β�: ��
 * ����: ��
 * ˵��: �� 
 ******************************************************************************/
void Usart_Init(void)
{
/*  
��ʼ��IO  
TxD  ���ó������������ģʽ 
RxD  ���ó������������ж�ģʽ   
  */
   CLK_PeripheralClockConfig (CLK_Peripheral_USART1,ENABLE);//����USARTʱ��    

      USART_DeInit(USART1); //���½�USART1����Ϊȱʡֵ��USART1
   
   
       GPIO_Init(USART_GPIO_PORT, TXD_GPIO_PINS, GPIO_Mode_Out_PP_High_Fast); 
       GPIO_Init(USART_GPIO_PORT, RXD_GPIO_PINS, GPIO_Mode_In_FL_No_IT);
   
/*        
	SYSCFG_REMAPDeInit();//���Ÿ��ù���
        SYSCFG_REMAPPinConfig(REMAP_Pin_USART1TxRxPortC,ENABLE);//��USART Tx--Rx����ӳ�䵽PC5-PC6
  */      
        
	/*
	 * ��UART1����Ϊ��
	 * ������ = 115200
	 * ����λ = 8
	 * 1λֹͣλ
	 * ��У��λ
	 * ʹ�ܽ��պͷ���
	 * ʹ�ܽ����ж�
	 */
       
      
 
    USART_Init(USART1,(uint32_t)115200,USART_WordLength_8b,USART_StopBits_1,USART_Parity_No,USART_Mode_Tx | USART_Mode_Rx);//����USART����115200��8N1������/����
   
    USART_ClearITPendingBit(USART1, USART_IT_RXNE);
    // USART_ITConfig(USART1, USART_IT_OR, ENABLE);      
   // USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
   USART_ITConfig(USART1, USART_IT_IDLE, DISABLE);
  //  USART_ITConfig (USART1,USART_IT_TC,ENABLE);//ʹ�ܽ����ж�,�ж�������Ϊ28
    USART_Cmd (USART1,ENABLE);//ʹ��USART
    
}
//��ʼ������
void Usart1_clear_init (void)
{
    memset(usart1.rx_data,0,sizeof(usart1.rx_data));
    memset(usart1.tx_data,0,sizeof(usart1.tx_data));
}
/*******************************************************************************
 * ����: UART1_SendByte
 * ����: UART1����һ���ֽ�
 * �β�: data -> Ҫ���͵��ֽ�
 * ����: ��
 * ˵��: �� 
 ******************************************************************************/

void USART_SendByte(u8 data)
{
	 USART_SendData8(USART1,(unsigned char)data);
	
	// �ȴ�������� 
	while (USART_GetFlagStatus (USART1,USART_FLAG_TXE) == RESET);
}

/*******************************************************************************
****��ڲ�������Ҫ���͵��ַ���
****���ڲ�������
****������ע��USART���ͺ���
****��Ȩ��Ϣ������Ƕ��ʽϵͳ
*******************************************************************************/

void USART_SendStr(unsigned char *Str) 
{
        while(*Str!=0)//��Ϊ����
        {
            USART_SendData8(USART1,*Str);     //�������� 
            while(!USART_GetFlagStatus (USART1,USART_FLAG_TXE));//�ȴ��������
            Str++;//��һ������
        }
}


/*******************************************************************************
 * ����: UART1_SendString
 * ����: UART1����len���ַ�
 * �β�: data -> ָ��Ҫ���͵��ַ���
 *       len -> Ҫ���͵��ֽ���
 * ����: ��
 * ˵��: �� 
 ******************************************************************************/

void USART_SendString(u8* Data,u16 len)
{
	u16 i=0;
	for(; i < len; i++)
	{
           // �ȴ�������� 
	    while (USART_GetFlagStatus (USART1,USART_FLAG_TXE) == RESET);
            USART_SendData8(USART1,Data[i]);	// ѭ�����÷���һ���ַ����� 
         }
        delay_ms(5);    
       while (USART_GetFlagStatus (USART1,USART_FLAG_TC) == RESET);
}

/*******************************************************************************
 * ����: UART1_ReceiveByte
 * ����: UART1����һ���ַ�
 * �β�: ��
 * ����: ���յ����ַ�
 * ˵��: �� 
 ******************************************************************************/
/*u8 USART_ReceiveByte(void)
{
	u8 USART_RX_BUF; 
	
	// �ȴ�������� 
	while (USART_GetFlagStatus(USART1, USART_FLAG_RXNE) == RESET);
	
	USART_RX_BUF = USART_ReceiveData8(USART1);
	
	return  USART_RX_BUF;
}
*/
/********************************************************
 * ��������itoa
 * ����  ������������ת�����ַ���
 * ����  ��-radix =10 ��ʾ10���ƣ��������Ϊ0
 *         -value Ҫת����������
 *         -buf ת������ַ���
 *         -radix = 10
 * ���  ����
 * ����  ����
 * ����  ����USART1_printf()����
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
 * ��������USART1_printf
 * ����  ����ʽ�������������C���е�printf��������û���õ�C��
 * ����  ��-USARTx ����ͨ��������ֻ�õ��˴���1����USART1
 *		     -Data   Ҫ���͵����ڵ����ݵ�ָ��
 *			   -...    ��������
 * ���  ����
 * ����  ���� 
 * ����  ���ⲿ����
 *         ����Ӧ��USART1_printf( "\r\n this is a demo \r\n" );
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

  while ( *Data != 0)     // �ж��Ƿ񵽴��ַ���������
  {				                          
      if ( *Data == 0x5c )  //'\'
      {									  
              switch ( *++Data )
              {
                      case 'r':							          //�س���
                              USART_SendData8(USART1, 0x0d);
                              Data ++;
                              break;

                      case 'n':							          //���з�
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
	    case 's':						//�ַ���
		s = va_arg(ap, const char *);
          for ( ; *s; s++) 
	  {
	    USART_SendData8(USART1, *s);
	    while (USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET);
          }
	  Data++;
          break;

          case 'd':	//ʮ����
          d = va_arg(ap, int);
          itoa(d, buf, 10);
          for (s = buf; *s; s++) 
	{
	  USART_SendData8(USART1, *s);
	  while (USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET);
        }
	Data++;
        
        break;
        
          case 'f':	//ʮ����
            
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
         case 'l':	//ʮ����
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
