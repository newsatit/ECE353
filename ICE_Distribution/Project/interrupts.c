#include "interrupts.h"

extern volatile uint8_t push_buttons;
extern volatile bool button_pushed;
extern volatile int16_t x_data;
extern volatile bool get_x_data;

extern PC_Buffer UART0_Tx_Buffer;
extern PC_Buffer UART0_Rx_Buffer;


volatile bool AlertOneSec = false;

 /****************************************************************************
 * The SysTick Handler 
 *
 * Register Information on the SysTick timer can be found on pages 129-132 
 * of the MCU data sheet.
 ****************************************************************************/
void SysTick_Handler(void)
{
  static int count = 0;
  
  // Clear the SysTick Counter 
	 uint32_t val = SysTick->VAL;// Clears the SysTick Interrupt

	if ( count == 0)
	{
			AlertOneSec = true;
			count = 0;
		printf("Systic");
	}
	else
	{
		count++;
	}

}


//*****************************************************************************
// GPIOF ISR is used to determine when the push buttons have been pressed
//*****************************************************************************
void GPIOF_Handler()
{
	//printf("GPIOF_Handler\n");
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
		//printf("timer1\n");
	
		AlertOneSec = true;
		//clear interupt
		gp_timer -> ICR |= 0x00000001;
}

void TIMER4A_Handler(void)
{	
		
		//typecast timer to timer0 type
		TIMER0_Type *gp_timer;
		//set gp timer to timer2 base
		gp_timer = (TIMER0_Type *)TIMER4_BASE;
		
//		DisableInterrupts();
//		spi_select(MODULE_1);
//		
//		x_data = accel_read_x();
//	
//		EnableInterrupts();
	
		get_x_data = true;
	
		//clear interupt
		gp_timer -> ICR |= 0x00000001;
}

void TIMER2A_Handler(void)
{	 
		//printf("timer2\n");
		TIMER2->ICR |= TIMER_ICR_TATOCINT; // Clear Interrupt
}