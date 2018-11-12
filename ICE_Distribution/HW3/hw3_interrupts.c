#include "hw3_interrupts.h"

//*****************************************************************************
// TIMER2 ISR is used to determine when to move the Invader
//*****************************************************************************
void TIMER2A_Handler(void)
{	
    
}

//*****************************************************************************
// TIMER3 ISR is used to determine when to move the spaceship
//*****************************************************************************
void TIMER3A_Handler(void)
{
    static PS2_DIR_t current_dir = PS2_DIR_CENTER;
    static uint16_t move_count = 0;  
    
}

//*****************************************************************************
// TIMER4 ISR is used to trigger the ADC
//*****************************************************************************
void TIMER4A_Handler(void)
{	
    
}

//*****************************************************************************
// ADC0 SS2 ISR
//*****************************************************************************
void ADC0SS2_Handler(void)
{

}
	// the ADC will average 8 samples of each channel before generating an interrupt
//	ADC0->SAC = ADC_SAC_AVG_8X;