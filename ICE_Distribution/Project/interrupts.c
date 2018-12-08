#include "interrupts.h"

extern volatile uint8_t push_buttons;
extern volatile bool button_pushed;
extern volatile uint16_t x_data;

//*****************************************************************************
// GPIOF ISR is used to determine when the push buttons have been pressed
//*****************************************************************************
void GPIOF_Handler()
{
	printf("GPIOF_Handler\n");
	DisableInterrupts();
	button_pushed = true;
	push_buttons = MCP23017_read_push_buttons();
	GPIOF->ICR |= SW2_IO_EXPANDER_INT;
}

void TIMER1A_Handler(void)
{	
		
		//typecast timer to timer0 type
		TIMER0_Type *gp_timer;
		//set gp timer to timer2 base
		gp_timer = (TIMER0_Type *)TIMER1_BASE;
		printf("timer1\n");
		//clear interupt
		gp_timer -> ICR |= 0x00000001;
}

void TIMER4A_Handler(void)
{	
		
		//typecast timer to timer0 type
		TIMER0_Type *gp_timer;
		//set gp timer to timer2 base
		gp_timer = (TIMER0_Type *)TIMER4_BASE;
	
		//x_data = accel_read_x();
//		printf("timer4\n");
		//clear interupt
		gp_timer -> ICR |= 0x00000001;
}

void TIMER2A_Handler(void)
{	 
		printf("timer2\n");
		TIMER2->ICR |= TIMER_ICR_TATOCINT; // Clear Interrupt
}