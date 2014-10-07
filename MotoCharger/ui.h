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
uint8_t ui_varType;		// ��� ��������� ������������� ���������� (UI_VAR_xxx)
uint8_t ui_varIndex;	// ����� ������� ������������� ���������� � ������

uint16_t ui_currentEditValue;	// ��� �������� ������� �������� ���������� � �������� �� ��������������
uint16_t meassuredU = 125;




void ui_Draw() {
	video_Clear();
	switch (ui_screen) {
		case SCREEN_MAIN:																			// ������� ���� (������� ���������� � ����� ������)
			video_WriteStr(STR_U_EQUAL);
			ui_PrintVar(UI_VAR_U_CHARGE, meassuredU, true);
			video_GotoXY(0, 1);
			video_Write(CHAR_RIGHT);
			video_WriteStr(pgm_read_word(&STR_MAIN_MODES[ui_index]));
			break;
		case SCREEN_SELECT_VALUE:																	// �������������� ���������
			ui_PrintVar(ui_varType, ui_vars[ui_varType][ui_index], true);
			break; 
		case SCREEN_SELECT_VALUE_FOR_EDIT:															// ����� ��������� ��� ��������������
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
		case SCREEN_POWER_SUPPLY:
			video_WriteStr(STR_POWER_SUPPLY);
			video_GotoXY(0, 1);
			video_WriteStr(STR_U_EQUAL);
			ui_PrintVar(UI_VAR_U_CHARGE, hw_valU, true);
			video_GotoXY(9, 1);
			video_WriteStr(STR_I_EQUAL);
			ui_PrintVar(UI_VAR_I_CHARGE, hw_valI, true);
			break;
		case SCREEN_DESULPHATION:
			break;
	}
}


/************************************************************************/
/* ��������� ���������� ���� � ����������� �������� �����/����          */
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


void ui_SetScreen(uint8_t screen) {
	ui_screen = screen;
	switch (screen) {
		case SCREEN_CHARGING:
			hw_SetLed(LED_GREEN);
			break;
		case SCREEN_DISCHARGING:
			hw_SetLed(LED_RED);
			break;
		case SCREEN_POWER_SUPPLY:
			//hw_SetLed(LED_YELLOW);
			break;
		default:
			hw_SetLed(LED_DISABLED);
			break;
	}
}

const uint8_t ui_map_main_menu[] PROGMEM = {SCREEN_CHARGING, SCREEN_DISCHARGING, SCREEN_POWER_SUPPLY, SCREEN_SETTINGS_MENU}; 

void ui_ProcessKeys() {
	uint16_t max;
	switch (ui_screen) {
		case SCREEN_MAIN:
			if (!ui_ProcessUpDownMenu(3)) {
				if (key_click_flag[KEY_ENTER]) {
					ui_SetScreen(pgm_read_byte(&ui_map_main_menu[ui_index]));
				}
			}
			break;
		case SCREEN_SETTINGS_MENU:
			if (!ui_ProcessUpDownMenu(5)) {
				if (key_click_flag[KEY_ENTER]) {
					ui_SetScreen(SCREEN_SELECT_VALUE_FOR_EDIT);
					ui_varType = ui_index;
					ui_index = 0;
				} else if (key_click_flag[KEY_BACK]) {
					ui_index = 3;	// settings
					ui_SetScreen(SCREEN_MAIN);
				}
			}
			break;
			
		case SCREEN_SELECT_VALUE_FOR_EDIT:
			if (!ui_ProcessUpDownMenu(UI_VARIABLES_COUNT-1)) {
				if (key_click_flag[KEY_ENTER]) {
					ui_SetScreen(SCREEN_EDIT_VALUE);
					ui_varIndex = ui_index;
					ui_currentEditValue = ui_vars[ui_varType][ui_varIndex];
				} else if (key_click_flag[KEY_BACK]) {
					ui_SetScreen(SCREEN_SETTINGS_MENU);
					ui_index = ui_varType;
				}
			}
			break;
			
		case SCREEN_EDIT_VALUE:
			max = ui_GetMaxVarLimit(ui_varType);
			if (key_click_flag[KEY_UP] || key_is_repeated(KEY_UP)) {
				if (ui_currentEditValue < max) {
					ui_currentEditValue++;
				}
			} else if (key_click_flag[KEY_DOWN] || key_is_repeated(KEY_DOWN)) {
				if (ui_currentEditValue > 1) {
					ui_currentEditValue--;
				}
			} else if (key_click_flag[KEY_BACK]) {
				ui_index = ui_varIndex;
				ui_SetScreen(SCREEN_SELECT_VALUE_FOR_EDIT);
			}
			break;
			
		case SCREEN_POWER_SUPPLY:
			if (key_click_flag[KEY_BACK]) {
				ui_SetScreen(SCREEN_MAIN);
			}
			break;		
			
		case SCREEN_CHARGING:
			if (key_click_flag[KEY_BACK]) {
				ui_SetScreen(SCREEN_MAIN);
			}
			break;
			
		case SCREEN_DISCHARGING:
			if (key_click_flag[KEY_BACK]) {
				ui_SetScreen(SCREEN_MAIN);
			}		
			break;			

	}
}


void ui_Init() {
	ui_SetScreen(SCREEN_MAIN);
}

#endif /* UI_H_ */