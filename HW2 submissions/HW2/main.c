// Copyright (c) 2015-16, Joe Krachey
// All rights reserved.
//
// Redistribution and use in source or binary form, with or without modification, 
// are permitted provided that the following conditions are met:
//
// 1. Redistributions in source form must reproduce the above copyright 
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

#include "main.h"

//*****************************************************************************
// This is an ARRAY of strings.  If you wanted to access the 3rd string
// ("P10000"), you could do so using COMMANDS[2].
//*****************************************************************************
char * COMMANDS[] = {
    "P200000",
    "R500",
    "P10000",
    "U1000",
    "P5000",
    "L2000",
    "P2500",
    "D2000",
    "P1250",
    "R94",
    "P1",
    "U142"
};

//*****************************************************************************
//*****************************************************************************
void init_hardware(void)
{
  initialize_serial_debug();
  lcd_config_gpio();
  lcd_config_screen();
  lcd_clear_screen(LCD_COLOR_BLACK);   
}

void init_saucer(void)
{
	int i;
	int j;
	// number of commands
	int command_len = sizeof(COMMANDS)/sizeof(COMMANDS[0]);	
	int pause = 0;

	uint16_t x = 120;
	uint16_t y = 160;
	// draw starting image at centered
	lcd_draw_image(
								x,                 // X Pos
								space_shipWidthPixels,   // Image Horizontal Width
								y,                 // Y Pos
								space_shipHeightPixels,  // Image Vertical Height
								space_shipBitmaps,       // Image
								LCD_COLOR_RED,      // Foreground Color
								LCD_COLOR_BLACK     // Background Color
	);
	
	for(i = 0; i < command_len; i++)
	{
		char *cmd = COMMANDS[i];
		char *cmd_num = COMMANDS[i] + 1;
		int dx = 0;
		int dy = 0;
		int new_x = x;
		int new_y = y;
		int x0;
		int x1;
		int y0;
		int y1;
		// determine the command
		switch(*cmd){
			case 'U':
			{
				dy = -1;
				new_y = y - atoi(cmd_num);
				break;
			}
			case 'D':
			{
				dy = 1;
				new_y = y + atoi(cmd_num);
				break;
			}
			case 'L':
			{
				dx = -1;
				new_x = x - atoi(cmd_num);
				break;
			}
			case 'R':
			{
				dx = 1;
				new_x = x + atoi(cmd_num);

				break;
			}
			case 'P':
			{
				pause = atoi(cmd_num);
				break;
			}
			
		}
		
		// move until reaches position specified by the command
		while(new_x != x || new_y != y) 
		{
			// x0 stores left x-position of the image
			x0 = x - (space_shipWidthPixels/2);
			// x1 stores right x-position of the image
			x1 = x + (space_shipWidthPixels/2);
			if( (space_shipWidthPixels & 0x01) == 0x00)
			{
				x1--;
			}
			// y0 stores top y-position of the image
			y0 = y  - (space_shipHeightPixels/2);
			// y1 stores bottom y-position of the image
			y1 = y  + (space_shipHeightPixels/2) ;
			if( (space_shipWidthPixels & 0x01) == 0x00)
			{
				y1--;
			}
			
			// draw image
			lcd_draw_image(
										(uint16_t) x,                 // X Pos
										space_shipWidthPixels,   // Image Horizontal Width
										(uint16_t) y,                 // Y Pos
										space_shipHeightPixels,  // Image Vertical Height
										space_shipBitmaps,       // Image
										LCD_COLOR_RED,      // Foreground Color
										LCD_COLOR_BLACK     // Background Color
			);
			
			// check if reaches edges
			if(x0 == 0 && dx == -1)
				break;
			if(x1 == 239 && dx == 1)
				break;
			if(y0 == 0 && dy == -1)
				break;
			if(y1 == 319 && dy == 1)
				break;
			
			// move the object
			x += dx;
			y += dy;
			
			// pause
			for(j = 0; j < pause; j++){}
		}
	}
}

//*****************************************************************************
//*****************************************************************************
int 
main(void)
{
  init_hardware();
    
  put_string("\n\r");
  put_string("******************************\n\r");
  put_string("ECE353 HW2 Fall 2018\n\r");
  put_string("Dawanit Satitsumpun\n\r");
  put_string("******************************\n\r");    
	
	init_saucer();

  // Reach infinite loop
  while(1){};
}
