#ifndef  __LightSensor_H__
#define  __LightSensor_H__

#include "stm8l15x.h"
#include "usart.h"
#include "SX1278.h"
/*************************************
*��������//PC0--SDA
*          PC1--SCL
***************************************/


#define IICSPEED     100000       //100K�ٶȣ�ʱ���ٶ�����1M��400K�ٶȣ�ʱ���ٶ�����4M
#define Slave_Address     0x94  //��ַ
#define MSB               0x03  //��λ
#define LSB               0x04     //��λ

//#define caijinum               "1"     //�ɼ����
//#define sensornum               "3"     //���������

#define IIC_SDA_GPIO_Pin                GPIO_Pin_0
#define IIC_SCL_GPIO_Pin                GPIO_Pin_1
#define IIC_GPIOx                       GPIOC

#define SCL_H         GPIO_Init(IIC_GPIOx,IIC_SCL_GPIO_Pin,GPIO_Mode_Out_PP_High_Slow)//set SCL pin as output high
#define SCL_L         GPIO_Init(IIC_GPIOx,IIC_SCL_GPIO_Pin,GPIO_Mode_Out_PP_Low_Slow)//set SCL pin as output low 
   
#define SDA_H         GPIO_Init(IIC_GPIOx,IIC_SDA_GPIO_Pin,GPIO_Mode_Out_PP_High_Slow)//set SDA pin as output high
#define SDA_L         GPIO_Init(IIC_GPIOx,IIC_SDA_GPIO_Pin,GPIO_Mode_Out_PP_Low_Slow)//set SDA pin as output low

//#define SCL_read      GPIO_ReadInputDataBit(IIC_GPIOx,IIC_SCL_GPIO_Pin)
#define SDA_read      GPIO_ReadInputDataBit(IIC_GPIOx,IIC_SDA_GPIO_Pin)




extern  float MAX44009light;


void I2C_GPIO_Config(void);
void I2C_Stop(void);
u8 Read_MAX44009(void);
void Init_MAX44009(void);

uint16_t get_MAX44009_data();

void Convert_MAX44009(void);


#endif
