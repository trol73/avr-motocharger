/*
 * power_supply.h
 *
 * Created: 08.10.2014 20:59:20
 *  Author: Trol
 */ 


#ifndef POWER_SUPPLY_H_
#define POWER_SUPPLY_H_

// ���� true, �� ���� ������� �������
bool ps_enable;

// ���� ���� ��������������� ��� ��������� ������ ����������� ��������� ����
bool ps_currentLimitMode;

// ������������� ���������� �� ������ ����� �������
uint16_t ps_U;

// ������������� ����������� ���� �� ������ ����� �������
uint16_t ps_maxI;

uint16_t ps_ConvertVoltageToPwm(uint16_t volts) {
	return volts*5;	// TODO
}


void ps_Enable(bool enable) {
	ps_enable = enable;
	hw_SetOutputU(enable ? ps_ConvertVoltageToPwm(ps_U) : 0);
}


#endif /* POWER_SUPPLY_H_ */