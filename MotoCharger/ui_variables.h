/*
 * ui_variables.h
 *
 * Created: 01.10.2014 21:56:52
 *  Author: Trol
 */ 


#ifndef UI_VARIABLES_H_
#define UI_VARIABLES_H_

#define VAR_LIST_SIZE					10	// number of variables per list

#define UI_VAR_U_CHARGE					0
#define UI_VAR_U_DISCHARGE				1
#define UI_VAR_I_CHARGE					2
#define UI_VAR_I_DISCHARGE				3
#define UI_VAR_TIME_CHARGE				4
#define UI_VAR_TIME_DISCHARGE			5
#define UI_VAR_CHARGE_PULSE_TIME		6
#define UI_VAR_DISCHARGE_PULSE_TIME		7

#define __UI_VAR_COUNT					8

#define UI_VARIABLES_COUNT				10	// сколько значений каждой переменной храним

uint16_t ui_vars[__UI_VAR_COUNT][UI_VARIABLES_COUNT];		// значения переменных

const uint16_t ui_vars_max_limits[] PROGMEM = {170, 130, 500, 100, 4*24*60, 4*24*60};

/************************************************************************/
/* Print a tie part: minutes or seconds. Two-digits number              */
/************************************************************************/
void ui_PrintTimePart(uint8_t val) {
	if (val < 10) {
		video_Write('0');
	} else {
		video_Write('0' + val / 10);
	}
	video_Write('0' + val % 10);
}

/************************************************************************/
/* Print a variable                                                     */
/************************************************************************/
void ui_PrintVar(uint8_t varType, uint16_t val, bool unit) {
	switch (varType) {
		case UI_VAR_U_CHARGE:
		case UI_VAR_U_DISCHARGE:
			// 12.5V
			video_WriteFloat(val, 100, 2);
			if (unit) {
				video_Write('V');
			}
			break;
		case UI_VAR_I_CHARGE:
		case UI_VAR_I_DISCHARGE:
			// 1.10A
			video_WriteFloat(val, 100, 1);
			if (unit) {
				video_Write('A');
			}
			break;
		case UI_VAR_TIME_CHARGE:
		case UI_VAR_TIME_DISCHARGE:
			// 10:00
			ui_PrintTimePart(val / 60);
			video_Write(':');
			ui_PrintTimePart(val % 60);
			break;
		case UI_VAR_CHARGE_PULSE_TIME:
		case UI_VAR_DISCHARGE_PULSE_TIME:
			// 10 0000mS
			video_WriteFloat(val, 10000, 5);
			if (unit) {
				video_Write('m');
				video_Write('S');
			}
			break;
	}
}

/************************************************************************/
/* Возвращает максимально возможное значение переменной                 */
/* (минимально допустимое значение для всех переменных - 1)             */
/************************************************************************/
uint16_t ui_GetMaxVarLimit(uint8_t varType) {
	return pgm_read_word(&ui_vars_max_limits[varType]);
}

#endif /* UI_VARIABLES_H_ */