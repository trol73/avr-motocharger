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

static void LCD_init();
static void LCD_reset();
void LCD_contrast(uint8_t val);

inline static void LCD_init() {
	i2c_init();

	DDR(TIC107_PORT_POR) |= _BV(TIC107_PIN_POR);

	LCD_reset();

	i2c_start();
	i2c_sendAddress(TIC107_ADDR);

	i2c_sendByte(0b00000000);              // Co=0, RS=0
	i2c_sendByte(0b00110100);              // DL=1, M=1, SL=0, H=0
	i2c_sendByte(0b00001100);              // D=1, C=0, B=0
	i2c_sendByte(0b00000110);              // I/D=1, S=0

	i2c_sendByte(0b00110101);              // DL=1, M=1, SL=0, H=1
	i2c_sendByte(0b00000011);              // L=1
	i2c_sendByte(0b00000100);              // P=0, Q=0
	i2c_sendByte(0b00001000);              // IM=0, IB=0
	i2c_sendByte(0b00010000);              // TC1=0, TC2=0
	i2c_sendByte(0b01000010);              // S1=1, S0=0 (4x)
	i2c_sendByte(0b10000000 | 23);         // V=0 (Va)

    i2c_stop();

}


inline static void LCD_reset() {
	PORT(TIC107_PORT_POR) |= _BV(TIC107_PIN_POR);
	_delay_ms(5);
	PORT(TIC107_PORT_POR) &= ~_BV(TIC107_PIN_POR);
	_delay_ms(10);
}


void LCD_contrast(uint8_t val) {
	i2c_start();
	i2c_sendAddress(TIC107_ADDR);


    i2c_sendByte(0b00000000);              // Co=0, RS=0
    i2c_sendByte(0b00110101);              // DL=1, M=1, SL=0, H=1
    i2c_sendByte(0b10000000 | val);        // V=0 (Va)

    i2c_stop();
}




#endif /* DISPLAY_H_ */
