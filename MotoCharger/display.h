/*
 * display.h
 *
 *  Created on: Nov 2, 2012
 *      Author: trol
 */

#ifndef DISPLAY_H_
#define DISPLAY_H_


#define TIC107_ADDR			0x74
#define TIC107_PIN_POR		2
#define TIC107_PORT_POR		C

#include "stddef.h"

static void LCD_Init();
static void LCD_Reset();
void LCD_Contrast(uint8_t val);

inline static void LCD_Init() {
	i2c_init();

	DDR(TIC107_PORT_POR) |= _BV(TIC107_PIN_POR);

	LCD_Reset();

	i2c_Start();
	i2c_SendAddress(TIC107_ADDR);

	i2c_SendByte(0b00000000);              // Co=0, RS=0
	i2c_SendByte(0b00110100);              // DL=1, M=1, SL=0, H=0
	i2c_SendByte(0b00001100);              // D=1, C=0, B=0
	i2c_SendByte(0b00000110);              // I/D=1, S=0

	i2c_SendByte(0b00110101);              // DL=1, M=1, SL=0, H=1
	i2c_SendByte(0b00000011);              // L=1
	i2c_SendByte(0b00000100);              // P=0, Q=0
	i2c_SendByte(0b00001000);              // IM=0, IB=0
	i2c_SendByte(0b00010000);              // TC1=0, TC2=0
	i2c_SendByte(0b01000010);              // S1=1, S0=0 (4x)
	i2c_SendByte(0b10000000 | 23);         // V=0 (Va)

    i2c_Stop();

}


inline static void LCD_Reset() {
	PORT(TIC107_PORT_POR) |= _BV(TIC107_PIN_POR);
	_delay_ms(5);
	PORT(TIC107_PORT_POR) &= ~_BV(TIC107_PIN_POR);
	_delay_ms(10);
}


void LCD_Contrast(uint8_t val) {
	i2c_Start();
	i2c_SendAddress(TIC107_ADDR);


    i2c_SendByte(0b00000000);              // Co=0, RS=0
    i2c_SendByte(0b00110101);              // DL=1, M=1, SL=0, H=1
    i2c_SendByte(0b10000000 | val);        // V=0 (Va)

    i2c_Stop();
}


void LCD_PrintChar(char ch) {
	i2c_Start();
	i2c_SendAddress(TIC107_ADDR);
	i2c_SendByte(0b01000000);              // Co=0, RS=1
	i2c_SendByte(ch+128);
	i2c_Stop();
}

void LCD_PrintStr(PGM_P str) {
	i2c_Start();
	i2c_SendAddress(TIC107_ADDR);
	i2c_SendByte(0b01000000);              // Co=0, RS=1

	for (;;) {
		uint8_t c = pgm_read_byte_near(str++);
		if (c == 0) {
			break;
		}
		i2c_SendByte(c+128);
	}
	i2c_Stop();
}

/************************************************************************/
/* Print string                                                         */
/*    str - array of 16 chars                                           */
/************************************************************************/
void LCD_PrintLine(char *str) {
	i2c_Start();
	i2c_SendAddress(TIC107_ADDR);
	i2c_SendByte(0b01000000);              // Co=0, RS=1

	for (uint8_t i = 0; i < 16; i++) {
		i2c_SendByte(str[i]+128);
	}
	i2c_Stop();
}

/************************************************************************/
/* x: column (0..15)                                                    */
/* y: row (0,1)															*/
/************************************************************************/
void LCD_GotoXY(uint8_t x, uint8_t y) {
	i2c_Start();

	i2c_SendAddress(TIC107_ADDR);
	i2c_SendByte(0b00000000);              // Co=0, RS=0
	i2c_SendByte(0b00110100);              // DL=1, M=1, SL=0, H=0
	uint8_t addr = y == 0 ? 0 : 0x40;
	i2c_SendByte(0b10000000 | (addr+x));       // set DDRAM address
	i2c_Stop();
}


void LCD_PrintInt(uint16_t val) {
	uint16_t num = 10000;
	bool started = false;

	i2c_Start();
	i2c_SendAddress(TIC107_ADDR);
	i2c_SendByte(0b01000000);              // Co=0, RS=1

	while (num > 0) {
		uint8_t b = val / num;
		if (b > 0 || started || num == 1) {
			i2c_SendByte('0' + b + 128);
			started = true;
		}
		val -= b * num;
		num /= 10;
	}
	i2c_Stop();
}


void LCD_Clear() {
	i2c_Start();
	i2c_SendAddress(TIC107_ADDR);
	i2c_SendByte(0b01000000);              // Co=0, RS=1

	for (uint8_t i = 0; i < 0x80; i++) {
		i2c_SendByte(' ' + 128);
	}
	i2c_Stop();
	LCD_GotoXY(0, 0);
}


// void LCD_Mirror() {
// 	i2c_Start();
// 	i2c_SendAddress(TIC107_ADDR);
// 	i2c_SendByte(0b01000000);              // Co=0, RS=1
// 	i2c_SendByte(0b00110101);              // DL=1, M=1, SL=0, H=1
// 	i2c_SendByte(0b00000100);              // P=0, Q=0
// 	i2c_Stop();
// }


#endif /* DISPLAY_H_ */
