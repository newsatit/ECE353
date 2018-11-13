// Copyright (c) 2014-16, Joe Krachey
// All rights reserved.
//
// Redistribution and use in binary form, with or without modification, 
// are permitted provided that the following conditions are met:
//
// 1. Redistributions in binary form must reproduce the above copyright 
//    notice, this list of conditions and the following disclaimer in 
//    the documentation and/or other materials provided with the distribution.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" 
// AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, 
// THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR 
// PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR 
// CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, 
// EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, 
// PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; 
// OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, 
// WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING 
// NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, 
// EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

#include "ps2.h"


/*******************************************************************************
* Function Name: initialize_adc_gpio_pins
********************************************************************************
* Used to initialize the PS2 joystick GPIO pins for both the analog inputs
*
* Configuration Info
*		Fill out relevant information in ps2.h.  ps2.h defines 
*		how various peripherals are physically connected to the board.
*******************************************************************************/
static void initialize_adc_gpio_pins(void)
{
	gpio_enable_port(PS2_GPIO_BASE);
	gpio_config_enable_input(PS2_GPIO_BASE, PS2_X_DIR_MASK|PS2_Y_DIR_MASK);
	gpio_config_analog_enable(PS2_GPIO_BASE, PS2_X_DIR_MASK|PS2_Y_DIR_MASK);
	gpio_config_alternate_function(PS2_GPIO_BASE, PS2_X_DIR_MASK|PS2_Y_DIR_MASK);
}

/*******************************************************************************
* Function Name: ps2_initialize
********************************************************************************
* Initializes the GPIO pins connected to the PS2 Joystick.  It also configures
* ADC0 to use Sample Sequencer #3 to convert a programmable channel number.
*******************************************************************************/
void ps2_initialize(void)
{
	initialize_adc_gpio_pins();
	initialize_adc(PS2_ADC_BASE);
}


/*******************************************************************************
* Function Name: ps2_get_x
********************************************************************************
*Returns the most current reading of the X direction  Only the lower 12-bits
* contain data.
********************************************************************************/
uint16_t ps2_get_x(void)
{
	uint16_t adc_val;
	adc_val = get_adc_value(PS2_ADC_BASE, PS2_X_ADC_CHANNEL);
  return adc_val;
}

/*******************************************************************************
* Function Name: ps2_get_y
********************************************************************************
* Returns the most current reading of the Y direction.  Only the lower 12-bits
*  contain data.
********************************************************************************/
uint16_t ps2_get_y(void)
{
  uint16_t adc_val;
	adc_val = get_adc_value(PS2_ADC_BASE, PS2_Y_ADC_CHANNEL);
  
  return adc_val;
}

/*******************************************************************************
* Function Name: ps2_hw3_initialize
********************************************************************************
* Initializes the GPIO pins connected to the PS2 Joystick for HW3.  It also configures
* ADC0 to use Sample Sequencer #2 to convert a programmable channel number.
*******************************************************************************/
void ps2_hw3_initialize(void)
{
	initialize_adc_gpio_pins();
    
  // Turn on the ADC0 Clock
  SYSCTL->RCGCADC |= SYSCTL_RCGCADC_R0;
  
  // Wait for ADC0 to become ready
  while(!(SYSCTL_PRADC_R0 & SYSCTL->PRADC)){}
  
  // ADD CODE
  // disable sample sequencer #2 by writing a 0 to the 
  // corresponding ASENn bit in the ADCACTSS register 
	ADC0->ACTSS &= ~ADC_ACTSS_ASEN2;
	
  // ADD CODE
  // Set the event multiplexer to trigger conversion on a processor trigger
  // for sample sequencer #2.
	ADC0->EMUX &= ~ADC_EMUX_EM2_M;
	
	// Configure first input select to be x-chanel and second input to be y-channel
	ADC0->SSMUX2 = (ADC_SSMUX2_MUX1_M & (PS2_Y_ADC_CHANNEL << ADC_SSMUX2_MUX1_S)) | (ADC_SSMUX2_MUX0_M & (PS2_X_ADC_CHANNEL << ADC_SSMUX2_MUX0_S));

  // ADD CODE
  // Set 2nd Sample to be the end of sequence and generate interrupt
	ADC0->SSCTL2 = ADC_SSCTL2_IE1 | ADC_SSCTL2_END1;
		
	// Config the ADC to average 8 samples of each channel before generating an interrupt
	ADC0->SAC = ADC_SAC_AVG_8X;
	
	// Enable the Interrupt Mask for SS2		
	ADC0->IM |= ADC_IM_MASK2;
	
	// Set the Priority
  NVIC_SetPriority(ADC0SS2_IRQn, 3);
 
  // Enable the Interrupt in the NVIC
  NVIC_EnableIRQ(ADC0SS2_IRQn);
	
	// Enable the sample sequencer #2
	ADC0->ACTSS |= ADC_ACTSS_ASEN2;
}



