#ifndef __GPIOINIT_H__
#include "stm32f10x_gpio.h"
#include "stm32f10x_rcc.h"
#include "misc.h"



/*
uint16_t GPIO_PIN       				：引脚					例如 GPIO_Pin_2
GPIO_TypeDef *GPIO_PORT					：端口号				例如 GPIOA
uint32_t RCC_GPIO_PORT					：对应的时钟	 	例如 RCC_APB2Periph_GPIOA
GPIOSpeed_TypeDef GPIO_SPEED		：速度					例如 GPIO_Speed_50MHz
GPIOMode_TypeDef GPIO_MODE			：引脚模式   	例如 GPIO_Mode_AF_PP
*/

void GPIOInit(uint16_t GPIO_PIN, 
							GPIO_TypeDef *GPIO_PORT,
							uint32_t RCC_GPIO, 
							GPIOSpeed_TypeDef GPIO_SPEED, 
							GPIOMode_TypeDef GPIO_MODE);




#endif

