#include "interrupts.h"

extern volatile uint8_t PUSH_BUTTONS;
extern volatile bool BUTTON_PUSHED;

//*****************************************************************************
// GPIOF ISR is used to determine when the push buttons have been pressed
//*****************************************************************************
void GPIOF_Handler()
{
	printf("GPIOF_Handler\n");
	DisableInterrupts();
	BUTTON_PUSHED = true;
	PUSH_BUTTONS = MCP23017_read_push_buttons();
	GPIOF->ICR |= SW2_IO_EXPANDER_INT;
}