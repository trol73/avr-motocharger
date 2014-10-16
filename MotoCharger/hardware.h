/*
 * hardware.h
 *
 * Created: 06.10.2014 19:20:00
 *  Author: Trol
 */

/************************************************************************/
/* ������ ����������� ������ � ��� � ���                                */
/************************************************************************/
#ifndef HARDWARE_H_
#define HARDWARE_H_

#define HW_MEASSURE_COUNT		5	// �� �������� ���������� ������ ���������� 


#define LED_RED					_BV(LED_PIN_RED)
#define LED_GREEN				_BV(LED_PIN_GREEN)
#define LED_YELLOW				(_BV(LED_PIN_RED) | _BV(LED_PIN_GREEN))
#define LED_DISABLED			0

bool hw_meassurePowerU;			// ���� ���� ���� ����������, � ������� ����� ����� ����������� ��������� ���������� �� ������ ��������� �������


volatile uint16_t hw_valU, hw_valI, hw_valUpower;	// ��� �������� ����������� ���������� ��������


uint16_t hw_StartMeassure(uint8_t pin);

// ����� ��� ��������� ��������� ���������, �� ������������� ��������������� ��� (����� �������� ������������ ����������� ADC_xxx) � ���� ����������
// ������� ����� ���������� �������� ���������� ������� � ����������� ���� � ���������� � ������� ����
volatile uint8_t hw_dataAvailFlags;

void hw_Init() {
	hw_valU = 0;
	hw_valI = 0;
	hw_valUpower = 0;
	hw_meassurePowerU = false;
	hw_dataAvailFlags = 0;
	hw_StartMeassure(ADC_U_OUT);
	
	// ������������� 0 �� ������� ���
	OCR1A = 0;
	OCR1B = 0;
	// ������������ �����
	TCCR1A = _BV(WGM11) | _BV(WGM10) | _BV(COM1A1) | _BV(COM1B1);
	TCCR1B = _BV(CS10);
	// ����-���������� ���, 10 ���, �������� ������� ��������
}


/************************************************************************/
/* ���������� ���                                                       */
/************************************************************************/
ISR(ADC_vect) {
	uint8_t admux = ADMUX & 0b00011111;
	uint16_t val = ADC;
	
	static uint16_t sumU, sumI, sumPower;	// ���� ����������� ���������� ��������
	static uint8_t cntU, cntI, cntPower;	// ���������� ����������� ���������

	switch (admux) {
		case _BV(ADC_U_OUT):
			sumU += val;
			cntU++;
			if (cntU >= HW_MEASSURE_COUNT) {
				hw_valU = sumU / cntU;
				hw_dataAvailFlags |= _BV(ADC_U_OUT);
				sumU = 0;
				cntU = 0;
			}
			hw_StartMeassure(ADC_I_OUT);
			break;
		case _BV(ADC_I_OUT):
			sumI += val;
			cntI++;
			if (cntI >= HW_MEASSURE_COUNT) {
				hw_valI = sumI / cntI;
				hw_dataAvailFlags |= _BV(ADC_I_OUT);
				sumI = 0;
				cntI = 0;
			}		
			hw_StartMeassure(hw_meassurePowerU ? ADC_U_POWER : ADC_U_OUT);
			break;
		case _BV(ADC_U_POWER):
			sumPower += val;
			cntPower++;
			if (cntPower >= HW_MEASSURE_COUNT) {
				hw_valUpower = sumPower / cntPower;
				hw_dataAvailFlags |= _BV(ADC_U_POWER);
				sumPower = 0;
				cntPower = 0;
			}
			hw_StartMeassure(ADC_U_OUT);
			break;
		default:
			hw_StartMeassure(ADC_U_OUT);
			break;
	}
}


/************************************************************************/
/* ��������� ��������� �� ��������� ������                              */
/*		pin: ADC_xxx ���������                                          */
/************************************************************************/
uint16_t hw_StartMeassure(uint8_t pin) {
	ADMUX = _BV(pin)|_BV(REFS0)|_BV(REFS1);
	//		REFSx - ������� �������� 2.56�
	ADCSRA = _BV(ADEN)|_BV(ADSC)|_BV(ADIE)|_BV(ADPS2)|_BV(ADPS1)|_BV(ADPS0);
	//		ADEN - ��������� ���
	//		ADSC - ���� ���������
	//		ADIE - ���������� ����������
	//		ADPSx - �������� ������� = 128, ����� �������������� = 104 ���
}


/************************************************************************/
/* ������������� �������� �� ������ ���, ������������ ����������        */
/************************************************************************/
inline void hw_SetOutputU(uint16_t val) {
	OCR1A = val;
}


/************************************************************************/
/* ������������� �������� �� ������ ���, ������������ ��� �������       */
/************************************************************************/
inline void hw_SetOutputDischarge(uint16_t val) {
	OCR1B = val;
}


inline void hw_SetLed(uint8_t val) {
	PORT(LED_DATA_PORT)	&= ~(_BV(LED_PIN_RED) | _BV(LED_PIN_GREEN));
	PORT(LED_DATA_PORT) |= val;
}

#endif /* HARDWARE_H_ */