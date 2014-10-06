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
#define CHAR_SQ_BRACKET_LEFT	(10)
#define CHAR_SQ_BRACKET_RIGHT	(32*6+1)

#define def_str(id, eng, rus)	const char PROGMEM STR_##id[] = rus;

const char PROGMEM STR_U_EQUAL[] = "U=";
const char PROGMEM STR_I_EQUAL[] = "I=";
// const char PROGMEM STR_CHARGE[] = "Charge";
// const char PROGMEM STR_DISCHARGE[] = "Discharge";
// const char PROGMEM STR_SETTINGS[] = "Settings";

def_str(CHARGE, "Charge", "\xc8\x61\x70\xfa\xd6\xfb\x61")
def_str(DISCHARGE, "Discharge", "\x50\x61\xd8\x70\xfa\xd6\xfb\x61")
def_str(POWER_SUPPLY, "Power supply", "\xc4\xdb\x6f\xfb \xed\xd9\xfd\x61\xfc\xd9\xfa")
def_str(SETTINGS, "Settings", "\x48\x61\x63\xfd\x70\x6f\xda\xfb\xd9")

def_str(VAL_I_CHARGE, "I.charge", "I.\xd8\x61\x70")
def_str(VAL_I_DISCHARGE, "I.discharge", "I.\x70\x61\xd8\x70")
def_str(VAL_U_CHARGE, "U.charge", "U.\xd8\x61\x70")
def_str(VAL_U_DISCHARGE, "U.discharge", "U.\x70\x61\xd8\x70")
def_str(VAL_T_CHARGE, "T.charge", "T.\xd8\x61\x70")
def_str(VAL_T_DISCHARGE, "T.discharge", "T.\x70\x61\xd8\x70")



PGM_P const STR_MAIN_MODES[] PROGMEM = {
	STR_CHARGE,
	STR_DISCHARGE,
	STR_POWER_SUPPLY,
	STR_SETTINGS,
};

PGM_P const STR_VALUES[] PROGMEM = {
	STR_VAL_U_CHARGE,
	STR_VAL_U_DISCHARGE,
	STR_VAL_I_CHARGE,
	STR_VAL_I_DISCHARGE,
	STR_VAL_T_CHARGE,
	STR_VAL_T_DISCHARGE,
};


#endif /* STRINGS_H_ */
