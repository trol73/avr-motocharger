/*
 * power_supply.h
 *
 * Created: 08.10.2014 20:59:20
 *  Author: Trol
 */ 


#ifndef POWER_SUPPLY_H_
#define POWER_SUPPLY_H_

// если true, то блок питания включен
bool ps_enable;

// этот флаг устанавливается при включении режима ограничения выходного тока
bool ps_currentLimitMode;

// установленное напряжение на выходе блока питания
uint16_t ps_U;

// установленное ограничение тока на выходе блока питания
uint16_t ps_maxI;

uint16_t ps_ConvertVoltageToPwm(uint16_t volts) {
	return volts*5;	// TODO
}


void ps_Enable(bool enable) {
	ps_enable = enable;
	hw_SetOutputU(enable ? ps_ConvertVoltageToPwm(ps_U) : 0);
}


#endif /* POWER_SUPPLY_H_ */