/*
 * ui.h
 *
 * Created: 01.10.2014 21:56:01
 *  Author: Trol
 */ 


#ifndef UI_H_
#define UI_H_


uint8_t ui_screen;		// index of current screen, SCREEN_xxx
uint8_t ui_index;		// index of current item on the screen (menu item etc.)
uint8_t ui_varType;		// тип выбранной редактируемой переменной (UI_VAR_xxx)
uint8_t ui_varIndex;	// номер текущей редактируемой переменной в списке

uint16_t ui_currentEditValue;	// тут хранится текущее значение переменной в процессе ее редактирования
uint16_t meassuredU = 125;




void ui_Draw() {
	video_Clear();
	switch (ui_screen) {
		case SCREEN_MAIN:																			// Главное меню (текущее напряжение и выбор режима)
			video_WriteStr(STR_U_EQUAL);
			ui_PrintVar(UI_VAR_U_CHARGE, meassuredU, true);
			video_GotoXY(0, 1);
			video_Write(CHAR_RIGHT);
			video_WriteStr(pgm_read_word(&STR_MAIN_MODES[ui_index]));
			break;
		case SCREEN_SELECT_VALUE:																	// Редактирование параметра
			ui_PrintVar(ui_varType, ui_vars[ui_varType][ui_index], true);
			break; 
		case SCREEN_SELECT_VALUE_FOR_EDIT:															// Выбор параметра для редактирования
			video_WriteStr(pgm_read_word(&STR_VALUES[ui_varType]));
			video_GotoXY(0, 1);
			video_Write(CHAR_RIGHT);
			video_WriteUInt8(ui_index);
			video_Write(':');
			video_Write(' ');
			ui_PrintVar(ui_varType, ui_vars[ui_varType][ui_index], true);
			break;
		case SCREEN_EDIT_VALUE:
			video_WriteStr(pgm_read_word(&STR_VALUES[ui_varType]));
			video_Write(' ');
			video_Write(CHAR_SQ_BRACKET_LEFT);
			video_WriteUInt8(ui_varIndex);
			video_Write(CHAR_SQ_BRACKET_RIGHT);
			video_GotoXY(0, 1);
			ui_PrintVar(ui_varType, ui_currentEditValue, true);
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
		case SCREEN_SELECT_VALUE_FOR_EDIT:
			ui_varType = ui_index;
			ui_index = 0;
			break;
		case SCREEN_EDIT_VALUE:
			ui_varIndex = ui_index;
			ui_currentEditValue = ui_vars[ui_varType][ui_varIndex];
			break;
	}
}

const uint8_t ui_map_main_menu[] PROGMEM = {SCREEN_CHARGING, SCREEN_DISCHARGING, SCREEN_SETTINGS_MENU}; 

void ui_ProcessKeys() {
	uint16_t max;
	switch (ui_screen) {
		case SCREEN_MAIN:
			if (!ui_ProcessUpDownMenu(2)) {
				if (key_click_flag[KEY_ENTER]) {
					ui_GotoScreen(pgm_read_byte(&ui_map_main_menu[ui_index]));
				}
			}
			break;
		case SCREEN_SETTINGS_MENU:
			if (!ui_ProcessUpDownMenu(5)) {
				if (key_click_flag[KEY_ENTER]) {
					ui_GotoScreen(SCREEN_SELECT_VALUE_FOR_EDIT);
				} else if (key_click_flag[KEY_BACK]) {
					ui_GotoScreen(SCREEN_MAIN);
				}
			}
			break;
			
		case SCREEN_SELECT_VALUE_FOR_EDIT:
			if (!ui_ProcessUpDownMenu(UI_VARIABLES_COUNT-1)) {
				if (key_click_flag[KEY_ENTER]) {
					ui_GotoScreen(SCREEN_EDIT_VALUE);
				} else if (key_click_flag[KEY_BACK]) {
					ui_GotoScreen(SCREEN_SETTINGS_MENU);
				}
			}
			break;
			
		case SCREEN_EDIT_VALUE:
			max = ui_GetMaxVarLimit(ui_varType);
			if (key_click_flag[KEY_UP]) {
MSG_DEC("+", ui_varType);
MSG_DEC("m ", max);
				if (ui_currentEditValue < max) {
					ui_currentEditValue++;
MSG_DEC("> ", ui_currentEditValue);					
				}
			} else if (key_click_flag[KEY_DOWN]) {
				if (ui_currentEditValue > 1) {
					ui_currentEditValue--;
				}
			} else if (key_click_flag[KEY_BACK]) {
				ui_GotoScreen(SCREEN_SELECT_VALUE_FOR_EDIT);
			}

			break;
			
	}
}


#endif /* UI_H_ */