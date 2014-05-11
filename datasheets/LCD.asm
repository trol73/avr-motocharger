.nolist
.include "m8def.inc"
.list

;	кварц 16МГц, TIC107 -  Reset = PORTC3  
;	TWI-interface (PORTC5,PORTC4)
;	светодиод ошибки = PORTD,0
;

.CSEG 
.ORG 0x00
	rjmp 	begin
	reti
	reti
	reti
	reti
	reti
	reti
	reti
	reti
	reti
	reti
	reti
	reti
	reti
	reti
	reti
	reti
	reti
	reti		
	reti	
pause:	clr	R16		;R17x0.1ms
c1:	dec	R16
	nop
	nop
	nop
	brne	c1
	dec	R17
	brne	c1
	ret	
start:	ldi r16, (1<<TWINT)|(1<<TWSTA)|(1<<TWEN)
	out TWCR, r16	;Send START
wait1:	in r16,TWCR
	sbrs r16,TWINT
	rjmp wait1		;Wait for TWINT Flag set
	in r16,TWSR	;Check value of TWI Status Register
	andi r16, 0xF8	;Mask prescaler bits
	cpi r16, 0x08	;If status different from START goto ERROR
	brne ERROR	
	ret

address:	out TWDR, r16	;Load R16 into TWDR Register
	ldi r16, (1<<TWINT) | (1<<TWEN)	;Clear TWINT bit in TWCR to start
	out TWCR, r16			;transmission of address
wait2:	in r16,TWCR	;Wait for TWINT Flag set. This indicates that the SLA+W has
	sbrs r16,TWINT	; been transmitted, and ACK/NACK has been received
	rjmp wait2
	in r16,TWSR	;Check value of TWI Status Register, Mask prescaler bits
	andi r16, 0xF8	;. If status different from MT_SLA_ACK goto ERROR
	cpi r16, 0x18
	brne ERROR
	ret

data:	out TWDR, r16	;Load DATA into TWDR Register
	ldi r16, (1<<TWINT) | (1<<TWEN)	;Clear TWINT bit in TWCR to start
	out TWCR, r16			;transmission of data
wait3:	in r16,TWCR	;Wait for TWINT Flag set. This indicates that the DATA has
	sbrs r16,TWINT	; been transmitted, and ACK/NACK has been received.
	rjmp wait3
	in r16,TWSR	;Check value of TWI Status Register. Mask prescaler bits
	andi r16, 0xF8	; If status different from MT_DATA_ACK goto ERROR
	cpi r16, 0x28
	brne ERROR
	ret
stop:	ldi r16, (1<<TWINT)|(1<<TWEN)|(1<<TWSTO)
	out TWCR, r16	;Transmit STOP
	ret
error:	sbi	PORTD,0
	rjmp error

begin:	ldi 	R16,high(RAMEND)
	out	SPH,R16 
	ldi	R16,low(RAMEND)
	out 	SPL,R16
	ldi	R16,1
	out	DDRD,R16
	cbi	PORTD,0
	ldi	R16,8
	out	DDRC,R16
	sbi	PORTC,3		;reset
	sbi	TWSR,TWPS0
	sbi	TWSR,TWPS1
	ldi	R16,0x45		;configuring TWI
	out	TWCR,R16
	ldi	R16,255
	out	TWBR,R16
	clr	R17
	inc	R17
	rcall	pause		;pause 0.1ms
	cbi	PORTC,3
	ldi	R17,100
	rcall	pause		;pause 10ms
	ldi	R16,0x00
	out	GICR,R16
	cli			;disable interrupts
	rcall	start		;start TWSTA=1
	ldi	R16,0x74		;address TIC107
	rcall	address
	ldi	ZL, 0		;data pointer at 0x0200
	ldi	ZH, 2
m0:	lpm	R16,Z+		;R16 - sending byte
	inc	R16
	breq	end1		;if byte=FF then stop
	dec	R16
	rcall	data
	rjmp	m0
end1:	rcall	stop		;TWSTO=1
	ldi	R17,100
	rcall	pause		;pause 10ms
	rcall	start		;резервные опции
	ldi	R16,0x74		;address TIC107
	rcall	address
	ldi	ZL, 16		;data pointer at 0x0210
	ldi	ZH, 2
m1:	lpm	R16,Z+		;R16 - sending byte
	inc	R16
	breq	end2		;if byte=FF then stop
	dec	R16
	rcall	data
	rjmp	m1
end2:	rcall	stop		;TWSTO=1
	ldi	R17,100
	rcall	pause		;pause 10ms
	rcall	start		;text "  "
	ldi	R16,0x74		;address TIC107
	rcall	address
	ldi	ZL, 32		;data pointer at 0x0220
	ldi	ZH, 2
m2:	lpm	R16,Z+		;R16 - sending byte
	inc	R16
	breq	end3		;if byte=FF then stop
	dec	R16
	rcall	data
	rjmp	m2
end3:	rcall	stop		;TWSTO=1	
end:	rjmp	end


.ORG 256
.db	0,0x34,0x0C,6,0x35,3,4,8,0x10,0x42,0x093,255,255	;3-я позиция с конца - контраст
.ORG 264
.db	0,0x0FF,0x0FF,255,255	;reserved для доп. опций
.ORG 272
.db	0x40,,255	; text коды текста лучше набивать в НЕХ-редакторах


