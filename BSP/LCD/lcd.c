#include "./LCD/lcd.h"
#include "stdio.h"
#include "./delay/delay.h"
struct lcdStruct lcdDev;



//设置寄存器
void LCD_Set_Reg(volatile uint16_t reg){
	reg = reg;
	LCD->LCD_REG = reg;
}
//设置寄存器为GRAM
void LCD_Set_RegGram(void){
	LCD->LCD_REG = lcdDev.wramcmd;
}
//写数据
void LCD_Write_Data(volatile uint16_t data){
	data = data;
	LCD->LCD_RAM = data;
}
//向指定寄存器 写入 数据
void LCD_Write_RegData(uint16_t reg, uint16_t data){
	LCD->LCD_REG = reg;
	LCD->LCD_RAM = data;
}
//读数据
static uint16_t LCD_Read_Data(void){
	volatile uint16_t data;
	data = LCD->LCD_RAM;
	return data;
}


//获取id
uint16_t LCD_Read_Id(){
	uint16_t id_heigh,id_low;
	
	LCD_Set_Reg(ID_REG);
	LCD_Read_Data();//假读
	LCD_Read_Data();//假读
	
	delay_us(10);
	
	id_heigh = LCD_Read_Data();//0x53
	id_low = LCD_Read_Data();//0x10
	return ((id_heigh<<8)|id_low);
}


//初始化
void LCD_Init(){
	FSMC_Init();
	initGpio();
	LCD_InitReg_5310();
	
	lcdDev.id = LCD_Read_Id();
	lcdDev.wramcmd = GRAM_REG;
	lcdDev.setxcmd = X_REG;
	lcdDev.setycmd = Y_REG;
	
	lcdDev.width = 320;
	lcdDev.height= 480;
	lcdDev.displaydir = 0;
	
	
	LCD_BL_Write(1);
	LCD_Display_Dir(0);
	
}



void LCD_Scan_Dir(uint8_t dir){
	  uint16_t regval = 0;
    uint16_t dirreg = DIR_REG;
    uint16_t temp;

    switch (dir)
    {
        case L2R_U2D:/* 从左到右,从上到下 */
            regval |= (0 << 7) | (0 << 6) | (0 << 5);
            break;

        case L2R_D2U:/* 从左到右,从下到上 */
            regval |= (1 << 7) | (0 << 6) | (0 << 5);
            break;

        case R2L_U2D:/* 从右到左,从上到下 */
            regval |= (0 << 7) | (1 << 6) | (0 << 5);
            break;

        case R2L_D2U:/* 从右到左,从下到上 */
            regval |= (1 << 7) | (1 << 6) | (0 << 5);
            break;

        case U2D_L2R:/* 从上到下,从左到右 */
            regval |= (0 << 7) | (0 << 6) | (1 << 5);
            break;

        case U2D_R2L:/* 从上到下,从右到左 */
            regval |= (0 << 7) | (1 << 6) | (1 << 5);
            break;

        case D2U_L2R:/* 从下到上,从左到右 */
            regval |= (1 << 7) | (0 << 6) | (1 << 5);
            break;

        case D2U_R2L:/* 从下到上,从右到左 */
            regval |= (1 << 7) | (1 << 6) | (1 << 5);
            break;
    }	
	
		//设置lcd扫描方向
		LCD_Write_RegData(dirreg, regval);
		
		if (regval & 0X20)
		{
				if (lcdDev.width < lcdDev.height)   /* 交换X,Y */
				{
						temp = lcdDev.width;
						lcdDev.width = lcdDev.height;
						lcdDev.height = temp;
				}
		}
		else
		{
				if (lcdDev.width > lcdDev.height)   /* 交换X,Y */
				{
						temp = lcdDev.width;
						lcdDev.width = lcdDev.height;
						lcdDev.height = temp;
				}
		}
		
		//设置活动窗口
		LCD_Set_Reg(lcdDev.setxcmd);
		LCD_Write_Data(0);
		LCD_Write_Data(0);
		LCD_Write_Data((lcdDev.width - 1) >> 8);
		LCD_Write_Data((lcdDev.width - 1) & 0XFF);
		LCD_Set_Reg(lcdDev.setycmd);
		LCD_Write_Data(0);
		LCD_Write_Data(0);
		LCD_Write_Data((lcdDev.height - 1) >> 8);
		LCD_Write_Data((lcdDev.height - 1) & 0XFF);
}





//初始化引脚
void initGpio(){
	RCC_APB2PeriphClockCmd(LCD_RCC_BL, ENABLE);
	
	GPIO_InitTypeDef GPIO_InitStructure;

	GPIO_InitStructure.GPIO_Pin = LCD_BL_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(LCD_BL_PORT,&GPIO_InitStructure);
}




//设置lcd的坐标
void LCD_Set_Cursor(uint16_t x, uint16_t y){
	LCD_Set_Reg(lcdDev.setxcmd);
	LCD_Write_Data(x>>8);
	LCD_Write_Data(x&0xFF);
	
	LCD_Set_Reg(lcdDev.setycmd);
	LCD_Write_Data(y>>8);
	LCD_Write_Data(y&0xFF);
}

//描绘一个像素点
void LCD_Draw_Point(uint16_t x, uint16_t y, uint32_t color){
	LCD_Set_Cursor(x,y);
	
	LCD_Set_RegGram();
	LCD->LCD_RAM = color;
}

//读取某个像素点的颜色
uint32_t LCD_Read_Point(uint16_t x, uint16_t y){
	if(x >= lcdDev.width || y >= lcdDev.height){
		return 0;
	}
	
	uint16_t r,g,b;
	LCD_Set_Cursor(x, y);
	LCD_Set_Reg(READ_COLOR_REG);
	
	r = LCD_Read_Data();//假读，有些型号的lcd不用假读，5310需要
	r = LCD_Read_Data();//实际的坐标颜色
	
	b = LCD_Read_Data();
 
	g = r & 0XFF;  
	g <<= 8; 
	
	return (((r >> 11) << 11) | ((g >> 10) << 5) | (b >> 11));
}

//lcd清屏为特定颜色
void LCD_Clear(uint16_t color){
	uint32_t totalPoint = lcdDev.width*lcdDev.height;
	LCD_Set_Cursor(0,0);
	LCD_Set_RegGram();
	for(uint32_t i=0;i<totalPoint;i++){
		LCD_Write_Data(color);
		//LCD->LCD_RAM = color;
	}
	
	
//	LCD_Set_Reg(lcdDev.setxcmd);
//	LCD_Write_Data(0>>8);
//	LCD_Write_Data(0&0XFF);
//	LCD_Write_Data((lcdDev.width-1)>>8);
//	LCD_Write_Data((lcdDev.width-1)&0XFF);
//	LCD_Set_Reg(lcdDev.setycmd);
//	LCD_Write_Data(0>>8);
//	LCD_Write_Data(0&0XFF);
//	LCD_Write_Data((lcdDev.height-1)>> 8);
//	LCD_Write_Data((lcdDev.height-1)&0XFF);
//	
//	LCD_Set_Reg(lcdDev.wramcmd);	
//	uint32_t totalpoint = lcdDev.width*lcdDev.height;
//	for (uint32_t index = 0; index < totalpoint; index++)
//	{
//			LCD_Write_Data(WHITE);
//	}	
	
}

//LCD关闭开启
void LCD_Display_On(){
	LCD_Set_Reg(DISPLAY_ON_REG);
}
//LCD关闭显示
void LCD_Display_Off(){
	LCD_Set_Reg(DISPLAY_OFF_REG);
}

//LCD屏幕方向
void LCD_Display_Dir(uint8_t dir){
	lcdDev.displaydir = dir;
	
	if(dir){
		lcdDev.width 	= LCD_HEIGHT;
		lcdDev.height = LCD_WIDTH;
	}else{
		lcdDev.width 	= LCD_WIDTH;
		lcdDev.height =	LCD_HEIGHT;
	}


	
	//设置默认扫描方向
	LCD_Scan_Dir(DFT_SCAN_DIR);
}



/*
为LVGL提供的函数
指定区域内填充颜色
*/
void LCD_Color_Fill(uint16_t sx, uint16_t sy, uint16_t ex, uint16_t ey, uint16_t *color){
	uint16_t width, height;
	uint16_t i,j;
	
	width = ex - sx + 1;
	height = ey - sy + 1;
	
	for(i=0;i<height;i++){
		LCD_Set_Cursor(sx, i+1);
		LCD_Set_RegGram();
		for(j=0;j<width;j++){
			LCD->LCD_RAM = color[i*width + j];
		}
	}

}



void LCD_InitReg_5310(void){
        LCD_Set_Reg(0xED);
        LCD_Write_Data(0x01);
        LCD_Write_Data(0xFE);

        LCD_Set_Reg(0xEE);
        LCD_Write_Data(0xDE);
        LCD_Write_Data(0x21);

        LCD_Set_Reg(0xF1);
        LCD_Write_Data(0x01);
        LCD_Set_Reg(0xDF);
        LCD_Write_Data(0x10);

        //VCOMvoltage//
        LCD_Set_Reg(0xC4);
        LCD_Write_Data(0x8F);      //5f

        LCD_Set_Reg(0xC6);
        LCD_Write_Data(0x00);
        LCD_Write_Data(0xE2);
        LCD_Write_Data(0xE2);
        LCD_Write_Data(0xE2);
        LCD_Set_Reg(0xBF);
        LCD_Write_Data(0xAA);

        LCD_Set_Reg(0xB0);
        LCD_Write_Data(0x0D);
        LCD_Write_Data(0x00);
        LCD_Write_Data(0x0D);
        LCD_Write_Data(0x00);
        LCD_Write_Data(0x11);
        LCD_Write_Data(0x00);
        LCD_Write_Data(0x19);
        LCD_Write_Data(0x00);
        LCD_Write_Data(0x21);
        LCD_Write_Data(0x00);
        LCD_Write_Data(0x2D);
        LCD_Write_Data(0x00);
        LCD_Write_Data(0x3D);
        LCD_Write_Data(0x00);
        LCD_Write_Data(0x5D);
        LCD_Write_Data(0x00);
        LCD_Write_Data(0x5D);
        LCD_Write_Data(0x00);

        LCD_Set_Reg(0xB1);
        LCD_Write_Data(0x80);
        LCD_Write_Data(0x00);
        LCD_Write_Data(0x8B);
        LCD_Write_Data(0x00);
        LCD_Write_Data(0x96);
        LCD_Write_Data(0x00);

        LCD_Set_Reg(0xB2);
        LCD_Write_Data(0x00);
        LCD_Write_Data(0x00);
        LCD_Write_Data(0x02);
        LCD_Write_Data(0x00);
        LCD_Write_Data(0x03);
        LCD_Write_Data(0x00);

        LCD_Set_Reg(0xB3);
        LCD_Write_Data(0x00);
        LCD_Write_Data(0x00);
        LCD_Write_Data(0x00);
        LCD_Write_Data(0x00);
        LCD_Write_Data(0x00);
        LCD_Write_Data(0x00);
        LCD_Write_Data(0x00);
        LCD_Write_Data(0x00);
        LCD_Write_Data(0x00);
        LCD_Write_Data(0x00);
        LCD_Write_Data(0x00);
        LCD_Write_Data(0x00);
        LCD_Write_Data(0x00);
        LCD_Write_Data(0x00);
        LCD_Write_Data(0x00);
        LCD_Write_Data(0x00);
        LCD_Write_Data(0x00);
        LCD_Write_Data(0x00);
        LCD_Write_Data(0x00);
        LCD_Write_Data(0x00);
        LCD_Write_Data(0x00);
        LCD_Write_Data(0x00);
        LCD_Write_Data(0x00);
        LCD_Write_Data(0x00);

        LCD_Set_Reg(0xB4);
        LCD_Write_Data(0x8B);
        LCD_Write_Data(0x00);
        LCD_Write_Data(0x96);
        LCD_Write_Data(0x00);
        LCD_Write_Data(0xA1);
        LCD_Write_Data(0x00);

        LCD_Set_Reg(0xB5);
        LCD_Write_Data(0x02);
        LCD_Write_Data(0x00);
        LCD_Write_Data(0x03);
        LCD_Write_Data(0x00);
        LCD_Write_Data(0x04);
        LCD_Write_Data(0x00);

        LCD_Set_Reg(0xB6);
        LCD_Write_Data(0x00);
        LCD_Write_Data(0x00);

        LCD_Set_Reg(0xB7);
        LCD_Write_Data(0x00);
        LCD_Write_Data(0x00);
        LCD_Write_Data(0x3F);
        LCD_Write_Data(0x00);
        LCD_Write_Data(0x5E);
        LCD_Write_Data(0x00);
        LCD_Write_Data(0x64);
        LCD_Write_Data(0x00);
        LCD_Write_Data(0x8C);
        LCD_Write_Data(0x00);
        LCD_Write_Data(0xAC);
        LCD_Write_Data(0x00);
        LCD_Write_Data(0xDC);
        LCD_Write_Data(0x00);
        LCD_Write_Data(0x70);
        LCD_Write_Data(0x00);
        LCD_Write_Data(0x90);
        LCD_Write_Data(0x00);
        LCD_Write_Data(0xEB);
        LCD_Write_Data(0x00);
        LCD_Write_Data(0xDC);
        LCD_Write_Data(0x00);

        LCD_Set_Reg(0xB8);
        LCD_Write_Data(0x00);
        LCD_Write_Data(0x00);
        LCD_Write_Data(0x00);
        LCD_Write_Data(0x00);
        LCD_Write_Data(0x00);
        LCD_Write_Data(0x00);
        LCD_Write_Data(0x00);
        LCD_Write_Data(0x00);

        LCD_Set_Reg(0xBA);
        LCD_Write_Data(0x24);
        LCD_Write_Data(0x00);
        LCD_Write_Data(0x00);
        LCD_Write_Data(0x00);

        LCD_Set_Reg(0xC1);
        LCD_Write_Data(0x20);
        LCD_Write_Data(0x00);
        LCD_Write_Data(0x54);
        LCD_Write_Data(0x00);
        LCD_Write_Data(0xFF);
        LCD_Write_Data(0x00);

        LCD_Set_Reg(0xC2);
        LCD_Write_Data(0x0A);
        LCD_Write_Data(0x00);
        LCD_Write_Data(0x04);
        LCD_Write_Data(0x00);

        LCD_Set_Reg(0xC3);
        LCD_Write_Data(0x3C);
        LCD_Write_Data(0x00);
        LCD_Write_Data(0x3A);
        LCD_Write_Data(0x00);
        LCD_Write_Data(0x39);
        LCD_Write_Data(0x00);
        LCD_Write_Data(0x37);
        LCD_Write_Data(0x00);
        LCD_Write_Data(0x3C);
        LCD_Write_Data(0x00);
        LCD_Write_Data(0x36);
        LCD_Write_Data(0x00);
        LCD_Write_Data(0x32);
        LCD_Write_Data(0x00);
        LCD_Write_Data(0x2F);
        LCD_Write_Data(0x00);
        LCD_Write_Data(0x2C);
        LCD_Write_Data(0x00);
        LCD_Write_Data(0x29);
        LCD_Write_Data(0x00);
        LCD_Write_Data(0x26);
        LCD_Write_Data(0x00);
        LCD_Write_Data(0x24);
        LCD_Write_Data(0x00);
        LCD_Write_Data(0x24);
        LCD_Write_Data(0x00);
        LCD_Write_Data(0x23);
        LCD_Write_Data(0x00);
        LCD_Write_Data(0x3C);
        LCD_Write_Data(0x00);
        LCD_Write_Data(0x36);
        LCD_Write_Data(0x00);
        LCD_Write_Data(0x32);
        LCD_Write_Data(0x00);
        LCD_Write_Data(0x2F);
        LCD_Write_Data(0x00);
        LCD_Write_Data(0x2C);
        LCD_Write_Data(0x00);
        LCD_Write_Data(0x29);
        LCD_Write_Data(0x00);
        LCD_Write_Data(0x26);
        LCD_Write_Data(0x00);
        LCD_Write_Data(0x24);
        LCD_Write_Data(0x00);
        LCD_Write_Data(0x24);
        LCD_Write_Data(0x00);
        LCD_Write_Data(0x23);
        LCD_Write_Data(0x00);

        LCD_Set_Reg(0xC4);
        LCD_Write_Data(0x62);
        LCD_Write_Data(0x00);
        LCD_Write_Data(0x05);
        LCD_Write_Data(0x00);
        LCD_Write_Data(0x84);
        LCD_Write_Data(0x00);
        LCD_Write_Data(0xF0);
        LCD_Write_Data(0x00);
        LCD_Write_Data(0x18);
        LCD_Write_Data(0x00);
        LCD_Write_Data(0xA4);
        LCD_Write_Data(0x00);
        LCD_Write_Data(0x18);
        LCD_Write_Data(0x00);
        LCD_Write_Data(0x50);
        LCD_Write_Data(0x00);
        LCD_Write_Data(0x0C);
        LCD_Write_Data(0x00);
        LCD_Write_Data(0x17);
        LCD_Write_Data(0x00);
        LCD_Write_Data(0x95);
        LCD_Write_Data(0x00);
        LCD_Write_Data(0xF3);
        LCD_Write_Data(0x00);
        LCD_Write_Data(0xE6);
        LCD_Write_Data(0x00);

        LCD_Set_Reg(0xC5);
        LCD_Write_Data(0x32);
        LCD_Write_Data(0x00);
        LCD_Write_Data(0x44);
        LCD_Write_Data(0x00);
        LCD_Write_Data(0x65);
        LCD_Write_Data(0x00);
        LCD_Write_Data(0x76);
        LCD_Write_Data(0x00);
        LCD_Write_Data(0x88);
        LCD_Write_Data(0x00);

        LCD_Set_Reg(0xC6);
        LCD_Write_Data(0x20);
        LCD_Write_Data(0x00);
        LCD_Write_Data(0x17);
        LCD_Write_Data(0x00);
        LCD_Write_Data(0x01);
        LCD_Write_Data(0x00);

        LCD_Set_Reg(0xC7);
        LCD_Write_Data(0x00);
        LCD_Write_Data(0x00);
        LCD_Write_Data(0x00);
        LCD_Write_Data(0x00);

        LCD_Set_Reg(0xC8);
        LCD_Write_Data(0x00);
        LCD_Write_Data(0x00);
        LCD_Write_Data(0x00);
        LCD_Write_Data(0x00);

        LCD_Set_Reg(0xC9);
        LCD_Write_Data(0x00);
        LCD_Write_Data(0x00);
        LCD_Write_Data(0x00);
        LCD_Write_Data(0x00);
        LCD_Write_Data(0x00);
        LCD_Write_Data(0x00);
        LCD_Write_Data(0x00);
        LCD_Write_Data(0x00);
        LCD_Write_Data(0x00);
        LCD_Write_Data(0x00);
        LCD_Write_Data(0x00);
        LCD_Write_Data(0x00);
        LCD_Write_Data(0x00);
        LCD_Write_Data(0x00);
        LCD_Write_Data(0x00);
        LCD_Write_Data(0x00);

        LCD_Set_Reg(0xE0);
        LCD_Write_Data(0x16);
        LCD_Write_Data(0x00);
        LCD_Write_Data(0x1C);
        LCD_Write_Data(0x00);
        LCD_Write_Data(0x21);
        LCD_Write_Data(0x00);
        LCD_Write_Data(0x36);
        LCD_Write_Data(0x00);
        LCD_Write_Data(0x46);
        LCD_Write_Data(0x00);
        LCD_Write_Data(0x52);
        LCD_Write_Data(0x00);
        LCD_Write_Data(0x64);
        LCD_Write_Data(0x00);
        LCD_Write_Data(0x7A);
        LCD_Write_Data(0x00);
        LCD_Write_Data(0x8B);
        LCD_Write_Data(0x00);
        LCD_Write_Data(0x99);
        LCD_Write_Data(0x00);
        LCD_Write_Data(0xA8);
        LCD_Write_Data(0x00);
        LCD_Write_Data(0xB9);
        LCD_Write_Data(0x00);
        LCD_Write_Data(0xC4);
        LCD_Write_Data(0x00);
        LCD_Write_Data(0xCA);
        LCD_Write_Data(0x00);
        LCD_Write_Data(0xD2);
        LCD_Write_Data(0x00);
        LCD_Write_Data(0xD9);
        LCD_Write_Data(0x00);
        LCD_Write_Data(0xE0);
        LCD_Write_Data(0x00);
        LCD_Write_Data(0xF3);
        LCD_Write_Data(0x00);

        LCD_Set_Reg(0xE1);
        LCD_Write_Data(0x16);
        LCD_Write_Data(0x00);
        LCD_Write_Data(0x1C);
        LCD_Write_Data(0x00);
        LCD_Write_Data(0x22);
        LCD_Write_Data(0x00);
        LCD_Write_Data(0x36);
        LCD_Write_Data(0x00);
        LCD_Write_Data(0x45);
        LCD_Write_Data(0x00);
        LCD_Write_Data(0x52);
        LCD_Write_Data(0x00);
        LCD_Write_Data(0x64);
        LCD_Write_Data(0x00);
        LCD_Write_Data(0x7A);
        LCD_Write_Data(0x00);
        LCD_Write_Data(0x8B);
        LCD_Write_Data(0x00);
        LCD_Write_Data(0x99);
        LCD_Write_Data(0x00);
        LCD_Write_Data(0xA8);
        LCD_Write_Data(0x00);
        LCD_Write_Data(0xB9);
        LCD_Write_Data(0x00);
        LCD_Write_Data(0xC4);
        LCD_Write_Data(0x00);
        LCD_Write_Data(0xCA);
        LCD_Write_Data(0x00);
        LCD_Write_Data(0xD2);
        LCD_Write_Data(0x00);
        LCD_Write_Data(0xD8);
        LCD_Write_Data(0x00);
        LCD_Write_Data(0xE0);
        LCD_Write_Data(0x00);
        LCD_Write_Data(0xF3);
        LCD_Write_Data(0x00);

        LCD_Set_Reg(0xE2);
        LCD_Write_Data(0x05);
        LCD_Write_Data(0x00);
        LCD_Write_Data(0x0B);
        LCD_Write_Data(0x00);
        LCD_Write_Data(0x1B);
        LCD_Write_Data(0x00);
        LCD_Write_Data(0x34);
        LCD_Write_Data(0x00);
        LCD_Write_Data(0x44);
        LCD_Write_Data(0x00);
        LCD_Write_Data(0x4F);
        LCD_Write_Data(0x00);
        LCD_Write_Data(0x61);
        LCD_Write_Data(0x00);
        LCD_Write_Data(0x79);
        LCD_Write_Data(0x00);
        LCD_Write_Data(0x88);
        LCD_Write_Data(0x00);
        LCD_Write_Data(0x97);
        LCD_Write_Data(0x00);
        LCD_Write_Data(0xA6);
        LCD_Write_Data(0x00);
        LCD_Write_Data(0xB7);
        LCD_Write_Data(0x00);
        LCD_Write_Data(0xC2);
        LCD_Write_Data(0x00);
        LCD_Write_Data(0xC7);
        LCD_Write_Data(0x00);
        LCD_Write_Data(0xD1);
        LCD_Write_Data(0x00);
        LCD_Write_Data(0xD6);
        LCD_Write_Data(0x00);
        LCD_Write_Data(0xDD);
        LCD_Write_Data(0x00);
        LCD_Write_Data(0xF3);
        LCD_Write_Data(0x00);
        LCD_Set_Reg(0xE3);
        LCD_Write_Data(0x05);
        LCD_Write_Data(0x00);
        LCD_Write_Data(0xA);
        LCD_Write_Data(0x00);
        LCD_Write_Data(0x1C);
        LCD_Write_Data(0x00);
        LCD_Write_Data(0x33);
        LCD_Write_Data(0x00);
        LCD_Write_Data(0x44);
        LCD_Write_Data(0x00);
        LCD_Write_Data(0x50);
        LCD_Write_Data(0x00);
        LCD_Write_Data(0x62);
        LCD_Write_Data(0x00);
        LCD_Write_Data(0x78);
        LCD_Write_Data(0x00);
        LCD_Write_Data(0x88);
        LCD_Write_Data(0x00);
        LCD_Write_Data(0x97);
        LCD_Write_Data(0x00);
        LCD_Write_Data(0xA6);
        LCD_Write_Data(0x00);
        LCD_Write_Data(0xB7);
        LCD_Write_Data(0x00);
        LCD_Write_Data(0xC2);
        LCD_Write_Data(0x00);
        LCD_Write_Data(0xC7);
        LCD_Write_Data(0x00);
        LCD_Write_Data(0xD1);
        LCD_Write_Data(0x00);
        LCD_Write_Data(0xD5);
        LCD_Write_Data(0x00);
        LCD_Write_Data(0xDD);
        LCD_Write_Data(0x00);
        LCD_Write_Data(0xF3);
        LCD_Write_Data(0x00);

        LCD_Set_Reg(0xE4);
        LCD_Write_Data(0x01);
        LCD_Write_Data(0x00);
        LCD_Write_Data(0x01);
        LCD_Write_Data(0x00);
        LCD_Write_Data(0x02);
        LCD_Write_Data(0x00);
        LCD_Write_Data(0x2A);
        LCD_Write_Data(0x00);
        LCD_Write_Data(0x3C);
        LCD_Write_Data(0x00);
        LCD_Write_Data(0x4B);
        LCD_Write_Data(0x00);
        LCD_Write_Data(0x5D);
        LCD_Write_Data(0x00);
        LCD_Write_Data(0x74);
        LCD_Write_Data(0x00);
        LCD_Write_Data(0x84);
        LCD_Write_Data(0x00);
        LCD_Write_Data(0x93);
        LCD_Write_Data(0x00);
        LCD_Write_Data(0xA2);
        LCD_Write_Data(0x00);
        LCD_Write_Data(0xB3);
        LCD_Write_Data(0x00);
        LCD_Write_Data(0xBE);
        LCD_Write_Data(0x00);
        LCD_Write_Data(0xC4);
        LCD_Write_Data(0x00);
        LCD_Write_Data(0xCD);
        LCD_Write_Data(0x00);
        LCD_Write_Data(0xD3);
        LCD_Write_Data(0x00);
        LCD_Write_Data(0xDD);
        LCD_Write_Data(0x00);
        LCD_Write_Data(0xF3);
        LCD_Write_Data(0x00);
        LCD_Set_Reg(0xE5);
        LCD_Write_Data(0x00);
        LCD_Write_Data(0x00);
        LCD_Write_Data(0x00);
        LCD_Write_Data(0x00);
        LCD_Write_Data(0x02);
        LCD_Write_Data(0x00);
        LCD_Write_Data(0x29);
        LCD_Write_Data(0x00);
        LCD_Write_Data(0x3C);
        LCD_Write_Data(0x00);
        LCD_Write_Data(0x4B);
        LCD_Write_Data(0x00);
        LCD_Write_Data(0x5D);
        LCD_Write_Data(0x00);
        LCD_Write_Data(0x74);
        LCD_Write_Data(0x00);
        LCD_Write_Data(0x84);
        LCD_Write_Data(0x00);
        LCD_Write_Data(0x93);
        LCD_Write_Data(0x00);
        LCD_Write_Data(0xA2);
        LCD_Write_Data(0x00);
        LCD_Write_Data(0xB3);
        LCD_Write_Data(0x00);
        LCD_Write_Data(0xBE);
        LCD_Write_Data(0x00);
        LCD_Write_Data(0xC4);
        LCD_Write_Data(0x00);
        LCD_Write_Data(0xCD);
        LCD_Write_Data(0x00);
        LCD_Write_Data(0xD3);
        LCD_Write_Data(0x00);
        LCD_Write_Data(0xDC);
        LCD_Write_Data(0x00);
        LCD_Write_Data(0xF3);
        LCD_Write_Data(0x00);

        LCD_Set_Reg(0xE6);
        LCD_Write_Data(0x11);
        LCD_Write_Data(0x00);
        LCD_Write_Data(0x34);
        LCD_Write_Data(0x00);
        LCD_Write_Data(0x56);
        LCD_Write_Data(0x00);
        LCD_Write_Data(0x76);
        LCD_Write_Data(0x00);
        LCD_Write_Data(0x77);
        LCD_Write_Data(0x00);
        LCD_Write_Data(0x66);
        LCD_Write_Data(0x00);
        LCD_Write_Data(0x88);
        LCD_Write_Data(0x00);
        LCD_Write_Data(0x99);
        LCD_Write_Data(0x00);
        LCD_Write_Data(0xBB);
        LCD_Write_Data(0x00);
        LCD_Write_Data(0x99);
        LCD_Write_Data(0x00);
        LCD_Write_Data(0x66);
        LCD_Write_Data(0x00);
        LCD_Write_Data(0x55);
        LCD_Write_Data(0x00);
        LCD_Write_Data(0x55);
        LCD_Write_Data(0x00);
        LCD_Write_Data(0x45);
        LCD_Write_Data(0x00);
        LCD_Write_Data(0x43);
        LCD_Write_Data(0x00);
        LCD_Write_Data(0x44);
        LCD_Write_Data(0x00);

        LCD_Set_Reg(0xE7);
        LCD_Write_Data(0x32);
        LCD_Write_Data(0x00);
        LCD_Write_Data(0x55);
        LCD_Write_Data(0x00);
        LCD_Write_Data(0x76);
        LCD_Write_Data(0x00);
        LCD_Write_Data(0x66);
        LCD_Write_Data(0x00);
        LCD_Write_Data(0x67);
        LCD_Write_Data(0x00);
        LCD_Write_Data(0x67);
        LCD_Write_Data(0x00);
        LCD_Write_Data(0x87);
        LCD_Write_Data(0x00);
        LCD_Write_Data(0x99);
        LCD_Write_Data(0x00);
        LCD_Write_Data(0xBB);
        LCD_Write_Data(0x00);
        LCD_Write_Data(0x99);
        LCD_Write_Data(0x00);
        LCD_Write_Data(0x77);
        LCD_Write_Data(0x00);
        LCD_Write_Data(0x44);
        LCD_Write_Data(0x00);
        LCD_Write_Data(0x56);
        LCD_Write_Data(0x00);
        LCD_Write_Data(0x23);
        LCD_Write_Data(0x00);
        LCD_Write_Data(0x33);
        LCD_Write_Data(0x00);
        LCD_Write_Data(0x45);
        LCD_Write_Data(0x00);

        LCD_Set_Reg(0xE8);
        LCD_Write_Data(0x00);
        LCD_Write_Data(0x00);
        LCD_Write_Data(0x99);
        LCD_Write_Data(0x00);
        LCD_Write_Data(0x87);
        LCD_Write_Data(0x00);
        LCD_Write_Data(0x88);
        LCD_Write_Data(0x00);
        LCD_Write_Data(0x77);
        LCD_Write_Data(0x00);
        LCD_Write_Data(0x66);
        LCD_Write_Data(0x00);
        LCD_Write_Data(0x88);
        LCD_Write_Data(0x00);
        LCD_Write_Data(0xAA);
        LCD_Write_Data(0x00);
        LCD_Write_Data(0xBB);
        LCD_Write_Data(0x00);
        LCD_Write_Data(0x99);
        LCD_Write_Data(0x00);
        LCD_Write_Data(0x66);
        LCD_Write_Data(0x00);
        LCD_Write_Data(0x55);
        LCD_Write_Data(0x00);
        LCD_Write_Data(0x55);
        LCD_Write_Data(0x00);
        LCD_Write_Data(0x44);
        LCD_Write_Data(0x00);
        LCD_Write_Data(0x44);
        LCD_Write_Data(0x00);
        LCD_Write_Data(0x55);
        LCD_Write_Data(0x00);

        LCD_Set_Reg(0xE9);
        LCD_Write_Data(0xAA);
        LCD_Write_Data(0x00);
        LCD_Write_Data(0x00);
        LCD_Write_Data(0x00);

        LCD_Set_Reg(0x00);
        LCD_Write_Data(0xAA);

        LCD_Set_Reg(0xCF);
        LCD_Write_Data(0x00);
        LCD_Write_Data(0x00);
        LCD_Write_Data(0x00);
        LCD_Write_Data(0x00);
        LCD_Write_Data(0x00);
        LCD_Write_Data(0x00);
        LCD_Write_Data(0x00);
        LCD_Write_Data(0x00);
        LCD_Write_Data(0x00);
        LCD_Write_Data(0x00);
        LCD_Write_Data(0x00);
        LCD_Write_Data(0x00);
        LCD_Write_Data(0x00);
        LCD_Write_Data(0x00);
        LCD_Write_Data(0x00);
        LCD_Write_Data(0x00);
        LCD_Write_Data(0x00);

        LCD_Set_Reg(0xF0);
        LCD_Write_Data(0x00);
        LCD_Write_Data(0x50);
        LCD_Write_Data(0x00);
        LCD_Write_Data(0x00);
        LCD_Write_Data(0x00);

        LCD_Set_Reg(0xF3);
        LCD_Write_Data(0x00);

        LCD_Set_Reg(0xF9);
        LCD_Write_Data(0x06);
        LCD_Write_Data(0x10);
        LCD_Write_Data(0x29);
        LCD_Write_Data(0x00);

        LCD_Set_Reg(0x3A);
        LCD_Write_Data(0x55);	//66

        LCD_Set_Reg(0x11);
        delay_us(100);
        LCD_Set_Reg(0x29);
        LCD_Set_Reg(0x35);
        LCD_Write_Data(0x00);

        LCD_Set_Reg(0x51);
        LCD_Write_Data(0xFF);
        LCD_Set_Reg(0x53);
        LCD_Write_Data(0x2C);
        LCD_Set_Reg(0x55);
        LCD_Write_Data(0x82);
        LCD_Set_Reg(0x2c);

}





























