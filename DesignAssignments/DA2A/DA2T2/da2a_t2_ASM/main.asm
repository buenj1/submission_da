;
; da2a_t2_ASM.asm
;
; Created: 3/1/2019 10:55:22 PM
; Author : John Patrick
;

;DA2A Task 2 Assembly Code 

.include <m328pdef.inc>
.MACRO DELAY		;delay for ~1s
	ldi R21, 102	;load value 102 into R21
	ldi R22, 118	;load value 118 into R22
	ldi R23, 194	;load value 194 into R23
l1: 
	dec R23			;decrement R23
	brne l1			;if R23 != 0 loop back to l1
	dec R22			;decrement R22 
	brne l1			;if R22 != 0 loop back to l1 
	dec R21			;decrement R21
	brne l1			;if R21 != 0 loop back to l1
.ENDMACRO

.org 0
	cbi DDRC, 2		;enable PORTC.2 as input 
	sbi PORTC, 2	;enable pull up 
	sbi DDRB, 2		;enable PORTB.2 as output
	sbi PORTB, 2	;turn off LED 

loop4ever: 
	sbic PINC, 2	;if PINC is pulled low (switch is pressed down), turn on LED 
	jmp turnOFFLED	;else, go to turnOFFLED 
	cbi PORTB, 2	;turn on LED 
	DELAY			;perform a ~1s delay 
	jmp loop4ever	;loop back to loop4ever 

turnOFFLED: 
	sbi PORTB, 2	;if PINC is pulled high, turn off LED 
	jmp loop4ever	;loop back to loop4ever


