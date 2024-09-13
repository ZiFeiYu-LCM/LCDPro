#ifndef __I2CTEMPLATE_H
#define __I2CTEMPLATE_H
#include <stdio.h>
#include "stm32f10x_i2c.h"

struct i2cStruct;
typedef void (*void_funcPtr)(struct i2cStruct*);
struct i2cStruct{
	uint16_t 			sda_pin; 
	uint16_t 			sck_pin; 
	GPIO_TypeDef 		*sda_port; 
	GPIO_TypeDef 		*sck_port;	
	
	uint32_t			rcc_sda_port;	//RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	uint32_t			rcc_sck_port;
	
	void_funcPtr		initGpio;
	
	void_funcPtr		start;
	void_funcPtr		stop;
	
	void_funcPtr		sdaInput;
	void_funcPtr		sdaOnput;
	
	void (*sendData)(struct i2cStruct*, uint8_t);
	uint8_t (*readData)(struct i2cStruct*);
};

void i2cInit(struct i2cStruct* i2c, uint16_t sda_pin, uint16_t sck_pin, 
																		GPIO_TypeDef *sda_port, GPIO_TypeDef *sck_port,
																		uint32_t rcc_sda_port, uint32_t rcc_sck_port);
//extern struct i2cStruct i2cinfo;






























#endif

