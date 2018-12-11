#include "init_hardware.h"
#define Core_frequency 50000000
#define Timer1_ticks 250000000
#define Timer4_ticks 100000

const bool SEND_FIRST = false;

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
/******************************************************************************
 * Timer1 is initialized to be a 32 bit count down timer
 * It generates an interrupt every 5 seconds
 *****************************************************************************/
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
	
	NVIC_SetPriority(TIMER1A_IRQn, 0);
	NVIC_EnableIRQ(TIMER1A_IRQn);
}
/******************************************************************************
 * Timer2 is initialized to be a 32 bit count down timer
 * It generates an interrupt every 1 ms
 *****************************************************************************/
void init_timer2(void){
	gp_timer_config_32(TIMER2_BASE, TIMER_TAMR_TAMR_PERIOD, false, true);
	// every 20 ms
	TIMER2->TAILR = (Core_frequency * 1)/1000;
	//clear interupts
	TIMER2->ICR |= TIMER_ICR_TATOCINT;
	// start timer
	TIMER2->CTL = TIMER_CTL_TAEN;
  // Set the Priority 0
  NVIC_SetPriority(TIMER2A_IRQn, 2);
  // Enable the Interrupt in the NVIC
  NVIC_EnableIRQ(TIMER2A_IRQn);
}
/******************************************************************************
 * Timer3 is initialized to be a 32 bit count down timer
 * It generates an interrupt every 1 Second
 *****************************************************************************/
void init_timer3(void){
	gp_timer_config_32(TIMER3_BASE, TIMER_TAMR_TAMR_PERIOD, false, true);
	// every 1 second
	TIMER3->TAILR = Core_frequency;
	//clear interupts
	TIMER3->ICR |= TIMER_ICR_TATOCINT;
	// start timer
	TIMER3->CTL = TIMER_CTL_TAEN;
  // Set the Priority 1
  NVIC_SetPriority(TIMER3A_IRQn, 1);
  // Enable the Interrupt in the NVIC
  NVIC_EnableIRQ(TIMER3A_IRQn);
}
/******************************************************************************
 * Timer4 is initialized to be a 16 bit count down timer
 * It uses a prescaler to generate an interrupt every 8 ms
 *****************************************************************************/
void init_timer4(void){
	
	TIMER0_Type *gp_timer;
	
	gp_timer_config_16(TIMER4_BASE,TIMER_TAMR_TAMR_PERIOD,false,true);

  // Type cast the base address to a TIMER0_Type struct	
	gp_timer = (TIMER0_Type *)TIMER4_BASE;

	//unenable timer	
	gp_timer->CTL &= ~TIMER_CTL_TAEN;
	gp_timer->CTL &= ~TIMER_CTL_TBEN;	
	
	//set prescalar to 3
	gp_timer->TAPR |= 00000011;
	//gp_timer->TAPR |= 11111111;

	//set ticks to count down from	
	gp_timer->TAILR &= ~gp_timer->TAILR;
	gp_timer->TAILR |= Timer4_ticks;

	//clear interupts
	gp_timer->ICR |= TIMER_ICR_TATOCINT;

	//enable timer	
	gp_timer->CTL |= TIMER_CTL_TAEN;
		
	NVIC_SetPriority(TIMER4A_IRQn, 3);
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
	spi_select_init();
	spi_select(NORDIC);
  wireless_configure_device(myID, remoteID );
  eeprom_init();
	ft6x06_init();	
	accel_initialize();
	MCP23017_init();
	init_timer1();
	init_timer4();
	init_timer2();
	init_timer3();
	init_lcd();
  EnableInterrupts();
}



