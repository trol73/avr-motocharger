/*
 * keyboard.h
 *
 *  Created on: May 12, 2014
 *      Author: trol
 */

#ifndef KEYBOARD_H_
#define KEYBOARD_H_


#define KEY_UP			0
#define KEY_DOWN		1
#define KEY_ENTER		2
#define KEY_BACK		3
#define KEY_UP_OR_DOWN	4

#define KEY_CLICK_MIN	10	// минимальное количество тактов удержания кнопки для засчитывания клика по ней (после отпускания)

uint8_t key_pressed_counter[4];				// счетчики удержания клавиш. содержат время удержания клавиши, 1 единица соответствует примерно 0.05 сек (0 - если клавиша отпущена)
uint8_t key_release_counter[4];				// счетчик отпускания клавиши. содержат время отпускания клавиши, 1 единица соответствует примерно 0.05 сек (0 - если клавиша нажата)
bool key_click_flag[5];						// флаги того, что клавиша была нажата и отпущена
uint8_t key_up_minus_down;					// разность (key_click_flag[KEY_UP] - key_click_flag[KEY_DOWN])


/**
 *
 */
inline bool key_is_repeated(uint8_t key) {
	return key_pressed_counter[key] > 50;//15;
}





/*
 * Scans keyboard with increment or reset key_counter's.
  */
static inline void keyboardCheck() {
	// configure keyboard pins to input
	DDR(KEYBOARD_DATA_PORT) &= ~(_BV(KEYBOARD_PIN_ENTER) | _BV(KEYBOARD_PIN_BACK) | _BV(KEYBOARD_PIN_UP) | _BV(KEYBOARD_PIN_DOWN));
	// enable pull-up
	PORT(KEYBOARD_DATA_PORT) |= _BV(KEYBOARD_PIN_ENTER) | _BV(KEYBOARD_PIN_BACK) | _BV(KEYBOARD_PIN_UP) | _BV(KEYBOARD_PIN_DOWN);

	key_click_flag[KEY_UP] = false;
	if (PIN(KEYBOARD_DATA_PORT) & _BV(KEYBOARD_PIN_UP)) {		// кнопка UP отпущена
		if (key_pressed_counter[KEY_UP] >= KEY_CLICK_MIN) {
			key_click_flag[KEY_UP] = true;
		}
		key_pressed_counter[KEY_UP] = 0;
	} else {													// кнопка UP нажата
		if (key_pressed_counter[KEY_UP] != 0xff) {
			key_pressed_counter[KEY_UP]++;
		}
	}

	key_click_flag[KEY_DOWN] = false;
	if (PIN(KEYBOARD_DATA_PORT) & _BV(KEYBOARD_PIN_DOWN)) {		// кнопка DOWN отпущена
		if (key_pressed_counter[KEY_DOWN] >= KEY_CLICK_MIN) {
			key_click_flag[KEY_DOWN] = true;
		}
		key_pressed_counter[KEY_DOWN] = 0;
	} else {													// кнопка DOWN нажата
		if (key_pressed_counter[KEY_DOWN] != 0xff) {
			key_pressed_counter[KEY_DOWN]++;
		}
	}

	key_click_flag[KEY_ENTER] = false;
	if (PIN(KEYBOARD_DATA_PORT) & _BV(KEYBOARD_PIN_ENTER)) {	// кнопка ENTER отпущена
		if ( key_pressed_counter[KEY_ENTER] >= KEY_CLICK_MIN) {
			key_click_flag[KEY_ENTER] = true;
		}
		key_pressed_counter[KEY_ENTER] = 0;
	} else {													// кнопка ENTER нажата
		if (key_pressed_counter[KEY_ENTER] != 0xff) {
			key_pressed_counter[KEY_ENTER]++;
		}
	}

	key_click_flag[KEY_BACK] = false;
	if (PIN(KEYBOARD_DATA_PORT) & _BV(KEYBOARD_PIN_BACK)) {		// кнопка BACK отпущена
		if (key_pressed_counter[KEY_BACK] >= KEY_CLICK_MIN) {
			key_click_flag[KEY_BACK] = true;
		}
		key_pressed_counter[KEY_BACK] = 0;
	} else {													// кнопка BACK нажата
		if (key_pressed_counter[KEY_BACK] != 0xff) {
			key_pressed_counter[KEY_BACK]++;
		}
	}

	key_click_flag[KEY_UP_OR_DOWN] = key_click_flag[KEY_UP] | key_click_flag[KEY_DOWN] | key_is_repeated(KEY_UP) | key_is_repeated(KEY_DOWN);

	key_up_minus_down = 0;
	if ( key_is_repeated(KEY_UP) || key_click_flag[KEY_UP] ) {
		key_up_minus_down++;
	}
	if ( key_is_repeated(KEY_DOWN) || key_click_flag[KEY_DOWN] ) {
		key_up_minus_down--;
	}

#if ENABLE_LCD_HIGHLIGHT
#endif
}


#endif /* KEYBOARD_H_ */
