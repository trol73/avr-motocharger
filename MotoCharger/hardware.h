/*
 * hardware.h
 *
 * Created: 06.10.2014 19:20:00
 *  Author: Trol
 */

/************************************************************************/
/* Модуль подпрограмм работы с ШИМ и АЦП                                */
/************************************************************************/
#ifndef HARDWARE_H_
#define HARDWARE_H_

#define HW_MEASSURE_COUNT		5	// по скольким измерениям делать усреднение 


#define LED_RED					_BV(LED_PIN_RED)
#define LED_GREEN				_BV(LED_PIN_GREEN)
#define LED_YELLOW				(_BV(LED_PIN_RED) | _BV(LED_PIN_GREEN))
#define LED_DISABLED			0

bool hw_meassurePowerU;			// если этот флаг установлен, в фоновом режме будет выполняться измерение напряжения на выходе источника питания

uint16_t hw_valUsum;			// сюда суммируются измеренные значения напряжения на батарее
uint16_t hw_valIsum;			// сюда суммируются измеренные значения тока через батарею
uint16_t hw_valUPowerSum;		// сюда суммируются измеренные значения напряжения на выходе источника питания

uint8_t hw_valUcnt;				// количество выполненных измерений напряжений на батареи
uint8_t hw_valIcnt;				// количество выполненных измерений тока через батарею
uint8_t hw_valUPowerCnt;		// количество выполненных измерений напряжений на выходе источника питания

uint16_t hw_valU, hw_valI, hw_valUpower;	// тут хранятся усредненные измеренные величины


uint16_t hw_StartMeassure(uint8_t pin);

// когда АЦП завершает очередное измерение, он устанавливает соответствующий бит (номер которого определяется константами ADC_xxx) в этой переменной
// главный поток производит пересчет измеренных величин в размерности тока и напряжения и очищает биты
volatile uint8_t hw_dataAvailFlags;

void hw_Init() {
	hw_valU = 0;
	hw_valI = 0;
	hw_valUpower = 0;
	hw_meassurePowerU = false;
	hw_dataAvailFlags = 0;
	hw_StartMeassure(ADC_U_OUT);
	
	// устанавливаем 0 на выходах ШИМ
	OCR1A = 0;
	OCR1B = 0;
	// конфигурация ШИМов
	TCCR1A = _BV(WGM11) | _BV(WGM10) | _BV(COM1A1) | _BV(COM1B1);
	TCCR1B = _BV(CS10);
	// фазо-корректная ШИМ, 10 бит, делитель частоты отключен
}


/************************************************************************/
/* Прерывание АЦП                                                       */
/************************************************************************/
ISR(ADC_vect) {
	uint8_t admux = ADMUX & 0b00011111;
	uint16_t val = ADC;

	switch (admux) {
		case ADC_U_OUT:
			hw_valUsum += val;
			hw_valUcnt++;
			if (hw_valUcnt >= HW_MEASSURE_COUNT) {
				hw_valU = hw_valUsum / hw_valUcnt;
				hw_dataAvailFlags |= _BV(ADC_U_OUT);
				hw_valUsum = 0;
				hw_valUcnt = 0;
			}
			hw_StartMeassure(ADC_I_OUT);
			break;
		case ADC_I_OUT:
			hw_valIsum += val;
			hw_valIcnt++;
			if (hw_valIcnt >= HW_MEASSURE_COUNT) {
				hw_valI = hw_valIsum / hw_valIcnt;
				hw_dataAvailFlags |= _BV(ADC_I_OUT);
				hw_valIsum = 0;
				hw_valIcnt = 0;
			}		
			hw_StartMeassure(hw_meassurePowerU ? ADC_U_POWER : ADC_U_OUT);
			break;
		case ADC_U_POWER:
			hw_valUPowerSum += val;
			hw_valUPowerCnt++;
			if (hw_valUPowerCnt >= HW_MEASSURE_COUNT) {
				hw_valUpower = hw_valUPowerSum / hw_valUPowerCnt;
				hw_dataAvailFlags |= _BV(ADC_U_POWER);
				hw_valUPowerSum = 0;
				hw_valUPowerCnt = 0;
			}
			hw_StartMeassure(ADC_U_OUT);
			break;
		default:
			hw_StartMeassure(ADC_U_OUT);
			break;
	}
}


/************************************************************************/
/* Запускает измерение на указанном канале                              */
/*		pin: ADC_xxx константа                                          */
/************************************************************************/
uint16_t hw_StartMeassure(uint8_t pin) {
	ADMUX = pin|_BV(REFS0)|_BV(REFS1);
	//		REFSx - опорный источник 2.56В
	ADCSRA = _BV(ADEN)|_BV(ADSC)|_BV(ADIE)|_BV(ADPS2)|_BV(ADPS1)|_BV(ADPS0);
	//		ADEN - включение АЦП
	//		ADSC - пуск измерения
	//		ADIE - разрешение прерывания
	//		ADPSx - делитель частоты = 128, время преобразования = 104 мкС
}


/************************************************************************/
/* Устанавливает значение на выводе ШИМ, регулирующим напряжение        */
/************************************************************************/
inline void hw_SetOutputU(uint16_t val) {
	OCR1A = val;
}


/************************************************************************/
/* Устанавливает значение на выводе ШИМ, регулирующим ток разряда       */
/************************************************************************/
inline void hw_SetOutputDischarge(uint16_t val) {
	OCR1B = val;
}


inline void hw_SetLed(uint8_t val) {
	PORT(LED_DATA_PORT)	&= ~(_BV(LED_PIN_RED) | _BV(LED_PIN_GREEN));
	PORT(LED_DATA_PORT) |= val;
}

#endif /* HARDWARE_H_ */