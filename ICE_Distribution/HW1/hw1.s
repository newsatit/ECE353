; hw1.s ECE353


    export hw1_search_memory
	import WS2812B_write

; String constants		
WORD	EQU	4
HALF	EQU	2
BYTE	EQU 1	
	
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
ascii_to_dec PROC
	
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
	PUSH {R0, R1}
	
	LDR R0, =(LED_ARRAY)
	MOV32 R1, #0
	; initialyze 4 bytes of LED_ARRAY 6 TIMES
	STR R1, [R0], #WORD
	STR R1, [R0], #WORD
	STR R1, [R0], #WORD
	STR R1, [R0], #WORD
	STR R1, [R0], #WORD
	STR R1, [R0], #WORD
	
	POP {R0, R1}
	
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
	
	PUSH {R1-R3}
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
	
	POP {R1-R3}
	
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
LOOP CMP R1, #0
	SUB R1, R1, #1
	BGT LOOP
	POP {R1}
	BX LR
	ENDP

; TODO

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
	PUSH {R1}
	
	MOV R0, #0x2F	; false
	BL hw1_ascii_to_hex
	MOV R0, #0x30	; 0
	BL hw1_ascii_to_hex
	MOV R0, #0x31	; 1
	BL hw1_ascii_to_hex
	MOV R0, #0x38	; 8
	BL hw1_ascii_to_hex	
	MOV R0, #0x39	; 9
	BL hw1_ascii_to_hex
	MOV R0, #0x3A	; False
	BL hw1_ascii_to_hex
	MOV R0, #0x40	; False
	BL hw1_ascii_to_hex
	MOV R0, #0x41	; 10
	BL hw1_ascii_to_hex
	MOV R0, #0x46	; 15
	BL hw1_ascii_to_hex
	MOV R0, #0x47	; false
	BL hw1_ascii_to_hex
	MOV R0, #0x60	; false
	BL hw1_ascii_to_hex
	MOV R0, #0x61	; 10
	BL hw1_ascii_to_hex
	MOV R0, #0x66	; 15
	BL hw1_ascii_to_hex
	MOV R0, #0x67	; false
	BL hw1_ascii_to_hex

	BL hw1_init
	BL hw1_update_leds

	MOV32 R0, #1000
	MOV32 R1, #10000
	MUL R0, R0, R1
	BL hw1_wait
		
	MOV R0, #1
	MOV32 R1, #0x00FF0000
	BL hw1_ledx
	BL hw1_update_leds
	
	MOV32 R0, #1000
	MOV32 R1, #10000
	MUL R0, R0, R1
	BL hw1_wait
	
	MOV R0, #7
	MOV32 R1, #0x00FFFF00
	BL hw1_ledx
	BL hw1_update_leds
	
	MOV32 R0, #1000
	MOV32 R1, #10000
	MUL R0, R0, R1
	BL hw1_wait
	
	BL hw1_init
	BL hw1_update_leds
	
	MOV32 R0, #1000
	MOV32 R1, #10000
	MUL R0, R0, R1
	BL hw1_wait
	
	POP {R1}

INFINITE_LOOP
	B INFINITE_LOOP

	BX LR
	ENDP
		
    align        
    
    END



