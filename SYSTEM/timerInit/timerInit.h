#ifndef __TIMERINIT_H
#define __TIMERINIT_H
#include "stm32f10x.h"



/* main函数示例
RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
timerInit(TIM2, TIM2_IRQn, 7199, TIM_CounterMode_Up, 9999, TIM_CKD_DIV1);

delay_ms(5000);
stopTimer(TIM2, TIM2_IRQn);
*/


/*
要自己启动对应定时器的时钟;

timer					:定时器名字     					TIM2
timerit				:设置定时器中断通道     	TIM2_IRQn
prescaler			:预分频									7199
countermode		:向上计数还是向下计数			TIM_CounterMode_Up
period				:自动装载								9999
clkdiv				:时钟分频								TIM_CKD_DIV1
*/
void timerInit(TIM_TypeDef *timer,IRQn_Type timerit, uint16_t prescaler,uint16_t countermode, uint16_t period, uint16_t clkdiv);

//停止定时器
void stopTimer(TIM_TypeDef *timer, IRQn_Type timerit);



#endif 



