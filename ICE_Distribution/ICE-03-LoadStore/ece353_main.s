; Filename:     main.s 
; Author:       ece353 staff 
; Description:  

    export __main

;**********************************************
; Add Symbolic Constants here
;**********************************************
BYTE      EQU 1
HALF_WORD EQU 2
WORD      EQU 4

;**********************************************
; SRAM
;**********************************************
    AREA    SRAM, READWRITE
ARRAY1	SPACE 8*HALF_WORD
ARRAY2	SPACE 8*HALF_WORD
    align
        
;**********************************************
; Constant Variables (FLASH) Segment
;**********************************************
    AREA    FLASH, CODE, READONLY
LTABLE	DCW	0
		DCW	1
		DCW 8
		DCW 27
		DCW 64
		DCW 125
		DCW 216
		DCW 343
    align

;**********************************************
; Code (FLASH) Segment
; main assembly program
;**********************************************
__main   PROC
	
	ADR	R0, LTABLE
	LDR R1, =(ARRAY1)
	LDR R2, =(ARRAY2)
	
	LDR R10, [R0, #12]

	LDRH R3, [R0, #0] 
	STRH R3, [R1, #0]
	LDRH R3, [R0, #2] 
	STRH R3, [R1, #2]
	LDRH R3, [R0, #4] 
	STRH R3, [R1, #4]
	LDRH R3, [R0, #6] 
	STRH R3, [R1, #6]
	LDRH R3, [R0, #8] 
	STRH R3, [R1, #8]
	LDRH R3, [R0, #10] 
	STRH R3, [R1, #10]
	LDRH R3, [R0, #12] 
	STRH R3, [R1, #12]
	LDRH R3, [R0, #14] 
	STRH R3, [R1, #14]
	
	
	LDRH R3, [R0], #2
	STRH R3, [R2], #2
	LDRH R3, [R0], #2
	STRH R3, [R2], #2
	LDRH R3, [R0], #2
	STRH R3, [R2], #2
	LDRH R3, [R0], #2
	STRH R3, [R2], #2
	LDRH R3, [R0], #2
	STRH R3, [R2], #2
	LDRH R3, [R0], #2
	STRH R3, [R2], #2
	LDRH R3, [R0], #2
	STRH R3, [R2], #2
	LDRH R3, [R0], #2
	STRH R3, [R2], #2
	


    
    ; DO NOT MODIFY ANTHING BELOW THIS LINE!!!	
        
INFINITE_LOOP
    B INFINITE_LOOP
    
    ENDP
    align
        

    END            