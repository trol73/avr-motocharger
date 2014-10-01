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

uint16_t ui_vars[__UI_VAR_COUNT];


/************************************************************************/
/* Print a float                                                        */
/* digits_mask - defines number of digits (1 for one-digits number,		*/
/*		10 - two digits, 100 - for three-digits etc.)					*/
/* digits_whole - number of digits in integral part						*/
/************************************************************************/
void ui_PrintFloat(uint16_t val, uint16_t digits_mask, uint8_t digits_whole) {	
	for (uint8_t i = 0; ; i++) {
		if (i == digits_whole) {
			LCD_PrintChar('.');
		}
		LCD_PrintChar('0' + val / digits_mask);
		val %= digits_mask;
		if (digits_mask == 1) {
			break;
		}
		digits_mask /= 10;
	}
}


/************************************************************************/
/* Print a tie part: minutes or seconds. Two-digits number              */
/************************************************************************/
void ui_PrintTimePart(uint8_t val) {
	if (val < 10) {
		LCD_PrintChar('0');
	} else {
		LCD_PrintChar(val / 10);
	}
	LCD_PrintChar(val % 10);
}

/************************************************************************/
/* Print a variable                                                     */
/************************************************************************/
void ui_PrintVar(uint8_t varType, uint16_t val) {
	switch (varType) {
		case UI_VAR_U_CHARGE:
		case UI_VAR_U_DISCHARGE:
			// 12.5V
			ui_PrintFloat(val, 100, 2);
			break;
		case UI_VAR_I_CHARGE:
		case UI_VAR_I_DISCHARGE:
			ui_PrintFloat(val, 100, 1);
			// 1.10A
			break;
		case UI_VAR_TIME_CHARGE:
		case UI_VAR_TIME_DISCHARGE:
			// 10:00
			ui_PrintTimePart(val / 60);
			LCD_PrintChar(':');
			ui_PrintTimePart(val % 60);
			break;
		case UI_VAR_CHARGE_PULSE_TIME:
		case UI_VAR_DISCHARGE_PULSE_TIME:
			// 000.000s
			break;
	}
}


#endif /* UI_VARIABLES_H_ */