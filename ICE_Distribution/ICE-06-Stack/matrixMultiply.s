    export matrixMultiply
    AREA    FLASH, CODE, READONLY
    ALIGN
    
;******************************************************************************
; Description
; Parameters
; 1st Parameter - Result Address
; 2nd Parameter - Matrix A Address
; 3rd Parameter - Matrix B Address
;******************************************************************************
matrixMultiply   PROC
     
     ; Save registers modifed by this function
     ; <ADD CODE 5 >
	 PUSH {R0-R10}


     ; Load the 1st parameter from the stack into R2
     ; <ADD CODE  6 >
	 LDR R2, [SP, #44]

     
     ; Load the 2nd parameter from the stack into R0
	; <ADD CODE  7 >
	LDR R0, [SP, #48]

     
     ; Load the 3rd parameter from the stack into R1
     ; <ADD CODE  8 >
	 LDR R1, [SP, #52]

     
     LDRSB   R3, [R0, #0] ; R3  - A00
     LDRSB   R4, [R0, #1] ; R4  - A01
     LDRSB   R5, [R0, #2] ; R5  - A10
     LDRSB   R6, [R0, #3] ; R6  - A11
     
     LDRSB   R7, [R1, #0] ; R7  - B00
     LDRSB   R8, [R1, #1] ; R8  - B01
     LDRSB   R9, [R1, #2] ; R9  - B10
     LDRSB   R10,[R1, #3] ; R10 - B11
     
     ; After this point, we no longer use R0
     ; or R1, so we will use them in the calculations
     
     ; Calculate C00 = (A00 * B00) + (A01 * B10)
     MUL    R0, R3, R7
     MUL    R1, R4, R9
     ADD    R0, R1, R0
     
     STRB   R0, [R2, #0]
     
     ; Calculate C01 = (A00 * B01) + (A01 * B11)
     MUL    R0, R3, R8
     MUL    R1, R4, R10
     ADD    R0, R1, R0
     
     STRB   R0, [R2, #1]
     
     ; Calculate C10 = (A10 * B00) + (A11 * B10)
     MUL    R0, R5, R7
     MUL    R1, R6, R9
     ADD    R0, R1, R0
     
     STRB   R0, [R2, #2]
     
     ; Calculate C11 = (A10 * B01) + (A11 * B11)
     MUL    R0, R5, R8
     MUL    R1, R6, R10
     ADD    R0, R1, R0
     
     STRB   R0, [R2, #3]
    
     ; Restore any saved registers from the stack
     ; <ADD CODE  9 >
	 POP {R0-R10}
     
     BX     LR
     ENDP
     
    END     

