#include "usartTemp.h"
#include "stdio.h"
#include "string.h"

#include "./gpioInit/gpioInit.h"

uint8_t rxBuf[RX_BUF_LEN];
uint8_t RX_INDEX = 0;
#if 1
// 串口初始化函数，使用 USART2，TX 引脚为 PA2，RX 引脚为 PA3
void UART_Init(uint32_t baudrate) {
    // 初始化结构体
    GPIO_InitTypeDef GPIO_InitStructure;
    USART_InitTypeDef USART_InitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;
			
    // 使能 GPIOA 和 USART2 时钟------------------------------------------------------------------这里要根据对应实际情况修改
		RCC_APB2PeriphClockCmd(RCC_UART, ENABLE); // 使能 USART2 时钟
    RCC_APB2PeriphClockCmd(RCC_GPIO_PORT, ENABLE);  // 使能 GPIOA 时钟
    /**/

    // 初始化 PA2 (TX) 为复用推挽输出模式
    GPIO_InitStructure.GPIO_Pin = UART_TX_PIN;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_Init(UART_TX_PORT, &GPIO_InitStructure);

    // 初始化 PA3 (RX) 为浮空输入模式
    GPIO_InitStructure.GPIO_Pin = UART_RX_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_Init(UART_RX_PORT, &GPIO_InitStructure);
		
	
	
    // 配置 USART2 参数
    USART_InitStructure.USART_BaudRate = baudrate;                      // 波特率
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;         // 8位数据
    USART_InitStructure.USART_StopBits = USART_StopBits_1;              // 1个停止位
    USART_InitStructure.USART_Parity = USART_Parity_No;                 // 无校验位
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None; // 无硬件流控制
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;     // 接收和发送模式
    USART_Init(UART_NAME, &USART_InitStructure);                           // 初始化 USART2

    // 配置 USART2 中断
    NVIC_InitStructure.NVIC_IRQChannel = UART_IRQ;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;           // 抢占优先级
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;                  // 子优先级
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);

    // 使能 USART 接收中断
    USART_ITConfig(UART_NAME, USART_IT_RXNE, ENABLE);

    // 使能 USART2
    USART_Cmd(UART_NAME, ENABLE);
}
#endif

//通过uartArray初始化多个串口
void UART_Init_Many(struct uartInfo* uartArray, uint8_t size) {
    // 初始化结构体
    GPIO_InitTypeDef GPIO_InitStructure;
    USART_InitTypeDef USART_InitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;

		for(uint8_t i=0;i<size;i++){
			// 使能 GPIOA 和 USART2 时钟
			RCC_APB2PeriphClockCmd(uartArray[i].rcc_gpio_port, ENABLE);  // 使能 GPIOA 时钟
			RCC_APB1PeriphClockCmd(uartArray[i].rcc_uart, ENABLE); // 使能 USART2 时钟

			// 初始化 PA2 (TX) 为复用推挽输出模式
			GPIO_InitStructure.GPIO_Pin = uartArray[i].uart_tx_pin;
			GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
			GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
			GPIO_Init(uartArray[i].uart_tx_port, &GPIO_InitStructure);

			// 初始化 PA3 (RX) 为浮空输入模式
			GPIO_InitStructure.GPIO_Pin = uartArray[i].uart_rx_pin;
			GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
			GPIO_Init(uartArray[i].uart_rx_port, &GPIO_InitStructure);

			// 配置 USART2 参数
			USART_InitStructure.USART_BaudRate = uartArray[i].baudrate;                      // 波特率
			USART_InitStructure.USART_WordLength = USART_WordLength_8b;         // 8位数据
			USART_InitStructure.USART_StopBits = USART_StopBits_1;              // 1个停止位
			USART_InitStructure.USART_Parity = USART_Parity_No;                 // 无校验位
			USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None; // 无硬件流控制
			USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;     // 接收和发送模式
			USART_Init(uartArray[i].uart_name, &USART_InitStructure);                           // 初始化 USART2

			// 配置 USART2 中断
			NVIC_InitStructure.NVIC_IRQChannel = uartArray[i].uart_irq;
			NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;           // 抢占优先级
			NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;                  // 子优先级
			NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
			NVIC_Init(&NVIC_InitStructure);

			// 使能 USART 接收中断
			USART_ITConfig(uartArray[i].uart_name, USART_IT_RXNE, ENABLE);

			// 使能 USART2
			USART_Cmd(uartArray[i].uart_name, ENABLE);
	}
}


//通过特宏定义的串口发送
// 向串口发送一个字符
void UART_SendChar(char ch) {
    USART_SendData(UART_NAME, (uint16_t) ch);
    while (USART_GetFlagStatus(UART_NAME, USART_FLAG_TC) == RESET);
}
// 向串口发送一个字符串
void UART_SendString(const char *str) {
    while (*str) {
        UART_SendChar(*str++);
    }
}


//通过特定串口发送
// 向串口发送一个字符
void UART_SendChar_By(USART_TypeDef* uartName, char ch) {
		while (USART_GetFlagStatus(uartName, USART_FLAG_TXE) == RESET){};//不加这部分可能printf第一个字符丢失
    USART_SendData(uartName, (uint16_t) ch);
    while (USART_GetFlagStatus(uartName, USART_FLAG_TC) == RESET){};
}
// 向串口发送一个字符串
void UART_SendString_By(USART_TypeDef* uartName,const char *str) {
    while (*str) {
        UART_SendChar_By(uartName,*str++);
    }
}

#if 1
//因为这里fputc和正点原子的usart冲突，所以先注释
int fputc(int ch, FILE *f) {
    if (ch == '\n') {
        uint8_t cr = '\r';
        UART_SendChar_By(UART_PRINT,cr);
    }
    UART_SendChar_By(UART_PRINT,ch);
    return ch;
}
#endif

// USART2 中断服务函数
void USART2_IRQHandler(void) {
    if (USART_GetITStatus(UART_NAME, USART_IT_RXNE) != RESET) {
				if(RX_INDEX == 0){
					memset(rxBuf,0,RX_BUF_LEN);
				}
        // 读取接收到的数据
        char data = (char)USART_ReceiveData(UART_NAME);
				if(data == '\n' || RX_INDEX == (RX_BUF_LEN-2)){
					RX_INDEX = 0;
					printf("%s\n",rxBuf);
				}else{
					rxBuf[RX_INDEX++] = data;
				}
    }
		
}







