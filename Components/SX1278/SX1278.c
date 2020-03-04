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
#include "SX1278.h"

//unsigned char   Frequency[3]       = {0x6c,0x80,0x00};//434MH 频率设置
unsigned char   Frequency[3]       = {0x6c,0xa5,0x00};//434MH 频率设置
unsigned char   power_data[8]      = {0X80,0X80,0X80,0X83,0X86,0x89,0x8c,0x8f};//功率配置寄存器
unsigned char   SpreadingFactor[6] = {7,8,9,10,11,12};         //扩频因子7-12
unsigned char   CodingRate[4]      = {1,2,3,4};                //1-4
unsigned char   Bw_Frequency[10]   = {0,1,2,3,4,5,6,7,8,9};    //带宽0-9//7.8KHz,10.4KHz,15.6KHz,20.8KHz,31.2KHz,41.7KHz,62.5KHz,125KHz,250KHz,500KHz
unsigned char   recv[200];
unsigned char   RF_EX0_STATUS;
unsigned char   CRC_Value;
unsigned char   SX1278_RLEN;


/******************************************************************************
* 函数名    : Delay
* 函数描述  : 延时
* 输入参数  : 无
* 输出参数  : 无
* 返回值    : 无
******************************************************************************/
/*
void Delay(void)
{
    uint16_t x,y;
    for(x=0; x<1000; x++)
    {
        for(y=0; y<110; y++);
    }
}
*/
void Delay_us(u8 x)//粗略延时,iic_40K
{
	u8 i=20;
	x=i*x;
	while(x--);
}
/*******************************************************************************
****函数名称:
****函数功能:初始化SPI
****版本:V1.0
****日期:14-2-2014
****入口参数:无
****出口参数:无
****说明:
********************************************************************************/
void SPI_SX1278_Init()
{

    CLK_PeripheralClockConfig (CLK_Peripheral_SPI1,ENABLE);//开启SPI时钟
    SPI_DeInit(SPI1);
    
    
       //SPI_CLOCK:PB5, SPI_MOSI: PB6, SPI_MISO: PB7  
    GPIO_Init(SX1278_SPI_GPIOX, SX1278_SPI_SCK, GPIO_Mode_Out_PP_Low_Fast);//GPIOB5-SCK
    GPIO_Init(SX1278_SPI_GPIOX, SX1278_SPI_MOSI, GPIO_Mode_Out_PP_High_Fast);//GPIOB6-MOSI  GPIO_Mode_Out_PP_High_Fast
    GPIO_Init(SX1278_SPI_GPIOX, SX1278_SPI_MISO, GPIO_Mode_In_PU_No_IT);//GPIOB7-MISO
  
    
     // 初始化SPI 

        GPIO_Init(SX1278_SPI_GPIOX, SX1278_RES, GPIO_Mode_Out_PP_High_Fast);   //reset  PB3 
         // 配置CS管脚 
        GPIO_Init(SX1278_SPI_GPIOX, SX1278_SPI_SC, GPIO_Mode_Out_PP_High_Fast); //CS 
        RF_CE_H; // 拉高不使能外部SPI设备 

	SPI_Init(SPI1,\
                  SPI_FirstBit_MSB, // MSB is transmitted first 
                  SPI_BaudRatePrescaler_4, // Baudrate is Fsysclk/2=8MHz 
                  SPI_Mode_Master, // Master Mode 
                  SPI_CPOL_Low, // SCK=0 when idle 
                  SPI_CPHA_1Edge,// The 1st clock transition is the 1st data capture edge 
                  SPI_Direction_2Lines_FullDuplex, // 2-line undirection data mode 
                  SPI_NSS_Soft, // Software slave management enabled 
                  0x07);
 	  
   
	SPI_Cmd(SPI1,ENABLE);	// 使能SPI  
       
}
void SPI_ENABLE (void)
{
   CLK->PCKENR1 |= 0X10;//开启SPI时钟
   SPI1->CR1 |= SPI_CR1_SPE;// 使能SPI 
  /*
  CLK_PeripheralClockConfig (CLK_Peripheral_SPI1,ENABLE);
  SPI_Cmd(SPI1,ENABLE);	 
  */
}

void SPI_DISABLE (void)
{
   CLK->PCKENR1 &= 0XEF;  //turn off the clock of spi
   SPI1->CR1 &= (uint8_t)(~SPI_CR1_SPE); 
  /*
  CLK_PeripheralClockConfig (CLK_Peripheral_SPI1,DISABLE);//开启SPI时钟
  SPI_Cmd(SPI1,DISABLE);	// 使能SPI  
*/
}

void cmdSwitchEn(cmdEntype_t cmdcmd)
{
	switch(cmdcmd)
	{
		case enOpen:
		{
			RF_CE_L;
			break;
		}
		case enClose:
		{
			RF_CE_H;
			break;
		}
		
		default:
			break;
	}
}

 /*******************************************************************************
****函数名称:
****函数功能:发送字节
****版本:V1.0
****日期:14-2-2014
****入口参数:无
****出口参数:SPI接收到的数据
****说明:
********************************************************************************/

unsigned char SPIWriteOneByte(unsigned char byte)
{

  // Loop while DR register if not empty 
  while (SPI_GetFlagStatus(SPI1, SPI_FLAG_TXE) == RESET);

  // Send byte through the SPI1 peripheral 
 SPI_SendData(SPI1,byte);

  //while(SPI_GetFlagStatus(SPI1,SPI_FLAG_BSY));{};
  // Wait to receive a byte 
 while (SPI_GetFlagStatus(SPI1,SPI_FLAG_RXNE) == RESET);
  // Return the byte read from the SPI bus 
  return SPI_ReceiveData(SPI1);	 

}
/******************************************************************************
* 函数名    : SPIWriteOneByteToAddress
* 函数描述  : 向一个地址写一个字节
* 输入参数  : address:the address you need to write
              byte:need to write the byte
* 输出参数  : 无
* 返回值    : 无
******************************************************************************/
void SPIWriteOneByteToAddress(u8 addr,u8 byte)
{
    SPI_ENABLE();
    gSwitchEnStatus(enOpen); //NSS = 0;
    addr = (addr | 0x80);    
    SPIWriteOneByte(addr);
    SPIWriteOneByte(byte);
    gSwitchEnStatus(enClose); //NSS = 1;
    SPI_DISABLE();
}
/******************************************************************************
* 函数名    : SPIReadOneByteFromAddress
* 函数描述  : read one byte from a address
* 输入参数  : address:the address you need to read
              byte:need to read the byte
* 输出参数  : none
* 返回值    : none
******************************************************************************/
u8 SPIReadOneByteFromAddress(u8 addr)
{
    u8 receivedata;
    SPI_ENABLE();
    gSwitchEnStatus(enOpen); //NSS = 0;
    addr = (addr & 0x7f);    
    SPIWriteOneByte(addr);
    receivedata = SPIWriteOneByte(0);
    gSwitchEnStatus(enClose); //NSS = 1;
    return receivedata;
    SPI_DISABLE();
}

/******************************************************************************
* 函数名    : SPIReadDataFromFIFO
* 函数描述  : read FIFO
* 输入参数  : buffer:data buffer
              len:data length
* 输出参数  : none
* 返回值    : none
******************************************************************************/

void SPIReadDataFromFIFO(u8* buffer,u8 len)
{
    SPIWriteOneByteToAddress(REG_LR_FIFOADDRPTR, 0);//重新映射基地址
    SPI_ENABLE();
    gSwitchEnStatus(enOpen); //NSS = 0;
    SPIWriteOneByte(0);
    for(u8 i=0;i<len;i++)
    {
        buffer[i] = SPIWriteOneByte(0);   
    }
    gSwitchEnStatus(enClose); //NSS = 1;
    SPI_DISABLE();
    USART_SendString(recv,16);//发送当前发送端的数据
}

/******************************************************************************
* 函数名    : SPIWriteDatatoFIFO
* 函数描述  : write FIFO
* 输入参数  : buffer:data buffer
              len:data length
* 输出参数  : none
* 返回值    : none
******************************************************************************/

void SPIWriteDatatoFIFO( u8* buffer,u8 len)
{
    SPI_ENABLE();
    gSwitchEnStatus(enOpen); //NSS = 0;
    SPIWriteOneByte(0x80);
    for(u8 i=0;i<len;i++)
    {
        SPIWriteOneByte(buffer[i]);   
    }
    gSwitchEnStatus(enClose); //NSS = 1;
   SPI_DISABLE();
}

/******************************************************************************
* 函数名    : SX1278Reset
* 函数描述  : 设置复位
* 输入参数  : opMode：复位
* 输出参数  : 无
* 返回值    : 无
******************************************************************************/
void SX1278Reset(void)
{
    RF_REST_L;
    delay_ms(1);     //1.4ms      
    RF_REST_H;  
    delay_ms(5);//6.8ms
}
/******************************************************************************
* 函数名    : SX1276LoRaSetOpMode
* 函数描述  : set the mode of sx1278
* 输入参数  : opMode:the mode
* 输出参数  : none
* 返回值    : none
******************************************************************************/
void SX1276LoRaSetOpMode(RFMode_SET opMode)
{
    unsigned char opModePrev = 0;
    opModePrev = SPIReadOneByteFromAddress(REG_LR_OPMODE);//读0x01模式寄存器
    opModePrev &= 0xf8;//清零低三位
    opModePrev |= (unsigned char)opMode;      //进行处理 //或上形参
    SPIWriteOneByteToAddress(REG_LR_OPMODE, opModePrev);//附上新值
}

/******************************************************************************
* 函数名    : SX1276LoRaFsk
* 函数描述  : 设置运行是LoRa模式还是FSK模式
* 输入参数  : opMode：运行模式
* 输出参数  : 无
* 返回值    : 无
******************************************************************************/
void SX1276LoRaFsk(Debugging_fsk_ook opMode)
{
    unsigned char opModePrev = 0;
    opModePrev = SPIReadOneByteFromAddress(REG_LR_OPMODE);//读0x01模式寄存器
    opModePrev &= 0x7F;//清零最高位
    opModePrev |= (unsigned char)opMode;      //进行处理
    SPIWriteOneByteToAddress(REG_LR_OPMODE, opModePrev);//附上新值   
}

/******************************************************************************
* 函数名    : SX1276LoRaSetRFFrequency
* 函数描述  : 设置1276的频率
* 输入参数  : 无
* 输出参数  : 无
* 返回值    : 无
******************************************************************************/	
void SX1276LoRaSetRFFrequency(void)
{
    SPIWriteOneByteToAddress(REG_LR_FRFMSB, Frequency[0]);//0x06射频载波频率最高有效位
    SPIWriteOneByteToAddress(REG_LR_FRFMID, Frequency[1]);//0x07射频载波频率中间有效位
    SPIWriteOneByteToAddress(REG_LR_FRFLSB, Frequency[2]);//0x08射频载波频率最低有效位
}

/******************************************************************************
* 函数名    : SX1276LoRaSetRFPower
* 函数描述  : 发射机的发射频率
* 输入参数  : power：功率选择
* 输出参数  : 无
* 返回值    : 无
******************************************************************************/
void SX1276LoRaSetRFPower(unsigned char power)
{
    SPIWriteOneByteToAddress(REG_LR_PADAC, 0x87);  //当OutputPower = 1111,，在PA_BOOST 上为+20dBm
    SPIWriteOneByteToAddress(REG_LR_PACONFIG,  power_data[power]);//当power = 7时，发射最大功率+20dBm
    SPIWriteOneByteToAddress(REG_LR_PACONFIG,  0xFF);
}

/******************************************************************************
* 函数名    : SX1276LoRaSetNbTrigPeaks
* 函数描述  : 设置数据包的有效长度
* 输入参数  : value：数据包长度
* 输出参数  : 无
* 返回值    : 无
******************************************************************************/	
void SX1276LoRaSetNbTrigPeaks(unsigned char value)
{
    unsigned char RECVER_DAT;
    RECVER_DAT = SPIReadOneByteFromAddress(0x31);
    RECVER_DAT = (RECVER_DAT & 0xF8) | value;
    SPIWriteOneByteToAddress(0x31, RECVER_DAT);
}

/******************************************************************************
* 函数名    : SX1276LoRaSetSpreadingFactor
* 函数描述  : 设置扩频因子
* 输入参数  : factor：扩频因子数,最大值为12
* 输出参数  : 无
* 返回值    : 无
******************************************************************************/	
void SX1276LoRaSetSpreadingFactor(unsigned char factor)
{
    unsigned char RECVER_DAT;
    SX1276LoRaSetNbTrigPeaks(3);//0x03-->SF7 to SF12
    RECVER_DAT = SPIReadOneByteFromAddress(REG_LR_MODEMCONFIG2);//读0x1E寄存器  	  
    RECVER_DAT = (RECVER_DAT & RFLR_MODEMCONFIG2_SF_MASK) | (factor << 4);
    SPIWriteOneByteToAddress(REG_LR_MODEMCONFIG2, RECVER_DAT);	 
}

/******************************************************************************
* 函数名    : SX1276LoRaSetErrorCoding
* 函数描述  : 设置lora的纠错编码率
* 输入参数  : value:编码率值
              001 -> 4/5
              010 -> 4/6
              011 -> 4/7
              100 -> 4/8
* 输出参数  : 无
* 返回值    : 无
******************************************************************************/	
void SX1276LoRaSetErrorCoding(unsigned char value)
{	
    unsigned char RECVER_DAT;
    RECVER_DAT = SPIReadOneByteFromAddress( REG_LR_MODEMCONFIG1);//读0x1D寄存器
    RECVER_DAT = (RECVER_DAT & RFLR_MODEMCONFIG1_CODINGRATE_MASK) | (value << 1);//纠错编码率，位3~1。
    SPIWriteOneByteToAddress(REG_LR_MODEMCONFIG1, RECVER_DAT);
}

/******************************************************************************
* 函数名    : SX1276LoRaSetPacketCrcOn
* 函数描述  : 发送端CRC信息使能开关
* 输入参数  : enable:开关,0或1
* 输出参数  : 无
* 返回值    : 无
******************************************************************************/		
void SX1276LoRaSetPacketCrcOn(u8 enable)
{	
    unsigned char RECVER_DAT;
    RECVER_DAT = SPIReadOneByteFromAddress(REG_LR_MODEMCONFIG2);//读0x1E寄存器 
    RECVER_DAT = (RECVER_DAT & RFLR_MODEMCONFIG2_RXPAYLOADCRC_MASK) | (enable << 2);
    SPIWriteOneByteToAddress(REG_LR_MODEMCONFIG2, RECVER_DAT);
}

/******************************************************************************
* 函数名    : SX1276LoRaSetSignalBandwidth
* 函数描述  : 设置信号带宽
* 输入参数  : bw:带宽值
              0000 -> 7.8kHz
              0001 -> 10.4kHz
              0010 -> 15.6kHz
              0011 -> 20.8kHz
              0100 -> 31.25kHz
              0101 -> 41.7kHz
              0110 -> 62.5kHz
              0111 -> 125kHz
              1000 -> 250kHz
              1001 -> 500kHz
* 输出参数  : 无
* 返回值    : 无
******************************************************************************/	
void SX1276LoRaSetSignalBandwidth(unsigned char bw)
{
    unsigned char RECVER_DAT;
    RECVER_DAT = SPIReadOneByteFromAddress(REG_LR_MODEMCONFIG1);//读0x1D寄存器
    RECVER_DAT = (RECVER_DAT & RFLR_MODEMCONFIG1_BW_MASK) | (bw << 4);
    SPIWriteOneByteToAddress(REG_LR_MODEMCONFIG1, RECVER_DAT);
}

/******************************************************************************
* 函数名    : SX1276LoRaSetImplicitHeaderOn
* 函数描述  : 设置隐性报头模式使能
* 输入参数  : enable:开关 0或1
* 输出参数  : 无
* 返回值    : 无
******************************************************************************/	
void SX1276LoRaSetImplicitHeaderOn(BOOL enable)
{
    unsigned char RECVER_DAT;
    RECVER_DAT = SPIReadOneByteFromAddress(REG_LR_MODEMCONFIG1); //读0x1D寄存器
    RECVER_DAT = (RECVER_DAT & RFLR_MODEMCONFIG1_IMPLICITHEADER_MASK) | (enable);
    SPIWriteOneByteToAddress(REG_LR_MODEMCONFIG1, RECVER_DAT);
}

/******************************************************************************
* 函数名    : SX1276LoRaSetSymbTimeout
* 函数描述  : RX操作超时值
* 输入参数  : value:超时值
* 输出参数  : 无
* 返回值    : 无
******************************************************************************/		
void SX1276LoRaSetSymbTimeout(unsigned int value )
{
    unsigned char RECVER_DAT[2];
    RECVER_DAT[0] = SPIReadOneByteFromAddress(REG_LR_MODEMCONFIG2); //读0x1E寄存器
    RECVER_DAT[1] = SPIReadOneByteFromAddress(REG_LR_SYMBTIMEOUTLSB); //读0x1F寄存器
    RECVER_DAT[0] = (RECVER_DAT[0] & RFLR_MODEMCONFIG2_SYMBTIMEOUTMSB_MASK) | (( value >> 8) & ~RFLR_MODEMCONFIG2_SYMBTIMEOUTMSB_MASK);
    RECVER_DAT[1] = value & 0xFF;
    SPIWriteOneByteToAddress(REG_LR_MODEMCONFIG2, RECVER_DAT[0]);
    SPIWriteOneByteToAddress(REG_LR_SYMBTIMEOUTLSB, RECVER_DAT[1]);
}

/******************************************************************************
* 函数名    : SX1276LoRaSetSymbTimeout
* 函数描述  : 设置负载字节长度 隐式报头模式下需要设置寄存器，以达到预期的
              数据包长度。不允许将寄存器值设置为0。
* 输入参数  : value:负载字节长度值
* 输出参数  : 无
* 返回值    : 无
******************************************************************************/	
void SX1276LoRaSetPayloadLength(unsigned char value)
{
    SPIWriteOneByteToAddress(REG_LR_PAYLOADLENGTH, value);//写0x22寄存器，RegPayloadLength
} 


long SX1276GetFeiValue()
{
  long error28,error29,error2A;
  error28 = SPIReadOneByteFromAddress(0x28);
  error29 = SPIReadOneByteFromAddress(0x29);
  error2A = SPIReadOneByteFromAddress(0x2A);
  return (error28<<16 + error29<<8 + error2A);
}

/******************************************************************************
* 函数名    : SX1276LoRaSetPreamLength
* 函数描述  : 设置前导码长度最低有效位
* 输入参数  : value:设置前导码长度
* 输出参数  : 无
* 返回值    : 无
******************************************************************************/	

void SX1276LoRaSetPreamLength(unsigned int value)
{
    unsigned char a[2];
    a[0] = (value&0xff00)>>8;
    a[1] = value&0xff;
    SPIWriteOneByteToAddress(REG_LR_PREAMBLEMSB, a[0]);
    SPIWriteOneByteToAddress(REG_LR_PREAMBLELSB, a[1]);
}

/******************************************************************************
* 函数名    : SX1276LoRaSetMobileNode
* 函数描述  : 设置低速率优化
* 输入参数  : value:使能开关
* 输出参数  : 无
* 返回值    : 无
******************************************************************************/
void SX1276LoRaSetMobileNode(BOOL enable)
{	 
    unsigned char RECVER_DAT;
    RECVER_DAT=SPIReadOneByteFromAddress( REG_LR_MODEMCONFIG3);//读0x26寄存器，低数据率优化
    RECVER_DAT = (RECVER_DAT & RFLR_MODEMCONFIG3_MOBILE_NODE_MASK) | (enable << 3);
    SPIWriteOneByteToAddress(REG_LR_MODEMCONFIG3, RECVER_DAT);
}

/******************************************************************************
* 函数名    : Sx1278LoRaInit
* 函数描述  : SX1276初始化，
主要是通过SPI口进行设置。首先设置为睡眠模式，接着设置为LoRa扩频调制模式，然后再设置为空闲模式，
然后依次设置各种参数：端口映射、频率、功率、扩频因子、纠错编码率、CRC校验、扩频带宽、同步头、有效负载长度、接收超时时间、低数据率优化等，
若需要接收功能，初始化的最后建议设置为连续接收模式

* 输入参数  : 无
* 输出参数  : 无
* 返回值    : 无
******************************************************************************/
u8 Version;
void Sx1278LoRaInit(void)
{
    SX1276LoRaSetOpMode(Sleep_mode);                     //设置睡眠模式
    SX1276LoRaFsk(LORA_mode);	                         //设置扩频模式
    SX1276LoRaSetOpMode(Stdby_mode);                     //设置为普通模式
    SPIWriteOneByteToAddress(REG_LR_DIOMAPPING1,GPIO_VARE_1);//写入0x40寄存器，DIO引脚映射设置，设为00
    SPIWriteOneByteToAddress(REG_LR_DIOMAPPING2,GPIO_VARE_2);//写入0x41寄存器
    Version = SPIReadOneByteFromAddress(REG_LR_VERSION);//读取版本号
    //SX1276LoRaSetRFFrequency();                          //频率设置
    SX1276LoRaSetRFPower(7);                             //设置发射功率为最大  直接赋值0xFF 设置为最大发射功率
    //SX1276LoRaSetSpreadingFactor(SpreadingFactor[0]);	 //扩频因子设置   {7,8,9,10,11,12}; 扩频因子7-12
    SX1276LoRaSetSpreadingFactor(7);
    //SX1276LoRaSetErrorCoding(CodingRate[1]);		 //有效数据比 4/6
    SX1276LoRaSetErrorCoding(2);
    SX1276LoRaSetPacketCrcOn(true);			 //CRC 校验打开
    //SX1276LoRaSetSignalBandwidth(Bw_Frequency[8]);	 //设置扩频带宽   250KHZ
    SX1276LoRaSetSignalBandwidth(9);
    SX1276LoRaSetImplicitHeaderOn(true);		 //同步头是显性模式  explicit header
    SX1276LoRaSetPayloadLength(128);                    //设置负载字节长度256
    SX1276LoRaSetSymbTimeout(0x3FF);                     //设置接收超时时间,TimOut = SymbTimeout * ts
    SX1276LoRaSetMobileNode(true); 			 //低数据的优化 
    //SX1276LoRaSetPreamLength(65535);
    SPIWriteOneByteToAddress(REG_LR_PREAMBLEMSB, 0x00);
    SPIWriteOneByteToAddress(REG_LR_PREAMBLELSB, 0xff);//maxmim preamble
    //SPIWriteOneByteToAddress( REG_LR_PREAMBLEMSB, 0x00);//前导码
    //SPIWriteOneByteToAddress( REG_LR_PREAMBLELSB, 0x15);
    //Sx1278ReceiveModeEnable();                           //进入接收模式
    //USART_SendStr("LORA 初始化配置完成\r\n");
}

/******************************************************************************
* 函数名    : Sx1278SendPacket
* 函数描述  : 数据包发送，
Sx1278的模式切换必须先经过空闲模式，而后才能切换到相应的模式。
所以在数据准备好之后，做发射操作之前，应先将模块设置为空闲模式，
然后再配置相应的操作，如跳频设置、端口映射、有效负载长度，FIFO基址等，
然后以SPI突发访问的方式往缓冲区中送数据，送完后即可设置为发射模式，让其自动去发射
* 输入参数  : 无
* 输出参数  : 无
* 返回值    : 无
******************************************************************************/	
/*
void Sx1278SendPacket(unsigned char *data,unsigned char len)
{
    SX1276LoRaSetOpMode(Stdby_mode);                                //进入待机模式
    SPIWriteOneByteToAddress(REG_LR_HOPPERIOD, 0);	            //不做频率跳变
    SPIWriteOneByteToAddress(REG_LR_IRQFLAGSMASK,IRQN_TXD_Value);   //打开发送中断
    SPIWriteOneByteToAddress(REG_LR_PAYLOADLENGTH, len);	    //最大数据包
    SPIWriteOneByteToAddress(REG_LR_FIFOTXBASEADDR, 0);             //设置FIFO基地址
    SPIWriteOneByteToAddress(REG_LR_FIFOADDRPTR, 0);
    SPIWriteDatatoFIFO(data,len);  
    SPIWriteOneByteToAddress(REG_LR_DIOMAPPING1,0x40);   //设置0x40寄存器为0100 0000b，即设置发射完成指示映射到DIO0引脚
    SPIWriteOneByteToAddress(REG_LR_DIOMAPPING2,0x00);             
    SX1276LoRaSetOpMode(Transmitter_mode);                         //进入传输模式
      
    SPIReadDataFromFIFO(recv,16); //测试发送的数据是？？？？？？？
    
   // USART_SendStr("LORA 发送配置完成\r\n");
}
*/
void Sx1278SendPacket(unsigned char *data,unsigned char len)
{
    SX1276LoRaSetOpMode(Stdby_mode);                                //进入待机模式
    SPIWriteOneByteToAddress(REG_LR_HOPPERIOD, 0);              //不做频率跳变
    SPIWriteOneByteToAddress(REG_LR_IRQFLAGSMASK,IRQN_TXD_Value);   //打开发送中断
    SPIWriteOneByteToAddress(REG_LR_PAYLOADLENGTH, len);        //最大数据包
    SPIWriteOneByteToAddress(REG_LR_FIFOTXBASEADDR, 0);             //设置FIFO基地址
    SPIWriteOneByteToAddress(REG_LR_FIFOADDRPTR, 0);
    SPIWriteDatatoFIFO(data,len);
    SPIWriteOneByteToAddress(REG_LR_DIOMAPPING1,0x40);   //设置0x40寄存器为0100 0000b，即设置发射完成指示映射到DIO0引脚
    SPIWriteOneByteToAddress(REG_LR_DIOMAPPING2,0x00);    
    SX1276LoRaSetOpMode(Transmitter_mode);                         //进入传输模式
    while(1)
    {
        RF_EX0_STATUS = SPIReadOneByteFromAddress(REG_LR_IRQFLAGS);
        if((RF_EX0_STATUS&0x08) == 0x08)
        {
            //发射完成后，应设置为接收模式，才能收到信号
            SPIWriteOneByteToAddress(REG_LR_IRQFLAGS, RF_EX0_STATUS|0x08);
//            SX1276LoRaSetOpMode(Stdby_mode);
//            SPIWriteOneByteToAddress(REG_LR_IRQFLAGSMASK,IRQN_RXD_Value);      //打开接收中断
//            SPIWriteOneByteToAddress(REG_LR_HOPPERIOD,   PACKET_MIAX_Value);//0x24寄存器，设置频率跳变周期最大
//            SPIWriteOneByteToAddress(REG_LR_DIOMAPPING1, 0X00);//端口映射恢复默认
//            SPIWriteOneByteToAddress(REG_LR_DIOMAPPING2, 0x00);
//            SX1276LoRaSetOpMode(Receiver_mode);
            break;
        }
    }
   // SPIReadDataFromFIFO(recv,16);                       //测试发送的数据是？？？？？？？
}
/******************************************************************************
* 函数名    : Sx1278ReceiveModeEnable
* 函数描述  : Sx1278数据接收准备
* 输入参数  : 无
* 输出参数  : 无
* 返回值    : 无
******************************************************************************/	
void Sx1278ReceiveModeEnable(void)
{
    SX1276LoRaSetOpMode(Stdby_mode);  
    SPIWriteOneByteToAddress(REG_LR_IRQFLAGSMASK,IRQN_RXD_Value);  ////0x11,打开接收中断
    SPIWriteOneByteToAddress(REG_LR_HOPPERIOD,	PACKET_MIAX_Value );//0x24寄存器，设置频率跳变周期为最大
    SPIWriteOneByteToAddress(REG_LR_DIOMAPPING1, 0X00);//DIO引脚映射设置，按默认
    SPIWriteOneByteToAddress(REG_LR_DIOMAPPING2, 0X00);	
    SX1276LoRaSetOpMode(Receiver_mode);//设置为连续接收模式
}

/******************************************************************************
* 函数名    : Sx1278CADReceiveModeEnable
* 函数描述  : Sx1278CAD模式数据接收准备
* 输入参数  : 无
* 输出参数  : 无
* 返回值    : 无
******************************************************************************/
void Sx1278CADReceiveModeEnable(void)
{
    SX1276LoRaSetOpMode(Stdby_mode);  
    SPIWriteOneByteToAddress(REG_LR_IRQFLAGSMASK, IRQN_CAD_Value);//打开发送中断
    SPIWriteOneByteToAddress(REG_LR_DIOMAPPING1, 0X80);
    SPIWriteOneByteToAddress(REG_LR_DIOMAPPING2, 0X00);	
    SX1276LoRaSetOpMode(CAD_mode);
}

/******************************************************************************
* 函数名    : Sx1278SleepModeEnable
* 函数描述  : Sx1278进入睡眠模式
* 输入参数  : 无
* 输出参数  : 无
* 返回值    : 无
******************************************************************************/
void Sx1278SleepModeEnable(void)
{
    SX1276LoRaSetOpMode(Stdby_mode);  
    SPIWriteOneByteToAddress(REG_LR_IRQFLAGSMASK,  IRQN_SEELP_Value);  //打开所有中断
    SPIWriteOneByteToAddress(REG_LR_DIOMAPPING1, 0X00);
    SPIWriteOneByteToAddress(REG_LR_DIOMAPPING2, 0X00);	
    SX1276LoRaSetOpMode(Sleep_mode);
}

/******************************************************************************
* 函数名    : Sx1278SleepModeEnable
* 函数描述  : Sx1278进入睡眠模式，
             Sx1278的接收可以通过判断映射脚的状态（默认为DIO0，有高电平出现）
或者通过查询中断标志寄存器0x12的第六位的状态来决定是否接收到了数据。
下面以查询寄存器的方法讲解一下Sx1278的接收。
当检查到中断标志寄存器0x12的第六位置1的时候，就说明芯片完成了一个数据包的接收，
根据是否开了CRC可以进一步检测CRC是否正确，若正确后，先设置FIFO的基址，
然后读取0x13寄存器可以知道最后一个包所接收到的字节数N，
然后以SPI突发访问的方式从FIFO中读取N个字节，即是所接收到的数据。
数据读取并处理完成后，为了不影响下次接收，
建议重新配置为连续接收模式并清零中断标志寄存器0x12中的标志位（往里面写1即是）
* 输入参数  : 无
* 输出参数  : 无
* 返回值    : 无
******************************************************************************/
void Sx1278InteruptHandler(void)
{//RegIrqFlags的八位依次为： RxTimeout, RxDone, PayloadCrcError,ValidHeader, TxDone, CadDone, FhssChangeChannel, CadDetected.
  RF_EX0_STATUS = SPIReadOneByteFromAddress(REG_LR_IRQFLAGS); //读取0x12寄存器，中断标志寄存器
    if((RF_EX0_STATUS&0x40) == 0x40)//接收完成
    {
        SPIWriteOneByteToAddress(REG_LR_IRQFLAGS, 0xff);//清零所有标志位，所有的DIOx口都恢复低电平
        
        CRC_Value = SPIReadOneByteFromAddress(REG_LR_MODEMCONFIG2);
        if(CRC_Value&0x04 == 0x04)//是否CRC校验完成
        {
            SPIWriteOneByteToAddress(REG_LR_FIFOADDRPTR,0x00);//设置SPI接口在FIFO缓冲区中的地址指针
            SX1278_RLEN = SPIReadOneByteFromAddress(REG_LR_NBRXBYTES);//读取最后一个包的字节数
            SPI_ENABLE();//开启时钟
            gSwitchEnStatus(enOpen); //NSS = 0;                                                    
            SPIWriteOneByte(0x00);//0x00突发访问，读
            for(unsigned char i=0;i<SX1278_RLEN;i++)
            {  
                SPIWriteOneByte(0x00);
                recv[i] = SPI_ReceiveData(SPI1);
                
            }
            USART_SendString(recv,SX1278_RLEN);
            gSwitchEnStatus(enClose); //NSS = 1;   
            SPI_DISABLE();//关闭始终
        }       
        SX1276LoRaSetOpMode(Stdby_mode);
        SPIWriteOneByteToAddress(REG_LR_IRQFLAGSMASK, IRQN_RXD_Value);      //打开接收中断
        SPIWriteOneByteToAddress(REG_LR_HOPPERIOD,    PACKET_MIAX_Value);//0x24寄存器，设置频率跳变周期最大
        SPIWriteOneByteToAddress(REG_LR_DIOMAPPING1, 0X00);//端口映射恢复默认
        SPIWriteOneByteToAddress(REG_LR_DIOMAPPING2, 0x00);	
        SX1276LoRaSetOpMode(Receiver_mode);
    }
    else if((RF_EX0_STATUS&0x08) == 0x08)//发送完成
    {//发射完成后，应设置为接收模式，才能收到信号
        SPIWriteOneByteToAddress(REG_LR_IRQFLAGS, 0xff);//清零所有标志位，所有的DIOx口都恢复低电平
        SX1276LoRaSetOpMode(Stdby_mode);
        SPIWriteOneByteToAddress(REG_LR_IRQFLAGSMASK,IRQN_RXD_Value);      //打开接收中断
        SPIWriteOneByteToAddress(REG_LR_HOPPERIOD,   PACKET_MIAX_Value);//0x24寄存器，设置频率跳变周期最大
        SPIWriteOneByteToAddress(REG_LR_DIOMAPPING1, 0X00);//端口映射恢复默认
        SPIWriteOneByteToAddress(REG_LR_DIOMAPPING2, 0x00);	
        SX1276LoRaSetOpMode(Receiver_mode);
    }
    else if((RF_EX0_STATUS&0x04) == 0x04)
    {  
        SPIWriteOneByteToAddress(REG_LR_IRQFLAGS, 0xff);//清零所有标志位，所有的DIOx口都恢复低电平
        if((RF_EX0_STATUS&0x01) == 0x01)
        {     
            SX1276LoRaSetOpMode(Stdby_mode);                      //表示CAD 检测到扩频信号 模块进入了接收状态来接收数据
            SPIWriteOneByteToAddress(REG_LR_IRQFLAGSMASK,IRQN_RXD_Value);  //打开接收中断
            SPIWriteOneByteToAddress(REG_LR_HOPPERIOD,   PACKET_MIAX_Value);//0x24寄存器，设置频率跳变周期最大
            SPIWriteOneByteToAddress(REG_LR_DIOMAPPING1, 0X02);//payloadCrcError映射到DIO3
            SPIWriteOneByteToAddress(REG_LR_DIOMAPPING2, 0x00);	
            SX1276LoRaSetOpMode(Receiver_mode);
        }
        else
        {                           // 没检测到
            SX1276LoRaSetOpMode(Stdby_mode);
            SPIWriteOneByteToAddress(REG_LR_IRQFLAGSMASK,  IRQN_SEELP_Value);  //打开所有中断
            SPIWriteOneByteToAddress(REG_LR_DIOMAPPING1, 0X00);
            SPIWriteOneByteToAddress(REG_LR_DIOMAPPING2, 0X00);	
            SX1276LoRaSetOpMode(Sleep_mode);
        }
    }
    /*
    else
    {
        SX1276LoRaSetOpMode(Stdby_mode);
        SPIWriteOneByteToAddress(REG_LR_IRQFLAGSMASK,IRQN_RXD_Value);  //打开接收中断
        SPIWriteOneByteToAddress(REG_LR_HOPPERIOD,PACKET_MIAX_Value);//0x24寄存器，设置频率跳变周期最大
        SPIWriteOneByteToAddress(REG_LR_DIOMAPPING1, 0X02);//payloadCrcError映射到DIO3
        SPIWriteOneByteToAddress(REG_LR_DIOMAPPING2, 0x00);	
        SX1276LoRaSetOpMode(Receiver_mode);
    }
    SPIWriteOneByteToAddress(REG_LR_IRQFLAGS, 0xff);//清零所有标志位，所有的DIOx口都恢复低电平
    */
}

/*--------------------------------------------------------------------------------------------------------
                   									   0ooo
                   								ooo0      (   )
                								(   )     ) /
                								 \ (     (_/
                								  \_)
----------------------------------------------------------------------------------------------------------*/





