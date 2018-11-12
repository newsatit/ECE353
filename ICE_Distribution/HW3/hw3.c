#include "hw3.h"

volatile uint16_t SHIP_X_COORD = 190;
volatile uint16_t SHIP_Y_COORD = 270;
volatile uint16_t INVADER_X_COORD = 50;
volatile uint16_t INVADER_Y_COORD = 40;
volatile bool MOVE_INVADER = true;
volatile bool MOVE_SHIP = true;

char STUDENT_NAME[] = "Dawanit Satitsumpun";

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
  
  // Initialize Timer 2
  // Initialize Timer 3

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
    init_hardware();
    hw3_hardware_validate();
    
    // ADD CODE BELOW
    
}
