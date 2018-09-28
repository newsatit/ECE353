; Filename:     main.s 
; Author:       ece353 staff 
; Description:  

    export __main
    import verifyArray

BYTE        EQU     1
WORD        EQU     4
MAX_INDEX   EQU     8
    
;**********************************************
; SRAM
;**********************************************
    AREA    SRAM, READWRITE
SORTED     SPACE   MAX_INDEX*BYTE
    align
        
;**********************************************
; Constant Variables (FLASH) Segment
;**********************************************
    AREA    FLASH, CODE, READONLY
UNSORTED    DCB     0x01
            DCB     0x40
            DCB     0x24
            DCB     0xFF
            DCB     0x15
            DCB     0x99
            DCB     0x33
            DCB     0x11
PRE_SORTED  DCB     0x01
            DCB     0x11
            DCB     0x15
            DCB     0x24
            DCB     0x33
            DCB     0x40
            DCB     0x99
            DCB     0xFF
    align

;**********************************************
; Code (FLASH) Segment
; main assembly program
;**********************************************
__main   PROC
    
    
    ; Load the address of UNSORTED into R0
    LDR     R0, =(UNSORTED)
    
    ; Load the address of SORTED into R1
    LDR     R1, =(SORTED)
    
    ; Load the address of PRE_SORTED into R2
    LDR     R2, =(PRE_SORTED)
    
    ; Move 8 into R3
    MOV     R3, #MAX_INDEX
    
    ; Initialize count of elements
    MOV     R4, #0

FOR_LOOP_START
    CMP     R4, R3
    BEQ     FOR_LOOP_END
    
    LDRB    R5, [R0, R4]
    STRB    R5, [R1, R4]
    ADD     R4, R4, #1
    B       FOR_LOOP_START
FOR_LOOP_END

    ; **********************************************************
    ; DO NOT MODIFY ABOVE
    ; **********************************************************
    
    ; Load the address of SORTED into R0

    ; Move the array size of 8 into R1
    
    ; Call Bubble Sort
    

    ; **********************************************************
    ; DO NOT MODIFY BELOW
    ; **********************************************************
    LDR R0, =(SORTED)
    MOV R1, #MAX_INDEX
    LDR R2,=(PRE_SORTED)
    BL      verifyArray
    
    MOVS    R0, R0
    BEQ     SORT_PASSED
    
SORT_FAILED
    B   SORT_FAILED
    
SORT_PASSED
    B   SORT_PASSED
    
    ENDP
    align
        

    END            