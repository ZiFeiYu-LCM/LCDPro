#include "./spi/spiTemplate.h"
#include "./delay/delay.h"



void spi_initGpio(struct spiStruct* spi){
	GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_APB2PeriphClockCmd(spi->rcc_cs_port,ENABLE);
	RCC_APB2PeriphClockCmd(spi->rcc_sck_port,ENABLE);
	RCC_APB2PeriphClockCmd(spi->rcc_mosi_port,ENABLE);
	RCC_APB2PeriphClockCmd(spi->rcc_miso_port,ENABLE);
	
	GPIO_InitStructure.GPIO_Pin = spi->cs;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	
	GPIO_Init(spi->cs_port, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = spi->sck;
	GPIO_Init(spi->sck_port, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = spi->mosi;
	GPIO_Init(spi->mosi_port, &GPIO_InitStructure);
	
	
	GPIO_InitStructure.GPIO_Pin = spi->miso;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(spi->miso_port, &GPIO_InitStructure);
	
	GPIO_SetBits(spi->cs_port,spi->cs);
}


void sendByte(struct spiStruct* spi, uint8_t data){
	if(spi->mode){
		//高位发送
		for(uint8_t i=0;i<8;i++){
			
			if(data & 0x80){
				GPIO_SetBits(spi->mosi_port,spi->mosi);
			}else{
				GPIO_ResetBits(spi->mosi_port,spi->mosi);
			}
			data<<=1;
			
			GPIO_SetBits(spi->sck_port,spi->sck);
			delay_us(1);
			GPIO_ResetBits(spi->sck_port,spi->sck);
			delay_us(1);
		}
	}else{
		//低位发送
		for(int i=0;i<8;i++){
			
			if(data & 0x01){
				GPIO_SetBits(spi->mosi_port,spi->mosi);
			}else{
				GPIO_ResetBits(spi->mosi_port,spi->mosi);
			}
			data>>=1;
			
			GPIO_SetBits(spi->sck_port,spi->sck);
			delay_us(1);
			GPIO_ResetBits(spi->sck_port,spi->sck);
			delay_us(1);
		}	
	}
}

uint8_t readByte(struct spiStruct* spi){
	uint8_t data = 0;
	if(spi->mode){
		//高位先接收
		for(uint8_t i=0;i<8;i++){
				GPIO_SetBits(spi->sck_port,spi->sck);
				delay_us(1);
		
				if(GPIO_ReadInputDataBit(spi->miso_port,spi->miso)){
					data |= 1;
				}else{
					data |= 0;
				}
				data<<=1;
				
				GPIO_ResetBits(spi->sck_port,spi->sck);
				delay_us(1);			
		}
	}else{
		//低位先接收
		for(uint8_t i=0;i<8;i++){
				GPIO_SetBits(spi->sck_port,spi->sck);
				delay_us(1);
		
				if(GPIO_ReadInputDataBit(spi->miso_port,spi->miso)){
					data |= 0x80;
				}else{
					data &= 0x7F;
				}
				data>>=1;
				
				GPIO_ResetBits(spi->sck_port,spi->sck);
				delay_us(1);	
		}
	}
	return data;
}


void spi_sendData(struct spiStruct* spi, uint8_t *data, uint8_t dataSize){
	GPIO_ResetBits(spi->cs_port,spi->cs);
	for(uint8_t i=0;i<dataSize;i++){
		sendByte(spi, data[i]);
	}
	GPIO_SetBits(spi->cs_port,spi->cs);
}

void spi_readData(struct spiStruct* spi, uint8_t *data, uint8_t dataSize){
	GPIO_ResetBits(spi->cs_port,spi->cs);
	for(uint8_t i=0;i<dataSize;i++){
		data[i] = readByte(spi);
	}
	GPIO_SetBits(spi->cs_port,spi->cs);
}

void sendANDread(struct spiStruct* spi, uint8_t *rxData,uint8_t *txData, uint8_t dataSize){
	GPIO_ResetBits(spi->cs_port,spi->cs);
	delay_us(1);
	for(uint8_t i=0;i<dataSize;i++){
		sendByte(spi,txData[i]);
		rxData[i] = readByte(spi);
	}
	GPIO_SetBits(spi->cs_port,spi->cs);
	delay_us(1);
}



void spiInit(struct spiStruct* spi, uint16_t cs, uint16_t sck, uint16_t miso, uint16_t mosi,
							GPIO_TypeDef *cs_port, GPIO_TypeDef *sck_port, GPIO_TypeDef *miso_port, GPIO_TypeDef *mosi_port,
							uint32_t rcc_cs_port, uint32_t rcc_sck_port, uint32_t rcc_miso_port, uint32_t rcc_mosi_port,
						uint8_t mode)
{
	spi->cs = cs;
	spi->sck = sck;
	spi->miso = miso;
	spi->mosi = mosi;
	
	spi->cs_port = cs_port;
	spi->sck_port = sck_port;	
	spi->miso_port = miso_port;
	spi->mosi_port = mosi_port;
	
	spi->rcc_cs_port = rcc_cs_port;
	spi->rcc_sck_port = rcc_sck_port;
	spi->rcc_miso_port = rcc_miso_port;
	spi->rcc_mosi_port = rcc_mosi_port;
	
	spi->mode = mode;
	
	spi->initGpio = spi_initGpio;
	spi->sendData = spi_sendData;
	spi->readData = spi_readData;
	spi->sendANDread = sendANDread;

}




