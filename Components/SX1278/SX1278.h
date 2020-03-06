#ifndef  __SX1278_H__
#define  __SX1278_H__

#include "stm8l15x.h"
#include "delay.h"

#define  gSwitchEnStatus 	cmdSwitchEn


#define SX1278_SPI_GPIOX      GPIOB
#define SX1278_RES            GPIO_Pin_3  
#define SX1278_SPI_SC         GPIO_Pin_4
#define SX1278_SPI_SCK        GPIO_Pin_5
#define SX1278_SPI_MOSI       GPIO_Pin_6
#define SX1278_SPI_MISO       GPIO_Pin_7


#define  SX1278_SDO                         GPIO_ReadInputDataBit(SX1278_SPI_GPIOX,SX1278_SPI_MOSI)


#define  RF_REST_L			    GPIO_ResetBits(SX1278_SPI_GPIOX, SX1278_RES)	  
#define  RF_REST_H			    GPIO_SetBits(SX1278_SPI_GPIOX, SX1278_RES)	  

#define  RF_CE_L                            GPIO_ResetBits(SX1278_SPI_GPIOX, SX1278_SPI_SC)      
#define  RF_CE_H                            GPIO_SetBits(SX1278_SPI_GPIOX, SX1278_SPI_SC)         

#define  RF_CKL_L                           GPIO_ResetBits(SX1278_SPI_GPIOX, SX1278_SPI_SCK)     
#define  RF_CKL_H                           GPIO_SetBits(SX1278_SPI_GPIOX, SX1278_SPI_SCK)      

#define  RF_SDI_L                           GPIO_ResetBits(SX1278_SPI_GPIOX, SX1278_SPI_MISO)     
#define  RF_SDI_H                           GPIO_SetBits(SX1278_SPI_GPIOX, SX1278_SPI_MISO)         



#define  REG_LR_FIFO                                 0x00 
 // Common settings
#define REG_LR_OPMODE                                0x01 
#define REG_LR_BANDSETTING                           0x04
#define REG_LR_FRFMSB                                0x06 
#define REG_LR_FRFMID                                0x07
#define REG_LR_FRFLSB                                0x08 
 // Tx settings
#define REG_LR_PACONFIG                              0x09 
#define REG_LR_PARAMP                                0x0A 
#define REG_LR_OCP                                   0x0B 
 // Rx settings
#define REG_LR_LNA                                   0x0C 
 // LoRa registers
#define REG_LR_FIFOADDRPTR                           0x0D 
#define REG_LR_FIFOTXBASEADDR                        0x0E 
#define REG_LR_FIFORXBASEADDR                        0x0F 
#define REG_LR_FIFORXCURRENTADDR                     0x10 
#define REG_LR_IRQFLAGSMASK                          0x11 
#define REG_LR_IRQFLAGS                              0x12 
#define REG_LR_NBRXBYTES                             0x13 
#define REG_LR_RXHEADERCNTVALUEMSB                   0x14 
#define REG_LR_RXHEADERCNTVALUELSB                   0x15
#define REG_LR_RXPACKETCNTVALUEMSB                   0x16
#define REG_LR_RXPACKETCNTVALUELSB                   0x17 
#define REG_LR_MODEMSTAT                             0x18 
#define REG_LR_PKTSNRVALUE                           0x19 
#define REG_LR_PKTRSSIVALUE                          0x1A 
#define REG_LR_RSSIVALUE                             0x1B 
#define REG_LR_HOPCHANNEL                            0x1C 
#define REG_LR_MODEMCONFIG1                          0x1D 
#define REG_LR_MODEMCONFIG2                          0x1E 
#define REG_LR_SYMBTIMEOUTLSB                        0x1F 
#define REG_LR_PREAMBLEMSB                           0x20 
#define REG_LR_PREAMBLELSB                           0x21 
#define REG_LR_PAYLOADLENGTH                         0x22 
#define REG_LR_PAYLOADMAXLENGTH                      0x23 
#define REG_LR_HOPPERIOD                             0x24 
#define REG_LR_FIFORXBYTEADDR                        0x25
#define REG_LR_MODEMCONFIG3                          0x26
 // end of documented register in datasheet
 // I/O settings
#define REG_LR_DIOMAPPING1                           0x40
#define REG_LR_DIOMAPPING2                           0x41
 // Version
#define REG_LR_VERSION                               0x42
 // Additional settings
#define REG_LR_PLLHOP                                0x44
#define REG_LR_TCXO                                  0x4B
#define REG_LR_PADAC                                 0x4D
#define REG_LR_FORMERTEMP                            0x5B
#define REG_LR_BITRATEFRAC                           0x5D
#define REG_LR_AGCREF                                0x61
#define REG_LR_AGCTHRESH1                            0x62
#define REG_LR_AGCTHRESH2                            0x63
#define REG_LR_AGCTHRESH3                            0x64

#define GPIO_VARE_1                                  0X00
#define GPIO_VARE_2                                  0X00
#define RFLR_MODEMCONFIG2_SF_MASK                    0x0f
#define RFLR_MODEMCONFIG1_CODINGRATE_MASK            0xF1 
#define RFLR_MODEMCONFIG2_RXPAYLOADCRC_MASK          0xFB 
#define RFLR_MODEMCONFIG1_BW_MASK                    0x0F 
#define RFLR_MODEMCONFIG1_IMPLICITHEADER_MASK        0xFE 
#define RFLR_MODEMCONFIG2_SYMBTIMEOUTMSB_MASK        0xfc
#define RFLR_MODEMCONFIG3_MOBILE_NODE_MASK           0xF7 

#define TIME_OUT_INT                                 0x80 
#define PACKET_RECVER_INT                            0x40 
#define CRC_ERROR_INT                                0x20 
#define RECVER_HEAR_INT                              0x10 
#define FIFO_SEND_OVER                               0x08 
#define RFLR_IRQFLAGS_CAD                            0x04 
#define RFLR_IRQFLAGS_FHSS                           0x02 
#define RFLR_IRQFLAGS_CADD                           0x01 

#define IRQN_TXD_Value                               0xF7
#define IRQN_RXD_Value                               0x9F
#define IRQN_CAD_Value                               0xFA
#define IRQN_SEELP_Value                             0xFF
#define PACKET_MIAX_Value                            0xff

/*!
 * RegDioMapping1
 */
#define RFLR_DIOMAPPING1_DIO0_MASK                  0x3F
#define RFLR_DIOMAPPING1_DIO0_00                    0x00  // Default
#define RFLR_DIOMAPPING1_DIO0_01                    0x40
#define RFLR_DIOMAPPING1_DIO0_10                    0x80
#define RFLR_DIOMAPPING1_DIO0_11                    0xC0

#define RFLR_DIOMAPPING1_DIO1_MASK                  0xCF
#define RFLR_DIOMAPPING1_DIO1_00                    0x00  // Default
#define RFLR_DIOMAPPING1_DIO1_01                    0x10
#define RFLR_DIOMAPPING1_DIO1_10                    0x20
#define RFLR_DIOMAPPING1_DIO1_11                    0x30

#define RFLR_DIOMAPPING1_DIO2_MASK                  0xF3
#define RFLR_DIOMAPPING1_DIO2_00                    0x00  // Default
#define RFLR_DIOMAPPING1_DIO2_01                    0x04
#define RFLR_DIOMAPPING1_DIO2_10                    0x08
#define RFLR_DIOMAPPING1_DIO2_11                    0x0C

#define RFLR_DIOMAPPING1_DIO3_MASK                  0xFC
#define RFLR_DIOMAPPING1_DIO3_00                    0x00  // Default
#define RFLR_DIOMAPPING1_DIO3_01                    0x01
#define RFLR_DIOMAPPING1_DIO3_10                    0x02
#define RFLR_DIOMAPPING1_DIO3_11                    0x03

typedef enum 
{
   Sleep_mode	        = (unsigned char)0x00, 
   Stdby_mode	        = (unsigned char)0x01, 
   TX_mode 	        = (unsigned char)0x02,
   Transmitter_mode	= (unsigned char)0x03,
   RF_mode 		= (unsigned char)0x04,
   Receiver_mode	= (unsigned char)0x05,
   receive_single	= (unsigned char)0x06,
   CAD_mode		= (unsigned char)0x07,
}RFMode_SET;

typedef enum 
{
   FSK_mode             = (unsigned char)0x00, 
   LORA_mode            = (unsigned char)0x80, 
}  Debugging_fsk_ook;

typedef enum{false=0,true=1}BOOL;

typedef enum{enOpen,enClose}cmdEntype_t;

typedef enum{rxOpen,txOpen}cmdpaType_t;



extern unsigned char   recv[200];

void SPI_SX1278_Init();

void Delay_us(u8 x);
extern unsigned char SPIWriteOneByte(unsigned char byte);

//void SPIWriteOneByte(uint8_t out);
//uint8_t RF_SPI_READ_BYTE();

void RF_SPI_MasterIO(uint8_t out);
uint8_t RF_SPI_READ_BYTE();

void SPIWriteOneByteToAddress(u8 addr,u8 byte);
u8 SPIReadOneByteFromAddress(u8 addr);
void SX1278Reset(void);
void SX1278_INIT(void);
void SX1276LoRaSetOpMode(RFMode_SET opMode);
void SX1276LoRaFsk(Debugging_fsk_ook opMode);
void SX1276LoRaSetRFFrequency(void);
void SX1276LoRaSetRFPower(unsigned char power);
void SX1276LoRaSetNbTrigPeaks(unsigned char value);
void SX1276LoRaSetSpreadingFactor(unsigned char factor);
void SX1276LoRaSetErrorCoding(unsigned char value);
void SX1276LoRaSetPacketCrcOn(u8 enable);
void SX1276LoRaSetSignalBandwidth(unsigned char bw);
void SX1276LoRaSetImplicitHeaderOn(BOOL enable);
void SX1276LoRaSetSymbTimeout(unsigned int value);
void SX1276LoRaSetPayloadLength(unsigned char value);
void SX1276LoRaSetPreamLength(unsigned int value);
void SX1276LoRaSetMobileNode(BOOL enable);
void Sx1278LoRaInit(void);
void Sx1278SendPacket(unsigned char *data,unsigned char len);
void Sx1278ReceiveModeEnable(void);
void Sx1278CADReceiveModeEnable(void);
void Sx1278SleepModeEnable(void);
void Sx1278InteruptHandler(void);
long SX1276GetFeiValue();

unsigned char RF_SPI_READ_BYTE();

void SPI_ENABLE (void);
void SPI_DISABLE (void);
void cmdSwitchEn(cmdEntype_t cmdcmd);
#endif 



