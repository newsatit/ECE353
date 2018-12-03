#include "interrupts.h"

void TIMER1A_Handler(void)
{	
		//typecast timer to timer0 type
		TIMER0_Type *gp_timer;
		//set gp timer to timer2 base
		gp_timer = (TIMER0_Type *)TIMER1_BASE;
	
		//printf("ffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffff");
		
		//clear interupt
		gp_timer -> ICR |= 0x00000001;
}

void TIMER4A_Handler(void)
{	
		//typecast timer to timer0 type
		TIMER0_Type *gp_timer;
		//set gp timer to timer2 base
		gp_timer = (TIMER0_Type *)TIMER4_BASE;
	
		//printf("timer4");
		
		//clear interupt
		gp_timer -> ICR |= 0x00000001;
}