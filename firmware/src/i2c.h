/*
 * i2c.h
 *
 *  Created on: Nov 2, 2012
 *      Author: trol
 */

#ifndef I2C_H_
#define I2C_H_


#include <util/twi.h>
#include <avr/io.h>
#include <stdbool.h>

#include "debug.h"



// defines the data direction (reading from I2C device) in i2c_start(), i2c_rep_start()
#define I2C_READ    1

// defines the data direction (writing to I2C device) in i2c_start(), i2c_rep_start()
#define I2C_WRITE   0

// I2C clock in Hz
#define SCL_CLOCK  100000L



static void i2c_init();
static bool i2c_sendAddress(uint8_t address);
static bool i2c_start();


static void i2c_init() {
	TWSR = 0;							// no prescaler
	TWBR = ((F_CPU/SCL_CLOCK)-16)/2;	// must be > 10 for stable operation
}

static void i2c_wait() {
	// wait until transmission completed
	uint16_t cnt = 0;
	while ( !(TWCR & _BV(TWINT)) ) {
		if ( --cnt == 0 ) {
			MSG("i2c_wait TIMEOUT!");
			break;
		}
	}
}


static bool i2c_start() {
	TWCR = _BV(TWINT)|_BV(TWEN)|_BV(TWSTA);	// send START condition
	i2c_wait();
	return TW_STATUS == TW_START;
}


static bool i2c_sendAddress(uint8_t address) {
	uint8_t status;

	if ( (address & 0x01) == 0 ) {
		status = TW_MT_SLA_ACK;
	} else {
		status = TW_MR_SLA_ACK;
	}

	TWDR = address;
	TWCR = _BV(TWINT)|_BV(TWEN);
	i2c_wait();
	return (TWSR & 0xF8) == status;
}


static bool i2c_sendByte(uint8_t byte) {
	TWDR = byte;
	TWCR = _BV(TWINT)| _BV(TWEN);
	i2c_wait();
	return (TWSR & 0xF8) == TW_MT_DATA_ACK;
}


static bool i2c_receiveByteACK(uint8_t *byte) {
	TWCR = _BV(TWEA)|_BV(TWINT)|_BV(TWEN);
	i2c_wait();
	bool result = (TWSR & 0xF8) == TW_MR_DATA_ACK;
	*byte = TWDR;
	return result;
}


static bool i2c_receiveByteNACK(uint8_t *byte) {
	TWCR =_BV(TWINT)|_BV(TWEN);
	i2c_wait();
	bool result = (TWSR & 0xF8) == TW_MR_DATA_NACK;
	*byte = TWDR;
	return result;
}


static bool i2c_stop() {
	TWCR = _BV(TWINT)|_BV(TWEN)|_BV(TWSTO); // send STOP condition
	uint16_t cnt = 0;
	while ( TWCR & _BV(TWSTO) ) {
		if ( --cnt == 0 ) {
			MSG("i2c_stop TIMEOUT!");
			return false;
		}
	}
	return true;
}



#endif /* I2C_H_ */
