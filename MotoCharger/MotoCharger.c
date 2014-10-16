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
// счетчики текущего времени
uint8_t time_hsec;	// считает 1/125 доли секунды
uint8_t time_sec;
uint8_t time_min;
uint8_t time_hour;

#include "hardware.h"
#include "power_supply.h"
#include "meassure.h"
#include "ui.h"

#define F_TIMER			125		// частота таймер на основе ТС1 на основе которого работают часы и обновление клавиатуры и экрана


uint8_t tc0_delta_count;	// используется для генерации прерывания с частотой 125 Гц


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
	
	static uint8_t blinkingCounter = 0;
	if (++blinkingCounter > 50) {
		blinkingCounter = 0;
	}
	video_blinkingState = blinkingCounter < 25;	// обновляем состояние мерцания для видеоподсистемы

	
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
		// примерно 20 раз в секунду опрашиваем клавиатуру и обновляем экран
		keyboardCheck();
		ui_ProcessKeys();
		
		// если есть готовые данные от АЦП, то вычисляем напряжения и токи
		if (hw_dataAvailFlags & _BV(ADC_U_OUT)) {
			hw_dataAvailFlags &= ~_BV(ADC_U_OUT);
			// дважды смотрим показания АЦП и сверяем для избежания искажения в случае вызова в этот момент обработчика прерывания
			uint16_t val;
			while (true) {
				val = hw_valU;
				uint16_t v2 = hw_valU;
				if (val == v2) {
					break;
				}
				MSG("u!");
			}
if (time_sec == 15)			
MSG_DEC("U ", hw_valU);			
			meassure_calcOutU(val);
		}
		if (hw_dataAvailFlags & _BV(ADC_I_OUT)) {
			hw_dataAvailFlags &= ~_BV(ADC_I_OUT);
			// дважды смотрим показания АЦП и сверяем для избежания искажения в случае вызова в этот момент обработчика прерывания
			uint16_t val;
			while (true) {
				val = hw_valI;
				uint16_t v2 = hw_valI;
				if (val == v2) {
					break;
				}
				MSG("i!");
			}
if (time_sec == 25)
MSG_DEC("I ", hw_valI);			
			meassure_calcOutI(val);
		}
		if (hw_dataAvailFlags & _BV(ADC_U_POWER)) {
			hw_dataAvailFlags &= ~_BV(ADC_U_POWER);
			// дважды смотрим показания АЦП и сверяем для избежания искажения в случае вызова в этот момент обработчика прерывания
			uint16_t val;
			while (true) {
				val = hw_valUpower;
				uint16_t v2 = hw_valUpower;
				if (val == v2) {
					break;
				}
			}
// if (time_sec == 45)
// MSG_DEC("P ", val);			
			meassure_calcPowerU(val);
		}
		
		ui_Draw();
		video_Repaint();
		_delay_ms(10);
	}

}
