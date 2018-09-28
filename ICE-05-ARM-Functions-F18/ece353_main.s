; Filename:     main.s 
; Author:       ece353 staff 
; Description:  

    export __main


; DEFINE the value required to check for 'WAIT'
WAIT_STR EQU 0x54494157

;**********************************************
; SRAM
;**********************************************
    AREA    SRAM, READWRITE
    align
        
;**********************************************
; Constant Variables (FLASH) Segment
;**********************************************
    AREA    FLASH, CODE, READONLY
MEM_BLK		DCB "WaIT WAIT1F00 WAIT0500"
    align


;**********************************************
; Check to see if an address holds the ASCII
; string 'WAIT'.
; 
; Parameters
; 	R0 :  Address to examine
; 
; Returns
;   R1 : 	0x00000000 if WAIT was found
;        	0xFFFFFFFF if WAIT was NOT FOUND 
;**********************************************
check_for_wait PROC
	
	; Load the 4 character string starting at R0
	; into a register
	LDR R2, [R0]
	
	; Load 'WAIT' to R3
	MOV32 R3, WAIT_STR

	; Check to see if the resulting value is 
	; equal to 'WAIT'
	CMP R2, R3
	
	; Set the return value
	MOVEQ R1, #0x00000000
	MOVNE R1, #0xFFFFFFFF
		
	; Return
	BX LR

	ENDP

;**********************************************
; Converts A single ASCII Decimal character to its 
; numerical value.  Valid characters are 0-9.
; 
; Parameters
; 	R5 : ASCII Char
;
; Returns
;	R1 : 	numerical value.  
;			If invalid, return 0xFFFFFFFF
;**********************************************
ascii_to_dec PROC
	
	SUB R2, R5, #0x30
	MOV R1, #0x00000000
	CMP R2, #0
	MOVLT R1, #0xFFFFFFFF
	CMP R2, #9
	MOVGT R1, #0xFFFFFFFF
	BX LR
	; Return
	
	ENDP
		
;**********************************************
; Code (FLASH) Segment
; main assembly program
;**********************************************
__main   PROC

	; Load the starting address of MEM_BLK
	LDR R0, =(MEM_BLK)

WAIT_LOOP_START

    ; Check the current address for 'WAIT'
	BL	check_for_wait
	
	; If WAIT was not found, advance the address 
	; by 1 and check again
	CMP R1, #0xFFFFFFFF
	ADDEQ R0, R0, #1
	BEQ WAIT_LOOP_START
	
	; If WAIT was found, check the next
	; four characters to see if each character
	; is an DECIMAL ASCII number.  
	
	; Initialize a loop count to 4
	MOV R3, #4
	
	; Advance the address past the WAIT string
	ADD R0, R0, #4

	; Stay in the loop until the loop count is 0.  If
	; the loop count gets to 0, branch to INFINITE_LOOP
	
DEC_LOOP_START

	CMP R3, #0
	BEQ INFINITE_LOOP
	
	; Load the next character
	LDRB R5, [R0]
	; Check to see if the next character is a DECIMAL
	; char.  If it is not, branch to WAIT_LOOP_START
	BL ascii_to_dec
	
	CMP R1, #0xFFFFFFFF
	BEQ WAIT_LOOP_START
	
	
	; decrement the loop count
	SUB R3, #1
	ADD R0, R0, #1

	B DEC_LOOP_START

INFINITE_LOOP
	B INFINITE_LOOP
	
    ENDP
    align
        

    END            