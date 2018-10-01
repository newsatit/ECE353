; Filename:     addMultiply.s 
; Author:       ece353 staff 
; Description:  

    export addMultiply

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
; Four arrays of 8-bit signed numbers are 
; passed via the first four paramters.
; The 5th paramter indicates the length of the
; arrays. For each entry in the array, the
; following operation takes place.
;
; Array3[i] = (Array0[i] + Array1[i]) * Array2[i]
;
; Parameter 0       Array Address 0
; Parameter 1       Array Address 1
; Parameter 2       Array Address 2
; Parameter 3       Array Address 3
; Parameter 4       Array Size 
;
; Returns
;   if ALL parameters are valid, return 0
;   else return -1.
;
;  An address is valid if it is non zero
;  The size is valid if it is greater than zero
;**********************************************
addMultiply PROC
    
    ; Validate Parameters
	CMP R0, #0
	MOVEQ R0, #-1
	BEQ DONE
	
	CMP R1, #0
	MOVEQ R0, #-1
	BEQ DONE
	
	CMP R2, #0
	MOVEQ R0, #-1
	BEQ DONE
	
	CMP R3, #0
	MOVEQ R0, #-1
	BEQ DONE

    ; Save required registers
	PUSH {R4-R6}
	LDR R4, [SP, #12]
	CMP R4, #0
	MOVLE R0, #-1
	BLE DONE
    
    ; For each index in the arrays, compute  
    ; Array3[i] = (Array0[i] + Array1[i]) * Array2[i]
LOOP
	CMP R4, #0
	MOVEQ R0, #0
	BEQ DONE
	
	LDRB R5, [R0], #1
	LDRB R6, [R1], #1
	ADD R5, R5, R6
	LDRB R6, [R2], #1
	MUL R5, R5, R6
	STRB R5, [R3], #1
		
	SUB R4, R4, #1
	B LOOP
	
 
DONE
    ; Restore registers saved to the stack
	POP {R4-R6}
    
    ; Return from the loop
	BX LR
    ENDP


        
    align
        

    END            
