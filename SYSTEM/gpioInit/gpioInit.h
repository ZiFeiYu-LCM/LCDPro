#ifndef __GPIOINIT_H__
#include "stm32f10x_gpio.h"
#include "stm32f10x_rcc.h"
#include "misc.h"



/*
uint16_t GPIO_PIN       				������					���� GPIO_Pin_2
GPIO_TypeDef *GPIO_PORT					���˿ں�				���� GPIOA
uint32_t RCC_GPIO_PORT					����Ӧ��ʱ��	 	���� RCC_APB2Periph_GPIOA
GPIOSpeed_TypeDef GPIO_SPEED		���ٶ�					���� GPIO_Speed_50MHz
GPIOMode_TypeDef GPIO_MODE			������ģʽ   	���� GPIO_Mode_AF_PP
*/

void GPIOInit(uint16_t GPIO_PIN, 
							GPIO_TypeDef *GPIO_PORT,
							uint32_t RCC_GPIO, 
							GPIOSpeed_TypeDef GPIO_SPEED, 
							GPIOMode_TypeDef GPIO_MODE);




#endif

