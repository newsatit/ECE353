#include "init_hardware.h"
#define Core_frequency 50000000
#define Timer1_ticks 250000000
#define Timer4_ticks 50000

void initializeBoard(void)
{
  DisableInterrupts();
  init_serial_debug(true, true);
  eeprom_init();
  EnableInterrupts();
}

void init_timer1(void){
	TIMER0_Type *gp_timer;

	gp_timer_config_32(TIMER1_BASE,TIMER_TAMR_TAMR_PERIOD,false,true);
	
  // Type cast the base address to a TIMER0_Type struct
  gp_timer = (TIMER0_Type *)TIMER1_BASE;
  
	//unenable timer
	gp_timer->CTL &= ~TIMER_CTL_TAEN;
	gp_timer->CTL &= ~TIMER_CTL_TBEN;
	
	//set ticks to count down from
	gp_timer->TAILR &= ~gp_timer->TAILR;
	gp_timer->TAILR |= Core_frequency;
	
	//clear interupts
	gp_timer->ICR |= TIMER_ICR_TATOCINT;
	
	//enable timer
	gp_timer->CTL |= TIMER_CTL_TAEN;
	
	NVIC_SetPriority(TIMER1A_IRQn, 1);
	NVIC_EnableIRQ(TIMER1A_IRQn);
}

void init_timer4(void){
	
	TIMER0_Type *gp_timer;
	
	gp_timer_config_16(TIMER4_BASE,TIMER_TAMR_TAMR_PERIOD,false,true);

  // Type cast the base address to a TIMER0_Type struct	
	gp_timer = (TIMER0_Type *)TIMER4_BASE;

	//unenable timer	
	gp_timer->CTL &= ~TIMER_CTL_TAEN;
	gp_timer->CTL &= ~TIMER_CTL_TBEN;	
		
	gp_timer->TAPR |= 11111111;

	//set ticks to count down from	
	gp_timer->TAILR &= ~gp_timer->TAILR;
	gp_timer->TAILR |= Timer4_ticks;

	//clear interupts
	gp_timer->ICR |= TIMER_ICR_TATOCINT;

	//enable timer	
	gp_timer->CTL |= TIMER_CTL_TAEN;
		
	NVIC_SetPriority(TIMER4A_IRQn, 0);
	NVIC_EnableIRQ(TIMER4A_IRQn);			
}





