#ifndef __SPITEMPLATE_H
#define __SPITEMPLATE_H
#include <stdio.h>
#include "stm32f10x_i2c.h"

struct spiStruct{
	uint16_t 			cs; 
	uint16_t 			sck; 
	uint16_t 			miso; 
	uint16_t 			mosi; 
	
	GPIO_TypeDef 		*cs_port; 
	GPIO_TypeDef 		*sck_port;	
	GPIO_TypeDef 		*miso_port; 
	GPIO_TypeDef 		*mosi_port;	
	
	uint32_t			rcc_cs_port;	//RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	uint32_t			rcc_sck_port;
	uint32_t			rcc_miso_port;	
	uint32_t			rcc_mosi_port;
	
	uint8_t				mode;//0 从低位开始发送    1 从高位发送 
	

	void (*initGpio)(struct spiStruct*);
	void (*sendData)(struct spiStruct* spi, uint8_t *data, uint8_t dataSize);
	void (*readData)(struct spiStruct* spi, uint8_t *data, uint8_t dataSize);
	void (*sendANDread)(struct spiStruct*, uint8_t*,uint8_t*, uint8_t);
};

void spiInit(struct spiStruct* spi, uint16_t cs, uint16_t sck, uint16_t miso, uint16_t mosi,
							GPIO_TypeDef *cs_port, GPIO_TypeDef *sck_port, GPIO_TypeDef *miso_port, GPIO_TypeDef *mosi_port,
							uint32_t rcc_cs_port, uint32_t rcc_sck_port, uint32_t rcc_miso_port, uint32_t rcc_mosi_port,
						uint8_t mode);
#endif
