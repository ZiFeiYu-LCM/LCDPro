#include "timerInit.h"
#include "stdio.h"
/*
要自己启动对应定时器的时钟;

timer					:定时器名字     					TIM2
timerit				:设置定时器中断通道     	TIM2_IRQn
prescaler			:预分频									7199
countermode		:向上计数还是向下计数			TIM_CounterMode_Up
period				:自动装载								9999
clkdiv				:时钟分频								TIM_CKD_DIV1
*/
void timerInit(TIM_TypeDef *timer,IRQn_Type timerit, uint16_t prescaler,uint16_t countermode, uint16_t period, uint16_t clkdiv){
	//这里注意要自己启动对应定时器的时钟
	//RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
	
	TIM_TimeBaseInitTypeDef timerInfo;
	timerInfo.TIM_Prescaler = prescaler;
	timerInfo.TIM_CounterMode = countermode;
	timerInfo.TIM_ClockDivision = clkdiv;
	
	//初始化定时器
	TIM_TimeBaseInit(timer,&timerInfo);
	//启用定时器中断
	TIM_ITConfig(timer,TIM_IT_Update,ENABLE);
	//启用定时器
	TIM_Cmd(timer,ENABLE);
	
	//配置NVIC响应定时器中断
	NVIC_InitTypeDef nvicInfo;
	nvicInfo.NVIC_IRQChannel = timerit;
	nvicInfo.NVIC_IRQChannelPreemptionPriority = 0;
	nvicInfo.NVIC_IRQChannelSubPriority = 0;
	NVIC_Init(&nvicInfo);
	
}


/*定时器中断函数
void TIM2_IRQHandler(void)
{
    if (TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET)
    {
        // 清除中断标志
        TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
        
        // 在这里添加要执行的任务
				printf("启TIM2_IRQHandlern\n");
    }
}
*/

void stopTimer(TIM_TypeDef *timer, IRQn_Type timerit) {
    // 1. 停止 TIM2 定时器
    TIM_Cmd(timer, DISABLE);
    
    // 2. 禁用 TIM2 的更新中断
    TIM_ITConfig(timer, TIM_IT_Update, DISABLE);
    
    // 3. 清除中断标志（如果有必要）
    TIM_ClearITPendingBit(timer, TIM_IT_Update);
    
    // 4. 禁用 NVIC 对应的中断
    NVIC_DisableIRQ(timerit);
    
    // 5. 完全复位 TIM2 定时器
    TIM_DeInit(timer);
}






