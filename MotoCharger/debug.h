#ifndef _DEBUG_H_
#define _DEBUG_H_

#include "config.h"

#if DEBUG

#include <stdint.h>
#include <avr/pgmspace.h>





#define UART_RATE_VALUE  0x340


void uart_init();

void uart_putc(uint8_t c);

void uart_putc_hex(uint8_t b);
void uart_putw_hex(uint16_t w);
void uart_putdw_hex(uint32_t dw);

void uart_putw_dec(uint16_t w);
void uart_putdw_dec(uint32_t dw);

void uart_puts(const char* str);
void uart_puts_p(PGM_P str);

uint8_t uart_getc();

void MSG_(char *str);

void MSG_HEX_(char *str, uint32_t val, uint8_t bytes);

void MSG_DEC_(char *str, uint32_t val);

#else
	inline void uart_init() {}
	inline void MSG_(char *str) {}
	inline void MSG_HEX(char *str, uint32_t val, uint8_t bytes) {}
	inline void MSG_DEC(char *str, uint32_t val) {}
	inline void MSG_HEX_(char *str, uint32_t val, uint8_t bytes) {}
	inline void MSG_DEC_(char *str, uint32_t val) {}
#endif // DEBUG




#define MSG(str)								MSG_(PSTR(str))
#define MSG_HEX(str, val, bytes)			MSG_HEX_(PSTR(str), (val), bytes)
#define MSG_DEC(str, val)					MSG_DEC_(PSTR(str), (val))


#endif // _DEBUG_H_
