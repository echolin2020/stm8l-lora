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
#include "LightSensor.h"
                                                

/*********************************************************************************/
/*
��������iic_init
��  �ܣ�����SDA ��SCL�˿�
��  ������
����ֵ����
*/
//��ʼ��IIC

int LuxHigh = 0,LuxLow = 0;
u8 MAX44009_BUF[8] = {0};
int exponent = 0,mantissa = 0;
float MAX44009light = 0; 
unsigned int Result = 0;

void i2c_Port_Init(void)
{
   GPIO_Init(IIC_GPIOx, IIC_SDA_GPIO_Pin | IIC_SCL_GPIO_Pin, GPIO_Mode_Out_PP_High_Fast);
   SCL_H;
   SDA_H;

}

//��ʼ��
void Init_MAX44009(void)
{
	i2c_Port_Init();
}


void delay_1us(u8 x)//������ʱ,iic_40K
{
	u8 i=20;
	x=i*x;
	while(x--);
}
////////IIC��ʼ����//////////
/*
IIC��ʼ:��SCL���ڸߵ�ƽ�ڼ䣬SDA�ɸߵ�ƽ��ɵ͵�ƽ����һ���½��أ�Ȼ��SCL����
*/
u8 I2C_Start(void)
{
		SDA_H; 
		delay_1us(5);	//��ʱ��֤ʱ��Ƶ�ʵ���40K���Ա�ӻ�ʶ��
		SCL_H;
		delay_1us(5);//��ʱ��֤ʱ��Ƶ�ʵ���40K���Ա�ӻ�ʶ��
		if(!SDA_read) return 0;//SDA��Ϊ�͵�ƽ������æ,�˳�
		SDA_L;   //SCL���ڸߵ�ƽ��ʱ��SDA����
		delay_1us(5);
	  if(SDA_read) return 0;//SDA��Ϊ�ߵ�ƽ�����߳���,�˳�
		SCL_L;
	  delay_1us(5);
	  return 1;
}
//**************************************
//IICֹͣ�ź�
/*
IICֹͣ:��SCL���ڸߵ�ƽ�ڼ䣬SDA�ɵ͵�ƽ��ɸߵ�ƽ����һ��������
*/
//**************************************
void I2C_Stop(void)
{
    SDA_L;
		SCL_L;
		delay_1us(5);
		SCL_H;
		delay_1us(5);
		SDA_H;//��SCL���ڸߵ�ƽ�ڼ䣬SDA�ɵ͵�ƽ��ɸߵ�ƽ             //��ʱ
}
//**************************************
//IIC����Ӧ���ź�
//��ڲ���:ack (0:ACK 1:NAK)
/*
Ӧ�𣺵��ӻ����յ����ݺ�����������һ���͵�ƽ�ź�
��׼����SDA��ƽ״̬����SCL�ߵ�ƽʱ����������SDA
*/
//**************************************
void I2C_SendACK(u8 i)
{
    if(1==i)
    {
	SDA_H;	             //׼����SDA��ƽ״̬����Ӧ��

    }
    else 
    {
	SDA_L;  						//׼����SDA��ƽ״̬��Ӧ�� 
    }	
	  SCL_H;                    //����ʱ����
    delay_1us(5);                 //��ʱ
    SCL_L ;                  //����ʱ����
    delay_1us(5);    
} 
///////�ȴ��ӻ�Ӧ��////////
/*
������(����)������һ�����ݺ󣬵ȴ��ӻ�Ӧ��
���ͷ�SDA���ôӻ�ʹ�ã�Ȼ��ɼ�SDA״̬
*/
/////////////////
u8 I2C_WaitAck(void) 	 //����Ϊ:=1��ACK,=0��ACK
{
	uint16_t i=0;
	SDA_H;	        //�ͷ�SDA
	SCL_H;         //SCL���߽��в���
	while(SDA_read)//�ȴ�SDA����
	{
		i++;      //�ȴ�����
		if(i==500)//��ʱ����ѭ��
		break;
	}
	if(SDA_read)//�ٴ��ж�SDA�Ƿ�����
	{
		SCL_L; 
		return RESET;//�ӻ�Ӧ��ʧ�ܣ�����0
	}
  delay_1us(5);//��ʱ��֤ʱ��Ƶ�ʵ���40K��
	SCL_L;
	delay_1us(5); //��ʱ��֤ʱ��Ƶ�ʵ���40K��
	return SET;//�ӻ�Ӧ��ɹ�������1
}
//**************************************
//��IIC���߷���һ���ֽ�����
/*
һ���ֽ�8bit,��SCL�͵�ƽʱ��׼����SDA��SCL�ߵ�ƽʱ���ӻ�����SDA
*/
//**************************************
void I2C_SendByte(u8 dat)
{
  u8 i;
	SCL_L;//SCL���ͣ���SDA׼��
  for (i=0; i<8; i++)         //8λ������
  {
		if(dat&0x80)//SDA׼��
		{
                  SDA_H;  
		
                }
                else 
		
                {SDA_L;}
    SCL_H;                //����ʱ�ӣ����ӻ�����
    delay_1us(5);        //��ʱ����IICʱ��Ƶ�ʣ�Ҳ�Ǹ��ӻ������г���ʱ��
    SCL_L;                //����ʱ�ӣ���SDA׼��
    delay_1us(5); 		  //��ʱ����IICʱ��Ƶ��
		dat <<= 1;          //�Ƴ����ݵ����λ  
  }					 
}
//**************************************
//��IIC���߽���һ���ֽ�����
//**************************************
u8 I2C_RecvByte()
{
    u8 i;
    u8 dat = 0;
    SDA_H;//�ͷ�SDA�����ӻ�ʹ��
    delay_1us(1);         //��ʱ���ӻ�׼��SDAʱ��            
    for (i=0; i<8; i++)         //8λ������
    { 
		  dat <<= 1;
			
      SCL_H;                //����ʱ���ߣ������ӻ�SDA
     
		  if(SDA_read) //������    
		   dat |=0x01;      
       delay_1us(5);     //��ʱ����IICʱ��Ƶ��		
       SCL_L;           //����ʱ���ߣ�������յ�������
       delay_1us(5);   //��ʱ���ӻ�׼��SDAʱ��
    } 
    return dat;
}
//**************************************
//��IIC�豸д��һ���ֽ�����
//**************************************
u8 Read_MAX44009(void)
{
//��λ
	  if(I2C_Start()==0)  //��ʼ�ź�
		{I2C_Stop(); return RESET;}           

    I2C_SendByte(Slave_Address);   //�����豸��ַ+д�ź�
 	  if(!I2C_WaitAck()){I2C_Stop(); return RESET;}
   
		I2C_SendByte(MSB);    //�ڲ��Ĵ�����ַ��
 	  if(!I2C_WaitAck()){I2C_Stop(); return RESET;}
/*   
		I2C_SendByte(data);       //�ڲ��Ĵ������ݣ�
	  if(!I2C_WaitAck()){I2C_Stop(); return RESET;}
*/
		if(I2C_Start()==0)  //��ʼ�ź�
		{I2C_Stop(); return RESET;}
		
		I2C_SendByte(Slave_Address+1);  //�����豸��ַ+���ź�
 	  if(!I2C_WaitAck()){I2C_Stop(); return RESET;}
	
		LuxHigh = I2C_RecvByte();
		I2C_SendACK(1);     //����ֹͣ�����ź�
		I2C_Stop();   //����ֹͣ�ź�
		
	////////��λ	
			  if(I2C_Start()==0)  //��ʼ�ź�
		{I2C_Stop(); return RESET;}           

    I2C_SendByte(Slave_Address);   //�����豸��ַ+д�ź�
 	  if(!I2C_WaitAck()){I2C_Stop(); return RESET;}
   
		I2C_SendByte(LSB);    //�ڲ��Ĵ�����ַ��
 	  if(!I2C_WaitAck()){I2C_Stop(); return RESET;}
/*   
		I2C_SendByte(data);       //�ڲ��Ĵ������ݣ�
	  if(!I2C_WaitAck()){I2C_Stop(); return RESET;}
*/
		if(I2C_Start()==0)  //��ʼ�ź�
		{I2C_Stop(); return RESET;}
		
		I2C_SendByte(Slave_Address+1);  //�����豸��ַ+���ź�
 	  if(!I2C_WaitAck()){I2C_Stop(); return RESET;}
	
		LuxLow = I2C_RecvByte();
		I2C_SendACK(1);     //����ֹͣ�����ź�
		I2C_Stop();   //����ֹͣ�ź�
			
		return SET;
}



//**************************************
//��IIC�豸��ȡһ���ֽ�����
//**************************************
/*
u8 Single_ReadI2C(u8 Slave_Address,u8 REG_Address,u8 *REG_data,u8 length)
{
 if(I2C_Start()==0)  //��ʼ�ź�
		{I2C_Stop(); return RESET;}          
	 
	I2C_SendByte(Slave_Address);    //�����豸��ַ+д�ź�
 	if(!I2C_WaitAck()){I2C_Stop(); return RESET;} 
	
	I2C_SendByte(REG_Address);     //���ʹ洢��Ԫ��ַ
 	if(!I2C_WaitAck()){I2C_Stop(); return RESET;} 
	
	if(I2C_Start()==0)  //��ʼ�ź�
			{I2C_Stop(); return RESET;}            

	I2C_SendByte(Slave_Address+1);  //�����豸��ַ+���ź�
 	if(!I2C_WaitAck()){I2C_Stop(); return RESET;}
	
	while(length-1)
	{
		*REG_data++=I2C_RecvByte();       //�����Ĵ�������
		I2C_SendACK(0);               //Ӧ��
		length--;
	}
	*REG_data=I2C_RecvByte();  
	I2C_SendACK(1);     //����ֹͣ�����ź�
	I2C_Stop();                    //ֹͣ�ź�
	return SET;
}
*/

void Convert_MAX44009(void)
{      

      char Light[20] = {0};
      char cJson[150] = {0};
      
       int len = 0;
       Read_MAX44009();
        
        exponent = (LuxHigh & 0xf0) >> 4;
	mantissa = (LuxHigh & 0x0f) << 4 | (LuxLow & 0x0f);
        
	MAX44009light = (float)(((0x1 << exponent) * (float)mantissa) *0.045);//�ϳ����ݣ�����������     
        
	//  Result=LuxHigh*256+LuxLow;
	//delay_ms(180);
        
       memset(Light, 0,  sizeof(Light));  
    //   sprintf(Light, "%0.1f",MAX44009light);
     //  sprintf(cJson,"{\"len\":110,\"equtype\" :\"Bfgzv1.0\",\"Ver\": \"V1.0\",\"col\":[{\"code\":1,\"sen\":[{\"Code\":1,\"T\":3,\"V\":%.*s,\"U\":\"Lx\"}]}]}",sizeof(Light),Light);
       len =  strlen(cJson);

       
       
      SPIReadOneByteFromAddress(REG_LR_IRQFLAGS); //��ȡ0x12�Ĵ������жϱ�־�Ĵ��� 
      Sx1278SendPacket(cJson, len);
      SPIReadOneByteFromAddress(REG_LR_IRQFLAGS); //��ȡ0x12�Ĵ������жϱ�־�Ĵ���
      SPIWriteOneByteToAddress(REG_LR_IRQFLAGS, 0xff);//�������б�־λ�����е�DIOx�ڶ��ָ��͵�ƽ
      USART_SendString((char*)cJson,len);//��������
      
    

}

/*--------------------------------------------------------------------------------------------------------
                   							                  0ooo
                   								ooo0     (   )
                								(   )     ) /
                								 \ (     (_/
                								  \_)
----------------------------------------------------------------------------------------------------------*/






