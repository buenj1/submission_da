;
; da2b_asmCode.asm
;
; Created: 3/7/2019 10:25:34 PM
; Author : John Patrick
;

.include <m328pdef.inc>
.MACRO DELAY			;delay for 1.25s 
    ldi  r18, @0
    ldi  r19, @1
    ldi  r20, @2
L1: dec  r20
    brne L1
    dec  r19
    brne L1
    dec  r18
    brne L1
.ENDMACRO 

.org 0 
	jmp main			;reset vector	

.org 0x02
	jmp EX0_ISR			;INT0 vector, jump to ISR routine 

main: 
	ldi R21, HIGH(RAMEND)
	out SPH, R21		;initialize the stack to store PC+1 addr 
	ldi R21, LOW(RAMEND) 
	out SPL, R21 
	
	sbi DDRB, 2			;set PB2 as output 
	sbi PORTD, 2		;activate pull-up for PD2
	sbi PORTB, 2		;turn LED off, initialize 
	ldi R21, 2 
	sts EICRA, R21		;activate IRQ on falling edge
	ldi R21, 1<<INT0	
	out EIMSK, R21		;enable INT0 ext interrupt 
	sei					;enable global interrupts 

whileLP: 
	jmp whileLP			;keep looping and do nothing until interrupt occurs 

EX0_ISR: 
	ldi R21, 0x04		;set PB2 
	in R22, PORTB		;load PORTB value in R22 
	eor R22, R21		;toggle LED 
	out PORTB, R22		;output onto PB2 
	DELAY 102, 118, 194	;call delay macro 
	reti				;return to next instruction (PC + 1)
