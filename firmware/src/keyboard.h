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


uint8_t key_counter[4];						// счетчики удержания клавиш, содержат время удержания клавиш в 1/20 сек, или 0, если клавиша отпущена
bool key_click_flag[5];					// флаги того, что клавиша была нажата и отпущена
uint8_t key_up_minus_down;					// занчение key_click_flag[KEY_UP] - key_click_flag[KEY_DOWN]


/**
 *
 */
inline uint8_t key_is_repeated(uint8_t key) {
	return key_counter[key] > 15;
}





/*
 * Опрашивает клавиатуру, инкрементируя или сбрасывая счетчики key_counter.
 * Вызов не должен пересекаться с вызывами процедур LCD.
 */
static inline void keyboardCheck() {
	// configure keyboard pins to input
	DDR(KEYBOARD_DATA_PORT) &= ~(_BV(KEYBOARD_PIN_ENTER) | _BV(KEYBOARD_PIN_BACK) | _BV(KEYBOARD_PIN_UP) | _BV(KEYBOARD_PIN_DOWN));
	// enable pull-up
	PORT(KEYBOARD_DATA_PORT) |= _BV(KEYBOARD_PIN_ENTER) | _BV(KEYBOARD_PIN_BACK) | _BV(KEYBOARD_PIN_UP) | _BV(KEYBOARD_PIN_DOWN);

	key_click_flag[KEY_UP] = false;
	if (PIN(KEYBOARD_DATA_PORT) & _BV(KEYBOARD_PIN_UP)) {
		if (key_counter[KEY_UP] != 0) {
			key_click_flag[KEY_UP] = true;
		}
		key_counter[KEY_UP] = 0;
	} else {
		if (key_counter[KEY_UP] != 0xff) {
			key_counter[KEY_UP]++;
		}
	}

	key_click_flag[KEY_DOWN] = false;
	if (PIN(KEYBOARD_DATA_PORT) & _BV(KEYBOARD_PIN_DOWN)) {
		if (key_counter[KEY_DOWN] != 0) {
			key_click_flag[KEY_DOWN] = true;
		}
		key_counter[KEY_DOWN] = 0;
	} else {
		if (key_counter[KEY_DOWN] != 0xff) {
			key_counter[KEY_DOWN]++;
		}
	}

	key_click_flag[KEY_ENTER] = false;
	if (PIN(KEYBOARD_DATA_PORT) & _BV(KEYBOARD_PIN_ENTER)) {
		if ( key_counter[KEY_ENTER] != 0) {
			key_click_flag[KEY_ENTER] = true;
		}
		key_counter[KEY_ENTER] = 0;
	} else {
		if (key_counter[KEY_ENTER] != 0xff) {
			key_counter[KEY_ENTER]++;
		}
	}

	key_click_flag[KEY_BACK] = false;
	if (PIN(KEYBOARD_DATA_PORT) & _BV(KEYBOARD_PIN_BACK)) {
		if (key_counter[KEY_BACK] != 0) {
			key_click_flag[KEY_BACK] = true;
		}
		key_counter[KEY_BACK] = 0;
	} else {
		if (key_counter[KEY_BACK] != 0xff) {
			key_counter[KEY_BACK]++;
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
