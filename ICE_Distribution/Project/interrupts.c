#include "interrupts.h"

extern volatile uint8_t push_buttons;		//records which push button is held down
extern volatile bool button_pushed;			//lets main know know that a button has been pushed
//extern volatile int16_t x_data;					
extern volatile bool get_x_data;				//lets main loop know that it should get accel data
extern volatile bool draw_puck;					//let main loop know that is should draw the puck
extern volatile int speed_count;				//used to determine how often the puck is draw
extern volatile bool print_bytes;				//lets main loop know that it should print number of bytes transmitted

//extern PC_Buffer UART0_Tx_Buffer;
//extern PC_Buffer UART0_Rx_Buffer;

static int count = 0;										//counts number of times timer2 interrupt has been called


volatile bool AlertOneSec = false;			//lets main loop know that one second has passed

//*****************************************************************************
// GPIOF ISR is used to determine when the push buttons have been pressed
//*****************************************************************************
void GPIOF_Handler()
{
	DisableInterrupts();
	button_pushed = true;
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