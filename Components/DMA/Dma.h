#ifndef  __DMA_H__
#define  __DMA_H__

#include "stm8l15x.h"



void MYDMA_Config(DMA_Channel_TypeDef* DMA_CHx,u32 RxBuffer,u16 cndtr);
extern void MYDMA_Enable(DMA_Channel_TypeDef*DMA_CHx);
extern void MYDMA_Disable(DMA_Channel_TypeDef*DMA_CHx);
/**********************************************************************
                        相关结构
**********************************************************************/

	
/**********************************************************************
                        相关函数
**********************************************************************/


#endif 
