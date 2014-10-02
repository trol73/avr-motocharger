/*
 * strings.h
 *
 *  Created on: May 12, 2014
 *      Author: trol
 */

#ifndef STRINGS_H_
#define STRINGS_H_


// const char PROGMEM STR_PROFILE[] = "Profile";
// const char PROGMEM STR_MEASSURE[] = "Meassure";
// const char PROGMEM STR_START[] = "Start";
// const char PROGMEM STR_SETTINGS[] = "Settings";

const char PROGMEM STR_U_EQUAL[] = "U=";
const char PROGMEM STR_CHARGE[] = "Charge";
const char PROGMEM STR_DISCHARGE[] = "Discharge";
const char PROGMEM STR_SETTINGS[] = "Settings";

PGM_P const STR_MAIN_MODES[] PROGMEM = {
	STR_CHARGE,
	STR_DISCHARGE,
	STR_SETTINGS,
};


#endif /* STRINGS_H_ */
