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
#include "SX1278.h"

//unsigned char   Frequency[3]       = {0x6c,0x80,0x00};//434MH Ƶ������
unsigned char   Frequency[3]       = {0x6c,0xa5,0x00};//434MH Ƶ������
unsigned char   power_data[8]      = {0X80,0X80,0X80,0X83,0X86,0x89,0x8c,0x8f};//�������üĴ���
unsigned char   SpreadingFactor[6] = {7,8,9,10,11,12};         //��Ƶ����7-12
unsigned char   CodingRate[4]      = {1,2,3,4};                //1-4
unsigned char   Bw_Frequency[10]   = {0,1,2,3,4,5,6,7,8,9};    //����0-9//7.8KHz,10.4KHz,15.6KHz,20.8KHz,31.2KHz,41.7KHz,62.5KHz,125KHz,250KHz,500KHz
unsigned char   recv[200];
unsigned char   RF_EX0_STATUS;
unsigned char   CRC_Value;
unsigned char   SX1278_RLEN;


/******************************************************************************
* ������    : Delay
* ��������  : ��ʱ
* �������  : ��
* �������  : ��
* ����ֵ    : ��
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
void Delay_us(u8 x)//������ʱ,iic_40K
{
	u8 i=20;
	x=i*x;
	while(x--);
}
/*******************************************************************************
****��������:
****��������:��ʼ��SPI
****�汾:V1.0
****����:14-2-2014
****��ڲ���:��
****���ڲ���:��
****˵��:
********************************************************************************/
void SPI_SX1278_Init()
{

    CLK_PeripheralClockConfig (CLK_Peripheral_SPI1,ENABLE);//����SPIʱ��
    SPI_DeInit(SPI1);
    
    
       //SPI_CLOCK:PB5, SPI_MOSI: PB6, SPI_MISO: PB7  
    GPIO_Init(SX1278_SPI_GPIOX, SX1278_SPI_SCK, GPIO_Mode_Out_PP_Low_Fast);//GPIOB5-SCK
    GPIO_Init(SX1278_SPI_GPIOX, SX1278_SPI_MOSI, GPIO_Mode_Out_PP_High_Fast);//GPIOB6-MOSI  GPIO_Mode_Out_PP_High_Fast
    GPIO_Init(SX1278_SPI_GPIOX, SX1278_SPI_MISO, GPIO_Mode_In_PU_No_IT);//GPIOB7-MISO
  
    
     // ��ʼ��SPI 

        GPIO_Init(SX1278_SPI_GPIOX, SX1278_RES, GPIO_Mode_Out_PP_High_Fast);   //reset  PB3 
         // ����CS�ܽ� 
        GPIO_Init(SX1278_SPI_GPIOX, SX1278_SPI_SC, GPIO_Mode_Out_PP_High_Fast); //CS 
        RF_CE_H; // ���߲�ʹ���ⲿSPI�豸 

	SPI_Init(SPI1,\
                  SPI_FirstBit_MSB, // MSB is transmitted first 
                  SPI_BaudRatePrescaler_4, // Baudrate is Fsysclk/2=8MHz 
                  SPI_Mode_Master, // Master Mode 
                  SPI_CPOL_Low, // SCK=0 when idle 
                  SPI_CPHA_1Edge,// The 1st clock transition is the 1st data capture edge 
                  SPI_Direction_2Lines_FullDuplex, // 2-line undirection data mode 
                  SPI_NSS_Soft, // Software slave management enabled 
                  0x07);
 	  
   
	SPI_Cmd(SPI1,ENABLE);	// ʹ��SPI  
       
}
void SPI_ENABLE (void)
{
   CLK->PCKENR1 |= 0X10;//����SPIʱ��
   SPI1->CR1 |= SPI_CR1_SPE;// ʹ��SPI 
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
  CLK_PeripheralClockConfig (CLK_Peripheral_SPI1,DISABLE);//����SPIʱ��
  SPI_Cmd(SPI1,DISABLE);	// ʹ��SPI  
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
****��������:
****��������:�����ֽ�
****�汾:V1.0
****����:14-2-2014
****��ڲ���:��
****���ڲ���:SPI���յ�������
****˵��:
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
* ������    : SPIWriteOneByteToAddress
* ��������  : ��һ����ַдһ���ֽ�
* �������  : address:the address you need to write
              byte:need to write the byte
* �������  : ��
* ����ֵ    : ��
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
* ������    : SPIReadOneByteFromAddress
* ��������  : read one byte from a address
* �������  : address:the address you need to read
              byte:need to read the byte
* �������  : none
* ����ֵ    : none
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
* ������    : SPIReadDataFromFIFO
* ��������  : read FIFO
* �������  : buffer:data buffer
              len:data length
* �������  : none
* ����ֵ    : none
******************************************************************************/

void SPIReadDataFromFIFO(u8* buffer,u8 len)
{
    SPIWriteOneByteToAddress(REG_LR_FIFOADDRPTR, 0);//����ӳ�����ַ
    SPI_ENABLE();
    gSwitchEnStatus(enOpen); //NSS = 0;
    SPIWriteOneByte(0);
    for(u8 i=0;i<len;i++)
    {
        buffer[i] = SPIWriteOneByte(0);   
    }
    gSwitchEnStatus(enClose); //NSS = 1;
    SPI_DISABLE();
    USART_SendString(recv,16);//���͵�ǰ���Ͷ˵�����
}

/******************************************************************************
* ������    : SPIWriteDatatoFIFO
* ��������  : write FIFO
* �������  : buffer:data buffer
              len:data length
* �������  : none
* ����ֵ    : none
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
* ������    : SX1278Reset
* ��������  : ���ø�λ
* �������  : opMode����λ
* �������  : ��
* ����ֵ    : ��
******************************************************************************/
void SX1278Reset(void)
{
    RF_REST_L;
    delay_ms(1);     //1.4ms      
    RF_REST_H;  
    delay_ms(5);//6.8ms
}
/******************************************************************************
* ������    : SX1276LoRaSetOpMode
* ��������  : set the mode of sx1278
* �������  : opMode:the mode
* �������  : none
* ����ֵ    : none
******************************************************************************/
void SX1276LoRaSetOpMode(RFMode_SET opMode)
{
    unsigned char opModePrev = 0;
    opModePrev = SPIReadOneByteFromAddress(REG_LR_OPMODE);//��0x01ģʽ�Ĵ���
    opModePrev &= 0xf8;//�������λ
    opModePrev |= (unsigned char)opMode;      //���д��� //�����β�
    SPIWriteOneByteToAddress(REG_LR_OPMODE, opModePrev);//������ֵ
}

/******************************************************************************
* ������    : SX1276LoRaFsk
* ��������  : ����������LoRaģʽ����FSKģʽ
* �������  : opMode������ģʽ
* �������  : ��
* ����ֵ    : ��
******************************************************************************/
void SX1276LoRaFsk(Debugging_fsk_ook opMode)
{
    unsigned char opModePrev = 0;
    opModePrev = SPIReadOneByteFromAddress(REG_LR_OPMODE);//��0x01ģʽ�Ĵ���
    opModePrev &= 0x7F;//�������λ
    opModePrev |= (unsigned char)opMode;      //���д���
    SPIWriteOneByteToAddress(REG_LR_OPMODE, opModePrev);//������ֵ   
}

/******************************************************************************
* ������    : SX1276LoRaSetRFFrequency
* ��������  : ����1276��Ƶ��
* �������  : ��
* �������  : ��
* ����ֵ    : ��
******************************************************************************/	
void SX1276LoRaSetRFFrequency(void)
{
    SPIWriteOneByteToAddress(REG_LR_FRFMSB, Frequency[0]);//0x06��Ƶ�ز�Ƶ�������Чλ
    SPIWriteOneByteToAddress(REG_LR_FRFMID, Frequency[1]);//0x07��Ƶ�ز�Ƶ���м���Чλ
    SPIWriteOneByteToAddress(REG_LR_FRFLSB, Frequency[2]);//0x08��Ƶ�ز�Ƶ�������Чλ
}

/******************************************************************************
* ������    : SX1276LoRaSetRFPower
* ��������  : ������ķ���Ƶ��
* �������  : power������ѡ��
* �������  : ��
* ����ֵ    : ��
******************************************************************************/
void SX1276LoRaSetRFPower(unsigned char power)
{
    SPIWriteOneByteToAddress(REG_LR_PADAC, 0x87);  //��OutputPower = 1111,����PA_BOOST ��Ϊ+20dBm
    SPIWriteOneByteToAddress(REG_LR_PACONFIG,  power_data[power]);//��power = 7ʱ�����������+20dBm
    SPIWriteOneByteToAddress(REG_LR_PACONFIG,  0xFF);
}

/******************************************************************************
* ������    : SX1276LoRaSetNbTrigPeaks
* ��������  : �������ݰ�����Ч����
* �������  : value�����ݰ�����
* �������  : ��
* ����ֵ    : ��
******************************************************************************/	
void SX1276LoRaSetNbTrigPeaks(unsigned char value)
{
    unsigned char RECVER_DAT;
    RECVER_DAT = SPIReadOneByteFromAddress(0x31);
    RECVER_DAT = (RECVER_DAT & 0xF8) | value;
    SPIWriteOneByteToAddress(0x31, RECVER_DAT);
}

/******************************************************************************
* ������    : SX1276LoRaSetSpreadingFactor
* ��������  : ������Ƶ����
* �������  : factor����Ƶ������,���ֵΪ12
* �������  : ��
* ����ֵ    : ��
******************************************************************************/	
void SX1276LoRaSetSpreadingFactor(unsigned char factor)
{
    unsigned char RECVER_DAT;
    SX1276LoRaSetNbTrigPeaks(3);//0x03-->SF7 to SF12
    RECVER_DAT = SPIReadOneByteFromAddress(REG_LR_MODEMCONFIG2);//��0x1E�Ĵ���  	  
    RECVER_DAT = (RECVER_DAT & RFLR_MODEMCONFIG2_SF_MASK) | (factor << 4);
    SPIWriteOneByteToAddress(REG_LR_MODEMCONFIG2, RECVER_DAT);	 
}

/******************************************************************************
* ������    : SX1276LoRaSetErrorCoding
* ��������  : ����lora�ľ��������
* �������  : value:������ֵ
              001 -> 4/5
              010 -> 4/6
              011 -> 4/7
              100 -> 4/8
* �������  : ��
* ����ֵ    : ��
******************************************************************************/	
void SX1276LoRaSetErrorCoding(unsigned char value)
{	
    unsigned char RECVER_DAT;
    RECVER_DAT = SPIReadOneByteFromAddress( REG_LR_MODEMCONFIG1);//��0x1D�Ĵ���
    RECVER_DAT = (RECVER_DAT & RFLR_MODEMCONFIG1_CODINGRATE_MASK) | (value << 1);//��������ʣ�λ3~1��
    SPIWriteOneByteToAddress(REG_LR_MODEMCONFIG1, RECVER_DAT);
}

/******************************************************************************
* ������    : SX1276LoRaSetPacketCrcOn
* ��������  : ���Ͷ�CRC��Ϣʹ�ܿ���
* �������  : enable:����,0��1
* �������  : ��
* ����ֵ    : ��
******************************************************************************/		
void SX1276LoRaSetPacketCrcOn(u8 enable)
{	
    unsigned char RECVER_DAT;
    RECVER_DAT = SPIReadOneByteFromAddress(REG_LR_MODEMCONFIG2);//��0x1E�Ĵ��� 
    RECVER_DAT = (RECVER_DAT & RFLR_MODEMCONFIG2_RXPAYLOADCRC_MASK) | (enable << 2);
    SPIWriteOneByteToAddress(REG_LR_MODEMCONFIG2, RECVER_DAT);
}

/******************************************************************************
* ������    : SX1276LoRaSetSignalBandwidth
* ��������  : �����źŴ���
* �������  : bw:����ֵ
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
* �������  : ��
* ����ֵ    : ��
******************************************************************************/	
void SX1276LoRaSetSignalBandwidth(unsigned char bw)
{
    unsigned char RECVER_DAT;
    RECVER_DAT = SPIReadOneByteFromAddress(REG_LR_MODEMCONFIG1);//��0x1D�Ĵ���
    RECVER_DAT = (RECVER_DAT & RFLR_MODEMCONFIG1_BW_MASK) | (bw << 4);
    SPIWriteOneByteToAddress(REG_LR_MODEMCONFIG1, RECVER_DAT);
}

/******************************************************************************
* ������    : SX1276LoRaSetImplicitHeaderOn
* ��������  : �������Ա�ͷģʽʹ��
* �������  : enable:���� 0��1
* �������  : ��
* ����ֵ    : ��
******************************************************************************/	
void SX1276LoRaSetImplicitHeaderOn(BOOL enable)
{
    unsigned char RECVER_DAT;
    RECVER_DAT = SPIReadOneByteFromAddress(REG_LR_MODEMCONFIG1); //��0x1D�Ĵ���
    RECVER_DAT = (RECVER_DAT & RFLR_MODEMCONFIG1_IMPLICITHEADER_MASK) | (enable);
    SPIWriteOneByteToAddress(REG_LR_MODEMCONFIG1, RECVER_DAT);
}

/******************************************************************************
* ������    : SX1276LoRaSetSymbTimeout
* ��������  : RX������ʱֵ
* �������  : value:��ʱֵ
* �������  : ��
* ����ֵ    : ��
******************************************************************************/		
void SX1276LoRaSetSymbTimeout(unsigned int value )
{
    unsigned char RECVER_DAT[2];
    RECVER_DAT[0] = SPIReadOneByteFromAddress(REG_LR_MODEMCONFIG2); //��0x1E�Ĵ���
    RECVER_DAT[1] = SPIReadOneByteFromAddress(REG_LR_SYMBTIMEOUTLSB); //��0x1F�Ĵ���
    RECVER_DAT[0] = (RECVER_DAT[0] & RFLR_MODEMCONFIG2_SYMBTIMEOUTMSB_MASK) | (( value >> 8) & ~RFLR_MODEMCONFIG2_SYMBTIMEOUTMSB_MASK);
    RECVER_DAT[1] = value & 0xFF;
    SPIWriteOneByteToAddress(REG_LR_MODEMCONFIG2, RECVER_DAT[0]);
    SPIWriteOneByteToAddress(REG_LR_SYMBTIMEOUTLSB, RECVER_DAT[1]);
}

/******************************************************************************
* ������    : SX1276LoRaSetSymbTimeout
* ��������  : ���ø����ֽڳ��� ��ʽ��ͷģʽ����Ҫ���üĴ������ԴﵽԤ�ڵ�
              ���ݰ����ȡ��������Ĵ���ֵ����Ϊ0��
* �������  : value:�����ֽڳ���ֵ
* �������  : ��
* ����ֵ    : ��
******************************************************************************/	
void SX1276LoRaSetPayloadLength(unsigned char value)
{
    SPIWriteOneByteToAddress(REG_LR_PAYLOADLENGTH, value);//д0x22�Ĵ�����RegPayloadLength
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
* ������    : SX1276LoRaSetPreamLength
* ��������  : ����ǰ���볤�������Чλ
* �������  : value:����ǰ���볤��
* �������  : ��
* ����ֵ    : ��
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
* ������    : SX1276LoRaSetMobileNode
* ��������  : ���õ������Ż�
* �������  : value:ʹ�ܿ���
* �������  : ��
* ����ֵ    : ��
******************************************************************************/
void SX1276LoRaSetMobileNode(BOOL enable)
{	 
    unsigned char RECVER_DAT;
    RECVER_DAT=SPIReadOneByteFromAddress( REG_LR_MODEMCONFIG3);//��0x26�Ĵ��������������Ż�
    RECVER_DAT = (RECVER_DAT & RFLR_MODEMCONFIG3_MOBILE_NODE_MASK) | (enable << 3);
    SPIWriteOneByteToAddress(REG_LR_MODEMCONFIG3, RECVER_DAT);
}

/******************************************************************************
* ������    : Sx1278LoRaInit
* ��������  : SX1276��ʼ����
��Ҫ��ͨ��SPI�ڽ������á���������Ϊ˯��ģʽ����������ΪLoRa��Ƶ����ģʽ��Ȼ��������Ϊ����ģʽ��
Ȼ���������ø��ֲ������˿�ӳ�䡢Ƶ�ʡ����ʡ���Ƶ���ӡ���������ʡ�CRCУ�顢��Ƶ����ͬ��ͷ����Ч���س��ȡ����ճ�ʱʱ�䡢���������Ż��ȣ�
����Ҫ���չ��ܣ���ʼ�������������Ϊ��������ģʽ

* �������  : ��
* �������  : ��
* ����ֵ    : ��
******************************************************************************/
u8 Version;
void Sx1278LoRaInit(void)
{
    SX1276LoRaSetOpMode(Sleep_mode);                     //����˯��ģʽ
    SX1276LoRaFsk(LORA_mode);	                         //������Ƶģʽ
    SX1276LoRaSetOpMode(Stdby_mode);                     //����Ϊ��ͨģʽ
    SPIWriteOneByteToAddress(REG_LR_DIOMAPPING1,GPIO_VARE_1);//д��0x40�Ĵ�����DIO����ӳ�����ã���Ϊ00
    SPIWriteOneByteToAddress(REG_LR_DIOMAPPING2,GPIO_VARE_2);//д��0x41�Ĵ���
    Version = SPIReadOneByteFromAddress(REG_LR_VERSION);//��ȡ�汾��
    //SX1276LoRaSetRFFrequency();                          //Ƶ������
    SX1276LoRaSetRFPower(7);                             //���÷��书��Ϊ���  ֱ�Ӹ�ֵ0xFF ����Ϊ����书��
    //SX1276LoRaSetSpreadingFactor(SpreadingFactor[0]);	 //��Ƶ��������   {7,8,9,10,11,12}; ��Ƶ����7-12
    SX1276LoRaSetSpreadingFactor(7);
    //SX1276LoRaSetErrorCoding(CodingRate[1]);		 //��Ч���ݱ� 4/6
    SX1276LoRaSetErrorCoding(2);
    SX1276LoRaSetPacketCrcOn(true);			 //CRC У���
    //SX1276LoRaSetSignalBandwidth(Bw_Frequency[8]);	 //������Ƶ����   250KHZ
    SX1276LoRaSetSignalBandwidth(9);
    SX1276LoRaSetImplicitHeaderOn(true);		 //ͬ��ͷ������ģʽ  explicit header
    SX1276LoRaSetPayloadLength(128);                    //���ø����ֽڳ���256
    SX1276LoRaSetSymbTimeout(0x3FF);                     //���ý��ճ�ʱʱ��,TimOut = SymbTimeout * ts
    SX1276LoRaSetMobileNode(true); 			 //�����ݵ��Ż� 
    //SX1276LoRaSetPreamLength(65535);
    SPIWriteOneByteToAddress(REG_LR_PREAMBLEMSB, 0x00);
    SPIWriteOneByteToAddress(REG_LR_PREAMBLELSB, 0xff);//maxmim preamble
    //SPIWriteOneByteToAddress( REG_LR_PREAMBLEMSB, 0x00);//ǰ����
    //SPIWriteOneByteToAddress( REG_LR_PREAMBLELSB, 0x15);
    //Sx1278ReceiveModeEnable();                           //�������ģʽ
    //USART_SendStr("LORA ��ʼ���������\r\n");
}

/******************************************************************************
* ������    : Sx1278SendPacket
* ��������  : ���ݰ����ͣ�
Sx1278��ģʽ�л������Ⱦ�������ģʽ����������л�����Ӧ��ģʽ��
����������׼����֮�����������֮ǰ��Ӧ�Ƚ�ģ������Ϊ����ģʽ��
Ȼ����������Ӧ�Ĳ���������Ƶ���á��˿�ӳ�䡢��Ч���س��ȣ�FIFO��ַ�ȣ�
Ȼ����SPIͻ�����ʵķ�ʽ���������������ݣ�����󼴿�����Ϊ����ģʽ�������Զ�ȥ����
* �������  : ��
* �������  : ��
* ����ֵ    : ��
******************************************************************************/	
/*
void Sx1278SendPacket(unsigned char *data,unsigned char len)
{
    SX1276LoRaSetOpMode(Stdby_mode);                                //�������ģʽ
    SPIWriteOneByteToAddress(REG_LR_HOPPERIOD, 0);	            //����Ƶ������
    SPIWriteOneByteToAddress(REG_LR_IRQFLAGSMASK,IRQN_TXD_Value);   //�򿪷����ж�
    SPIWriteOneByteToAddress(REG_LR_PAYLOADLENGTH, len);	    //������ݰ�
    SPIWriteOneByteToAddress(REG_LR_FIFOTXBASEADDR, 0);             //����FIFO����ַ
    SPIWriteOneByteToAddress(REG_LR_FIFOADDRPTR, 0);
    SPIWriteDatatoFIFO(data,len);  
    SPIWriteOneByteToAddress(REG_LR_DIOMAPPING1,0x40);   //����0x40�Ĵ���Ϊ0100 0000b�������÷������ָʾӳ�䵽DIO0����
    SPIWriteOneByteToAddress(REG_LR_DIOMAPPING2,0x00);             
    SX1276LoRaSetOpMode(Transmitter_mode);                         //���봫��ģʽ
      
    SPIReadDataFromFIFO(recv,16); //���Է��͵������ǣ�������������
    
   // USART_SendStr("LORA �����������\r\n");
}
*/
void Sx1278SendPacket(unsigned char *data,unsigned char len)
{
    SX1276LoRaSetOpMode(Stdby_mode);                                //�������ģʽ
    SPIWriteOneByteToAddress(REG_LR_HOPPERIOD, 0);              //����Ƶ������
    SPIWriteOneByteToAddress(REG_LR_IRQFLAGSMASK,IRQN_TXD_Value);   //�򿪷����ж�
    SPIWriteOneByteToAddress(REG_LR_PAYLOADLENGTH, len);        //������ݰ�
    SPIWriteOneByteToAddress(REG_LR_FIFOTXBASEADDR, 0);             //����FIFO����ַ
    SPIWriteOneByteToAddress(REG_LR_FIFOADDRPTR, 0);
    SPIWriteDatatoFIFO(data,len);
    SPIWriteOneByteToAddress(REG_LR_DIOMAPPING1,0x40);   //����0x40�Ĵ���Ϊ0100 0000b�������÷������ָʾӳ�䵽DIO0����
    SPIWriteOneByteToAddress(REG_LR_DIOMAPPING2,0x00);    
    SX1276LoRaSetOpMode(Transmitter_mode);                         //���봫��ģʽ
    while(1)
    {
        RF_EX0_STATUS = SPIReadOneByteFromAddress(REG_LR_IRQFLAGS);
        if((RF_EX0_STATUS&0x08) == 0x08)
        {
            //������ɺ�Ӧ����Ϊ����ģʽ�������յ��ź�
            SPIWriteOneByteToAddress(REG_LR_IRQFLAGS, RF_EX0_STATUS|0x08);
//            SX1276LoRaSetOpMode(Stdby_mode);
//            SPIWriteOneByteToAddress(REG_LR_IRQFLAGSMASK,IRQN_RXD_Value);      //�򿪽����ж�
//            SPIWriteOneByteToAddress(REG_LR_HOPPERIOD,   PACKET_MIAX_Value);//0x24�Ĵ���������Ƶ�������������
//            SPIWriteOneByteToAddress(REG_LR_DIOMAPPING1, 0X00);//�˿�ӳ��ָ�Ĭ��
//            SPIWriteOneByteToAddress(REG_LR_DIOMAPPING2, 0x00);
//            SX1276LoRaSetOpMode(Receiver_mode);
            break;
        }
    }
   // SPIReadDataFromFIFO(recv,16);                       //���Է��͵������ǣ�������������
}
/******************************************************************************
* ������    : Sx1278ReceiveModeEnable
* ��������  : Sx1278���ݽ���׼��
* �������  : ��
* �������  : ��
* ����ֵ    : ��
******************************************************************************/	
void Sx1278ReceiveModeEnable(void)
{
    SX1276LoRaSetOpMode(Stdby_mode);  
    SPIWriteOneByteToAddress(REG_LR_IRQFLAGSMASK,IRQN_RXD_Value);  ////0x11,�򿪽����ж�
    SPIWriteOneByteToAddress(REG_LR_HOPPERIOD,	PACKET_MIAX_Value );//0x24�Ĵ���������Ƶ����������Ϊ���
    SPIWriteOneByteToAddress(REG_LR_DIOMAPPING1, 0X00);//DIO����ӳ�����ã���Ĭ��
    SPIWriteOneByteToAddress(REG_LR_DIOMAPPING2, 0X00);	
    SX1276LoRaSetOpMode(Receiver_mode);//����Ϊ��������ģʽ
}

/******************************************************************************
* ������    : Sx1278CADReceiveModeEnable
* ��������  : Sx1278CADģʽ���ݽ���׼��
* �������  : ��
* �������  : ��
* ����ֵ    : ��
******************************************************************************/
void Sx1278CADReceiveModeEnable(void)
{
    SX1276LoRaSetOpMode(Stdby_mode);  
    SPIWriteOneByteToAddress(REG_LR_IRQFLAGSMASK, IRQN_CAD_Value);//�򿪷����ж�
    SPIWriteOneByteToAddress(REG_LR_DIOMAPPING1, 0X80);
    SPIWriteOneByteToAddress(REG_LR_DIOMAPPING2, 0X00);	
    SX1276LoRaSetOpMode(CAD_mode);
}

/******************************************************************************
* ������    : Sx1278SleepModeEnable
* ��������  : Sx1278����˯��ģʽ
* �������  : ��
* �������  : ��
* ����ֵ    : ��
******************************************************************************/
void Sx1278SleepModeEnable(void)
{
    SX1276LoRaSetOpMode(Stdby_mode);  
    SPIWriteOneByteToAddress(REG_LR_IRQFLAGSMASK,  IRQN_SEELP_Value);  //�������ж�
    SPIWriteOneByteToAddress(REG_LR_DIOMAPPING1, 0X00);
    SPIWriteOneByteToAddress(REG_LR_DIOMAPPING2, 0X00);	
    SX1276LoRaSetOpMode(Sleep_mode);
}

/******************************************************************************
* ������    : Sx1278SleepModeEnable
* ��������  : Sx1278����˯��ģʽ��
             Sx1278�Ľ��տ���ͨ���ж�ӳ��ŵ�״̬��Ĭ��ΪDIO0���иߵ�ƽ���֣�
����ͨ����ѯ�жϱ�־�Ĵ���0x12�ĵ���λ��״̬�������Ƿ���յ������ݡ�
�����Բ�ѯ�Ĵ����ķ�������һ��Sx1278�Ľ��ա�
����鵽�жϱ�־�Ĵ���0x12�ĵ���λ��1��ʱ�򣬾�˵��оƬ�����һ�����ݰ��Ľ��գ�
�����Ƿ���CRC���Խ�һ�����CRC�Ƿ���ȷ������ȷ��������FIFO�Ļ�ַ��
Ȼ���ȡ0x13�Ĵ�������֪�����һ���������յ����ֽ���N��
Ȼ����SPIͻ�����ʵķ�ʽ��FIFO�ж�ȡN���ֽڣ����������յ������ݡ�
���ݶ�ȡ��������ɺ�Ϊ�˲�Ӱ���´ν��գ�
������������Ϊ��������ģʽ�������жϱ�־�Ĵ���0x12�еı�־λ��������д1���ǣ�
* �������  : ��
* �������  : ��
* ����ֵ    : ��
******************************************************************************/
void Sx1278InteruptHandler(void)
{//RegIrqFlags�İ�λ����Ϊ�� RxTimeout, RxDone, PayloadCrcError,ValidHeader, TxDone, CadDone, FhssChangeChannel, CadDetected.
  RF_EX0_STATUS = SPIReadOneByteFromAddress(REG_LR_IRQFLAGS); //��ȡ0x12�Ĵ������жϱ�־�Ĵ���
    if((RF_EX0_STATUS&0x40) == 0x40)//�������
    {
        SPIWriteOneByteToAddress(REG_LR_IRQFLAGS, 0xff);//�������б�־λ�����е�DIOx�ڶ��ָ��͵�ƽ
        
        CRC_Value = SPIReadOneByteFromAddress(REG_LR_MODEMCONFIG2);
        if(CRC_Value&0x04 == 0x04)//�Ƿ�CRCУ�����
        {
            SPIWriteOneByteToAddress(REG_LR_FIFOADDRPTR,0x00);//����SPI�ӿ���FIFO�������еĵ�ַָ��
            SX1278_RLEN = SPIReadOneByteFromAddress(REG_LR_NBRXBYTES);//��ȡ���һ�������ֽ���
            SPI_ENABLE();//����ʱ��
            gSwitchEnStatus(enOpen); //NSS = 0;                                                    
            SPIWriteOneByte(0x00);//0x00ͻ�����ʣ���
            for(unsigned char i=0;i<SX1278_RLEN;i++)
            {  
                SPIWriteOneByte(0x00);
                recv[i] = SPI_ReceiveData(SPI1);
                
            }
            USART_SendString(recv,SX1278_RLEN);
            gSwitchEnStatus(enClose); //NSS = 1;   
            SPI_DISABLE();//�ر�ʼ��
        }       
        SX1276LoRaSetOpMode(Stdby_mode);
        SPIWriteOneByteToAddress(REG_LR_IRQFLAGSMASK, IRQN_RXD_Value);      //�򿪽����ж�
        SPIWriteOneByteToAddress(REG_LR_HOPPERIOD,    PACKET_MIAX_Value);//0x24�Ĵ���������Ƶ�������������
        SPIWriteOneByteToAddress(REG_LR_DIOMAPPING1, 0X00);//�˿�ӳ��ָ�Ĭ��
        SPIWriteOneByteToAddress(REG_LR_DIOMAPPING2, 0x00);	
        SX1276LoRaSetOpMode(Receiver_mode);
    }
    else if((RF_EX0_STATUS&0x08) == 0x08)//�������
    {//������ɺ�Ӧ����Ϊ����ģʽ�������յ��ź�
        SPIWriteOneByteToAddress(REG_LR_IRQFLAGS, 0xff);//�������б�־λ�����е�DIOx�ڶ��ָ��͵�ƽ
        SX1276LoRaSetOpMode(Stdby_mode);
        SPIWriteOneByteToAddress(REG_LR_IRQFLAGSMASK,IRQN_RXD_Value);      //�򿪽����ж�
        SPIWriteOneByteToAddress(REG_LR_HOPPERIOD,   PACKET_MIAX_Value);//0x24�Ĵ���������Ƶ�������������
        SPIWriteOneByteToAddress(REG_LR_DIOMAPPING1, 0X00);//�˿�ӳ��ָ�Ĭ��
        SPIWriteOneByteToAddress(REG_LR_DIOMAPPING2, 0x00);	
        SX1276LoRaSetOpMode(Receiver_mode);
    }
    else if((RF_EX0_STATUS&0x04) == 0x04)
    {  
        SPIWriteOneByteToAddress(REG_LR_IRQFLAGS, 0xff);//�������б�־λ�����е�DIOx�ڶ��ָ��͵�ƽ
        if((RF_EX0_STATUS&0x01) == 0x01)
        {     
            SX1276LoRaSetOpMode(Stdby_mode);                      //��ʾCAD ��⵽��Ƶ�ź� ģ������˽���״̬����������
            SPIWriteOneByteToAddress(REG_LR_IRQFLAGSMASK,IRQN_RXD_Value);  //�򿪽����ж�
            SPIWriteOneByteToAddress(REG_LR_HOPPERIOD,   PACKET_MIAX_Value);//0x24�Ĵ���������Ƶ�������������
            SPIWriteOneByteToAddress(REG_LR_DIOMAPPING1, 0X02);//payloadCrcErrorӳ�䵽DIO3
            SPIWriteOneByteToAddress(REG_LR_DIOMAPPING2, 0x00);	
            SX1276LoRaSetOpMode(Receiver_mode);
        }
        else
        {                           // û��⵽
            SX1276LoRaSetOpMode(Stdby_mode);
            SPIWriteOneByteToAddress(REG_LR_IRQFLAGSMASK,  IRQN_SEELP_Value);  //�������ж�
            SPIWriteOneByteToAddress(REG_LR_DIOMAPPING1, 0X00);
            SPIWriteOneByteToAddress(REG_LR_DIOMAPPING2, 0X00);	
            SX1276LoRaSetOpMode(Sleep_mode);
        }
    }
    /*
    else
    {
        SX1276LoRaSetOpMode(Stdby_mode);
        SPIWriteOneByteToAddress(REG_LR_IRQFLAGSMASK,IRQN_RXD_Value);  //�򿪽����ж�
        SPIWriteOneByteToAddress(REG_LR_HOPPERIOD,PACKET_MIAX_Value);//0x24�Ĵ���������Ƶ�������������
        SPIWriteOneByteToAddress(REG_LR_DIOMAPPING1, 0X02);//payloadCrcErrorӳ�䵽DIO3
        SPIWriteOneByteToAddress(REG_LR_DIOMAPPING2, 0x00);	
        SX1276LoRaSetOpMode(Receiver_mode);
    }
    SPIWriteOneByteToAddress(REG_LR_IRQFLAGS, 0xff);//�������б�־λ�����е�DIOx�ڶ��ָ��͵�ƽ
    */
}

/*--------------------------------------------------------------------------------------------------------
                   									   0ooo
                   								ooo0      (   )
                								(   )     ) /
                								 \ (     (_/
                								  \_)
----------------------------------------------------------------------------------------------------------*/





