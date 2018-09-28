; Filename:     main.s 
; Author:       ece353 staff 
; Description:  

    export __main

;******************************************
; Symbolic Constants
;****************************************** 
WORD    EQU     4
  
  
;******************************************
; SRAM
;******************************************
    AREA    SRAM, READWRITE
;******************************************
; Load Store Review
;******************************************
RESULT	SPACE WORD


;******************************************
; Load Store Multiple
;******************************************
DEST_ARRAY     SPACE   32*WORD
DEST2_ARRAY    SPACE   32*WORD
DEST3_ARRAY    SPACE   32*WORD
    align
        
        
        
        
        
;**********************************************
; FLASH
;**********************************************
    AREA    FLASH, CODE, READONLY
;******************************************
; Load Store Review
;******************************************
ASCII_ARRAY   	DCB     "ECE353"
				DCB     0

;******************************************
; Load Store Multiple
;******************************************
SRC_ARRAY   DCD         0x00000000
            DCD         0x00001111
            DCD         0x00002222
            DCD         0x00003333
            DCD         0x00004444
            DCD         0x00005555
            DCD         0x00006666
            DCD         0x00007777
            DCD         0x00008888 
            DCD         0x00009999
            DCD         0x0000AAAA
            DCD         0x0000BBBB
            DCD         0x0000CCCC
            DCD         0x0000DDDD
            DCD         0x0000EEEE
            DCD         0x0000FFFF
            DCD         0x11110000
            DCD         0x11111111
            DCD         0x11112222
            DCD         0x11113333
            DCD         0x11114444
            DCD         0x11115555
            DCD         0x11116666
            DCD         0x11117777
            DCD         0x11118888
            DCD         0x11119999
            DCD         0x1111AAAA
            DCD         0x1111BBBB
            DCD         0x1111CCCC
            DCD         0x1111DDDD
            DCD         0x1111EEEE
            DCD         0x1111FFFF
    align





;**********************************************
; Code (FLASH) Segment
; main assembly program
;**********************************************
__main   PROC
  
    ;******************************************
    ; Load Store Review
    ;******************************************
	LDR R0, =(ASCII_ARRAY)
	LDR R1, =(RESULT)
	MOV R3, #0
	
START_LOOP
	LDRB R2, [R0]
	CMP R2, #0
	BEQ END_LOOP
	SUB R2, R2, 0x30
	ADD R3, R3, R2
	CMP R2, #0
	SUBLT R3, R2
	CMP R2, #9
	SUBGT R3, R2
	ADD R0, R0, #1
	B START_LOOP
END_LOOP
	STR R3, [R1]
		
		
    
    ;******************************************
    ; Load Store Multiple
    ;******************************************
    ; Load the address of SRC_ARRAY into R0
	LDR R0, =(SRC_ARRAY)

    
    ; Load the address of DEST_ARRAY into R1
	LDR R1, =(DEST_ARRAY)

    
    ; Load the address of DEST2_ARRAY into R2
	LDR R2, =(DEST2_ARRAY)

       
    ; Using LDM, load the first 8 WORDs in SRC_ARRAY
    ; Use Registers R3-R10 as the destination 
    ; of the LDM instruction
	LDM R0, {R3-R10}

    
    ; Use STM to store R3-R10 to DEST_ARRAY
	STM R1, {R3-R10}

    
     ; Use STM to store R3-R10 to DEST2_ARRAY
     ; Specify the order of the registers in
     ; revers order (STM R2, {R10, R9, R8, R7, R6, R5, R4, R3})
     ; Observe if the order of the registers changes
     ; how the data arranged in SRAM
	 STM R2, {R10, R9, R8, R7, R6, R5, R4, R3}



    
    ; Using LDM, load the second 8 WORDs in SRC_ARRAY
    ; Use Registers R3-R10 as the destination 
    ; of the LDM instruction.
    ; Hint, you will need to modify R0 so that
    ; it contains the address of SRC_ARRAY[8]
	ADD R0, R0, #8*WORD
	LDM R0, {R3-R10}




    ; Use STM to store R3-R10 to DEST_ARRAY[8]
    ; Hint, you will need to modify R1 so that
    ; it contains the address of DEST_ARRAY[8]
	ADD R1, R1, #8*WORD
	STM R1, {R3-R10}

    
    
    ; Copy the contents of SRC_ARRAY to DEST2_ARRAY
    ; 8 WORDs at a time using LDM/STM. This time use
    ; the version of LDM/STM that updates the base address
    ; Load the address of SRC_ARRAY into R0
	LDR R0, =(SRC_ARRAY)
	LDM R0!, {R3-R10}
	STM R2!, {R3-R10}
	LDM R0!, {R3-R10}
	STM R2!, {R3-R10}

    
    
    ; Write code that uses a loop to copies SRC_ARRAY
    ; to DEST3_ARRAY.  Each iteraction of the loop should
    ; copy 4 WORDs of the array
    ; 
    ; for(i = 0; i < 8; i++)
    ; {
    ;       Copy 4 WORDs from SRC_ARRAY to DEST3_ARRAY
    ; }
	
	; R5 For Storing tempdata
	; R4 as counter
	LDR R0, =(SRC_ARRAY)
	LDR R3, =(DEST3_ARRAY)
	MOV R4, #0
START_FOR
	CMP R4, #8
	BGE END_FOR
	LDR R5, [R0], #WORD
	STR R5, [R3], #WORD
	LDR R5, [R0], #WORD
	STR R5, [R3], #WORD
	LDR R5, [R0], #WORD
	STR R5, [R3], #WORD
	LDR R5, [R0], #WORD
	STR R5, [R3], #WORD
	ADD R4, R4, #1
	B START_FOR
END_FOR

    
 
    

    ; DO NOT MODIFY ANTHING BELOW THIS LINE!!!
        
INFINITE_LOOP
    B INFINITE_LOOP
    
    ENDP
    align
        

    END            
