#include "init_hardware.h"
#define Core_frequency 50000000
#define Timer1_ticks 250000000
#define Timer4_ticks 100000

const bool SEND_FIRST = true;

/******************************************************************************
 * Global Variables
 *****************************************************************************/
#if SEND_FIRST
uint8_t myID[]      = { 1,1,1,2,3};
uint8_t remoteID[]  = { 2,2,3,1,4};
#else
uint8_t myID[]      = { 2,2,3,1,4};
uint8_t remoteID[]  = { 1,1,1,2,3};
#endif

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
	gp_timer->TAILR |= Timer1_ticks;
	
	//clear interupts
	gp_timer->ICR |= TIMER_ICR_TATOCINT;
	
	//enable timer
	gp_timer->CTL |= TIMER_CTL_TAEN;
	
	NVIC_SetPriority(TIMER1A_IRQn, 2);
	NVIC_EnableIRQ(TIMER1A_IRQn);
}
void init_timer2(void){
	gp_timer_config_32(TIMER2_BASE, TIMER_TAMR_TAMR_PERIOD, false, true);
	// every 20 ms
	TIMER2->TAILR = (Core_frequency * 20)/1000;
	// start timer
	TIMER2->CTL = TIMER_CTL_TAEN;
  // Set the Priority 0
  NVIC_SetPriority(TIMER2A_IRQn, 1);
  // Enable the Interrupt in the NVIC
  NVIC_EnableIRQ(TIMER2A_IRQn);
}
void init_timer4(void){
	
	TIMER0_Type *gp_timer;
	
	gp_timer_config_16(TIMER4_BASE,TIMER_TAMR_TAMR_PERIOD,false,true);

  // Type cast the base address to a TIMER0_Type struct	
	gp_timer = (TIMER0_Type *)TIMER4_BASE;

	//unenable timer	
	gp_timer->CTL &= ~TIMER_CTL_TAEN;
	gp_timer->CTL &= ~TIMER_CTL_TBEN;	
	
	//set prescalar to 3
	gp_timer->TAPR |= 00000010;
	//gp_timer->TAPR |= 11111111;

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

void init_lcd(void){
	lcd_config_gpio();
  lcd_config_screen();
  lcd_clear_screen(LCD_COLOR_BLACK);
}

void initializeBoard(void)
{	
  DisableInterrupts();
  init_serial_debug(true, true);
	//spi_select_init();
	//spi_select(NORDIC);
  //wireless_configure_device(myID, remoteID );
  SysTick_Config(10000);
  eeprom_init();
	ft6x06_init();	
	accel_initialize();
	MCP23017_init();
	init_timer1();
	init_timer4();
	init_lcd();
  EnableInterrupts();
}



