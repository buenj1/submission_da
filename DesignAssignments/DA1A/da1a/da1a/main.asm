;John Patrick Buen 
;NSHE: 2000900752
;RebelMail: buenj1@unlv.nevada.edu
;CPE 301: Design Assignment 1A 


.include <m328pdef.inc>
.SET num = 0xffff			;16-bit multiplicand
		
.ORG 00
	ldi R22, 0xff				;8-bit multiplier / counter
	ldi R24, LOW(num)		;low byte of 16-bit multiplicand  
	ldi R25, HIGH(num)		;high byte of 16-bit multiplicand 

	cpi R22, 0				;check if multiplier is 0
	breq mul0				;branch to mul0 if R22 is 0
	cpi R22, 1				;check if multiplier is 1
	breq done 				;branch to done if equal 

	cpi R24, 0				;check if low-byte of multiplicand is 0
	breq chkR25				;jump to chkR25 to check high-byte
	jmp initialize			;jump to initialize 

chkR25: 
	cpi R25, 0				;check if high-byte multiplicand is 0
	breq mul0				;if equal, branch to mul0
	

initialize: 
	dec R22					;decrement R22, not counting/multiplying with 1
	mov R16, R24			;fixed adder for low byte  
	mov R17, R25			;fixed adder for high byte 
	ldi R23, 0				;fixed 0 to add with R20 in case of a carry bit when adding R25 

lp1: 
	adc R24, R16			;add with carry for low byte of multiplicand 
	adc R25, R17			;add with carry for high byte of multiplicand 
	adc R20, R23			;add with carry for R20 in case there is a carry bit when adding the high byte (R25)
	dec R22					;decrement the multiplier 
	breq done				;if R22 is 0, branch to done 
	jmp lp1					;else loop back to lp1

done: 	
	movw R19:R18, R25:R24	;store 16-bit multiplicand to R19:R18
	jmp terminate			;jump to terminate to end the program
	 
mul0:
	ldi R18, 0				;store 0 in R18 if multiplier or low byte is 0
	ldi R19, 0				;store 0 in R19 if multiplier or high byte is 0

	
	 
terminate: 

	jmp terminate			;infinite loop to terminate to stop executing the program
	