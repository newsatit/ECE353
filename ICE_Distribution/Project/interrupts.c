#include "interrupts.h"

extern volatile uint8_t push_buttons;
extern volatile bool button_pushed;
extern volatile int16_t x_data;
extern volatile bool get_x_data;
extern volatile bool draw_puck;
extern volatile int speed_count;
extern volatile bool print_bytes;

extern PC_Buffer UART0_Tx_Buffer;
extern PC_Buffer UART0_Rx_Buffer;

static int count = 0;


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
//*****************************************************************************
// Timer 1A interrupts every 5 seconds
// It is used to determine when to send number of bytes that have been wirelessly
// transmitted
//*****************************************************************************
void TIMER1A_Handler(void)
{	
		
		//typecast timer to timer0 type
		TIMER0_Type *gp_timer;
		//set gp timer to timer2 base
		gp_timer = (TIMER0_Type *)TIMER1_BASE;
		//printf("timer1\n");
		print_bytes = true;
		//clear interupt
		gp_timer -> ICR |= 0x00000001;
}
//*****************************************************************************
// Timer 4A interrupts every 8 ms
// It is used to determine when to get new accelerometer data
//*****************************************************************************
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
//*****************************************************************************
// Timer 2A interrupts every 1 ms
// It updates the speed counter which is used to determine how often to draw
// the puck
//*****************************************************************************
void TIMER2A_Handler(void)
{	 
		if(count >= speed_count){
				draw_puck = true;
				count = 0;
		}else{
			count++;
		}

		TIMER2->ICR |= TIMER_ICR_TATOCINT; // Clear Interrupt
}
//*****************************************************************************
// Timer 3A interrupts every 1 second
// It is used to decrement the game clock
//*****************************************************************************
void TIMER3A_Handler(void)
{	 
		AlertOneSec = true;
		// Clear Interrupt
		TIMER3->ICR |= TIMER_ICR_TATOCINT;

}