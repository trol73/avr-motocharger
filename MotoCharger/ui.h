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



void ui_Draw() {
	switch (ui_screen) {
		case SCREEN_MAIN:
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


#endif /* UI_H_ */