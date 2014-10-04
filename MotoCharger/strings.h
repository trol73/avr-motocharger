/*
 * strings.h
 *
 *  Created on: May 12, 2014
 *      Author: trol
 */

#ifndef STRINGS_H_
#define STRINGS_H_

#define CHAR_LEFT		(32*5+2)
#define CHAR_RIGHT		(32*5+2+16)
#define CHAR_UP			(32*4+16)
#define CHAR_DOWN		(32*4+16+2)
#define CHAR_UP_DOWN	(32*4+16+1)

#define def_str(id, eng)	const char PROGMEM STR_##id[] = eng;

const char PROGMEM STR_U_EQUAL[] = "U=";
// const char PROGMEM STR_CHARGE[] = "Charge";
// const char PROGMEM STR_DISCHARGE[] = "Discharge";
// const char PROGMEM STR_SETTINGS[] = "Settings";

def_str(CHARGE, "Charge")
def_str(DISCHARGE, "Discharge")
def_str(SETTINGS, "Settings")

def_str(VAL_I_CHARGE, "I.charge")
def_str(VAL_I_DISCHARGE, "I.discharge")
def_str(VAL_U_CHARGE, "U.charge")
def_str(VAL_U_DISCHARGE, "U.discharge")
def_str(VAL_T_CHARGE, "T.charge")
def_str(VAL_T_DISCHARGE, "T.discharge")


PGM_P const STR_MAIN_MODES[] PROGMEM = {
	STR_CHARGE,
	STR_DISCHARGE,
	STR_SETTINGS,
};

PGM_P const STR_VALUES[] PROGMEM = {
	STR_VAL_I_CHARGE,
	STR_VAL_I_DISCHARGE,
	STR_VAL_U_CHARGE,
	STR_VAL_U_DISCHARGE,
	STR_VAL_T_CHARGE,
	STR_VAL_T_DISCHARGE,
};


#endif /* STRINGS_H_ */
