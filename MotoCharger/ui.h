/*
 * ui.h
 *
 * Created: 01.10.2014 21:56:01
 *  Author: Trol
 */ 


#ifndef UI_H_
#define UI_H_


uint8_t ui_screen;	// index of current screen, SCREEN_xxx
uint8_t ui_index;	// index of current item on the screen (menu item etc.)

uint16_t meassuredU = 125;



void ui_Draw() {
	switch (ui_screen) {
		case SCREEN_MAIN:
			// [U=12.3V         ]
			// [ Charge         ]
			video_WriteStr(STR_U_EQUAL);
			ui_PrintVar(UI_VAR_U_CHARGE, meassuredU, true);
			video_Write('V');
			video_GotoXY(0, 1);
			video_WriteStr(STR_MAIN_MODES[ui_index]);
			break;
		case SCREEN_SELECT_VALUE:
			break;
		case SCREEN_SELECT_VALUE_FOR_EDIT:
			break;
		case SCREEN_SETTINGS_MENU:
			break;
		case SCREEN_CHARGING:
			break;
		case SCREEN_DISCHARGING:
			break;
		case SCREEN_DESULPHATION:
			break;
	}
	
}


void ui_processKeys() {
	switch (ui_screen) {
		case SCREEN_MAIN:
				if (key_click_flag[KEY_UP]) {
					ui_index++;
					if (ui_index > 2) {
						ui_index = 0;
					}
				} else if (key_click_flag[KEY_DOWN]) {
					if (ui_index == 0) {
						ui_index = 2;
					} else {
						ui_index--;
					}
				}
			break;
	}
}


#endif /* UI_H_ */