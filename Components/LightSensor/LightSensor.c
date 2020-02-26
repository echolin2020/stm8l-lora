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
#include "LightSensor.h"
                                                

/*********************************************************************************/
/*
函数名：iic_init
功  能：配置SDA 和SCL端口
参  数：无
返回值：无
*/
//初始化IIC

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

//初始化
void Init_MAX44009(void)
{
	i2c_Port_Init();
}


void delay_1us(u8 x)//粗略延时,iic_40K
{
	u8 i=20;
	x=i*x;
	while(x--);
}
////////IIC起始函数//////////
/*
IIC起始:当SCL处于高电平期间，SDA由高电平变成低电平出现一个下降沿，然后SCL拉低
*/
u8 I2C_Start(void)
{
		SDA_H; 
		delay_1us(5);	//延时保证时钟频率低于40K，以便从机识别
		SCL_H;
		delay_1us(5);//延时保证时钟频率低于40K，以便从机识别
		if(!SDA_read) return 0;//SDA线为低电平则总线忙,退出
		SDA_L;   //SCL处于高电平的时候，SDA拉低
		delay_1us(5);
	  if(SDA_read) return 0;//SDA线为高电平则总线出错,退出
		SCL_L;
	  delay_1us(5);
	  return 1;
}
//**************************************
//IIC停止信号
/*
IIC停止:当SCL处于高电平期间，SDA由低电平变成高电平出现一个上升沿
*/
//**************************************
void I2C_Stop(void)
{
    SDA_L;
		SCL_L;
		delay_1us(5);
		SCL_H;
		delay_1us(5);
		SDA_H;//当SCL处于高电平期间，SDA由低电平变成高电平             //延时
}
//**************************************
//IIC发送应答信号
//入口参数:ack (0:ACK 1:NAK)
/*
应答：当从机接收到数据后，向主机发送一个低电平信号
先准备好SDA电平状态，在SCL高电平时，主机采样SDA
*/
//**************************************
void I2C_SendACK(u8 i)
{
    if(1==i)
    {
	SDA_H;	             //准备好SDA电平状态，不应答

    }
    else 
    {
	SDA_L;  						//准备好SDA电平状态，应答 
    }	
	  SCL_H;                    //拉高时钟线
    delay_1us(5);                 //延时
    SCL_L ;                  //拉低时钟线
    delay_1us(5);    
} 
///////等待从机应答////////
/*
当本机(主机)发送了一个数据后，等待从机应答
先释放SDA，让从机使用，然后采集SDA状态
*/
/////////////////
u8 I2C_WaitAck(void) 	 //返回为:=1有ACK,=0无ACK
{
	uint16_t i=0;
	SDA_H;	        //释放SDA
	SCL_H;         //SCL拉高进行采样
	while(SDA_read)//等待SDA拉低
	{
		i++;      //等待计数
		if(i==500)//超时跳出循环
		break;
	}
	if(SDA_read)//再次判断SDA是否拉低
	{
		SCL_L; 
		return RESET;//从机应答失败，返回0
	}
  delay_1us(5);//延时保证时钟频率低于40K，
	SCL_L;
	delay_1us(5); //延时保证时钟频率低于40K，
	return SET;//从机应答成功，返回1
}
//**************************************
//向IIC总线发送一个字节数据
/*
一个字节8bit,当SCL低电平时，准备好SDA，SCL高电平时，从机采样SDA
*/
//**************************************
void I2C_SendByte(u8 dat)
{
  u8 i;
	SCL_L;//SCL拉低，给SDA准备
  for (i=0; i<8; i++)         //8位计数器
  {
		if(dat&0x80)//SDA准备
		{
                  SDA_H;  
		
                }
                else 
		
                {SDA_L;}
    SCL_H;                //拉高时钟，给从机采样
    delay_1us(5);        //延时保持IIC时钟频率，也是给从机采样有充足时间
    SCL_L;                //拉低时钟，给SDA准备
    delay_1us(5); 		  //延时保持IIC时钟频率
		dat <<= 1;          //移出数据的最高位  
  }					 
}
//**************************************
//从IIC总线接收一个字节数据
//**************************************
u8 I2C_RecvByte()
{
    u8 i;
    u8 dat = 0;
    SDA_H;//释放SDA，给从机使用
    delay_1us(1);         //延时给从机准备SDA时间            
    for (i=0; i<8; i++)         //8位计数器
    { 
		  dat <<= 1;
			
      SCL_H;                //拉高时钟线，采样从机SDA
     
		  if(SDA_read) //读数据    
		   dat |=0x01;      
       delay_1us(5);     //延时保持IIC时钟频率		
       SCL_L;           //拉低时钟线，处理接收到的数据
       delay_1us(5);   //延时给从机准备SDA时间
    } 
    return dat;
}
//**************************************
//向IIC设备写入一个字节数据
//**************************************
u8 Read_MAX44009(void)
{
//高位
	  if(I2C_Start()==0)  //起始信号
		{I2C_Stop(); return RESET;}           

    I2C_SendByte(Slave_Address);   //发送设备地址+写信号
 	  if(!I2C_WaitAck()){I2C_Stop(); return RESET;}
   
		I2C_SendByte(MSB);    //内部寄存器地址，
 	  if(!I2C_WaitAck()){I2C_Stop(); return RESET;}
/*   
		I2C_SendByte(data);       //内部寄存器数据，
	  if(!I2C_WaitAck()){I2C_Stop(); return RESET;}
*/
		if(I2C_Start()==0)  //起始信号
		{I2C_Stop(); return RESET;}
		
		I2C_SendByte(Slave_Address+1);  //发送设备地址+读信号
 	  if(!I2C_WaitAck()){I2C_Stop(); return RESET;}
	
		LuxHigh = I2C_RecvByte();
		I2C_SendACK(1);     //发送停止传输信号
		I2C_Stop();   //发送停止信号
		
	////////低位	
			  if(I2C_Start()==0)  //起始信号
		{I2C_Stop(); return RESET;}           

    I2C_SendByte(Slave_Address);   //发送设备地址+写信号
 	  if(!I2C_WaitAck()){I2C_Stop(); return RESET;}
   
		I2C_SendByte(LSB);    //内部寄存器地址，
 	  if(!I2C_WaitAck()){I2C_Stop(); return RESET;}
/*   
		I2C_SendByte(data);       //内部寄存器数据，
	  if(!I2C_WaitAck()){I2C_Stop(); return RESET;}
*/
		if(I2C_Start()==0)  //起始信号
		{I2C_Stop(); return RESET;}
		
		I2C_SendByte(Slave_Address+1);  //发送设备地址+读信号
 	  if(!I2C_WaitAck()){I2C_Stop(); return RESET;}
	
		LuxLow = I2C_RecvByte();
		I2C_SendACK(1);     //发送停止传输信号
		I2C_Stop();   //发送停止信号
			
		return SET;
}



//**************************************
//从IIC设备读取一个字节数据
//**************************************
/*
u8 Single_ReadI2C(u8 Slave_Address,u8 REG_Address,u8 *REG_data,u8 length)
{
 if(I2C_Start()==0)  //起始信号
		{I2C_Stop(); return RESET;}          
	 
	I2C_SendByte(Slave_Address);    //发送设备地址+写信号
 	if(!I2C_WaitAck()){I2C_Stop(); return RESET;} 
	
	I2C_SendByte(REG_Address);     //发送存储单元地址
 	if(!I2C_WaitAck()){I2C_Stop(); return RESET;} 
	
	if(I2C_Start()==0)  //起始信号
			{I2C_Stop(); return RESET;}            

	I2C_SendByte(Slave_Address+1);  //发送设备地址+读信号
 	if(!I2C_WaitAck()){I2C_Stop(); return RESET;}
	
	while(length-1)
	{
		*REG_data++=I2C_RecvByte();       //读出寄存器数据
		I2C_SendACK(0);               //应答
		length--;
	}
	*REG_data=I2C_RecvByte();  
	I2C_SendACK(1);     //发送停止传输信号
	I2C_Stop();                    //停止信号
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
        
	MAX44009light = (float)(((0x1 << exponent) * (float)mantissa) *0.045);//合成数据，即光照数据     
        
	//  Result=LuxHigh*256+LuxLow;
	//delay_ms(180);
        
       memset(Light, 0,  sizeof(Light));  
    //   sprintf(Light, "%0.1f",MAX44009light);
     //  sprintf(cJson,"{\"len\":110,\"equtype\" :\"Bfgzv1.0\",\"Ver\": \"V1.0\",\"col\":[{\"code\":1,\"sen\":[{\"Code\":1,\"T\":3,\"V\":%.*s,\"U\":\"Lx\"}]}]}",sizeof(Light),Light);
       len =  strlen(cJson);

       
       
      SPIReadOneByteFromAddress(REG_LR_IRQFLAGS); //读取0x12寄存器，中断标志寄存器 
      Sx1278SendPacket(cJson, len);
      SPIReadOneByteFromAddress(REG_LR_IRQFLAGS); //读取0x12寄存器，中断标志寄存器
      SPIWriteOneByteToAddress(REG_LR_IRQFLAGS, 0xff);//清零所有标志位，所有的DIOx口都恢复低电平
      USART_SendString((char*)cJson,len);//测试数据
      
    

}

/*--------------------------------------------------------------------------------------------------------
                   							                  0ooo
                   								ooo0     (   )
                								(   )     ) /
                								 \ (     (_/
                								  \_)
----------------------------------------------------------------------------------------------------------*/






