#ifndef __LCD_H
#define __LCD_H
#define STM32F10X_HD 1
#include "stm32f10x.h"
#include "./fsmc/fsmc.h"
#include <stdio.h>
#include <stdint.h>
#include "./delay/delay.h"

struct lcdStruct;

typedef struct{
	volatile uint16_t LCD_REG;
	volatile uint16_t LCD_RAM;
}LCD_TypeDef;

struct lcdStruct{
	uint16_t  id;
	
	uint16_t width;
	uint16_t height;
	uint8_t	 displaydir; //0为竖屏， 1为横屏
	
	uint16_t wramcmd;
	uint16_t setxcmd;
	uint16_t setycmd;
};


extern struct lcdStruct lcdDev;

#define TOUCH_ENABLE      1

#define LCD_WIDTH 				320
#define LCD_HEIGHT 				480

//LCD基地址
#define LCD_BASE 					(uint32_t)((0X60000000 + (0X4000000 * (FSMC_NEx - 1))) | (((1 << FSMC_Ax) * 2) -2))
#define LCD								((LCD_TypeDef*)LCD_BASE)

//引脚
#define LCD_BL_PIN				GPIO_Pin_0
#define LCD_BL_PORT				GPIOB
#define LCD_RCC_BL		 		RCC_APB2Periph_GPIOB
#define LCD_BL_Write(x) 	do{ x?GPIO_SetBits(LCD_BL_PORT,LCD_BL_PIN):GPIO_ResetBits(LCD_BL_PORT,LCD_BL_PIN);}while(0)


/* 扫描方向定义 */
#define L2R_U2D         0           /* 从左到右,从上到下 */
#define L2R_D2U         1           /* 从左到右,从下到上 */
#define R2L_U2D         2           /* 从右到左,从上到下 */
#define R2L_D2U         3           /* 从右到左,从下到上 */

#define U2D_L2R         4           /* 从上到下,从左到右 */
#define U2D_R2L         5           /* 从上到下,从右到左 */
#define D2U_L2R         6           /* 从下到上,从左到右 */
#define D2U_R2L         7           /* 从下到上,从右到左 */

#define DFT_SCAN_DIR    L2R_U2D     /* 默认的扫描方向 */

/* 常用画笔颜色 */
#define WHITE           0xFFFF      /* 白色 */
#define BLACK           0x0000      /* 黑色 */
#define RED             0xF800      /* 红色 */
#define GREEN           0x07E0      /* 绿色 */
#define BLUE            0x001F      /* 蓝色 */ 
#define MAGENTA         0XF81F      /* 品红色/紫红色 = BLUE + RED */
#define YELLOW          0XFFE0      /* 黄色 = GREEN + RED */
#define CYAN            0X07FF      /* 青色 = GREEN + BLUE */  

/* 非常用颜色 */
#define BROWN           0XBC40      /* 棕色 */
#define BRRED           0XFC07      /* 棕红色 */
#define GRAY            0X8430      /* 灰色 */ 
#define DARKBLUE        0X01CF      /* 深蓝色 */
#define LIGHTBLUE       0X7D7C      /* 浅蓝色 */ 
#define GRAYBLUE        0X5458      /* 灰蓝色 */ 
#define LIGHTGREEN      0X841F      /* 浅绿色 */  
#define LGRAY           0XC618      /* 浅灰色(PANNEL),窗体背景色 */ 
#define LGRAYBLUE       0XA651      /* 浅灰蓝色(中间层颜色) */ 
#define LBBLUE          0X2B12      /* 浅棕蓝色(选择条目的反色) */ 



//常用REG
#define ID_REG 					0xD4			//读取ID
#define DIR_REG 				0x36			//屏幕方向
#define GRAM_REG				0x2C			//GRAM地址
#define X_REG						0x2A			//设置x坐标	
#define Y_REG						0x2B			//设置y坐标
#define READ_COLOR_REG	0x2E			//读取某个点的颜色
#define DISPLAY_ON_REG	0x29			//LCD开启显示
#define DISPLAY_OFF_REG	0x28			//LCD关闭显示





void LCD_Init(void);
void LCD_Scan_Dir(uint8_t dir);
void initGpio(void);
void LCD_Set_Cursor(uint16_t x, uint16_t y);
void LCD_Draw_Point(uint16_t x, uint16_t y, uint16_t color);
uint32_t LCD_Read_Point(uint16_t x, uint16_t y);
void LCD_Clear(uint16_t color);
void LCD_Display_On(void);
void LCD_Display_Off(void);
void LCD_Display_Dir(uint8_t dir);
void LCD_InitReg_5310(void);
void LCD_Color_Fill(uint16_t sx, uint16_t sy, uint16_t ex, uint16_t ey, uint16_t *color);





//触摸屏
#if TOUCH_ENABLE
#include "./spi/spiTemplate.h"
#define CT_MAX_TOUCH 			10


struct tpStruct{
		//存储坐标，用 x[0],y[0]代表:此次扫描时,触屏的坐标,用 x[9],y[9]存储第一次按下时的坐标
		uint16_t x[CT_MAX_TOUCH];
		uint16_t y[CT_MAX_TOUCH];
	
		//点击状态
		uint16_t satate;	
	
		//校准因子
		float xfac;
		float yfac;
		short xc;
		short yc;

};

extern struct tpStruct tpDev;
extern struct spiStruct tpSpi;


//命令
#define XCMD							0xD0
#define YCMD							0x90

//引脚
#define SPI_CS_PIN				GPIO_Pin_11
#define SPI_MISO_PIN			GPIO_Pin_2
#define SPI_MOSI_PIN			GPIO_Pin_9
#define SPI_SCK_PIN				GPIO_Pin_1
#define TOUCH_IT_PIN			GPIO_Pin_10

#define SPI_CS_PORT				GPIOF
#define SPI_MISO_PORT			GPIOB
#define SPI_MOSI_PORT			GPIOF
#define SPI_SCK_PORT			GPIOB
#define TOUCH_IT_PORT			GPIOF

#define	RCC_CS_PORT				RCC_APB2Periph_GPIOF
#define RCC_MISO_PORT			RCC_APB2Periph_GPIOB
#define RCC_MOSI_PORT			RCC_APB2Periph_GPIOF
#define RCC_SCK_PORT			RCC_APB2Periph_GPIOB
#define RCC_IT_PORT				RCC_APB2Periph_GPIOF




#define TP_READ_TIMES			10	
#define TP_XY_ERR_RANGE		50
#define TP_PRES_DOWN    	0x8000
#define TOUCH_IT					(uint8_t)GPIO_ReadInputDataBit(TOUCH_IT_PORT, TOUCH_IT_PIN)


#endif



#endif
