; hw1.s ECE353


    export hw1_search_memory
	import WS2812B_write

; String constants		
WORD	EQU	4
HALF	EQU	2
BYTE	EQU 1

; String constansts to check for commands
INIT_STR EQU 0x54494E49
WAIT_STR EQU 0x54494157
STOP_STR EQU 0x504F5453
LED_STR EQU	0x0044454C
	
;**********************************************
; SRAM
;**********************************************
    AREA    SRAM, READWRITE
LED_ARRAY SPACE BYTE*24	; 3 byte color of each of the 8 LEDS
    align
		
;**********************************************
; Constant Variables (FLASH) Segment
;**********************************************
    AREA    FLASH, CODE, READONLY
		
;**********************************************
; Updates the WS2812B LEDs with the current values in the LED_ARRAY. Use the
; supplied function WS2812B_write to update the colors. 
; 
; Parameters
;	NONE
;
; Return
;	Nothing
;**********************************************	
hw1_update_leds	PROC
	PUSH {R1, R2, LR}
	
	LDR R1, =(LED_ARRAY)
	MOV R2, #8
	BL WS2812B_write 
	
	POP {R1, R2, LR}
	
	BX LR
	ENDP
		
;**********************************************
; Converts A ASCII HEX character to its 
; numerical value.  Valid characters are 0-9,a-f,A-F.
; 
; Parameters
; 	R0 : ASCII Char
;
; Returns
;	R0 : 	numerical value.  
;			If invalid, return 0xFFFFFFFF
;**********************************************		
hw1_ascii_to_hex	PROC
	PUSH {R2}
	
	; Check for 0-9
	SUB R2, R0, #0x30
	CMP R2, #0
	MOVLT R2, #0xFFFFFFFF
	CMP R2, #9
	MOVGT R2, #0xFFFFFFFF
	CMP R2, #0xFFFFFFFF
	BNE DONE
	
	; Check for a-f
	SUB R2, R0, #0x61
	CMP R2, #0
	MOVLT R2, #0xFFFFFFFF
	CMP R2, #5
	MOVGT R2, #0xFFFFFFFF
	CMP R2, #0xFFFFFFFF
	ADDNE R2, #10
	BNE DONE
	
	; check for A-F
	SUB R2, R0, #0x41
	CMP R2, #0
	MOVLT R2, #0xFFFFFFFF
	CMP R2, #5
	MOVGT R2, #0xFFFFFFFF
	CMP R2, #0xFFFFFFFF
	ADDNE R2, #10
	BNE DONE
		
DONE MOV R0, R2 
	POP {R2}
	
	BX LR	
	ENDP

;**********************************************
; Converts A single ASCII Decimal character to its 
; numerical value.  Valid characters are 0-9.
; 
; Parameters
; 	R0 : ASCII Char
;
; Returns
;	R0 : 	numerical value.  
;			If invalid, return 0xFFFFFFFF
;**********************************************
hw1_ascii_to_dec PROC
	
	PUSH {R2}
	
	SUB R0, R0, #0x30
	CMP R0, #0
	MOVLT R0, #0xFFFFFFFF
	CMP R0, #9
	MOVGT R0, #0xFFFFFFFF
	
	POP {R2}
	
	BX LR	
	ENDP

;**********************************************
; Turns off all of the WSB28128 by writing 
; 0x000000 to each LED
;
; Parameters
;	NONE
;
; Return
;	Nothing
;**********************************************
hw1_init	PROC
	PUSH {R0, R1, LR}
	
	LDR R0, =(LED_ARRAY)
	MOV32 R1, #0
	; initialyze 4 bytes of LED_ARRAY 6 TIMES
	STR R1, [R0], #WORD
	STR R1, [R0], #WORD
	STR R1, [R0], #WORD
	STR R1, [R0], #WORD
	STR R1, [R0], #WORD
	STR R1, [R0], #WORD
	
	BL hw1_update_leds
	POP {R0, R1, LR}
	
	BX LR
	ENDP
	
		
;**********************************************
; Updates the color of the specified LED. 
; All other LEDs should maintain their current
; color. Makes use of the hw1_update_leds to 
; update the color of the LEDs.
;
; Parameters
;	R0 – LED number to be updated
;	R1 – An unsigned 32bit number. Bits 31-24 are unused. 
;	Bits 23-0 represent the color value to write to the LED.
;
; Return
;	Nothing
;**********************************************	
hw1_ledx	PROC
	
	PUSH {R1-R3, LR}
	; R2 Stores stores the adress of LED_ARRAY
	LDR R2, =(LED_ARRAY)
	MOV R3, #3
	; offset from LED_ARRAY
	MUL R3, R3, R0
	ADD R2, R2, R3
	; set bits 0-15
	STRH R1, [R2], #HALF
	; set bits 16-23
	LSR R1, R1, #HALF*8
	STRB R1, [R2]
	
	; update colors of led
	BL hw1_update_leds
	
	POP {R1-R3, LR}
	
	BX LR
	ENDP
		
	
; check with unsigned 
;**********************************************
; Delays the examination of the next memory address 
; by a variable amount of time.
;
; Parameters
;	R0 – A 32-bit unsigned number representing the number of iterations 
;	of an empty for loop that must be executed.
;
; Return
;	Nothing
;**********************************************	
hw1_wait	PROC
	PUSH {R1}
	MOV R1, R0
WAIT_LOOP CMP R1, #0
	SUB R1, R1, #1
	BGT WAIT_LOOP
	POP {R1}
	BX LR
	ENDP
		
;**********************************************
; Check to see if an address holds the Command
; 'INIT'
; 
; Parameters
; 	R0 :  Address to examine
; 
; Returns
;   R1 : 	0x00000000 if INIT was found
;        	0xFFFFFFFF if INIT was NOT FOUND 
;**********************************************
check_for_init PROC
	
	PUSH {R2, R3}
	
	; Load the 4 character string starting at R0
	; into a register
	LDR R2, [R0]

	; Load 'INIT' to R3
	MOV32 R3, INIT_STR

	; Check to see if the resulting value is 
	; equal to 'INIT'
	CMP R2, R3
	
	; Set the return value
	MOVEQ R1, #0x00000000
	MOVNE R1, #0xFFFFFFFF
	
	POP {R2, R3}
		
	; Return
	BX LR

	ENDP
		
;**********************************************
; Check to see if an address holds the Command
; 'STOP' in which the program should enter
; infinite loop
; 
; Parameters
; 	R0 :  Address to examine
; 
; Returns
;   R1 : 	0x00000000 if STOP was found
;        	0xFFFFFFFF if STOP was NOT FOUND 
;**********************************************
check_for_stop PROC
	
	PUSH {R2, R3}
	
	; Load the 4 character string starting at R0
	; into a register
	LDR R2, [R0]

	; Load 'INIT' to R3
	MOV32 R3, STOP_STR

	; Check to see if the resulting value is 
	; equal to 'INIT'
	CMP R2, R3
	
	; Set the return value
	MOVEQ R1, #0x00000000
	MOVNE R1, #0xFFFFFFFF
	
	POP {R2, R3}
		
	; Return
	BX LR

	ENDP

;**********************************************
; Check to see if an address holds the Command
; 'WAIT(4-digit-decimal)' in which the program sould execute
; 4-digit-decimal*10,000 iterations of empty loop
; 
; Parameters
; 	R0 :  Address to examine
; 
; Returns
;   R1 : 	4-digit-demimal in hex 
;        	0xFFFFFFFF if WAIT was NOT FOUND or invalid number
;**********************************************
check_for_wait PROC
	
	PUSH {R0, R2-R5, LR}
	
	; Load the 4 character string starting at R0
	; into a register
	LDR R2, [R0]

	; Load 'WAIT' to R3
	MOV32 R3, WAIT_STR

	; Check to see if the resulting value is 
	; equal to 'WAIT'
	CMP R2, R3
	
	; Set the return value
	MOVNE R1, #0xFFFFFFFF
	BLNE FINISH_WAIT
	
	; Check to see if there is 4-digit decimal after 'WAIT'
	; R2 Stores address of number
	ADD R2, R0, #BYTE*4
	; R4 Stores counts of loop iteration
	MOV R4, #4
	; R1 Stores decimal value
	MOV R1, #0
LOOP_WAIT
	CMP R4, #0
	BEQ FINISH_WAIT
	LDRB R0, [R2], #1
	BL hw1_ascii_to_dec
	CMP R0, #0xFFFFFFFF
	; If it is not a valid number
	MOVEQ R1, #0xFFFFFFFF
	BEQ FINISH_WAIT
	; If it is a valid number
	MOV R5, #10
	MUL R1, R1, R5
	ADD R1, R1, R0
	SUB R4, R4, #1
	B LOOP_WAIT
	
FINISH_WAIT	POP {R0, R2-R5, LR}
	BX LR

	ENDP
		
;**********************************************
; Check to see if an address holds the Command
; LEDx, for example (‘LED2FF8800') in which the program should change the 
; color of LED numbered 2 and to color FF8800
; 
; Parameters
; 	R0 :  Address to examine
; 
; Returns
;   R1 : 	number of the led
;        	0xFFFFFFFF if LEDx was NOT FOUND or invalid led number or invalid color number
;	R3 :	color to be changed
;
;**********************************************
check_for_ledx PROC
	
	PUSH {R0, R2, R4-R5, LR}
	
	; Load the 4 character string starting at R0
	; into a register
	LDR R2, [R0]
	AND R2, R2, #0x00FFFFFF

	; Load 'LED' to R3
	MOV32 R3, LED_STR

	; Check to see if the resulting value is 
	; equal to 'LED'
	CMP R2, R3
	
	; Set the return value
	MOVNE R1, #0xFFFFFFFF
	BLNE FINISH_LED
	
	; R2 Stores address of LEDx	
	MOV R2, R0
	
	; Check to see if there is valid led number after 'LED'
	LDRB R0, [R2, #BYTE*3]
	BL hw1_ascii_to_dec
	CMP R0, #0xFFFFFFFF
	; If it is not a valid LED number
	MOVEQ R1, #0xFFFFFFFF
	BEQ FINISH_LED
	; If it is a valid LED number
	MOV R1, R0
	
	; Check to see if there is valid hex color decimal after 'LEDx'
	; R2 Stores address of Color hex
	ADD R2, R2, #BYTE*9
	; R4 Stores counts of loop iteration
	MOV R4, #6
	; R3 Stores hex value
	MOV R3, #0
LOOP_LED
	CMP R4, #0
	BEQ FINISH_LED
	LDRB R0, [R2], #-1
	BL hw1_ascii_to_hex
	CMP R0, #0xFFFFFFFF
	; If it is not a valid hex
	MOVEQ R1, #0xFFFFFFFF
	BEQ FINISH_LED
	; If it is a valid hex
	MOV R5, #16
	MUL R3, R3, R5
	ADD R3, R3, R0
	SUB R4, R4, #1
	B LOOP_LED
	
FINISH_LED	POP {R0, R2, R4-R5, LR}
	BX LR

	ENDP

;**********************************************
; This function will search through memory a byte at a time looking for valid DISPLAY
; commands. When a valid DISPLAY command is found, carry out the corresponding
; behavior. This function will examine memory until and STOP command
; is found.
; Parameters
;	R0 : The starting address of the memory that contains the commands to update the
;	LEDs.
;
; Return
;	Nothing
;**********************************************	
hw1_search_memory PROC
	
	PUSH {R0-R3, LR}
	
	; R2 stores the current address of the command
	MOV R2, R0
	
	; check for STOP
LOOP	MOV R0, R2
	BL check_for_stop
	CMP R1, #0
	BEQ STOP
	
	; check for INIT
	MOV R0, R2
	BL check_for_init
	CMP R1, #0
	BLEQ hw1_init
	
	; check for WAIT
	MOV R0, R2
	BL check_for_wait
	CMP R1, #0xFFFFFFFF
	MOVNE R0, R1
	MOVNE R3, #10000
	MULNE R0, R0, R3
	BLNE hw1_wait
	
	; check for LEDx
	MOV R0, R2
	BL check_for_ledx
	CMP R1, #0xFFFFFFFF
	MOVNE R0, R1
	MOVNE R1, R3
	BLNE hw1_ledx
	
	; Increment the current address of the command
	ADD R2, R2, #1
	
	B LOOP
	

STOP
	B STOP
	
	POP {R0-R3, LR}
	
	BX LR
	ENDP
		
    align        
    
    END



