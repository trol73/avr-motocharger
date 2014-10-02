/*
 * MotoCharger.c
 *
 * Created: 02.11.2012 21:19:10
 *  Author: Trol
 */ 


#include <avr/io.h>
#include <stdlib.h>
#include <stdbool.h>
#include <avr/interrupt.h>
#include <avr/wdt.h>
#include <util/delay.h>

#include "i2c.h"
#include "display.h"
#include "video.h"
#include "debug.h"

#include "strings.h"
#include "keyboard.h"
#include "ui_constants.h"
#include "ui_variables.h"
#include "ui.h"


static void init() {
	DDRA = 0;
	DDRB = 0;
	DDR(LED_DATA_PORT) = _BV(LED_PIN_GREEN) | _BV(LED_PIN_RED);
	DDR(BEEPER_PORT) = _BV(BEEPER_PIN);
}

void beep(bool enable)  {
	if (enable) {
		PORT(BEEPER_PORT) |= _BV(BEEPER_PIN);
	} else {
		PORT(BEEPER_PORT) &= ~_BV(BEEPER_PIN);
	}
}

int main(void) {
	init();
	
	beep(false);

	uart_init();

	LCD_Init();
	LCD_Contrast(0xff);
	video_Reset();

	//LCD_PrintStr(STR_CHARGE);
	
	
//	LCD_GotoXY(1, 1);

// 	for (uint8_t i = 0; i < 16;i++) {
// 		LCD_PrintChar(40+i);
// 	}

	video_Write('!');
	video_GotoXY(0, 1);
	video_Write('A');

	uint16_t i = 0;
	while (true) {
		video_GotoXY(0, 0);
		video_WriteStr(STR_CHARGE);
		video_GotoXY(0, 1);
		video_WriteFloat(i++, 100, 3);
		video_Repaint();
		_delay_ms(20000);
	}

// 	while (true) {
// 		keyboardCheck();
// 		ui_processKeys();
// 		
// 		ui_Draw();
// 		video_Repaint();
// 		_delay_ms(100);
// 	}

}
