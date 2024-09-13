#include "./i2c/i2cTemplate.h"
#include "stm32f10x_gpio.h"


//初始化i2c引脚
void i2c_initGpio(struct i2cStruct* i2c){
	GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_APB2PeriphClockCmd(i2c->rcc_sda_port,ENABLE);
	RCC_APB2PeriphClockCmd(i2c->rcc_sck_port,ENABLE);
	
	GPIO_InitStructure.GPIO_Pin = i2c->sck_pin;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	
	GPIO_Init(i2c->sck_port, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = i2c->sda_pin;
	GPIO_Init(i2c->sda_port, &GPIO_InitStructure);
}

//发送一位高信号
void sdaH(struct i2cStruct* i2c){
	
	GPIO_WriteBit(i2c->sda_port,i2c->sda_pin,Bit_SET);
	GPIO_WriteBit(i2c->sck_port,i2c->sck_pin,Bit_SET);
	
	GPIO_WriteBit(i2c->sck_port,i2c->sck_pin,Bit_RESET);
	GPIO_WriteBit(i2c->sda_port,i2c->sda_pin,Bit_RESET);
}
//发送一位低信号
void sdaL(struct i2cStruct* i2c){
	GPIO_WriteBit(i2c->sda_port,i2c->sda_pin,Bit_RESET);
	GPIO_WriteBit(i2c->sck_port,i2c->sck_pin,Bit_SET);
	
	GPIO_WriteBit(i2c->sck_port,i2c->sck_pin,Bit_RESET);
}

//i2c开始
void start(struct i2cStruct* i2c){
	GPIO_SetBits(i2c->sda_port,i2c->sda_pin);
	
	GPIO_SetBits(i2c->sck_port,i2c->sck_pin);
	GPIO_ResetBits(i2c->sda_port,i2c->sda_pin);
	
	GPIO_ResetBits(i2c->sck_port,i2c->sck_pin);
}
//i2c结束
void stop(struct i2cStruct* i2c){
	GPIO_ResetBits(i2c->sda_port,i2c->sda_pin);
	
	GPIO_SetBits(i2c->sck_port,i2c->sck_pin);
	GPIO_SetBits(i2c->sda_port,i2c->sda_pin);
	
	GPIO_ResetBits(i2c->sck_port,i2c->sck_pin);
}

//读取一位
uint8_t readBit(struct i2cStruct* i2c){
	GPIO_SetBits(i2c->sck_port, i2c->sck_pin);
  uint8_t bit = GPIO_ReadInputDataBit(i2c->sda_port, i2c->sda_pin);
	GPIO_ResetBits(i2c->sck_port, i2c->sck_pin);
  return bit;
}
//读取一个字节
uint8_t i2c_readData(struct i2cStruct* i2c){
	uint8_t data = 0;
	for (int i = 0; i < 8; i++) {
			data <<= 1;
			if (readBit(i2c)) {
					data |= 0x01;
			}
	}
	sdaL(i2c);
	return data;
}
//发送一个字节
void i2c_sendData(struct i2cStruct* i2c, uint8_t data){
	for(uint8_t i=0;i<8;i++){
		if(data & 0x80){
			sdaH(i2c);
		}else{
			sdaL(i2c);
		}
		data <<= 1;
	}
//	if(readBit(i2c) != 0)//读取ACK
//		printf("没有接收到ACK\n");
}


//设置SDA为输入引脚
void sdaInput(struct i2cStruct* i2c){
    GPIO_InitTypeDef GPIO_InitStructure;

    GPIO_InitStructure.GPIO_Pin = i2c->sda_pin;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING; // 配置为浮空输入模式
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(i2c->sda_port, &GPIO_InitStructure);
}
//设置SDA为输出引脚
void sdaOnput(struct i2cStruct* i2c){
	  GPIO_InitTypeDef GPIO_InitStructure;

    GPIO_InitStructure.GPIO_Pin = i2c->sda_pin;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD; // 配置为开漏输出模式
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(i2c->sda_port, &GPIO_InitStructure);
}



void i2cInit(struct i2cStruct* i2c, uint16_t sda_pin, uint16_t sck_pin, 
																		GPIO_TypeDef *sda_port, GPIO_TypeDef *sck_port,
																		uint32_t rcc_sda_port, uint32_t rcc_sck_port)
{
	i2c->sda_pin = sda_pin;
	i2c->sda_port = sda_port;
	i2c->sck_pin = sck_pin;
	i2c->sck_port = sck_port;
	i2c->rcc_sck_port = rcc_sck_port;
	i2c->rcc_sda_port = rcc_sda_port;
	
	i2c->initGpio = i2c_initGpio;
	i2c->start = start;
	i2c->stop = stop;
	i2c->sdaInput = sdaInput;
	i2c->sdaOnput = sdaOnput;
	i2c->sendData = i2c_sendData;
	i2c->readData = i2c_readData;
}



















