;
; da2a_t1_ASM.asm
;
; Created: 3/2/2019 4:43:38 PM
; Author : John Patrick
;
.include <m328pdef.inc>
.MACRO DELAY		;delay function respective to the 60% duty cycle 
	ldi R18, @0
	ldi R19, @1
	ldi R20, @2
l1: 
	dec R20			;dec by R20 value 
	brne l1			;if not 0, go to l1
	dec R19			;dec by R19 value 
	brne l1			;if not 0, go to l1
	dec R18			;dec by R18 value 
	brne l1			;if not 0, go to l1
	nop				;do nothing
.ENDMACRO 

.org 0
	sbi DDRB, 2		;enable PORTB.2 as output 
	sbi PORTB, 2	;turn off LED first 

loop: 
	sbis PORTB, 2		;if LED is off, delay for 290ms 
	jmp toggleLEDon		;else if LED is on...
	DELAY 24, 138, 232	;this will delay for 290ms 
	cbi PORTB, 2		;turn LED on (toggle)
	jmp loop			;loop back to 'loop'

toggleLEDon: 
	DELAY 36, 79, 221	;LED that is on will delay for 435ms 
	sbi PORTB, 2		;turn LED off (toggle)
	jmp loop			;loop back to 'loop'


