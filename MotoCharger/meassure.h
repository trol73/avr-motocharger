/*
 * meassure.h
 *
 * Created: 08.10.2014 21:03:03
 *  Author: Trol
 */ 


#ifndef MEASSURE_H_
#define MEASSURE_H_

#define MEASSURE_NUMBER_OF_CALIBRATE_U		20		// ������� �������� �������� ��� ��������� ��������� ���������� ������

// ������������� ����������� ��� ��������� ����������
uint16_t meassure_calibrateU[MEASSURE_NUMBER_OF_CALIBRATE_U];

// ���������� ���������� �� ������ ����������
uint16_t meassure_outU;

// ���������� ���������� �� ������ ����������� ��������� �������
uint16_t meassure_powerU;

// ���������� ���
uint16_t meassure_outI;

// ������������� ��������� ��� � ���������� �� ������ ����������
void meassure_calcOutU(uint16_t adcVal) {
	// TODO ��������
	uint32_t result = adcVal;
	result *= 1000;
	if (adcVal <= 34) {
		result /= 2500;
	} else if (adcVal <= 55) {
		result /= 2683;
	} else if (adcVal <= 76) {
		result /= 2804;
	} else if (adcVal <= 160) {
		result /= 2946;
	} else if (adcVal <= 224) {
		result /= 2983;
	} else if (adcVal <= 412) {
		result /= 3018;
	} else {
		result /= 3025;
	}
	meassure_outU = (uint16_t)result;	
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


/************************************************************************/
/* ��������� ���������� ��������� ����������                            */
/************************************************************************/
void meassure_CalibrateU() {
	uint16_t calibrate[MEASSURE_NUMBER_OF_CALIBRATE_U]; 
	for (uint8_t u = 1; u < 210; u++) {
		uint16_t pwmU = ps_ConvertVoltageToPwm(u);
		hw_SetOutputU(pwmU);
		_delay_ms(500);
		uint16_t adc = hw_valU;
		uint32_t c = 1000;
		c *= u;
		c /= adc;
		uart_putc('(');
		uart_putw_dec(adc);
		uart_putc(' ');
		uart_putc(';');
		uart_putc(' ');
		uart_putw_dec(c);
		uart_putc(')');
		uart_putc('\n');
// 		MSG_DEC("u= ", u);
// 		MSG_DEC("a= ", adc);
// 		MSG_DEC("c= ", c);
		
	}
}

#endif /* MEASSURE_H_ */