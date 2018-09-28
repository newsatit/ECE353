    export verifyArray
    AREA    FLASH, CODE, READONLY
    ALIGN
    
;******************************************************************************
; Description
; Uses Bubble Sort to sort an array of unsigned 8-bit numbers.
; Parameters
; R0 - User Sorted Array Address
; R1 - Array Size
; R2 - Sorted Array Address
;******************************************************************************
verifyArray PROC

    ; Initialize count of elements
    MOV     R4, #0

FOR_LOOP_START
    CMP     R4, R1
    BEQ     FOR_LOOP_END
    
    LDRB    R5, [R0, R4]
    LDRB    R6, [R2, R4]
    CMP     R5, R6
    BNE     SORT_ERROR
    ADD     R4, R4, #1
    B       FOR_LOOP_START

FOR_LOOP_END
    MOV     R0, #0
    BX      LR
    
SORT_ERROR
    MOV     R0, #-1
    BX      LR
    
    ENDP
         
    END     

