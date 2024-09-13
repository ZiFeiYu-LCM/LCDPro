#ifndef __TIMERINIT_H
#define __TIMERINIT_H
#include "stm32f10x.h"



/* main����ʾ��
RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
timerInit(TIM2, TIM2_IRQn, 7199, TIM_CounterMode_Up, 9999, TIM_CKD_DIV1);

delay_ms(5000);
stopTimer(TIM2, TIM2_IRQn);
*/


/*
Ҫ�Լ�������Ӧ��ʱ����ʱ��;

timer					:��ʱ������     					TIM2
timerit				:���ö�ʱ���ж�ͨ��     	TIM2_IRQn
prescaler			:Ԥ��Ƶ									7199
countermode		:���ϼ����������¼���			TIM_CounterMode_Up
period				:�Զ�װ��								9999
clkdiv				:ʱ�ӷ�Ƶ								TIM_CKD_DIV1
*/
void timerInit(TIM_TypeDef *timer,IRQn_Type timerit, uint16_t prescaler,uint16_t countermode, uint16_t period, uint16_t clkdiv);

//ֹͣ��ʱ��
void stopTimer(TIM_TypeDef *timer, IRQn_Type timerit);



#endif 



