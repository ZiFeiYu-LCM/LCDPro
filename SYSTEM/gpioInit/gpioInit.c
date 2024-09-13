#include "./gpioInit/gpioInit.h"


/*
uint16_t GPIO_PIN       				：引脚					例如 GPIO_Pin_2
GPIO_TypeDef *GPIO_PORT					：端口号				例如 GPIOA
uint32_t RCC_GPIO_PORT					：对应的时钟	 	例如 RCC_APB2Periph_GPIOA
GPIOSpeed_TypeDef GPIO_SPEED		：速度					例如 GPIO_Speed_50MHz
GPIOMode_TypeDef GPIO_MODE			：引脚模式   	例如 GPIO_Mode_AF_PP
*/

void GPIOInit(uint16_t GPIO_PIN, GPIO_TypeDef *GPIO_PORT,uint32_t RCC_GPIO, GPIOSpeed_TypeDef GPIO_SPEED,  GPIOMode_TypeDef GPIO_MODE){
	GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_GPIO, ENABLE);  // 使能 GPIOA 时钟
	
	GPIO_InitStructure.GPIO_Pin = GPIO_PIN;
	GPIO_InitStructure.GPIO_Speed = GPIO_SPEED;
	GPIO_InitStructure.GPIO_Mode = GPIO_MODE;
	GPIO_Init(GPIO_PORT, &GPIO_InitStructure);
}



