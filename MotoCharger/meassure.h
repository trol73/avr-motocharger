/*
 * meassure.h
 *
 * Created: 08.10.2014 21:03:03
 *  Author: Trol
 */ 


#ifndef MEASSURE_H_
#define MEASSURE_H_

// ���������� ���������� �� ������ ����������
uint16_t meassure_outU;

// ���������� ���������� �� ������ ����������� ��������� �������
uint16_t meassure_powerU;

// ���������� ���
uint16_t meassure_outI;

// ������������� ��������� ��� � ���������� �� ������ ����������
void meassure_calcOutU(uint16_t adcVal) {
	// TODO ��������
	meassure_outU = adcVal;	
}

// ������������� ��������� ��� � ���������� �� ������ ����������� ��������� �������
void meassure_calcPowerU(uint16_t adcVal) {
	// TODO ��������
	meassure_powerU = adcVal;
}


// ������������� ��������� ��� � �������� ��������� ����
void meassure_calcOutI(uint16_t adcVal) {
	// TODO ��������
	meassure_outI = adcVal;
}



#endif /* MEASSURE_H_ */