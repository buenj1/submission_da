;John Patrick Buen 
;NSHE: 2000900752
;Email: buenj1@unlv.nevada.edu
;Github Address: 
;Directory:  

.include <m328pdef.inc>
.SET num = 0x0002			;16-bit multiplicand stored in R25:R24
		
.ORG 00
	ldi R22, 1				;8-bit multiplier / counter
	ldi R24, LOW(num)		;16-bit... 
	ldi R25, HIGH(num)		;Multiplicand  

	cpi R22, 0				;check if multiplier is 0
	breq mul0				;branch to mul0 if equal
	cpi R22, 1				;check if multiplier is 1
	breq done 				;branch to done if equal 

	cpi R24, 0				;check if low-byte multiplicand is 0
	breq chkR25				;jump to chkR25 to check high-byte
	jmp initialize			;unconditional jump to initialize 

chkR25: 
	cpi R25, 0				;check if high-byte multiplicand is 0
	breq mul0				;if equal, branch to mul0
	

initialize: 
	dec R22					;not counting/multiplying with 1
	mov R16, R24			;fixed adder for low byte  
	mov R17, R25			;fixed adder for high byte 
	
lp1: 
	add R24, R16 
	brcs addCarry 
	add R25, R17
	brcs addCarry2
	dec R22 
	breq done
	jmp lp1

addCarry: 
	adc R25, R17
	brcs addCarry2
	dec R22
	breq done 
	jmp lp1

addCarry2: 
	adc R23, R15 
	dec R22
	breq done 
	jmp lp1
	
done: 	
	movw R19:R18, R25:R24
	mov R20, R23
	jmp terminate 
	 
mul0:
	ldi R18, 0
	ldi R19, 0

	
	 
terminate: 
	;program is finished 
	