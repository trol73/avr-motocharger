/*
 * settings.h
 *
 * Created: 28.10.2014 20:27:25
 *  Author: Trol
 */ 


#ifndef SETTINGS_H_
#define SETTINGS_H_

#define SETTINGS_PS_U						0
#define SETTINGS_PS_MAX_I					1
#define SETTINGS_PS_CALIBRATE_U				2


void settings_Check();


/************************************************************************/
/* Загружает настройки из EEPROM                                        */
/************************************************************************/
inline void settings_Load() {
	ps_U = eeprom_read_word((uint16_t *) SETTINGS_PS_U);	ps_maxI = eeprom_read_word((uint16_t *) SETTINGS_PS_MAX_I);
	for (uint8_t i = 0; i < MEASSURE_NUMBER_OF_CALIBRATE_U; i++) {
		meassure_calibrateU[i] = eeprom_read_word((uint16_t *)SETTINGS_PS_CALIBRATE_U + i);
	}
	
//ui_vars[__UI_VAR_COUNT][UI_VARIABLES_COUNT];		// значения переменных	
	settings_Check();
}


/************************************************************************/
/* Сохраняет настройки в EEPROM                                         */
/************************************************************************/
void settings_Save() {
	eeprom_update_word((uint16_t *)SETTINGS_PS_U, ps_U);
	eeprom_update_word((uint16_t *)SETTINGS_PS_MAX_I, ps_maxI);
	for (uint8_t i = 0; i < MEASSURE_NUMBER_OF_CALIBRATE_U; i++) {
		eeprom_update_word((uint16_t *)SETTINGS_PS_CALIBRATE_U + i, meassure_calibrateU[i]);
	}
}

/************************************************************************/
/* Проверяет валидность настроек, загруженных из EEPROM и корректирует  */
/* неверные значения													*/
/************************************************************************/
inline void settings_Check() {
	bool errors = false;
	if (ps_U > 200) {
		ps_U = 1;
		errors = true;
	}
	if (ps_maxI > 1000) {
		ps_maxI = 1;
		errors = true;
	}
	if (errors) {
		settings_Save();
	}
}







#endif /* SETTINGS_H_ */