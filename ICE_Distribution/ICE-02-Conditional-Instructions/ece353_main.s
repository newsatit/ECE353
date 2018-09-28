; Filename:     main.s 
; Author:       ece353 staff 
; Description:  

    export __main

;**********************************************
; SRAM
;**********************************************
    AREA    SRAM, READWRITE
    align
        
;**********************************************
; Constant Variables (FLASH) Segment
;**********************************************
    AREA    FLASH, CODE, READONLY
    align

;**********************************************
; Code (FLASH) Segment
; main assembly program
;**********************************************
__main   PROC
    
    ; Initialization of R0 and R1
    MOV     R0, #15
    MOV     R1, #100
    
    ; Using only conditional instructions (no branches)
    ; Implement the if statemens below.
    
    
    
    ; (1) If R0 == 15, then R2 = 100 , else R2 = 1
	CMP R0, #15
	MOVEQ R2, #100
	MOVNE R2, #1

    ; (2) If R0 != 15, then R3 = 200 , else R3 = 2
	CMP R0, #15
	MOVEQ R3, #200
	MOVNE R3, #2
    
    ; (3) If R0 < 15, then  R4 = 300 , else R4 = 3
	CMP R0, #15
	MOVLT R4, #300
	MOVGE R4, #3

    ; (4) If R0 <= 15, then R5 = 400 , else R5 = 4
	MOVLE R5, #400
	MOVGT R5, #4
    
    ; (5) If R0 > 15,  then R6 =500 , else R6 = 5
	MOVGT R6, #500
	MOVLE R6, #5
    
    ; (6) If R0 >= 15, then R7 = 600 , else R7 = 6
	MOVGE R7, #600
	MOVLT R7, #6
    
    


    MOV     R0, #0x2
    
    ; (7) Implement the following switch statement using only conditional instructions
    
    ; Hint: Start by setting R1 to the default value and then checking all of the
    ; other cases.
    MOV R1, #0xFFFFFFFF
    ; switch(R0)
    ; {
    ;      case 0:
    ;      {
    ;           R1 = 0;
    ;           break;
    ;      }
	CMP R1, #0
	MOV R1, #0
    ;      case 1:
    ;      {
    ;           R1 = 10;
    ;           break;
    ;      }
	CMP R1, #1
	MOV R1, #10
    ;      case 2:
    ;      {
    ;           R1 = 20;
    ;           break;
    ;      }
	CMP R1, #2
	MOV R1, #20
    ;      case 3:
    ;      {
    ;           R1 = 30;
    ;           break;
    ;      }
	CMP R1, #3
	MOV R1, #30
    ;      default:
    ;      {
    ;           R1 = 0xFFFFFFFF;
    ;           break;
    ;      }
    ;  }
    


    ; Initialization of R0 and R1
    MOV     R0, #15
    MOV     R1, #100
    
    ; (8) If R0 >= 15 OR R1 < 150, then R8 = 700 , else R8 = 7
	MOV R8, #7
	CMP R0, #15
	MOVGE R8, #700
	CMP R1, #150
	MOVLT R8, #700
    
    
    ; (9) If R0 >= 15 AND R1 < 150, then R9 = 800, else R9 = 8
	MOV R9, #8
	CMP R0, #15
	MOVLT R9, #800
	MOVGE R9, #800

    MOV32   R0, #0xFFF000FF
    ; (10) If bit 2 OR bit 21 of register R0 is a 1, then R1 = 900 , else R1 = 9
	MOV R1, #9
	TST R0, #0x4
	MOVNE R1, #900
	TST R0, #0x200000
	MOVNE R1, #900
    
    
    
    ; DO NOT MODIFY ANTHING BELOW THIS LINE!!!	
        
INFINITE_LOOP
    B INFINITE_LOOP
    
    ENDP
    align
        

    END            
