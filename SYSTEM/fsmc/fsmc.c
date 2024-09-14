#include "./fsmc/fsmc.h"
#include "./delay/delay.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x.h"
#include "stm32f10x_fsmc.h"

void FSMC_Init(void){

	
	//��ʼ��GPIO
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD|RCC_APB2Periph_GPIOE|RCC_APB2Periph_GPIOG, ENABLE);
	GPIO_InitTypeDef GPIO_InitStructure;
	

	GPIO_InitStructure.GPIO_Pin = FSMC_D0|FSMC_D1|FSMC_D2|FSMC_D3|FSMC_D13|FSMC_D14|FSMC_D15;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(FSMC_D_GPIO,&GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = FSMC_D4|FSMC_D5|FSMC_D6|FSMC_D7|FSMC_D8|FSMC_D9|FSMC_D10|FSMC_D11|FSMC_D12;
	GPIO_Init(FSMC_E_GPIO,&GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = FSMC_NE4|FSMC_A10;
	GPIO_Init(FSMC_G_GPIO,&GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = FSMC_NWE|FSMC_NOE;
	GPIO_Init(FSMC_D_GPIO,&GPIO_InitStructure);	

	
	//Ĭ������ߵ�ƽ
	GPIO_SetBits(FSMC_G_GPIO, FSMC_NE4|FSMC_A10);
	GPIO_SetBits(FSMC_D_GPIO, FSMC_NWE|FSMC_NOE);
	
	
	
	//ʹ��ʱ��
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_FSMC,ENABLE);	
	//����FSMC
	FSMC_NORSRAMInitTypeDef 			FSMC_NORSRAMInitStructure;
	


/*
	FSMC_NORSRAMTimingInitTypeDef FSMC_NORSRAMTimingInitStructure;
	// ���� FSMC ʱ��
	FSMC_NORSRAMTimingInitStructure.FSMC_AddressSetupTime = 0;
  FSMC_NORSRAMTimingInitStructure.FSMC_AddressHoldTime = 0;
  FSMC_NORSRAMTimingInitStructure.FSMC_DataSetupTime = 15;
  FSMC_NORSRAMTimingInitStructure.FSMC_BusTurnAroundDuration = 0;
  FSMC_NORSRAMTimingInitStructure.FSMC_CLKDivision = 0;
  FSMC_NORSRAMTimingInitStructure.FSMC_DataLatency = 0;
  FSMC_NORSRAMTimingInitStructure.FSMC_AccessMode = FSMC_AccessMode_A;	
*/


	FSMC_NORSRAMTimingInitTypeDef TimingInitStruct;
	FSMC_NORSRAMTimingInitTypeDef ExtTimingInitStruct;
	
	TimingInitStruct.FSMC_AddressSetupTime = 15;
  TimingInitStruct.FSMC_AddressHoldTime = 15;
  TimingInitStruct.FSMC_DataSetupTime = 60;
  TimingInitStruct.FSMC_BusTurnAroundDuration = 0;
  TimingInitStruct.FSMC_CLKDivision = 16;
  TimingInitStruct.FSMC_DataLatency = 17;
  TimingInitStruct.FSMC_AccessMode = FSMC_AccessMode_A;

  ExtTimingInitStruct.FSMC_AddressSetupTime = 9;
  ExtTimingInitStruct.FSMC_AddressHoldTime = 15;
  ExtTimingInitStruct.FSMC_DataSetupTime = 8;
  ExtTimingInitStruct.FSMC_BusTurnAroundDuration = 0;
  ExtTimingInitStruct.FSMC_CLKDivision = 16;
  ExtTimingInitStruct.FSMC_DataLatency = 17;
  ExtTimingInitStruct.FSMC_AccessMode = FSMC_AccessMode_A;
	
	// ���� FSMC SRAM
	FSMC_NORSRAMInitStructure.FSMC_Bank = FSMC_Bank1_NORSRAM4;// ʹ�� NE4
	FSMC_NORSRAMInitStructure.FSMC_DataAddressMux = FSMC_DataAddressMux_Disable;// ��ַ/�����߲�����
	FSMC_NORSRAMInitStructure.FSMC_MemoryType = FSMC_MemoryType_SRAM;// SRAM �ʹ洢��-
	FSMC_NORSRAMInitStructure.FSMC_MemoryDataWidth = FSMC_MemoryDataWidth_16b;// 16 λ���ݿ��
	FSMC_NORSRAMInitStructure.FSMC_BurstAccessMode = FSMC_BurstAccessMode_Disable;// ����ͻ������ģʽ
	FSMC_NORSRAMInitStructure.FSMC_WaitSignalPolarity = FSMC_WaitSignalPolarity_Low;// �ȴ��źż��Ե�
	FSMC_NORSRAMInitStructure.FSMC_WrapMode = FSMC_WrapMode_Disable;// ���ð���ģʽ
	FSMC_NORSRAMInitStructure.FSMC_WaitSignalActive = FSMC_WaitSignalActive_BeforeWaitState;// �ȴ��ź��ڵȴ�����֮ǰ����-
	FSMC_NORSRAMInitStructure.FSMC_WriteOperation = FSMC_WriteOperation_Enable;// ����д����
	FSMC_NORSRAMInitStructure.FSMC_WaitSignal = FSMC_WaitSignal_Disable;// ���õȴ��ź�
	FSMC_NORSRAMInitStructure.FSMC_ExtendedMode = FSMC_ExtendedMode_Enable;// ������չģʽ
	FSMC_NORSRAMInitStructure.FSMC_AsynchronousWait = FSMC_AsynchronousWait_Disable;// �����첽�ȴ�
	FSMC_NORSRAMInitStructure.FSMC_WriteBurst = FSMC_WriteBurst_Disable;// ����дͻ��ģʽ
	FSMC_NORSRAMInitStructure.FSMC_ReadWriteTimingStruct = &ExtTimingInitStruct;
	FSMC_NORSRAMInitStructure.FSMC_WriteTimingStruct = &TimingInitStruct;
	
	
	FSMC_NORSRAMInit(&FSMC_NORSRAMInitStructure);
	// ʹ�� FSMC ����
	FSMC_NORSRAMCmd(FSMC_Bank1_NORSRAM4, ENABLE);	
	
	delay_ms(50);
}


