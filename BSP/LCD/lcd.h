#ifndef __LCD_H
#define __LCD_H
#include "stm32f10x.h"
#include "./fsmc/fsmc.h"

struct lcdStruct;

typedef struct{
	volatile uint16_t LCD_REG;
	volatile uint16_t LCD_RAM;
}LCD_TypeDef;

struct lcdStruct{
	uint16_t  id;
	
	uint16_t width;
	uint16_t height;
	uint8_t	 displaydir; //0Ϊ������ 1Ϊ����
	
	uint16_t wramcmd;
	uint16_t setxcmd;
	uint16_t setycmd;
};


extern struct lcdStruct lcdDev;


#define LCD_WIDTH 				320
#define LCD_HEIGHT 				480

//LCD����ַ
#define LCD_BASE 					(uint32_t)((0X60000000 + (0X4000000 * (FSMC_NEx - 1))) | (((1 << FSMC_Ax) * 2) -2))
#define LCD								((LCD_TypeDef*)LCD_BASE)

//����
#define LCD_BL_PIN				GPIO_Pin_0
#define LCD_BL_PORT				GPIOB
#define LCD_RCC_BL		 		RCC_APB2Periph_GPIOB
#define LCD_BL_Write(x) 	do{ x?GPIO_SetBits(LCD_BL_PORT,LCD_BL_PIN):GPIO_ResetBits(LCD_BL_PORT,LCD_BL_PIN);}while(0)


/* ɨ�跽���� */
#define L2R_U2D         0           /* ������,���ϵ��� */
#define L2R_D2U         1           /* ������,���µ��� */
#define R2L_U2D         2           /* ���ҵ���,���ϵ��� */
#define R2L_D2U         3           /* ���ҵ���,���µ��� */

#define U2D_L2R         4           /* ���ϵ���,������ */
#define U2D_R2L         5           /* ���ϵ���,���ҵ��� */
#define D2U_L2R         6           /* ���µ���,������ */
#define D2U_R2L         7           /* ���µ���,���ҵ��� */

#define DFT_SCAN_DIR    L2R_U2D     /* Ĭ�ϵ�ɨ�跽�� */

/* ���û�����ɫ */
#define WHITE           0xFFFF      /* ��ɫ */
#define BLACK           0x0000      /* ��ɫ */
#define RED             0xF800      /* ��ɫ */
#define GREEN           0x07E0      /* ��ɫ */
#define BLUE            0x001F      /* ��ɫ */ 
#define MAGENTA         0XF81F      /* Ʒ��ɫ/�Ϻ�ɫ = BLUE + RED */
#define YELLOW          0XFFE0      /* ��ɫ = GREEN + RED */
#define CYAN            0X07FF      /* ��ɫ = GREEN + BLUE */  

/* �ǳ�����ɫ */
#define BROWN           0XBC40      /* ��ɫ */
#define BRRED           0XFC07      /* �غ�ɫ */
#define GRAY            0X8430      /* ��ɫ */ 
#define DARKBLUE        0X01CF      /* ����ɫ */
#define LIGHTBLUE       0X7D7C      /* ǳ��ɫ */ 
#define GRAYBLUE        0X5458      /* ����ɫ */ 
#define LIGHTGREEN      0X841F      /* ǳ��ɫ */  
#define LGRAY           0XC618      /* ǳ��ɫ(PANNEL),���屳��ɫ */ 
#define LGRAYBLUE       0XA651      /* ǳ����ɫ(�м����ɫ) */ 
#define LBBLUE          0X2B12      /* ǳ����ɫ(ѡ����Ŀ�ķ�ɫ) */ 



//REG
#define ID_REG 					0xD4			//��ȡID
#define DIR_REG 				0X36			//��Ļ����
#define GRAM_REG				0X2C			//GRAM��ַ
#define X_REG						0X2A			//����x����	
#define Y_REG						0X2B			//����y����
#define READ_COLOR_REG	0x2E			//��ȡĳ�������ɫ
#define DISPLAY_ON_REG	0x29			//LCD������ʾ
#define DISPLAY_OFF_REG	0x28			//LCD�ر���ʾ





void LCD_Init(void);
void LCD_Scan_Dir(uint8_t dir);
void initGpio(void);
void LCD_Set_Cursor(uint16_t x, uint16_t y);
void LCD_Draw_Point(uint16_t x, uint16_t y, uint32_t color);
uint32_t LCD_Read_Point(uint16_t x, uint16_t y);
void LCD_Clear(uint16_t color);
void LCD_Display_On(void);
void LCD_Display_Off(void);
void LCD_Display_Dir(uint8_t dir);
void LCD_InitReg_5310(void);
void LCD_Color_Fill(uint16_t sx, uint16_t sy, uint16_t ex, uint16_t ey, uint16_t *color);










#endif
