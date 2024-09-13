#ifndef __FSMC_H
#define __FSMC_H
#include "stm32f10x.h"
#include "stm32f10x_fsmc.h"



#define FSMC_D0 		GPIO_Pin_14
#define FSMC_D1 		GPIO_Pin_15
#define FSMC_D2 		GPIO_Pin_0
#define FSMC_D3 		GPIO_Pin_1
#define FSMC_D13		GPIO_Pin_8
#define FSMC_D14		GPIO_Pin_9
#define FSMC_D15		GPIO_Pin_10

#define FSMC_NEx		4
#define FSMC_Ax			10
#define FSMC_NE4		GPIO_Pin_12
#define FSMC_A10		GPIO_Pin_0
#define FSMC_D_GPIO	GPIOD


#define FSMC_D4 		GPIO_Pin_7
#define FSMC_D5 		GPIO_Pin_8
#define FSMC_D6 		GPIO_Pin_9
#define FSMC_D7 		GPIO_Pin_10
#define FSMC_D8 		GPIO_Pin_11
#define FSMC_D9 		GPIO_Pin_12
#define FSMC_D10		GPIO_Pin_13
#define FSMC_D11		GPIO_Pin_14
#define FSMC_D12		GPIO_Pin_15
#define FSMC_E_GPIO	GPIOE



#define FSMC_NWE		GPIO_Pin_5
#define FSMC_NOE		GPIO_Pin_4
#define FSMC_G_GPIO	GPIOG




#define FSMC_NE_Write(x) 	do{ x?GPIO_SetBits(FSMC_D_GPIO,FSMC_NE4);:GPIO_ResetBits(FSMC_D_GPIO,FSMC_NE4);}while(0)
#define FSMC_A_Write(x)	 	do{ x?GPIO_SetBits(FSMC_D_GPIO,FSMC_A10);:GPIO_ResetBits(FSMC_D_GPIO,FSMC_A10);}while(0)
#define FSMC_NEW_Write(x) do{ x?GPIO_SetBits(FSMC_G_GPIO,FSMC_NWE);:GPIO_ResetBits(FSMC_G_GPIO,FSMC_NWE);}while(0)
#define FSMC_NOE_Write(x) do{ x?GPIO_SetBits(FSMC_G_GPIO,FSMC_NOE);:GPIO_ResetBits(FSMC_G_GPIO,FSMC_NOE);}while(0)



void FSMC_Init(void);


#endif
