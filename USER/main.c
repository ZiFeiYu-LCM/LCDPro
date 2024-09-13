#include "stm32f10x.h"
#include "stdio.h"
#include "./uartTemp/usartTemp.h"
#include "./timerInit/timerInit.h"
#include "./delay/delay.h"
#include "./usart/usart.h"
#include "./i2c/i2cTemplate.h"
#include "./spi/spiTemplate.h"
#include "./LCD/lcd.h"
void RCC_Configuration(void);

int main(void){
	RCC_Configuration();
	delay_init(72);
	
	UART_Init(115200);
	
//	struct i2cStruct myI2c;
//	i2cInit(&myI2c,GPIO_Pin_13,GPIO_Pin_15,GPIOB,GPIOB,RCC_APB2Periph_GPIOB,RCC_APB2Periph_GPIOB);
//	myI2c.initGpio(&myI2c);
//	myI2c.start(&myI2c);
//	myI2c.sendData(&myI2c,0x78);
//	myI2c.stop(&myI2c);
	
	
//	struct spiStruct mySpi;
//	spiInit(&mySpi,GPIO_Pin_13,GPIO_Pin_15,GPIO_Pin_9,GPIO_Pin_11,
//	GPIOB,GPIOB,GPIOD,GPIOD,RCC_APB2Periph_GPIOB,RCC_APB2Periph_GPIOB,RCC_APB2Periph_GPIOD,RCC_APB2Periph_GPIOD,1);
//	uint8_t data[4] = {0x11,0x22,0x33,0x88};
//	mySpi.initGpio(&mySpi);
//	mySpi.sendData(&mySpi,data,4);
	
	printf("123\n");
	
	LCD_Init();
	LCD_Display_On();
	printf("LCD ID  =  %04X\n",lcdDev.id);
	LCD_Clear(BLUE);
	LCD_Draw_Point(20,20,RED);


	while(1){
		

	}
}

void RCC_Configuration(void){
	RCC_DeInit();
	RCC_HSEConfig(RCC_HSE_ON);
	
	if(RCC_WaitForHSEStartUp() == SUCCESS){
		FLASH_SetLatency(FLASH_Latency_2);
		FLASH_PrefetchBufferCmd(ENABLE);
		
		RCC_PLLConfig(RCC_PLLSource_HSE_Div1,RCC_PLLMul_9);
		RCC_PLLCmd(ENABLE);
		
		while(RCC_GetFlagStatus(RCC_FLAG_PLLRDY) == RESET);
		
		RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);
		
		while(RCC_GetSYSCLKSource()!= 0x08);
		
		RCC_HCLKConfig(RCC_SYSCLK_Div1);
		RCC_PCLK1Config(RCC_SYSCLK_Div2);
		RCC_PCLK2Config(RCC_SYSCLK_Div1);
	}
}













