#include "usartTemp.h"
#include "stdio.h"
#include "string.h"

#include "./gpioInit/gpioInit.h"

uint8_t rxBuf[RX_BUF_LEN];
uint8_t RX_INDEX = 0;
#if 1
// ���ڳ�ʼ��������ʹ�� USART2��TX ����Ϊ PA2��RX ����Ϊ PA3
void UART_Init(uint32_t baudrate) {
    // ��ʼ���ṹ��
    GPIO_InitTypeDef GPIO_InitStructure;
    USART_InitTypeDef USART_InitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;
			
    // ʹ�� GPIOA �� USART2 ʱ��------------------------------------------------------------------����Ҫ���ݶ�Ӧʵ������޸�
		RCC_APB2PeriphClockCmd(RCC_UART, ENABLE); // ʹ�� USART2 ʱ��
    RCC_APB2PeriphClockCmd(RCC_GPIO_PORT, ENABLE);  // ʹ�� GPIOA ʱ��
    /**/

    // ��ʼ�� PA2 (TX) Ϊ�����������ģʽ
    GPIO_InitStructure.GPIO_Pin = UART_TX_PIN;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_Init(UART_TX_PORT, &GPIO_InitStructure);

    // ��ʼ�� PA3 (RX) Ϊ��������ģʽ
    GPIO_InitStructure.GPIO_Pin = UART_RX_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_Init(UART_RX_PORT, &GPIO_InitStructure);
		
	
	
    // ���� USART2 ����
    USART_InitStructure.USART_BaudRate = baudrate;                      // ������
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;         // 8λ����
    USART_InitStructure.USART_StopBits = USART_StopBits_1;              // 1��ֹͣλ
    USART_InitStructure.USART_Parity = USART_Parity_No;                 // ��У��λ
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None; // ��Ӳ��������
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;     // ���պͷ���ģʽ
    USART_Init(UART_NAME, &USART_InitStructure);                           // ��ʼ�� USART2

    // ���� USART2 �ж�
    NVIC_InitStructure.NVIC_IRQChannel = UART_IRQ;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;           // ��ռ���ȼ�
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;                  // �����ȼ�
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);

    // ʹ�� USART �����ж�
    USART_ITConfig(UART_NAME, USART_IT_RXNE, ENABLE);

    // ʹ�� USART2
    USART_Cmd(UART_NAME, ENABLE);
}
#endif

//ͨ��uartArray��ʼ���������
void UART_Init_Many(struct uartInfo* uartArray, uint8_t size) {
    // ��ʼ���ṹ��
    GPIO_InitTypeDef GPIO_InitStructure;
    USART_InitTypeDef USART_InitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;

		for(uint8_t i=0;i<size;i++){
			// ʹ�� GPIOA �� USART2 ʱ��
			RCC_APB2PeriphClockCmd(uartArray[i].rcc_gpio_port, ENABLE);  // ʹ�� GPIOA ʱ��
			RCC_APB1PeriphClockCmd(uartArray[i].rcc_uart, ENABLE); // ʹ�� USART2 ʱ��

			// ��ʼ�� PA2 (TX) Ϊ�����������ģʽ
			GPIO_InitStructure.GPIO_Pin = uartArray[i].uart_tx_pin;
			GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
			GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
			GPIO_Init(uartArray[i].uart_tx_port, &GPIO_InitStructure);

			// ��ʼ�� PA3 (RX) Ϊ��������ģʽ
			GPIO_InitStructure.GPIO_Pin = uartArray[i].uart_rx_pin;
			GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
			GPIO_Init(uartArray[i].uart_rx_port, &GPIO_InitStructure);

			// ���� USART2 ����
			USART_InitStructure.USART_BaudRate = uartArray[i].baudrate;                      // ������
			USART_InitStructure.USART_WordLength = USART_WordLength_8b;         // 8λ����
			USART_InitStructure.USART_StopBits = USART_StopBits_1;              // 1��ֹͣλ
			USART_InitStructure.USART_Parity = USART_Parity_No;                 // ��У��λ
			USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None; // ��Ӳ��������
			USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;     // ���պͷ���ģʽ
			USART_Init(uartArray[i].uart_name, &USART_InitStructure);                           // ��ʼ�� USART2

			// ���� USART2 �ж�
			NVIC_InitStructure.NVIC_IRQChannel = uartArray[i].uart_irq;
			NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;           // ��ռ���ȼ�
			NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;                  // �����ȼ�
			NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
			NVIC_Init(&NVIC_InitStructure);

			// ʹ�� USART �����ж�
			USART_ITConfig(uartArray[i].uart_name, USART_IT_RXNE, ENABLE);

			// ʹ�� USART2
			USART_Cmd(uartArray[i].uart_name, ENABLE);
	}
}


//ͨ���غ궨��Ĵ��ڷ���
// �򴮿ڷ���һ���ַ�
void UART_SendChar(char ch) {
    USART_SendData(UART_NAME, (uint16_t) ch);
    while (USART_GetFlagStatus(UART_NAME, USART_FLAG_TC) == RESET);
}
// �򴮿ڷ���һ���ַ���
void UART_SendString(const char *str) {
    while (*str) {
        UART_SendChar(*str++);
    }
}


//ͨ���ض����ڷ���
// �򴮿ڷ���һ���ַ�
void UART_SendChar_By(USART_TypeDef* uartName, char ch) {
		while (USART_GetFlagStatus(uartName, USART_FLAG_TXE) == RESET){};//�����ⲿ�ֿ���printf��һ���ַ���ʧ
    USART_SendData(uartName, (uint16_t) ch);
    while (USART_GetFlagStatus(uartName, USART_FLAG_TC) == RESET){};
}
// �򴮿ڷ���һ���ַ���
void UART_SendString_By(USART_TypeDef* uartName,const char *str) {
    while (*str) {
        UART_SendChar_By(uartName,*str++);
    }
}

#if 1
//��Ϊ����fputc������ԭ�ӵ�usart��ͻ��������ע��
int fputc(int ch, FILE *f) {
    if (ch == '\n') {
        uint8_t cr = '\r';
        UART_SendChar_By(UART_PRINT,cr);
    }
    UART_SendChar_By(UART_PRINT,ch);
    return ch;
}
#endif

// USART2 �жϷ�����
void USART2_IRQHandler(void) {
    if (USART_GetITStatus(UART_NAME, USART_IT_RXNE) != RESET) {
				if(RX_INDEX == 0){
					memset(rxBuf,0,RX_BUF_LEN);
				}
        // ��ȡ���յ�������
        char data = (char)USART_ReceiveData(UART_NAME);
				if(data == '\n' || RX_INDEX == (RX_BUF_LEN-2)){
					RX_INDEX = 0;
					printf("%s\n",rxBuf);
				}else{
					rxBuf[RX_INDEX++] = data;
				}
    }
		
}







