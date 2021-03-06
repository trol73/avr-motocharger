
#define DEBUG									1

#define F_CPU		16000000


#define KEYBOARD_DATA_PORT					B			// порт линий данных клавиатуры

#define KEYBOARD_PIN_UP			7
#define KEYBOARD_PIN_DOWN		5
#define KEYBOARD_PIN_ENTER		4
#define KEYBOARD_PIN_BACK		6


#define LED_DATA_PORT			C

#define LED_PIN_RED				4
#define LED_PIN_GREEN			3

#define BEEPER_PORT				D
#define BEEPER_PIN				6

#define ADC_U_OUT				0		// вход АЦП для измерения выходного напряжения		
#define ADC_I_OUT				1		// вход АЦП для измерения тока
#define ADC_U_POWER				2		// вход АЦП для измерения выходного напряжения источника питания