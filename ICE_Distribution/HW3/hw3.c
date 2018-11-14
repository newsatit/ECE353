#include "hw3.h"

// CPU Core Frequency
#define CORE_FEQ 50000000

#define LCD_WIDTH 240
#define LCD_HEIGHT 320

volatile uint16_t SHIP_X_COORD = 190;
volatile uint16_t SHIP_Y_COORD = 270;
volatile uint16_t INVADER_X_COORD = 50;
volatile uint16_t INVADER_Y_COORD = 40;
volatile bool MOVE_INVADER = true;
volatile bool MOVE_SHIP = true;

char STUDENT_NAME[] = "Dawanit Satitsumpun";

//*****************************************************************************
//*****************************************************************************
void DisableInterrupts(void)
{
  __asm {
         CPSID  I
  }
}

//*****************************************************************************
//*****************************************************************************
void EnableInterrupts(void)
{
  __asm {
    CPSIE  I
  }
}

//*****************************************************************************
// If any part of the image would be off the screen if the image
// is moved in the specified direction, return true.  If the image would not
// be in contact with the edge of the screen, return false.
//*****************************************************************************
bool contact_edge(
    volatile PS2_DIR_t direction,
    volatile uint16_t x_coord, 
    volatile uint16_t y_coord, 
    uint8_t image_height, 
    uint8_t image_width
)
{ 
	uint16_t x0;
	uint16_t x1;
	uint16_t y0;
	uint16_t y1;
	// x0 stores leftmost x-position of the image
	x0 = x_coord - (image_width/2);
	// x1 stores rightmost x-position of the image
	x1 = x_coord + (image_width/2);
	if( (image_width & 0x01) == 0x00)
	{
		x1--;
	}
	// y0 stores topmost y-position of the image
	y0 = y_coord  - (image_height/2);
	// y1 stores bottommost y-position of the image
	y1 = y_coord  + (image_height/2) ;
	if( (image_height & 0x01) == 0x00)
	{
		y1--;
	}
	// Check the contact base on direction
	switch(direction)
	{
		case PS2_DIR_LEFT:
			if(x0 == 0)
				return true;
			break;
		case PS2_DIR_RIGHT:
			if(x1 == LCD_WIDTH - 1)
				return true;
			break;
		case PS2_DIR_UP:
			if(y0 == 0)
				return true;
			break;
		case PS2_DIR_DOWN:
			if(y1 == LCD_HEIGHT - 1)
				return true;
			break;
	}
	return false;
}

//*****************************************************************************
// Moves the image by one pixel in the provided direction.  The second and 
// third parameter should modify the current location of the image (pass by
// reference)
//*****************************************************************************
void move_image(
        volatile PS2_DIR_t direction,
        volatile uint16_t *x_coord, 
        volatile uint16_t *y_coord, 
        uint8_t image_height, 
        uint8_t image_width
)
{ 
	switch(direction)
	{
		case PS2_DIR_LEFT:
			*x_coord = *x_coord - 1;
			break;
		case PS2_DIR_RIGHT:
			*x_coord = *x_coord + 1;
			break;
		case PS2_DIR_UP:
			*y_coord = *y_coord - 1;
			break;
		case PS2_DIR_DOWN:
			*y_coord = *y_coord + 1;
			break;
	}
}

//*****************************************************************************
// Determines is any portion of the two images are overlapping.  An image is
// considered to be overlapping if the two rectangles determined by the image
// height and widths are overlapping.  An overlap occurs even if the area that
// overlaps are portions of the images where the pixels do not display on the
// screen.
//
// If the two images are overlapping, return true.
// If the two images are NOT overlapping, return false.
//*****************************************************************************
bool check_game_over(
        volatile uint16_t ship_x_coord, 
        volatile uint16_t ship_y_coord, 
        uint8_t ship_height, 
        uint8_t ship_width,
        volatile uint16_t invader_x_coord, 
        volatile uint16_t invader_y_coord, 
        uint8_t invader_height, 
        uint8_t invader_width
)
{
	int x_diff; // the distance between the center of each object in x
	int x_dist; // the minimum distance it need to be in x
	int y_diff; // the distance between the center of each object in y
	int y_dist; // the minimum distance it need to be in y
	
	x_diff = ship_x_coord - invader_x_coord;
	// maintain positive distance
	if(x_diff < 0)
		x_diff = -x_diff;
	x_dist = (ship_width + invader_width)/2;
	
	y_diff = ship_y_coord - invader_y_coord;
	// maintain positive distance
	if(y_diff < 0)
		y_diff = -y_diff;
	y_dist = (ship_height + invader_height)/2;
	
	// contact with both x-axis and y-axis overlap
	return (x_diff < x_dist) && (y_diff < y_dist);
	
	
}

//*****************************************************************************
// Initializes all of the peripherls used in HW3
//*****************************************************************************
void init_hardware(void)
{
  // Initialize LCD Screen
  lcd_config_gpio();
  lcd_config_screen();
  lcd_clear_screen(LCD_COLOR_BLACK);   
    
  // Initialize ADC and Timer used to provide analog data from the PS2
	ps2_hw3_initialize();
	//Initialize_adc Timer 4
	// periodic, count-down, enable interrupt
	gp_timer_config_32(TIMER4_BASE, TIMER_TAMR_TAMR_PERIOD, false, true);
	// every 1ms
	TIMER4->TAILR = (CORE_FEQ * 1)/1000;
	// start timer
	TIMER4->CTL = TIMER_CTL_TAEN;
  // Set the Priority 2
  NVIC_SetPriority(TIMER4A_IRQn, 2);
  // Enable the Interrupt in the NVIC
  NVIC_EnableIRQ(TIMER4A_IRQn);
  
  // Initialize Timer 2
	// periodic, count-down, enable interrupt
	gp_timer_config_32(TIMER2_BASE, TIMER_TAMR_TAMR_PERIOD, false, true);
	// every 20 ms
	TIMER2->TAILR = (CORE_FEQ * 20)/1000;
	// start timer
	TIMER2->CTL = TIMER_CTL_TAEN;
  // Set the Priority 0
  NVIC_SetPriority(TIMER2A_IRQn, 0);
  // Enable the Interrupt in the NVIC
  NVIC_EnableIRQ(TIMER2A_IRQn);
  
	// Initialize Timer 3
	// periodic, count-down, enable interrupt
	gp_timer_config_32(TIMER3_BASE, TIMER_TAMR_TAMR_PERIOD, false, true);
	// every 10 ms
	TIMER3->TAILR = (CORE_FEQ * 10)/1000;
	// start timer
	TIMER3->CTL = TIMER_CTL_TAEN;
  // Set the Priority 1
  NVIC_SetPriority(TIMER3A_IRQn, 1);
  // Enable the Interrupt in the NVIC
  NVIC_EnableIRQ(TIMER3A_IRQn);
}

//*****************************************************************************
// Main application for HW3
// 
// This function should return after the INVADER and SHIP images are determined
// to overalp.
//
// This function is responsible for calling the lcd_draw_image function that 
// will move the SHIP and the INVADER
//*****************************************************************************
void hw3_main(void)
{
		int i;
    init_hardware();
    hw3_hardware_validate();
    
    // ADD CODE BELOW
		while(!check_game_over(
			SHIP_X_COORD, SHIP_Y_COORD, 
			space_shipHeightPixels, space_shipWidthPixels,
			INVADER_X_COORD, INVADER_Y_COORD, 
			invaderHeightPixels, invaderWidthPixels
		))
		{
			// have to display new invader
			if(MOVE_INVADER)
			{
				lcd_draw_image(
					INVADER_X_COORD, 
					invaderWidthPixels, 
					INVADER_Y_COORD, 
					invaderHeightPixels, 
					invaderBitmaps, 
					LCD_COLOR_RED, 
					LCD_COLOR_BLACK     
				);
				MOVE_INVADER = false;
				EnableInterrupts();
			}
			
			// have to display new saucer
			if(MOVE_SHIP)
			{
				lcd_draw_image(
					SHIP_X_COORD, 
					space_shipWidthPixels, 
					SHIP_Y_COORD, 
					space_shipHeightPixels, 
					space_shipBitmaps, 
					LCD_COLOR_BLUE, 
					LCD_COLOR_BLACK     
				);
				MOVE_SHIP = false;
				EnableInterrupts();
			}
		}
}
