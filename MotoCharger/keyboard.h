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

#define KEY_MIN_PRESS_DURATION		30	// ����������� ���������� ������ ��������� ������ ��� ������������ ����� �� ��� (����� ����������)
#define KEY_MIN_RELEASE_DURATION	60

uint8_t key_pressed_counter[4];				// �������� ��������� ������. �������� ����� ��������� �������, 1 ������� ������������� �������� 0.05 ��� (0 - ���� ������� ��������)
uint8_t key_release_counter[4];				// ������� ���������� �������. �������� ����� ���������� �������, 1 ������� ������������� �������� 0.05 ��� (0 - ���� ������� ������)
bool key_click_flag[5];						// ����� ����, ��� ������� ���� ������ � ��������
uint8_t key_up_minus_down;					// �������� (key_click_flag[KEY_UP] - key_click_flag[KEY_DOWN])
uint8_t key_repeated_counter;
	

void key_check_key(uint8_t index, bool pressed) {
	key_click_flag[index] = false;
	if (pressed) {
		if (key_pressed_counter[index] < 0xff) {
			key_pressed_counter[index]++;
		}
		if (key_pressed_counter[index] == KEY_MIN_PRESS_DURATION) {
			key_release_counter[index] = 0;
		}
		if (key_release_counter[index] > 0) {
			key_release_counter[index]--;
		}
	} else {
		if (key_pressed_counter[index] >= KEY_MIN_PRESS_DURATION) {
			if (key_release_counter[index] < 0xff) {
				key_release_counter[index]++;
			}
			if (key_release_counter[index] >= KEY_MIN_RELEASE_DURATION) {
				key_click_flag[index] = true;
				key_pressed_counter[index] = 0;
			}
		} else {
			if (key_pressed_counter[index] > 0) {
				key_pressed_counter[index]--;
			}
		}
	}	
}
/**
 *
 */
bool key_is_repeated(uint8_t key) {
	if (key_pressed_counter[key] > 250) {
		return key_repeated_counter % 50 == 0;
	} else if (key_pressed_counter[key] > 200) {
		return key_repeated_counter % 150 == 0;
	} else if (key_pressed_counter[key] > 150) {
		return key_repeated_counter % 250 == 0;
	}
	return false;
}





/*
 * Scans keyboard with increment or reset key_counter's.
  */
static inline void keyboardCheck() {
	// configure keyboard pins to input
	DDR(KEYBOARD_DATA_PORT) &= ~(_BV(KEYBOARD_PIN_ENTER) | _BV(KEYBOARD_PIN_BACK) | _BV(KEYBOARD_PIN_UP) | _BV(KEYBOARD_PIN_DOWN));
	// enable pull-up
	PORT(KEYBOARD_DATA_PORT) |= _BV(KEYBOARD_PIN_ENTER) | _BV(KEYBOARD_PIN_BACK) | _BV(KEYBOARD_PIN_UP) | _BV(KEYBOARD_PIN_DOWN);
	
	key_repeated_counter++;
	
	key_check_key(KEY_UP, !(PIN(KEYBOARD_DATA_PORT) & _BV(KEYBOARD_PIN_UP)));
	key_check_key(KEY_DOWN, !(PIN(KEYBOARD_DATA_PORT) & _BV(KEYBOARD_PIN_DOWN)));
	key_check_key(KEY_ENTER, !(PIN(KEYBOARD_DATA_PORT) & _BV(KEYBOARD_PIN_ENTER)));
	key_check_key(KEY_BACK, !(PIN(KEYBOARD_DATA_PORT) & _BV(KEYBOARD_PIN_BACK)));
/*
	key_click_flag[KEY_UP] = false;
	if (PIN(KEYBOARD_DATA_PORT) & _BV(KEYBOARD_PIN_UP)) {		// ������ UP ��������
		if (key_pressed_counter[KEY_UP] >= KEY_CLICK_MIN) {
			key_release_counter[KEY_UP] = KEY_RELEASE_MIN;
			//key_click_flag[KEY_UP] = true;
		} else if (key_release_counter[KEY_UP] > 0) {
			if (--key_release_counter[KEY_UP] == 0) {
				key_click_flag[KEY_UP] = true;
			}
		}
		key_pressed_counter[KEY_UP] = 0;
	} else {													// ������ UP ������
		if (key_pressed_counter[KEY_UP] != 0xff) {
			key_pressed_counter[KEY_UP]++;
		}
		key_release_counter[KEY_UP] = 0;
	}

	key_click_flag[KEY_DOWN] = false;
	if (PIN(KEYBOARD_DATA_PORT) & _BV(KEYBOARD_PIN_DOWN)) {		// ������ DOWN ��������
		if (key_pressed_counter[KEY_DOWN] >= KEY_CLICK_MIN) {
			key_click_flag[KEY_DOWN] = true;
		}
		key_pressed_counter[KEY_DOWN] = 0;
	} else {													// ������ DOWN ������
		if (key_pressed_counter[KEY_DOWN] != 0xff) {
			key_pressed_counter[KEY_DOWN]++;
		}
	}

	key_click_flag[KEY_ENTER] = false;
	if (PIN(KEYBOARD_DATA_PORT) & _BV(KEYBOARD_PIN_ENTER)) {	// ������ ENTER ��������
// 		if ( key_pressed_counter[KEY_ENTER] >= KEY_CLICK_MIN) {
// 			key_click_flag[KEY_ENTER] = true;
// 		}
// 		key_pressed_counter[KEY_ENTER] = 0;
// 	} else {													// ������ ENTER ������
// 		if (key_pressed_counter[KEY_ENTER] != 0xff) {
// 			key_pressed_counter[KEY_ENTER]++;
// 		}
		if (key_pressed_counter[KEY_ENTER] >= KEY_CLICK_MIN) {
			key_release_counter[KEY_ENTER] = KEY_RELEASE_MIN;
		} else if (key_release_counter[KEY_ENTER] > 0) {
			if (--key_release_counter[KEY_ENTER] == 0) {
				key_click_flag[KEY_ENTER] = true;
			}
		}
		key_pressed_counter[KEY_ENTER] = 0;
	} else {													// ������ ENTER ������
		if (key_pressed_counter[KEY_ENTER] != 0xff) {
			key_pressed_counter[KEY_ENTER]++;
		}
		key_release_counter[KEY_ENTER] = 0;
	}
	
	key_click_flag[KEY_BACK] = false;
	if (PIN(KEYBOARD_DATA_PORT) & _BV(KEYBOARD_PIN_BACK)) {		// ������ BACK ��������
		if (key_pressed_counter[KEY_BACK] >= KEY_CLICK_MIN) {
			key_click_flag[KEY_BACK] = true;
		}
		key_pressed_counter[KEY_BACK] = 0;
	} else {													// ������ BACK ������
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
*/
	
#if ENABLE_LCD_HIGHLIGHT
#endif
}


#endif /* KEYBOARD_H_ */
