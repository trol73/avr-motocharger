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
	video_Clear();
	switch (ui_screen) {
		case SCREEN_MAIN:					// Главное меню (текущее напряжение и выбор режима)
			video_WriteStr(STR_U_EQUAL);
			ui_PrintVar(UI_VAR_U_CHARGE, meassuredU, true);
			video_GotoXY(0, 1);
			video_Write(CHAR_RIGHT);
			video_WriteStr(pgm_read_word(&STR_MAIN_MODES[ui_index]));
			break;
		case SCREEN_SELECT_VALUE:			// Выбор параметра перед началом операции
			break;
		case SCREEN_SELECT_VALUE_FOR_EDIT:		// Выбор параметра для редактирования
			video_WriteStr(pgm_read_word(&STR_VALUES[ui_index]));
			video_GotoXY(0, 1);
			video_Write(CHAR_RIGHT);
			video_Write(' ');
			video_WriteUInt8(ui_index + 1);
			video_Write(' ');
			break;
		case SCREEN_SETTINGS_MENU:
			video_WriteStr(STR_SETTINGS);
			video_GotoXY(0, 1);
			video_Write(CHAR_RIGHT);
			video_WriteStr(pgm_read_word(&STR_VALUES[ui_index]));
			break;
		case SCREEN_CHARGING:
			break;
		case SCREEN_DISCHARGING:
			break;
		case SCREEN_DESULPHATION:
			break;
	}
	
}


/************************************************************************/
/* Обработка клавиатуры меню с управлением кнопками ВВЕРХ/ВНИЗ          */
/************************************************************************/
bool ui_ProcessUpDownMenu(uint8_t max) {
	if (key_click_flag[KEY_UP]) {
		ui_index++;
		if (ui_index > max) {
			ui_index = 0;
		}
		return true;
	} 
	if (key_click_flag[KEY_DOWN]) {
		if (ui_index == 0) {
			ui_index = max;
		} else {
			ui_index--;
		}
		return true;
	}
	return false;
}

void ui_GotoScreen(uint8_t screen) {
	ui_screen = screen;
	switch (screen) {
		case SCREEN_SETTINGS_MENU:
			ui_index = 0;
			break;
	}
}

void ui_processKeys() {
	switch (ui_screen) {
		case SCREEN_MAIN:
			if (!ui_ProcessUpDownMenu(2)) {
				if (key_click_flag[KEY_ENTER]) {
					switch (ui_index) {
						case 0:
							ui_GotoScreen(SCREEN_CHARGING);
							break;
						case 0:
							ui_GotoScreen(SCREEN_DISCHARGING);
							break;
						case 0:
							ui_GotoScreen(SCREEN_SETTINGS_MENU);
							break;							
					}
				}
			}
			break;
		case SCREEN_SETTINGS_MENU:
			if (!ui_ProcessUpDownMenu(5)) {
			}
			break;
			
	}
}


#endif /* UI_H_ */