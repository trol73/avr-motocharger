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

#include "strings.h"
#include "keyboard.h"


const char PROGMEM STR_HELLO[] = "Hello!";
const char PROGMEM STR_WORLD[] = "Мир";

static void init() {
	DDRA = 0;
	DDRB = 0;
	DDR(LED_DATA_PORT) = _BV(LED_PIN_GREEN) | _BV(LED_PIN_RED);
	DDR(BEEPER_PORT) = _BV(BEEPER_PIN);
}

int main(void) {
	init();

	uart_init();
	MSG("INIT");
//	cli();

	LCD_init();

	LCD_contrast(0xff);

	LCD_PrintStr(STR_HELLO);

	LCD_GotoXY(1, 1);

	//LCD_PrintStr(STR_WORLD);
	//LCD_PrintInt((uint8_t)'А');
	// en  A - 65 => 65+128 = 193
	// rus A - 144
//	LCD_PrintInt(a);
//
//	LCD_PrintChar(' ');
	for (uint8_t i = 0; i < 16;i++) {
		LCD_PrintChar(40+i);
	}

	uint8_t i = 0;
	while (true) {
		keyboardCheck();
		bool update = false;
		if (key_click_flag[KEY_UP]) {
			if (i < 2) {
				i++;
				update = true;
			}
		}
		if (key_click_flag[KEY_DOWN]) {
			if (i != 0) {
				i--;
				update = true;
			}
		}
		if (update) {
			LCD_Clear();
			switch(i) {
				case 0:
					LCD_PrintStr(STR_START);
					break;
				case 1:
					LCD_PrintStr(STR_PROFILE);
					break;
				case 2:
					LCD_PrintStr(STR_SETTINGS);
					break;
			}
			LCD_GotoXY(0, 1);
			LCD_PrintInt(i);
			_delay_ms(500);
		}

//		LCD_GotoXY(0, 1);
//		LCD_PrintInt(key_counter[KEY_UP]);
//		LCD_PrintChar(' ');
//		LCD_PrintInt(key_counter[KEY_DOWN]);
//		LCD_PrintChar(' ');
//		LCD_PrintInt(key_click_flag[KEY_UP]);
		_delay_us(1000);
	}
}
