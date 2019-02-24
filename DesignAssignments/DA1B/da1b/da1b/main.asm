;John Patrick Buen 
;NSHE: 2000900752 
;Design Assignment 1B 
;CPE 301 

.include <m328pdef.inc>

.MACRO DIV							;macro for dividing numbers
	mov R20, @0						;dividend
	ldi R21, @1						;divisor 
	mov R22, @2						;remainder
l1: 
	sub R20, R21					;subtract dividend with divisor 
	brcc l1							;if carry is cleared, branch back to l1
	mov R22, R20					;else, store R20 into R22 
	add R22, R21					;add R22 with R21 to get the remainder 
.ENDMACRO 

.EQU STARTADDS = 0x0200				;populate memory with 99 numbers starting at addr 0x200
	ldi R20, 99						;counter 
	ldi R21, 11						;number that will be stored and incremented 
	ldi XL, low(STARTADDS)			;store low byte of addr at X register
	ldi XH, high(STARTADDS)			;store high byte of addr at X register 

populateMEM: 
	cpi R20, 0						;compare counter with 0
	breq chkDivBy3					;if counter is 0, storing of 99 numbers is complete, branch to chkDivBy3
	st X+, R21						;else, keep storing numbers in memory locations
	inc R21							;increment number to be stored next 
	dec R20							;decrement counter 
	jmp populateMEM					;loop back to populateMEM

chkDivBy3: 
	ldi XL, low(STARTADDS)			;initialize low byte of STARTADDS for reading
	ldi XH, high(STARTADDS)			;initialize high byte of STARTADDS for reading 
	ldi YL, low(0x0400)				;initialize low byte of 0x0400 to store div by 3 numbers 
	ldi YH, high(0x0400)			;initialize high byte of 0x0400 to store div by 3 numbers 
	ldi ZL, low(0x0600)				;initialize low byte of 0x0600 to store non-div by 3 numbers 
	ldi ZH, high(0x0600)			;initialize high byte of 0x0600 to store non-div by 3 numbers 
	
	ldi R24, 99						;initialize counter again
	ldi R16, 0						;running sum for div by 3 numbers (low byte) 
	ldi R17, 0						;running sum for div by 3 numbers (high byte) 
	ldi R18, 0						;running sum for non-div by 3 numbers (low byte) 
	ldi R19, 0						;running sum for non-div by 3 numbers (high byte)
	ldi R25, 0						;high byte add with 0 when there is a carry from low byte

divBy3:
	cpi R24, 0						;compare counter with 0
	breq done						;if counter is 0, checking numbers if div by 3 is finished, branch to done
	ld R20, X+						;else, load number to R20 from memory location, then increment memory location
	ldi R22, 0						;initialize remainder 
	mov R23, R20					;save X value 
	DIV R20, 3, R22					;call DIV macro 
	cpi R22, 0						;compare if remainder is 0
	brne notDivBy3					;if remainder is not 0, number is not div by 3, branch to notDivBy3
	st Y+, R23						;else, number is divisible, store starting from mem location 0x400
	add R16, R23					;running sum of div by 3 numbers (low byte)
	adc R17, R25					;if there is a carry, add with carry and with 0
	dec R24							;decrement counter 
	jmp divBy3						;loop back to divBy3

notDivBy3:   
	st Z+, R23						;store non div by 3 number into mem location starting from add 0x600
	add R18, R23					;running sum of non div by 3 numbers 
	adc R19, R25					;if there is a carry, add with carry and with 0
	dec R24							;decrement counter 
	jmp divBy3						;loop back to divBy3

done:
	jmp done						;infinite loop to done to stop executing program