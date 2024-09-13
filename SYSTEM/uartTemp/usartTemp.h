#ifndef __USART_TEMP_H__
#define __USART_TEMP_H__
#include "stm32f10x_usart.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_rcc.h"
#include "misc.h"




#define RX_BUF_LEN  			200  	//定义最大接收字节数 200
extern uint8_t rxBuf[RX_BUF_LEN];

#define UART_PRINT  		USART1


#define UART_TX_PIN   	GPIO_Pin_9
#define UART_RX_PIN   	GPIO_Pin_10 
#define UART_TX_PORT  	GPIOA
#define UART_RX_PORT  	GPIOA
#define UART_NAME				USART1
#define RCC_GPIO_PORT 	RCC_APB2Periph_GPIOA
#define RCC_UART				RCC_APB2Periph_USART1
#define UART_IRQ				USART1_IRQn


//可以通过这个结构体  和  UART_Init_Many 初始化多个串口
struct uartInfo{
	uint16_t 				uart_tx_pin; 
	uint16_t 				uart_rx_pin; 
	GPIO_TypeDef 		*uart_tx_port; 
	GPIO_TypeDef 		*uart_rx_port;	
	USART_TypeDef 	*uart_name;	
	uint32_t				rcc_gpio_port;	
	uint32_t				rcc_uart;		
	IRQn_Type				uart_irq;	
	uint32_t 				baudrate;	
};
	/*
	struct uartInfo info[1];
	info[0].baudrate 				= 115200;
	info[0].rcc_gpio_port		= RCC_GPIO_PORT;
	info[0].rcc_uart				= RCC_UART;
	info[0].uart_irq				= UART_IRQ;
	info[0].uart_name				= UART_NAME;
	info[0].uart_rx_pin			= UART_RX_PIN;
	info[0].uart_rx_port		= UART_RX_PORT;
	info[0].uart_tx_pin			= UART_TX_PIN;
	info[0].uart_tx_port		= UART_TX_PORT;
	UART_Init_Many(info,1);
	*/


void UART_Init(uint32_t baudrate);
void UART_SendString(const char *str);
void UART_SendChar(char ch);

//初始化多个串口
void UART_Init_Many(struct uartInfo* uartArray, uint8_t size);
//通过某个串口发送数据
void UART_SendChar_By(USART_TypeDef* uartName, char ch);
void UART_SendString_By(USART_TypeDef* uartName,const char *str);

//UART2中断函数
void USART2_IRQHandler(void);

#endif


