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
#include "hardware.h"
#include "ui.h"

#define F_TIMER			125		// частота таймер на основе ТС1 на основе которого работают часы и обновление клавиатуры и экрана


uint8_t tc0_delta_count;	// используется для генерации прерывания с частотой 125 Гц

// счетчики текущего времени
uint8_t time_hsec;	// считает 1/125 доли секунды
uint8_t time_sec;
uint8_t time_min;
uint8_t time_hour;

static void init() {
	DDRA = 0;
	DDRB = 0;
	DDRC = 0;
	DDRD = _BV(4) | _BV(5);	// выходы ШИМ
	PORTA = 0;
	DDR(LED_DATA_PORT) |= _BV(LED_PIN_GREEN) | _BV(LED_PIN_RED);
	DDR(BEEPER_PORT) |= _BV(BEEPER_PIN);
	
	tc0_delta_count = F_CPU/1024/F_TIMER;
	// конфигурирование таймера-счетчика TC0	 - нормальный режим с делителем 1024
	TCCR0 = _BV(CS00) | _BV(CS02);
	TIMSK |= _BV(OCIE0);	// включение прерывания при OCF0
	
}

void beep(bool enable)  {
	if (enable) {
		PORT(BEEPER_PORT) |= _BV(BEEPER_PIN);
	} else {
		PORT(BEEPER_PORT) &= ~_BV(BEEPER_PIN);
	}
}


/************************************************************************/
/* Обработчик прерывания таймера-счетчика-1                             */
/* Используется для формирования временных интервалов кратных 1 сек,    */
/* а также для обновления состояния клавиатуры и обновления экрана      */
/************************************************************************/
ISR(TIMER0_COMP_vect) {
	OCR0 += tc0_delta_count;
	time_hsec++;
	
	
	uint8_t hsec5 = time_hsec % 5; 
	if (hsec5 == 1) {
		// опрашиваем клавиатуру с частотой 25Гц
		keyboardCheck();
		ui_ProcessKeys();
	} else if (hsec5 == 3) {
		// обновляем экран с частотой 25Гц
		ui_Draw();
		video_Repaint();
	}
	
	if (time_hsec >= F_TIMER) {
		time_hsec = 0;
		time_sec++;
ui_vars[UI_VAR_TIME_CHARGE][0]++;			// TODO временная отладка
		if (time_sec == 60) {
			time_sec = 0;
			time_min++;
			if (time_min == 60) {
				time_min = 0;

				time_hour++;
				if (time_hour == 24) {
					time_hour = 0;
					
				}
			}
		}
	}
}

int main(void) {
	uart_init();
	MSG("INIT");
	
	init();
	hw_Init();
	sei();	
	
	beep(false);

	LCD_Init();
	LCD_Contrast(0xff);
	video_Reset();
	ui_Init();


	ui_screen = SCREEN_MAIN;
	meassuredU = 125;

	while (true) {
		_delay_ms(100);
	}

}
