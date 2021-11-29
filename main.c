/* MAIN.C file
 * 
 * Copyright (c) 2002-2005 STMicroelectronics
 */

#include "stm8s.h"

#define LCD_GPIO GPIOC
#define LCD_CS GPIO_PIN_3
#define LCD_WR GPIO_PIN_4
#define LCD_DATA GPIO_PIN_5

#define  BIAS     0x52
#define  SYSDIS   0X00
#define  SYSEN    0X02
#define  LCDON    0X06
#define  RC256    0X30
#define  WDTDIS1  0X0A
#define  BUFFERSIZE 12

static void delay(uint32_t t);
static void lcdData(uint8_t data, uint8_t cnt);
static void lcdCommand(uint8_t command);
static void lcdAddress(uint8_t addr, uint8_t data1);
static void lcdStart(void);
static void lcdClear(void);
static void lcdShowNumber(uint32_t number);
static void lcdDisplay(void);
static void lcdSetSymbol(uint8_t addr, uint8_t symb);
static void lcdUnsetSymbol(uint8_t addr, uint8_t symb);

uint8_t lcdBuffer[21];
uint8_t numbers[10] = {0xAF, 0x06, 0x6D, 0x4F, 0xC6, 0xCB, 0xEB, 0x8E, 0xEF, 0xCF};

void lcdCommand(uint8_t command) {  //100
	GPIO_WriteLow(LCD_GPIO, LCD_CS);
	lcdData(0x80, 4);
	lcdData(command, 8);
	GPIO_WriteHigh(LCD_GPIO, LCD_CS);
}

void lcdData(uint8_t data, uint8_t cnt) {
	unsigned char i;
	for (i = 0; i < cnt; i++) {
		GPIO_WriteLow(LCD_GPIO, LCD_WR);
		delay(10);
		if (data & 0x80) {
		GPIO_WriteHigh(LCD_GPIO, LCD_DATA);
		}
		else
		{
		GPIO_WriteLow(LCD_GPIO, LCD_DATA);
		}
		GPIO_WriteHigh(LCD_GPIO, LCD_WR);
		delay(10);
		data <<= 1;
	}
}

void lcdAddress(uint8_t addr, uint8_t data1)
{
	addr <<= 2;
	GPIO_WriteLow(LCD_GPIO, LCD_CS);
	lcdData(0xa0, 3);
	lcdData(addr, 6);
	lcdData(data1, 8);
	GPIO_WriteHigh(LCD_GPIO, LCD_CS);
}

void delay(uint32_t t) {
    while(--t);
}

void lcdStart(void) {
		uint8_t i;
	  GPIO_DeInit(LCD_GPIO);
    GPIO_Init(LCD_GPIO, LCD_CS, GPIO_MODE_OUT_PP_LOW_FAST);
		GPIO_Init(LCD_GPIO, LCD_WR, GPIO_MODE_OUT_PP_LOW_FAST);
		GPIO_Init(LCD_GPIO, LCD_DATA, GPIO_MODE_OUT_PP_LOW_FAST);
		for(i = 0; i < 21; i++) lcdBuffer[i] = 0x00;
		lcdCommand(BIAS);
		lcdCommand(RC256);
		lcdCommand(SYSDIS);
		lcdCommand(WDTDIS1);
		lcdCommand(SYSEN);
		lcdCommand(LCDON);
}

void lcdClear(void) {
	uint8_t i;
	for(i = 0; i < 21; i++) {
		lcdAddress(i,0);
		lcdBuffer[i] = 0x00;
	}
}

void lcdShowNumber(uint32_t number) {
	uint8_t i = 0;
	for(i = 1; i < 20; i += 3) {
		if(number) lcdBuffer[i] = numbers[number % 10];
		lcdBuffer[i-1] = lcdBuffer[i] << 4;
		lcdBuffer[i+1] = lcdBuffer[i] >> 4;
		number /= 10;
		}
	}

void lcdDisplay(void) {
	uint8_t i = 0; 
	for(i = 0; i < 21; i++) lcdAddress(20-i,lcdBuffer[i]);
}

void lcdSetSymbol(uint8_t addr, uint8_t symb) {
	lcdBuffer[20-addr] |= (1 << symb);
}

void lcdUnsetSymbol(uint8_t addr, uint8_t symb) {
	lcdBuffer[20-addr] &= ~(1 << symb);
}

main()
{
		lcdStart();
		lcdClear();
		lcdShowNumber(7908066);
		lcdDisplay();
		while(1);;

}

#ifdef USE_FULL_ASSERT

/**
  * @brief  Reports the name of the source file and the source line number
  *   where the assert_param error has occurred.
  * @param file: pointer to the source file name
  * @param line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t* file, uint32_t line)
{
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

  /* Infinite loop */
  while (1)
  {
  }
}
#endif