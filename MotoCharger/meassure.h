/*
 * meassure.h
 *
 * Created: 08.10.2014 21:03:03
 *  Author: Trol
 */ 


#ifndef MEASSURE_H_
#define MEASSURE_H_

// измеренное напр€жение на выходе устройства
uint16_t meassure_outU;

// измеренное напр€жение на выходе внутреннего источника питани€
uint16_t meassure_powerU;

// измеренный ток
uint16_t meassure_outI;

// пересчитывает показани€ ј÷ѕ в напр€жение на выходе устройства
void meassure_calcOutU(uint16_t adcVal) {
	// TODO заглушка
	meassure_outU = adcVal;	
}

// пересчитывает показани€ ј÷ѕ в напр€жение на выходе внутреннего источника питани€
void meassure_calcPowerU(uint16_t adcVal) {
	// TODO заглушка
	meassure_powerU = adcVal;
}


// пересчитывает показани€ ј÷ѕ в значение выходного тока
void meassure_calcOutI(uint16_t adcVal) {
	// TODO заглушка
	meassure_outI = adcVal;
}



#endif /* MEASSURE_H_ */