#include "hockey_main.h"

// for wireless

#define LCD_WIDTH 240								//width of LCD screen in pixels
#define LCD_HEIGHT 320							//height of LCD screen in pixels
#define WORD_START 150							//starting x drawing position of game clock timer
#define TIMER_HEIGHT 10							//starting y drawing position of game clock timer
#define	COLOR_START 80							//starting x drawing position of "select color" word
#define BORDER_HEIGHT 20						//starting y drawing position of top game border
#define START_WIDTH 85							
#define START_HEIGHT 160
#define TOUCH_MIN 200								//number of capacitive touch events necessary before game registers a touch
#define TOP_PADDING 30				
#define TEN_PT_FONT_WIDTH 15				//Letter width in pixels for 10 point font
#define TEN_PT_FONT_HEIGHT 16				//Letter height in pixels for 10 point font
#define SCORE_X 10									//starting x drawing position of game score
#define SCORE_Y 10									//starting y drawing position of game score
#define FT_PT_FONT_WIDTH 21					//Letter width in pixels for 14 point font
#define FT_PT_FONT_HEIGHT 21				//Letter height in pixels for 14 point font
#define WAITING_START 20
#define GAME_OVER_X 60							//starting x drawing position of game over
#define GAME_OVER_Y 100							//starting y drawing position of game over
#define UP_BUTTON 254
#define DOWN_BUTTON 253
#define FULL_POWER 0xFF

/******************************************************************************
 * Global Variables
 *****************************************************************************/
 
 
 //data to write to EEPROM
 char first_line[80] = "Student 1: Dawanit Satitsumpun";
 char second_line[80] = "Student 2: Joshua Newman";
 char third_line[80] = "Team Number: 26";

//records the number of bytes sent and recieved 
uint32_t bytes_sent = 0;
uint32_t bytes_received = 0;

volatile uint8_t push_buttons = 0;
volatile bool button_pushed = false;
volatile int16_t x_data = 0;
extern const bool SEND_FIRST;
volatile bool get_x_data;
volatile bool draw_puck = false;
volatile bool print_bytes = false;
bool player1_ready=false;
bool player2_ready=false;
bool color_selected = false;
bool touch_start = false;
bool move_paddle = false;
bool move_puck = false;
bool puck_here = false;
bool fast = false;
bool pause = false;

uint16_t scored = 0;
bool send = false;
uint16_t my_score = 0;
uint16_t opponent_score = 0;
uint8_t	power = 0;

volatile int speed_count;

volatile uint32_t PADDLE_X_COORD;
volatile uint32_t PADDLE_Y_COORD;
volatile uint32_t PADDLE2_X_COORD;
volatile uint32_t PADDLE2_Y_COORD;
volatile uint32_t PADDLE_PADDING = 20;
volatile int32_t PUCK_X_COORD = 120;
volatile int32_t PUCK_Y_COORD = TOP_PADDING;
volatile int32_t PUCK_DX = 1;
volatile int32_t PUCK_DY = 1;

static const Direction  MOV_DIR[] = {DIR_LEFT, DIR_RIGHT};

uint32_t draw_color;

typedef enum 
{
  DEBOUNCE_ONE,
  DEBOUNCE_1ST_ZERO,
  DEBOUNCE_2ND_ZERO,
  DEBOUNCE_PRESSED
} DEBOUNCE_STATES;

typedef enum {
	ALL_OFF,
	RED_ON,
	BLUE_ON,
	GREEN_ON
} LED_STATES;
static LED_STATES state = ALL_OFF;


bool sw2_debounce_fsm(void)
{
  static DEBOUNCE_STATES state = DEBOUNCE_ONE;
  bool pin_logic_level;
  
  pin_logic_level = lp_io_read_pin(SW2_BIT);
  
  switch (state)
  {
    case DEBOUNCE_ONE:
    {
      if(pin_logic_level)
      {
        state = DEBOUNCE_ONE;
      }
      else
      {
        state = DEBOUNCE_1ST_ZERO;
      }
      break;
    }
    case DEBOUNCE_1ST_ZERO:
    {
      if(pin_logic_level)
      {
        state = DEBOUNCE_ONE;
      }
      else
      {
        state = DEBOUNCE_2ND_ZERO;
      }
      break;
    }
    case DEBOUNCE_2ND_ZERO:
    {
      if(pin_logic_level)
      {
        state = DEBOUNCE_ONE;
      }
      else
      {
        state = DEBOUNCE_PRESSED;
      }
      break;
    }
    case DEBOUNCE_PRESSED:
    {
      if(pin_logic_level)
      {
        state = DEBOUNCE_ONE;
      }
      else
      {
        state = DEBOUNCE_PRESSED;
      }
      break;
    }
    default:
    {
      while(1){};
    }
  }
  
  if(state == DEBOUNCE_2ND_ZERO )
  {
    return true;
  }
  else
  {
    return false;
  }
}
void debounce_wait(void) 
{
  int i = 10000;
  // Delay
  while(i > 0)
  {
    i--;
  }
}
//*****************************************************************************
// This function is called at the beginning of each game. It allows players to
// chose the color of their paddles. It also reads data from the EEPROM.
// type - Void
// inputs - None
//*****************************************************************************
void start_screen(){
	wireless_com_status_t status;
	uint32_t data;
	uint16_t x_touch = 0;
	uint16_t y_touch = 0;
	uint8_t touch_event = 0;
	uint8_t touch_counter = 0;
	int index;
	uint16_t addr;
	uint8_t read_val;
	int a;
	
	printf("\n\r");
  printf("**************************************\n\r");
  printf("* ECE353 - Final Project\n\r");
  printf("**************************************\n\r");
  printf("\n\r");
	index = 0;
	printf("Reading EEPROM\n");
	
	
	for(addr = ADDR_START; addr <(ADDR_START+240); addr++)
	{
			eeprom_byte_read(I2C1_BASE,addr, &read_val);
			printf("%c",read_val);
			if(index == 79 || index == 159 || index == 239) {
				printf("\n");
			}
			index++;
	}
	eeprom_byte_read(I2C1_BASE,ADDR_START + 240, &read_val);
	printf("WINS: %d\n", read_val);
	eeprom_byte_read(I2C1_BASE,ADDR_START + 241, &read_val);
	printf("TIES: %d\n", read_val);
	eeprom_byte_read(I2C1_BASE,ADDR_START + 242, &read_val);
	printf("LOSSES: %d\n", read_val);
	lcd_draw_image(LCD_WIDTH/2,start_screenWidthPixels,LCD_HEIGHT/2,start_screenHeightPixels,start_screenBitmaps,LCD_COLOR_RED,LCD_COLOR_BLACK);
	while(!touch_start){
			touch_event = ft6x06_read_td_status();
			if(touch_event == 1 | touch_event == 2) {
				touch_counter++;
			} else {
				touch_counter = 0;
			}
			if(touch_counter > TOUCH_MIN){
				touch_start = true;
			}
	}
	lcd_clear_screen(LCD_COLOR_BLACK);
	//print select color
	lcd_draw_image(COLOR_START,6,LCD_HEIGHT/2,7,letter_S,LCD_COLOR_RED,LCD_COLOR_BLACK);
	lcd_draw_image(COLOR_START+7,6,LCD_HEIGHT/2,7,letter_e,LCD_COLOR_RED,LCD_COLOR_BLACK);
	lcd_draw_image(COLOR_START+14,6,LCD_HEIGHT/2,7,letter_l,LCD_COLOR_RED,LCD_COLOR_BLACK);
	lcd_draw_image(COLOR_START+21,6,LCD_HEIGHT/2,7,letter_e,LCD_COLOR_RED,LCD_COLOR_BLACK);
	lcd_draw_image(COLOR_START+28,6,LCD_HEIGHT/2,7,letter_c,LCD_COLOR_RED,LCD_COLOR_BLACK);
	lcd_draw_image(COLOR_START+35,6,LCD_HEIGHT/2,7,letter_t,LCD_COLOR_RED,LCD_COLOR_BLACK);
	lcd_draw_image(COLOR_START+49,6,LCD_HEIGHT/2,7,letter_c,LCD_COLOR_RED,LCD_COLOR_BLACK);
	lcd_draw_image(COLOR_START+56,6,LCD_HEIGHT/2,7,letter_o,LCD_COLOR_RED,LCD_COLOR_BLACK);
	lcd_draw_image(COLOR_START+63,6,LCD_HEIGHT/2,7,letter_l,LCD_COLOR_RED,LCD_COLOR_BLACK);
	lcd_draw_image(COLOR_START+70,6,LCD_HEIGHT/2,7,letter_o,LCD_COLOR_RED,LCD_COLOR_BLACK);
	lcd_draw_image(COLOR_START+77,6,LCD_HEIGHT/2,7,letter_r,LCD_COLOR_RED,LCD_COLOR_BLACK);
	//print four colors
	lcd_draw_image(60,colorboxWidthPixels,80,colorboxHeightPixels,colorboxBitmaps,LCD_COLOR_BLUE,LCD_COLOR_BLACK);
	lcd_draw_image(180,colorboxWidthPixels,80,colorboxHeightPixels,colorboxBitmaps,LCD_COLOR_RED,LCD_COLOR_BLACK);
	lcd_draw_image(60,colorboxWidthPixels,240,colorboxHeightPixels,colorboxBitmaps,LCD_COLOR_GREEN,LCD_COLOR_BLACK);
	lcd_draw_image(180,colorboxWidthPixels,240,colorboxHeightPixels,colorboxBitmaps,LCD_COLOR_YELLOW,LCD_COLOR_BLACK);
	for(a = 0; a < 1000000; a = a+1){}
	while(!color_selected){
		touch_event = ft6x06_read_td_status();
		x_touch = ft6x06_read_x();
		y_touch = ft6x06_read_y();
		if(touch_event == 1 | touch_event == 2) {
			touch_counter++;
		} else {
			touch_counter = 0;
		}
		if(touch_counter > TOUCH_MIN){
				color_selected = true;
				if(x_touch >= LCD_WIDTH/2){
					if(y_touch >= LCD_HEIGHT/2){
						draw_color = LCD_COLOR_YELLOW;
					}else{
						draw_color = LCD_COLOR_RED;
					}
				}else{
					if(y_touch < LCD_HEIGHT/2){
						draw_color = LCD_COLOR_BLUE;
					}else{
						draw_color = LCD_COLOR_GREEN;
					}
				}
			}
	}
	
}
//*****************************************************************************
// This function draws the current value of the game timer on the LCD
// type - Void
// input(s) - uint16_t time_value -> current value of game_timer
//*****************************************************************************
void draw_timer(uint16_t time_value){
	uint16_t last_digit = 0;
	uint16_t first_digit = 0;
	
	//draw the word "TIMER:" in the top right corner of LCD screen
	lcd_draw_image(WORD_START,TEN_PT_FONT_WIDTH,TIMER_HEIGHT,TEN_PT_FONT_HEIGHT,numbers[14],LCD_COLOR_RED,LCD_COLOR_BLACK);
	lcd_draw_image(WORD_START+12,TEN_PT_FONT_WIDTH,TIMER_HEIGHT,TEN_PT_FONT_HEIGHT,numbers[12],LCD_COLOR_RED,LCD_COLOR_BLACK);
	lcd_draw_image(WORD_START+22,TEN_PT_FONT_WIDTH,TIMER_HEIGHT,TEN_PT_FONT_HEIGHT,numbers[13],LCD_COLOR_RED,LCD_COLOR_BLACK);
	lcd_draw_image(WORD_START+32,TEN_PT_FONT_WIDTH,TIMER_HEIGHT,TEN_PT_FONT_HEIGHT,numbers[11],LCD_COLOR_RED,LCD_COLOR_BLACK);
	lcd_draw_image(WORD_START+42,TEN_PT_FONT_WIDTH,TIMER_HEIGHT,TEN_PT_FONT_HEIGHT,numbers[10],LCD_COLOR_RED,LCD_COLOR_BLACK);	
	
	//determine if the time remaining is over a minute
	if(time_value >= 60){
		last_digit = (time_value-60) %10; 		//grab last digit of time_value (in seconds)
		first_digit = (time_value-60) / 10;  	//grab first digit of time_value (in seconds)
		//Draw time remaining in form 0:00
		lcd_draw_image(WORD_START+52,TEN_PT_FONT_WIDTH,TIMER_HEIGHT,TEN_PT_FONT_HEIGHT,numbers[1],LCD_COLOR_RED,LCD_COLOR_BLACK);
		lcd_draw_image(WORD_START+62,TEN_PT_FONT_WIDTH,TIMER_HEIGHT,TEN_PT_FONT_HEIGHT,numbers[10],LCD_COLOR_RED,LCD_COLOR_BLACK);
		lcd_draw_image(WORD_START+72,TEN_PT_FONT_WIDTH,TIMER_HEIGHT,TEN_PT_FONT_HEIGHT,numbers[first_digit],LCD_COLOR_RED,LCD_COLOR_BLACK);
		lcd_draw_image(WORD_START+82,TEN_PT_FONT_WIDTH,TIMER_HEIGHT,TEN_PT_FONT_HEIGHT,numbers[last_digit],LCD_COLOR_RED,LCD_COLOR_BLACK);
	}else{
		last_digit = time_value %10;			//grab last digit of time_value (in seconds)
		first_digit = time_value / 10;		//grab first digit of time_value (in seconds)
		//Draw time remaining in form 0:00
		lcd_draw_image(WORD_START+52,TEN_PT_FONT_WIDTH,TIMER_HEIGHT,TEN_PT_FONT_HEIGHT,numbers[0],LCD_COLOR_RED,LCD_COLOR_BLACK);
		lcd_draw_image(WORD_START+62,TEN_PT_FONT_WIDTH,TIMER_HEIGHT,TEN_PT_FONT_HEIGHT,numbers[10],LCD_COLOR_RED,LCD_COLOR_BLACK);
		lcd_draw_image(WORD_START+72,TEN_PT_FONT_WIDTH,TIMER_HEIGHT,TEN_PT_FONT_HEIGHT,numbers[first_digit],LCD_COLOR_RED,LCD_COLOR_BLACK);
		lcd_draw_image(WORD_START+82,TEN_PT_FONT_WIDTH,TIMER_HEIGHT,TEN_PT_FONT_HEIGHT,numbers[last_digit],LCD_COLOR_RED,LCD_COLOR_BLACK);	
	}
}
//*****************************************************************************
// This function draws the current score of the game timer on the LCD
// type - Void
// input(s) - uint16_t my_score -> current score of player,
// uint16_t thier_score -> current score of opposing player
//*****************************************************************************
void draw_score(uint16_t my_score, uint16_t their_score){
		lcd_draw_image(SCORE_X,TEN_PT_FONT_WIDTH,SCORE_Y,TEN_PT_FONT_HEIGHT,numbers[13],draw_color,LCD_COLOR_BLACK);											//M
		lcd_draw_image(SCORE_X+10,TEN_PT_FONT_WIDTH,SCORE_Y,TEN_PT_FONT_HEIGHT,numbers[11],draw_color,LCD_COLOR_BLACK);										//E
		lcd_draw_image(SCORE_X+20,TEN_PT_FONT_WIDTH,SCORE_Y,TEN_PT_FONT_HEIGHT,numbers[10],draw_color,LCD_COLOR_BLACK);										//:
		lcd_draw_image(SCORE_X+30,TEN_PT_FONT_WIDTH,SCORE_Y,TEN_PT_FONT_HEIGHT,numbers[my_score],LCD_COLOR_WHITE,LCD_COLOR_BLACK);				//"score"
		lcd_draw_image(SCORE_X+45,TEN_PT_FONT_WIDTH,SCORE_Y,TEN_PT_FONT_HEIGHT,numbers[15],LCD_COLOR_RED,LCD_COLOR_BLACK);								//O
		lcd_draw_image(SCORE_X+57,TEN_PT_FONT_WIDTH,SCORE_Y,TEN_PT_FONT_HEIGHT,numbers[16],LCD_COLOR_RED,LCD_COLOR_BLACK);								//P
		lcd_draw_image(SCORE_X+67,TEN_PT_FONT_WIDTH,SCORE_Y,TEN_PT_FONT_HEIGHT,numbers[16],LCD_COLOR_RED,LCD_COLOR_BLACK);								//P
		lcd_draw_image(SCORE_X+77,TEN_PT_FONT_WIDTH,SCORE_Y,TEN_PT_FONT_HEIGHT,numbers[10],LCD_COLOR_RED,LCD_COLOR_BLACK);								//:
		lcd_draw_image(SCORE_X+87,TEN_PT_FONT_WIDTH,SCORE_Y,TEN_PT_FONT_HEIGHT,numbers[their_score],LCD_COLOR_WHITE,LCD_COLOR_BLACK);			//"score"
}
//*****************************************************************************
// This function updates the x and y coordinates of the paddle
// Type - Void
// input(s) - Direction direction -> direction to move the paddle
//					- uint32_t *x_coord -> location of x_coord variable
//					- uint32_t *y_coord -> location of y_coord variable
//*****************************************************************************
void move_image(
        volatile Direction direction,
        volatile uint32_t *x_coord, 
        volatile uint32_t *y_coord 
)
{ 
	move_paddle = true;
	switch(direction)
	{
		case DIR_LEFT:
			*x_coord = *x_coord - 1;
			break;
		case DIR_RIGHT:
			*x_coord = *x_coord + 1;
			break;
	}
}

//*****************************************************************************
// This function updates the x and y coordinates of the puck and
// direction of the puck as well as change the speed of the puck if the puck is
// fast and contact a paddle
//*****************************************************************************
void update_puck(){
	int32_t diff;
	move_puck = true;
	// check if the player concede a goal
	if(PUCK_Y_COORD + puckHeightPixels/2 >= LCD_HEIGHT - 1){
		pause = true;
		fast = false;
		speed_count = 5;
		scored++;
		lcd_draw_image(PUCK_X_COORD,puckWidthPixels,PUCK_Y_COORD,puckHeightPixels,puckBitmaps,LCD_COLOR_BLACK,LCD_COLOR_BLACK);
		PUCK_X_COORD = LCD_WIDTH/2;
		PUCK_Y_COORD = TOP_PADDING + puckHeightPixels/2;
		PUCK_DX = 1;
		PUCK_DY = 1;
		// update and draw the score
		opponent_score++;
		draw_score(my_score, opponent_score);
		return;
	// check if the puck pass the half-field line
	// the player will no longer be able to see the puck until the puck comes back
	} else if(PUCK_Y_COORD  - puckHeightPixels/2 <= TOP_PADDING && PUCK_DY == -1){
		lcd_draw_image(PUCK_X_COORD,puckWidthPixels,PUCK_Y_COORD,puckHeightPixels,puckBitmaps,LCD_COLOR_BLACK,LCD_COLOR_BLACK);
		// have to send the pos of the puck to the other board
		send = true;
		puck_here = false;
		return;
	} else if(PUCK_Y_COORD + puckHeightPixels/2 + paddleHeightPixels + PADDLE_PADDING >= LCD_HEIGHT - 1){
		// top of the paddle
		if(PUCK_DY > 0) {
			if(PUCK_X_COORD + puckWidthPixels/2 >= (PADDLE_X_COORD - paddleWidthPixels/2) && PUCK_X_COORD - puckWidthPixels/2 <= (PADDLE_X_COORD + paddleWidthPixels/2)) {
				speed_count = 5;
				fast = false;
				power = (power == 0) ? 1 : (power << 1) + 1;
				MCP23017_write_leds(power);
				diff = PUCK_X_COORD - PADDLE_X_COORD;
				if(diff > paddleWidthPixels/4) {
					// bounce to right
					PUCK_DX = 1;
				} else if( diff < -(int32_t)paddleWidthPixels/4 ) {
					// bounce to left
					PUCK_DX = -1;
				} else {
					// bounce to center
					PUCK_DX = 0;
				}
				PUCK_DY = -PUCK_DY;	
			} 
		}
	}else if(PUCK_X_COORD - puckWidthPixels/2 <= 0 || 
		(PUCK_X_COORD - puckWidthPixels/2 <= PADDLE_X_COORD + paddleWidthPixels/2 && PADDLE_Y_COORD - paddleHeightPixels/2 <= PUCK_Y_COORD + puckHeightPixels/2 )){
		// left edge of the screen or right of paddle
		if(PUCK_DX < 0) {
			PUCK_DX = -PUCK_DX;
		}
	} else if(PUCK_X_COORD + puckWidthPixels/2 >= LCD_WIDTH - 1 ||
			(PUCK_X_COORD + puckWidthPixels/2 >= PADDLE_X_COORD - paddleWidthPixels/2 && PADDLE_Y_COORD - paddleHeightPixels/2 <= PUCK_Y_COORD + puckHeightPixels/2 )){
		// right edge of the screen or left of paddle
		if(PUCK_DX > 0) {
			PUCK_DX = -PUCK_DX;
		}
	} else if(PUCK_Y_COORD + puckHeightPixels/2 + paddleHeightPixels + PADDLE_PADDING >= LCD_HEIGHT - 1){
		// top of the paddle
		if(PUCK_DY > 0) {
			if(PUCK_X_COORD + puckWidthPixels/2 >= (PADDLE_X_COORD - paddleWidthPixels/2) && PUCK_X_COORD - puckWidthPixels/2 <= (PADDLE_X_COORD + paddleWidthPixels/2)) {
				speed_count = 5;
				fast = false;
				power = (power == 0) ? 1 : (power << 1) + 1;
				MCP23017_write_leds(power);
				diff = PUCK_X_COORD - PADDLE_X_COORD;
				if(diff > paddleWidthPixels/4) {
					// bounce to right
					PUCK_DX = 1;
				} else if( diff < -(int32_t)paddleWidthPixels/4 ) {
					// bounce to left
					PUCK_DX = -1;
				} else {
					// bounce to center
					PUCK_DX = 0;
				}
				PUCK_DY = -PUCK_DY;	
			} 
		}
	}
	PUCK_X_COORD += PUCK_DX;
	PUCK_Y_COORD += PUCK_DY;
}


//*****************************************************************************
// This function check the acceloremeter to determine if the direction of 
// the paddle have to be changed
//*****************************************************************************
void update_paddle(){
		spi_select(MODULE_1);
		x_data = accel_read_x();	
		
		// to the left
		if(x_data > 3000){
			if(PADDLE_X_COORD - paddleWidthPixels/2 > 0)
				move_image(DIR_LEFT,&PADDLE_X_COORD,&PADDLE_Y_COORD);	
		// to the right
		}else if(x_data < -3000){
			if(PADDLE_X_COORD + paddleWidthPixels/2 < LCD_WIDTH - 1)
				move_image(DIR_RIGHT,&PADDLE_X_COORD,&PADDLE_Y_COORD);	
		}
		move_paddle = true;
}

//*****************************************************************************
// This function tries to received the info of the puck if there is any
//*****************************************************************************
void receive() {
		uint32_t receive_data;
		wireless_com_status_t status;
		spi_select(NORDIC);
		status = wireless_get_32(false, &receive_data);
		if(status == NRF24L01_RX_SUCCESS) {
			bytes_received++;
			scored = 0;
			puck_here = true;
			PUCK_X_COORD = receive_data & 0xFF;
			PUCK_X_COORD = LCD_WIDTH - 1 - PUCK_X_COORD;
			PUCK_Y_COORD = TOP_PADDING + puckHeightPixels/2;
			// determine the x-direction of the puck
			if(receive_data & MID_DIR_M) {
				PUCK_DX = 0;
			} else if(receive_data & X_DIR_M) {
				PUCK_DX = -1;
			} else {
				PUCK_DX = 1;	
			}						
			PUCK_DY = 1;
			// determine if you scored one more points
			my_score += (receive_data & SCORED_M) ? (receive_data & SCORED_M) >> SCORED_SH : 0;
			draw_score(my_score, opponent_score);
			// determine if the puck is in fast mode and update the speed
			fast = (receive_data & FAST_M) ? true : false;
			speed_count = fast ? 2 : 5;
		}
}

//*****************************************************************************
// This function send the info of the puck when it get pass to the other 
// player's side consisting of the x-pos, direction, speed, and score that
// have been changed
//*****************************************************************************
void transmit() {
	uint32_t send_data;
	wireless_com_status_t status;
	if(send) {
		//
		send_data = (PUCK_X_COORD & 0xFF) | (PUCK_DX == 0 ? MID_DIR_M : 0) | (PUCK_DX == 1 ? X_DIR_M : 0) | 
		(fast ? FAST_M : 0) | ((scored << SCORED_SH) & SCORED_M) | (fast ? FAST_M : 0);
		spi_select(NORDIC);
		status = wireless_send_32(true, true, send_data);
		if(status == NRF24L01_TX_SUCCESS) {
				bytes_sent++;
		}
		scored = 0;
		send = false;
		puck_here = false;
	}	
}
//*****************************************************************************
// This function is called after the player has choosen the color and is ready
// to start the game. It first transmits to the opposing player that the player
// is ready. It then listens for the a transmision from the opposing player. Once
// a transmision is recieved the funtion exits.

// While the player is waiting it displays a waiting annimation
// Type - Void
// input(s) - none
//*****************************************************************************
void wait_screen(){
	int wait_count = 0;      				//used to animate the wait screen
	//int loop_counter = 0;					
	wireless_com_status_t status;		//wireless com status variable
	bool recieved = false;					//check is message from opposing player has been revieved
	uint32_t data;									//stores data recieved from opposing player
	
	lcd_clear_screen(LCD_COLOR_BLACK);
	spi_select(NORDIC);
	status = wireless_send_32(true, true, 1);			//send any data to opposing player to notify that this player is ready to start game
	if(status == NRF24L01_TX_SUCCESS) {
			bytes_sent++;															//update number of bytes sent
	}
	
	//draw "WAITING"
	lcd_draw_image(WAITING_START,FT_PT_FONT_WIDTH,LCD_HEIGHT/2,FT_PT_FONT_HEIGHT,text14ptBitmaps[24],LCD_COLOR_RED,LCD_COLOR_BLACK);
	lcd_draw_image(WAITING_START+21,FT_PT_FONT_WIDTH,LCD_HEIGHT/2,FT_PT_FONT_HEIGHT,text14ptBitmaps[2],LCD_COLOR_RED,LCD_COLOR_BLACK);
	lcd_draw_image(WAITING_START+42,FT_PT_FONT_WIDTH,LCD_HEIGHT/2,FT_PT_FONT_HEIGHT,text14ptBitmaps[10],LCD_COLOR_RED,LCD_COLOR_BLACK);
	lcd_draw_image(WAITING_START+63,FT_PT_FONT_WIDTH,LCD_HEIGHT/2,FT_PT_FONT_HEIGHT,text14ptBitmaps[21],LCD_COLOR_RED,LCD_COLOR_BLACK);
	lcd_draw_image(WAITING_START+85,FT_PT_FONT_WIDTH,LCD_HEIGHT/2,FT_PT_FONT_HEIGHT,text14ptBitmaps[10],LCD_COLOR_RED,LCD_COLOR_BLACK);
	lcd_draw_image(WAITING_START+107,FT_PT_FONT_WIDTH,LCD_HEIGHT/2,FT_PT_FONT_HEIGHT,text14ptBitmaps[15],LCD_COLOR_RED,LCD_COLOR_BLACK);
	lcd_draw_image(WAITING_START+129,FT_PT_FONT_WIDTH,LCD_HEIGHT/2,FT_PT_FONT_HEIGHT,text14ptBitmaps[8],LCD_COLOR_RED,LCD_COLOR_BLACK);
	
	//wait for data from opposing player to be recieved
	do{
		spi_select(NORDIC);
		status = wireless_get_32(false, &data);
		printf("%s\n\r",wireless_error_messages[status]);
		//animate waiting image every one second
		if(AlertOneSec){
			AlertOneSec = false;
			switch(wait_count){
				case 0:
					lcd_draw_image(WAITING_START+151,FT_PT_FONT_WIDTH,LCD_HEIGHT/2,FT_PT_FONT_HEIGHT,text14ptBitmaps[0],LCD_COLOR_BLACK,LCD_COLOR_BLACK);
					lcd_draw_image(WAITING_START+173,FT_PT_FONT_WIDTH,LCD_HEIGHT/2,FT_PT_FONT_HEIGHT,text14ptBitmaps[0],LCD_COLOR_BLACK,LCD_COLOR_BLACK);
					lcd_draw_image(WAITING_START+194,FT_PT_FONT_WIDTH,LCD_HEIGHT/2,FT_PT_FONT_HEIGHT,text14ptBitmaps[0],LCD_COLOR_BLACK,LCD_COLOR_BLACK);
					wait_count++;
					break;
				case 1:
					lcd_draw_image(WAITING_START+151,FT_PT_FONT_WIDTH,LCD_HEIGHT/2,FT_PT_FONT_HEIGHT,text14ptBitmaps[0],LCD_COLOR_RED,LCD_COLOR_BLACK);
					wait_count++;
					break;
				case 2:
					lcd_draw_image(WAITING_START+151,FT_PT_FONT_WIDTH,LCD_HEIGHT/2,FT_PT_FONT_HEIGHT,text14ptBitmaps[0],LCD_COLOR_RED,LCD_COLOR_BLACK);
					lcd_draw_image(WAITING_START+173,FT_PT_FONT_WIDTH,LCD_HEIGHT/2,FT_PT_FONT_HEIGHT,text14ptBitmaps[0],LCD_COLOR_RED,LCD_COLOR_BLACK);
					wait_count++;
					break;
				case 3:
					lcd_draw_image(WAITING_START+151,FT_PT_FONT_WIDTH,LCD_HEIGHT/2,FT_PT_FONT_HEIGHT,text14ptBitmaps[0],LCD_COLOR_RED,LCD_COLOR_BLACK);
					lcd_draw_image(WAITING_START+173,FT_PT_FONT_WIDTH,LCD_HEIGHT/2,FT_PT_FONT_HEIGHT,text14ptBitmaps[0],LCD_COLOR_RED,LCD_COLOR_BLACK);
					lcd_draw_image(WAITING_START+194,FT_PT_FONT_WIDTH,LCD_HEIGHT/2,FT_PT_FONT_HEIGHT,text14ptBitmaps[0],LCD_COLOR_RED,LCD_COLOR_BLACK);
					wait_count = 0;
					break;
			}		
		}	
	}while(status != NRF24L01_RX_SUCCESS);
	bytes_received++;
}
//*****************************************************************************
// This function is called whe the game timer reaches zero. It displays the
// winner and updates the record of Wins/losses/ties in the EEPROM. It then 
// enters an infinite loop
// Type - Void
// input(s) - uint16_t my_score, uint16_t their_score
//*****************************************************************************
void game_over(uint16_t my_score, uint16_t their_score){
	//bool pressed = false;
	//uint8_t values[20];
	uint16_t addr;						//address to write to EEPROM
	uint8_t read_val;					//value read from EEPROM
	int8_t result;						//records if game was a win, loss or tie
	
	
	//Draw "game_over"
	lcd_draw_image(GAME_OVER_X,FT_PT_FONT_WIDTH,GAME_OVER_Y,FT_PT_FONT_HEIGHT,text14ptBitmaps[8],LCD_COLOR_RED,LCD_COLOR_BLACK);
	lcd_draw_image(GAME_OVER_X+15,FT_PT_FONT_WIDTH,GAME_OVER_Y,FT_PT_FONT_HEIGHT,text14ptBitmaps[2],LCD_COLOR_RED,LCD_COLOR_BLACK);
	lcd_draw_image(GAME_OVER_X+30,FT_PT_FONT_WIDTH,GAME_OVER_Y,FT_PT_FONT_HEIGHT,text14ptBitmaps[14],LCD_COLOR_RED,LCD_COLOR_BLACK);
	lcd_draw_image(GAME_OVER_X+45,FT_PT_FONT_WIDTH,GAME_OVER_Y,FT_PT_FONT_HEIGHT,text14ptBitmaps[6],LCD_COLOR_RED,LCD_COLOR_BLACK);
	lcd_draw_image(GAME_OVER_X+70,FT_PT_FONT_WIDTH,GAME_OVER_Y,FT_PT_FONT_HEIGHT,text14ptBitmaps[16],LCD_COLOR_RED,LCD_COLOR_BLACK);	
	lcd_draw_image(GAME_OVER_X+85,FT_PT_FONT_WIDTH,GAME_OVER_Y,FT_PT_FONT_HEIGHT,text14ptBitmaps[23],LCD_COLOR_RED,LCD_COLOR_BLACK);
	lcd_draw_image(GAME_OVER_X+100,FT_PT_FONT_WIDTH,GAME_OVER_Y,FT_PT_FONT_HEIGHT,text14ptBitmaps[6],LCD_COLOR_RED,LCD_COLOR_BLACK);
	lcd_draw_image(GAME_OVER_X+115,FT_PT_FONT_WIDTH,GAME_OVER_Y,FT_PT_FONT_HEIGHT,text14ptBitmaps[19],LCD_COLOR_RED,LCD_COLOR_BLACK);
	
	// Draw "you"
	lcd_draw_image(GAME_OVER_X+15,FT_PT_FONT_WIDTH,GAME_OVER_Y+20,FT_PT_FONT_HEIGHT,text14ptBitmaps[26],LCD_COLOR_RED,LCD_COLOR_BLACK);
	lcd_draw_image(GAME_OVER_X+30,FT_PT_FONT_WIDTH,GAME_OVER_Y+20,FT_PT_FONT_HEIGHT,text14ptBitmaps[16],LCD_COLOR_RED,LCD_COLOR_BLACK);
	lcd_draw_image(GAME_OVER_X+45,FT_PT_FONT_WIDTH,GAME_OVER_Y+20,FT_PT_FONT_HEIGHT,text14ptBitmaps[22],LCD_COLOR_RED,LCD_COLOR_BLACK);
	
	//if game was won, draw "win", if lost draw "lost", if tied draw "tie"
	if(my_score > their_score){
	result = 1;
	// draw win
	lcd_draw_image(GAME_OVER_X+70,FT_PT_FONT_WIDTH,GAME_OVER_Y+20,FT_PT_FONT_HEIGHT,text14ptBitmaps[24],LCD_COLOR_GREEN,LCD_COLOR_BLACK);
	lcd_draw_image(GAME_OVER_X+85,FT_PT_FONT_WIDTH,GAME_OVER_Y+20,FT_PT_FONT_HEIGHT,text14ptBitmaps[10],LCD_COLOR_GREEN,LCD_COLOR_BLACK);
	lcd_draw_image(GAME_OVER_X+100,FT_PT_FONT_WIDTH,GAME_OVER_Y+20,FT_PT_FONT_HEIGHT,text14ptBitmaps[15],LCD_COLOR_GREEN,LCD_COLOR_BLACK);
	}else if(my_score < their_score){
	//lose
	result = -1;
		// draw lost
	lcd_draw_image(GAME_OVER_X+70,FT_PT_FONT_WIDTH,GAME_OVER_Y+20,FT_PT_FONT_HEIGHT,text14ptBitmaps[13],LCD_COLOR_RED,LCD_COLOR_BLACK);
	lcd_draw_image(GAME_OVER_X+85,FT_PT_FONT_WIDTH,GAME_OVER_Y+20,FT_PT_FONT_HEIGHT,text14ptBitmaps[16],LCD_COLOR_RED,LCD_COLOR_BLACK);
	lcd_draw_image(GAME_OVER_X+100,FT_PT_FONT_WIDTH,GAME_OVER_Y+20,FT_PT_FONT_HEIGHT,text14ptBitmaps[20],LCD_COLOR_RED,LCD_COLOR_BLACK);
	lcd_draw_image(GAME_OVER_X+115,FT_PT_FONT_WIDTH,GAME_OVER_Y+20,FT_PT_FONT_HEIGHT,text14ptBitmaps[6],LCD_COLOR_RED,LCD_COLOR_BLACK);
	}else{
	//tie
	result = 0;
		// draw tied
	lcd_draw_image(GAME_OVER_X+70,FT_PT_FONT_WIDTH,GAME_OVER_Y+20,FT_PT_FONT_HEIGHT,text14ptBitmaps[21],LCD_COLOR_CYAN,LCD_COLOR_BLACK);
	lcd_draw_image(GAME_OVER_X+85,FT_PT_FONT_WIDTH,GAME_OVER_Y+20,FT_PT_FONT_HEIGHT,text14ptBitmaps[10],LCD_COLOR_CYAN,LCD_COLOR_BLACK);
	lcd_draw_image(GAME_OVER_X+100,FT_PT_FONT_WIDTH,GAME_OVER_Y+20,FT_PT_FONT_HEIGHT,text14ptBitmaps[6],LCD_COLOR_CYAN,LCD_COLOR_BLACK);
	lcd_draw_image(GAME_OVER_X+115,FT_PT_FONT_WIDTH,GAME_OVER_Y+20,FT_PT_FONT_HEIGHT,text14ptBitmaps[5],LCD_COLOR_CYAN,LCD_COLOR_BLACK);
	}
	//update records in EEPROM
	if(result == 1) {
		addr = ADDR_START + 240;
		eeprom_byte_read(I2C1_BASE,addr, &read_val);
		eeprom_byte_write(I2C1_BASE,addr, read_val + 1);
	}
	if(result == 0) {
		addr = ADDR_START + 241;
		eeprom_byte_read(I2C1_BASE,addr, &read_val);
		eeprom_byte_write(I2C1_BASE,addr, read_val + 1);
	}
	if(result == -1) {
		addr = ADDR_START + 242;
		eeprom_byte_read(I2C1_BASE,addr, &read_val);
		eeprom_byte_write(I2C1_BASE,addr, read_val + 1);
	}
		while(1){
		
		}
}		
//*****************************************************************************
// This function initializes the data in EEPROM at the addresses used to zero
// should only be used the first time the a board is loaded with this program
// Type - Void
// input(s) - None
//*****************************************************************************
void init_EEPROM() {
	uint16_t addr;
	for(addr = ADDR_START; addr <(ADDR_START+243); addr++)
	{
			eeprom_byte_write(I2C1_BASE,addr, 0);
	}
}
void hockey_main(){
	
	uint16_t addr;
  uint8_t values[20];
  uint8_t read_val;
	uint8_t push_val;
	uint8_t touch_event;
	int16_t accel;
	uint16_t game_timer;
	uint32_t i;
	bool pressed = false;
	int16_t x,y,z;
	char msg[80];
	int index;
	
	// wireless demo var
  uint32_t data;
  bool validate;	 
	i = 0;
	//init_EEPROM();
	
	game_timer = 75;
		
	lcd_clear_screen(LCD_COLOR_BLACK);
	
	// intialize the pos of the paddle to be the center
	PADDLE_X_COORD = 120;
	PADDLE_Y_COORD = 319 - paddleHeightPixels/2 - PADDLE_PADDING;	
	
	// draw border and game timer
	lcd_draw_image(LCD_WIDTH/2,borderWidthPixels,BORDER_HEIGHT,borderHeightPixels,borderBitmaps,draw_color,LCD_COLOR_BLACK);
	draw_timer(game_timer);
	
	// determine whether the player is the person who get the puck first
	puck_here = (SEND_FIRST ? true : false);		
	if(SEND_FIRST)
		lcd_draw_image(PUCK_X_COORD,puckWidthPixels,PUCK_Y_COORD,puckHeightPixels,puckBitmaps,LCD_COLOR_RED,LCD_COLOR_BLACK);
	// draw the initial paddle pos
	lcd_draw_image(PADDLE_X_COORD,paddleWidthPixels,PADDLE_Y_COORD,paddleHeightPixels,paddleBitmaps,draw_color,LCD_COLOR_BLACK);
	
	draw_score(0,0);
	speed_count = 5;

	// keep playing when the timer is not over
	while(game_timer > 0){
		spi_select(MODULE_1);
		
		// read the acceloremeter every 8 ms
		if(get_x_data){
			get_x_data = false;
			// try getting data from other board
			receive();
			update_paddle();		
		}
		// draw paddle if the pos have been changed
		if(move_paddle){
			lcd_draw_image(PADDLE_X_COORD,paddleWidthPixels,PADDLE_Y_COORD,paddleHeightPixels,paddleBitmaps,draw_color,LCD_COLOR_BLACK);
			move_paddle = false;	
		}
		// update puck when the timer interrupt and the puck is in the player's side
		if(draw_puck){
			draw_puck = false;
			if(puck_here && !pause) {
				update_puck();		
				move_puck = true;					
			}		
		}
		// draw the puck if the pos has been changed
		if(move_puck && puck_here && !pause){
				DisableInterrupts();
				lcd_draw_image(PUCK_X_COORD,puckWidthPixels,PUCK_Y_COORD,puckHeightPixels,puckBitmaps,LCD_COLOR_RED,LCD_COLOR_BLACK);
				EnableInterrupts();
				move_puck = false;	
		}
		// display timer every one second
		if(AlertOneSec){
			game_timer = game_timer - 1;
			draw_timer(game_timer); 
			// stop the game when timer is up
			if(game_timer == 0)
				break;
			AlertOneSec = false;					
		}
		
		if(button_pushed) {
			button_pushed = false;
			push_buttons = MCP23017_read_push_buttons();
			// increase the speed of the puck
			if(push_buttons == UP_BUTTON && power == FULL_POWER){
				power = 0;
				fast = true;
				speed_count = 2;
				MCP23017_write_leds(0x00);
			// release the ball after concede a goal
			} else if(push_buttons == DOWN_BUTTON) {
				pause = false;
			}
			EnableInterrupts();
		}
		// print bytes received and sent every
		if(print_bytes) {
			print_bytes = false;
			printf("bytes_sent : %d\n", bytes_sent);
			printf("bytes_received: %d\n", bytes_received);
		}
		// write data to the eeprom
		DisableInterrupts();
		debounce_wait();
		pressed = sw2_debounce_fsm();		
		if(pressed){
				eeprom_byte_write(I2C1_BASE,addr, values[addr-ADDR_START]);
				printf("Writing \n");	
				index = 0;
				for(addr = ADDR_START; addr <(ADDR_START+80); addr++)
				{
						printf("%c",first_line[index]);
						eeprom_byte_write(I2C1_BASE, addr, first_line[index]);
						index++;
				}
				printf("\n");
				index = 0;
				for(addr = ADDR_START+80; addr <(ADDR_START+160); addr++)
				{
						printf("%c",second_line[index]);
						eeprom_byte_write(I2C1_BASE, addr, second_line[index]);
						index++;
				}
				printf("\n");
				index = 0;
				for(addr = ADDR_START+160; addr <(ADDR_START+240); addr++)
				{
						printf("%c",third_line[index]);
						eeprom_byte_write(I2C1_BASE, addr, third_line[index]);
						index++;
				}
				printf("\n");
		}
		EnableInterrupts();				
		// send the data to other board if required
		transmit();		
	}
	
	// display the game over screen
	game_over(my_score,opponent_score);

}
