#include "timerInit.h"
#include "stdio.h"
/*
Ҫ�Լ�������Ӧ��ʱ����ʱ��;

timer					:��ʱ������     					TIM2
timerit				:���ö�ʱ���ж�ͨ��     	TIM2_IRQn
prescaler			:Ԥ��Ƶ									7199
countermode		:���ϼ����������¼���			TIM_CounterMode_Up
period				:�Զ�װ��								9999
clkdiv				:ʱ�ӷ�Ƶ								TIM_CKD_DIV1
*/
void timerInit(TIM_TypeDef *timer,IRQn_Type timerit, uint16_t prescaler,uint16_t countermode, uint16_t period, uint16_t clkdiv){
	//����ע��Ҫ�Լ�������Ӧ��ʱ����ʱ��
	//RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
	
	TIM_TimeBaseInitTypeDef timerInfo;
	timerInfo.TIM_Prescaler = prescaler;
	timerInfo.TIM_CounterMode = countermode;
	timerInfo.TIM_ClockDivision = clkdiv;
	
	//��ʼ����ʱ��
	TIM_TimeBaseInit(timer,&timerInfo);
	//���ö�ʱ���ж�
	TIM_ITConfig(timer,TIM_IT_Update,ENABLE);
	//���ö�ʱ��
	TIM_Cmd(timer,ENABLE);
	
	//����NVIC��Ӧ��ʱ���ж�
	NVIC_InitTypeDef nvicInfo;
	nvicInfo.NVIC_IRQChannel = timerit;
	nvicInfo.NVIC_IRQChannelPreemptionPriority = 0;
	nvicInfo.NVIC_IRQChannelSubPriority = 0;
	NVIC_Init(&nvicInfo);
	
}


/*��ʱ���жϺ���
void TIM2_IRQHandler(void)
{
    if (TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET)
    {
        // ����жϱ�־
        TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
        
        // ���������Ҫִ�е�����
				printf("��TIM2_IRQHandlern\n");
    }
}
*/

void stopTimer(TIM_TypeDef *timer, IRQn_Type timerit) {
    // 1. ֹͣ TIM2 ��ʱ��
    TIM_Cmd(timer, DISABLE);
    
    // 2. ���� TIM2 �ĸ����ж�
    TIM_ITConfig(timer, TIM_IT_Update, DISABLE);
    
    // 3. ����жϱ�־������б�Ҫ��
    TIM_ClearITPendingBit(timer, TIM_IT_Update);
    
    // 4. ���� NVIC ��Ӧ���ж�
    NVIC_DisableIRQ(timerit);
    
    // 5. ��ȫ��λ TIM2 ��ʱ��
    TIM_DeInit(timer);
}






