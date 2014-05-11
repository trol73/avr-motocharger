/*
 * main.c
 *
 *  Created on: Nov 2, 2012
 *      Author: trol
 */


#include <avr/io.h>
#include <stdlib.h>
#include <stdbool.h>
#include <avr/interrupt.h>
#include <avr/wdt.h>
#include <util/delay.h>

#include "i2c.h"
#include "display.h"
#include "debug.h"


int main(void) {
	uart_init();
	MSG("INIT");
//	cli();

/*
	DDRC = 0xff;
	MSG_HEX("DDRC = ", DDRC, 1);
	while (true) {
		PORTC = 0xff;
		MSG_HEX("PORTC = ", PORTC, 1);
		MSG_HEX("PINC = ", PINC, 1);
		_delay_ms(2000);
		PORTC = 0x0;
		MSG_HEX("PORTC = ", PORTC, 1);
		MSG_HEX("PINC = ", PINC, 1);
		_delay_ms(2000);

	}
*/

	LCD_init();

	LCD_contrast(0xff);

	i2c_start();

	i2c_sendAddress(TIC107_ADDR);

	i2c_sendByte(0b01000000);              // Co=0, RS=1

	i2c_sendByte('1'+128);
	i2c_sendByte('A'+128);
	i2c_sendByte('F'+128);
	i2c_sendByte('B'+128);
	i2c_sendByte('2'+128);
	i2c_sendByte('a'+128);
	i2c_sendByte('f'+128);
	i2c_sendByte('b'+128);
	i2c_sendByte('3'+128);
	i2c_sendByte('d'+128);
	i2c_sendByte('D'+128);
	i2c_sendByte('E'+128);
	i2c_sendByte('7'+128);
	i2c_sendByte('3'+128);
	i2c_sendByte('5'+128);
	i2c_sendByte('L'+128);
	i2c_stop();


	 // смещаем указатель на вторую строку
	i2c_start();

	i2c_sendAddress(TIC107_ADDR);
	i2c_sendByte(0b00000000);              // Co=0, RS=0
	i2c_sendByte(0b00110100);              // DL=1, M=1, SL=0, H=0
	i2c_sendByte(0b10000000 | 0x40);       // set DDRAM address
	i2c_stop();





	i2c_start();

	i2c_sendAddress(TIC107_ADDR);

	i2c_sendByte(0b01000000);              // Co=0, RS=1

	i2c_sendByte('1'+128);
	i2c_sendByte('A'+128);
	i2c_sendByte('F'+128);
	i2c_sendByte('B'+128);
	i2c_sendByte('2'+128);
	i2c_sendByte('a'+128);
	i2c_sendByte('f'+128);
	i2c_sendByte('b'+128);
	i2c_sendByte('3'+128);
	i2c_sendByte('d'+128);
	i2c_sendByte('D'+128);
	i2c_sendByte('E'+128);
	i2c_sendByte('7'+128);
	i2c_sendByte('3'+128);
	i2c_sendByte('5'+128);
	i2c_sendByte('L'+128);
	i2c_stop();

	while (true) {

	}
}
