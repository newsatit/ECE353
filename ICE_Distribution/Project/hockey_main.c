#include "hockey_main.h"

// for wireless

#define LCD_WIDTH 240
#define LCD_HEIGHT 320
#define WORD_START 150
#define TIMER_HEIGHT 10
#define	COLOR_START 80
#define BORDER_HEIGHT 20
#define START_WIDTH 85
#define START_HEIGHT 160
#define TOUCH_MIN 200
#define TOP_PADDING 30
#define TEN_PT_FONT_WIDTH 15
#define TEN_PT_FONT_HEIGHT 16
#define SCORE_X 10
#define SCORE_Y 10
#define FT_PT_FONT_WIDTH 21
#define FT_PT_FONT_HEIGHT 21
#define WAITING_START 20
#define UP_BUTTON 254
#define DOWN_BUTTON 253
#define FULL_POWER 0xFF

/******************************************************************************
 * Global Variables
 *****************************************************************************/

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


bool sw1_debounce_fsm(void)
{
  static DEBOUNCE_STATES state = DEBOUNCE_ONE;
  bool pin_logic_level;
  
  pin_logic_level = lp_io_read_pin(SW1_BIT);
  
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
void start_screen(){
	wireless_com_status_t status;
	uint32_t data;
	uint16_t x_touch = 0;
	uint16_t y_touch = 0;
	uint8_t touch_event = 0;
	uint8_t touch_counter = 0;
	int a;
	//lcd_draw_image(START_WIDTH,15,START_HEIGHT,13,start[2],LCD_COLOR_RED,LCD_COLOR_BLACK);
	//lcd_draw_image(START_WIDTH+16,15,START_HEIGHT,13,start[3],LCD_COLOR_RED,LCD_COLOR_BLACK);
	//lcd_draw_image(START_WIDTH+32,15,START_HEIGHT,13,start[0],LCD_COLOR_RED,LCD_COLOR_BLACK);
	//lcd_draw_image(START_WIDTH+48,15,START_HEIGHT,13,start[1],LCD_COLOR_RED,LCD_COLOR_BLACK);
	//lcd_draw_image(START_WIDTH+64,15,START_HEIGHT,13,start[3],LCD_COLOR_RED,LCD_COLOR_BLACK);
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
void draw_timer(uint16_t time_value){
	uint16_t last_digit = 0;
	uint16_t first_digit = 0;
	
	//draw TIMER:
	lcd_draw_image(WORD_START,TEN_PT_FONT_WIDTH,TIMER_HEIGHT,TEN_PT_FONT_HEIGHT,numbers[14],LCD_COLOR_RED,LCD_COLOR_BLACK);
	lcd_draw_image(WORD_START+12,TEN_PT_FONT_WIDTH,TIMER_HEIGHT,TEN_PT_FONT_HEIGHT,numbers[12],LCD_COLOR_RED,LCD_COLOR_BLACK);
	lcd_draw_image(WORD_START+22,TEN_PT_FONT_WIDTH,TIMER_HEIGHT,TEN_PT_FONT_HEIGHT,numbers[13],LCD_COLOR_RED,LCD_COLOR_BLACK);
	lcd_draw_image(WORD_START+32,TEN_PT_FONT_WIDTH,TIMER_HEIGHT,TEN_PT_FONT_HEIGHT,numbers[11],LCD_COLOR_RED,LCD_COLOR_BLACK);
	lcd_draw_image(WORD_START+42,TEN_PT_FONT_WIDTH,TIMER_HEIGHT,TEN_PT_FONT_HEIGHT,numbers[10],LCD_COLOR_RED,LCD_COLOR_BLACK);	
	
	if(time_value >= 60){
		last_digit = (time_value-60) %10;
		first_digit = (time_value-60) / 10;
		lcd_draw_image(WORD_START+52,TEN_PT_FONT_WIDTH,TIMER_HEIGHT,TEN_PT_FONT_HEIGHT,numbers[1],LCD_COLOR_RED,LCD_COLOR_BLACK);
		lcd_draw_image(WORD_START+62,TEN_PT_FONT_WIDTH,TIMER_HEIGHT,TEN_PT_FONT_HEIGHT,numbers[10],LCD_COLOR_RED,LCD_COLOR_BLACK);
		lcd_draw_image(WORD_START+72,TEN_PT_FONT_WIDTH,TIMER_HEIGHT,TEN_PT_FONT_HEIGHT,numbers[first_digit],LCD_COLOR_RED,LCD_COLOR_BLACK);
		lcd_draw_image(WORD_START+82,TEN_PT_FONT_WIDTH,TIMER_HEIGHT,TEN_PT_FONT_HEIGHT,numbers[last_digit],LCD_COLOR_RED,LCD_COLOR_BLACK);
	}else{
		last_digit = time_value %10;
		first_digit = time_value / 10;
		lcd_draw_image(WORD_START+52,TEN_PT_FONT_WIDTH,TIMER_HEIGHT,TEN_PT_FONT_HEIGHT,numbers[0],LCD_COLOR_RED,LCD_COLOR_BLACK);
		lcd_draw_image(WORD_START+62,TEN_PT_FONT_WIDTH,TIMER_HEIGHT,TEN_PT_FONT_HEIGHT,numbers[10],LCD_COLOR_RED,LCD_COLOR_BLACK);
		lcd_draw_image(WORD_START+72,TEN_PT_FONT_WIDTH,TIMER_HEIGHT,TEN_PT_FONT_HEIGHT,numbers[first_digit],LCD_COLOR_RED,LCD_COLOR_BLACK);
		lcd_draw_image(WORD_START+82,TEN_PT_FONT_WIDTH,TIMER_HEIGHT,TEN_PT_FONT_HEIGHT,numbers[last_digit],LCD_COLOR_RED,LCD_COLOR_BLACK);	
	}
}
void draw_score(uint16_t my_score, uint16_t their_score){
		lcd_draw_image(SCORE_X,TEN_PT_FONT_WIDTH,SCORE_Y,TEN_PT_FONT_HEIGHT,numbers[13],draw_color,LCD_COLOR_BLACK);
		lcd_draw_image(SCORE_X+10,TEN_PT_FONT_WIDTH,SCORE_Y,TEN_PT_FONT_HEIGHT,numbers[11],draw_color,LCD_COLOR_BLACK);
		lcd_draw_image(SCORE_X+20,TEN_PT_FONT_WIDTH,SCORE_Y,TEN_PT_FONT_HEIGHT,numbers[10],draw_color,LCD_COLOR_BLACK);
		lcd_draw_image(SCORE_X+30,TEN_PT_FONT_WIDTH,SCORE_Y,TEN_PT_FONT_HEIGHT,numbers[my_score],LCD_COLOR_WHITE,LCD_COLOR_BLACK);
		lcd_draw_image(SCORE_X+45,TEN_PT_FONT_WIDTH,SCORE_Y,TEN_PT_FONT_HEIGHT,numbers[15],LCD_COLOR_RED,LCD_COLOR_BLACK);
		lcd_draw_image(SCORE_X+57,TEN_PT_FONT_WIDTH,SCORE_Y,TEN_PT_FONT_HEIGHT,numbers[16],LCD_COLOR_RED,LCD_COLOR_BLACK);
		lcd_draw_image(SCORE_X+67,TEN_PT_FONT_WIDTH,SCORE_Y,TEN_PT_FONT_HEIGHT,numbers[16],LCD_COLOR_RED,LCD_COLOR_BLACK);
		lcd_draw_image(SCORE_X+77,TEN_PT_FONT_WIDTH,SCORE_Y,TEN_PT_FONT_HEIGHT,numbers[10],LCD_COLOR_RED,LCD_COLOR_BLACK);	
		lcd_draw_image(SCORE_X+87,TEN_PT_FONT_WIDTH,SCORE_Y,TEN_PT_FONT_HEIGHT,numbers[their_score],LCD_COLOR_WHITE,LCD_COLOR_BLACK);	
}
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

void update_puck(){
	int32_t diff;
	move_puck = true;
	if(PUCK_Y_COORD + puckHeightPixels/2 == LCD_HEIGHT - 1){
		pause = true;
		fast = false;
		speed_count = 5;
		scored++;
		lcd_draw_image(PUCK_X_COORD,puckWidthPixels,PUCK_Y_COORD,puckHeightPixels,puckBitmaps,LCD_COLOR_BLACK,LCD_COLOR_BLACK);
		PUCK_X_COORD = LCD_WIDTH/2;
		PUCK_Y_COORD = TOP_PADDING + puckHeightPixels/2;
		PUCK_DX = 1;
		PUCK_DY = 1;
		opponent_score++;
		draw_score(my_score, opponent_score);
		return;
	} else if(PUCK_Y_COORD  - puckHeightPixels/2 == TOP_PADDING && PUCK_DY == -1){
		lcd_draw_image(PUCK_X_COORD,puckWidthPixels,PUCK_Y_COORD,puckHeightPixels,puckBitmaps,LCD_COLOR_BLACK,LCD_COLOR_BLACK);
		send = true;
		puck_here = false;
		return;
	}else if(PUCK_X_COORD - puckWidthPixels/2 == 0 || 
		(PUCK_X_COORD - puckWidthPixels/2 == PADDLE_X_COORD + paddleWidthPixels/2 && PADDLE_Y_COORD - paddleHeightPixels/2 <= PUCK_Y_COORD + puckHeightPixels/2 )){
		// left or right of paddle
		if(PUCK_DX < 0) {
			PUCK_DX = -PUCK_DX;
		}
	} else if(PUCK_X_COORD + puckWidthPixels/2 == LCD_WIDTH - 1 ||
			(PUCK_X_COORD + puckWidthPixels/2 == PADDLE_X_COORD - paddleWidthPixels/2 && PADDLE_Y_COORD - paddleHeightPixels/2 <= PUCK_Y_COORD + puckHeightPixels/2 )){
		// right or left of paddle
		if(PUCK_DX > 0) {
			PUCK_DX = -PUCK_DX;
		}
	} else if(PUCK_Y_COORD + puckHeightPixels/2 + paddleHeightPixels + PADDLE_PADDING == LCD_HEIGHT - 1){
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

void update_paddle(){
		spi_select(MODULE_1);
		x_data = accel_read_x();	
		if(x_data > 3000){
			if(PADDLE_X_COORD - paddleWidthPixels/2 > 0)
				move_image(DIR_LEFT,&PADDLE_X_COORD,&PADDLE_Y_COORD);	
		}else if(x_data < -3000){
			if(PADDLE_X_COORD + paddleWidthPixels/2 < LCD_WIDTH - 1)
				move_image(DIR_RIGHT,&PADDLE_X_COORD,&PADDLE_Y_COORD);	
		}
		move_paddle = true;
}
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
			fast = (receive_data & FAST_M) ? true : false;
			speed_count = fast ? 2 : 5;
		}
}

void transmit() {
	uint32_t send_data;
	wireless_com_status_t status;
	if(send) {
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

void wait_screen(){
	int wait_count = 0;
	int loop_counter = 0;
	wireless_com_status_t status;
	bool recieved = false;
	uint32_t data;
	
	lcd_clear_screen(LCD_COLOR_BLACK);
	spi_select(NORDIC);
	status = wireless_send_32(true, true, 1);
	if(status == NRF24L01_TX_SUCCESS) {
			bytes_sent++;
	}
	lcd_draw_image(WAITING_START,FT_PT_FONT_WIDTH,LCD_HEIGHT/2,FT_PT_FONT_HEIGHT,text14ptBitmaps[24],LCD_COLOR_RED,LCD_COLOR_BLACK);
	lcd_draw_image(WAITING_START+21,FT_PT_FONT_WIDTH,LCD_HEIGHT/2,FT_PT_FONT_HEIGHT,text14ptBitmaps[2],LCD_COLOR_RED,LCD_COLOR_BLACK);
	lcd_draw_image(WAITING_START+42,FT_PT_FONT_WIDTH,LCD_HEIGHT/2,FT_PT_FONT_HEIGHT,text14ptBitmaps[10],LCD_COLOR_RED,LCD_COLOR_BLACK);
	lcd_draw_image(WAITING_START+63,FT_PT_FONT_WIDTH,LCD_HEIGHT/2,FT_PT_FONT_HEIGHT,text14ptBitmaps[21],LCD_COLOR_RED,LCD_COLOR_BLACK);
	lcd_draw_image(WAITING_START+85,FT_PT_FONT_WIDTH,LCD_HEIGHT/2,FT_PT_FONT_HEIGHT,text14ptBitmaps[10],LCD_COLOR_RED,LCD_COLOR_BLACK);
	lcd_draw_image(WAITING_START+107,FT_PT_FONT_WIDTH,LCD_HEIGHT/2,FT_PT_FONT_HEIGHT,text14ptBitmaps[15],LCD_COLOR_RED,LCD_COLOR_BLACK);
	lcd_draw_image(WAITING_START+129,FT_PT_FONT_WIDTH,LCD_HEIGHT/2,FT_PT_FONT_HEIGHT,text14ptBitmaps[8],LCD_COLOR_RED,LCD_COLOR_BLACK);
	
	do{
		spi_select(NORDIC);
		status = wireless_get_32(false, &data);
		printf("%s\n\r",wireless_error_messages[status]);
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
	
	// wireless demo var
  uint32_t data;
  bool validate;	 
	i = 0;
	
	game_timer = 60;
  printf("\n\r");
  printf("**************************************\n\r");
  printf("* ECE353 - Final Project\n\r");
  printf("**************************************\n\r");
  printf("\n\r");
  
  //printf("MyID:%i%i%i%i%i\n\r",myID[0],myID[1],myID[2],myID[3],myID[4]);
  //printf("RmID:%i%i%i%i%i\n\r",remoteID[0],remoteID[1],remoteID[2],remoteID[3],remoteID[4]);
  
  printf("\n\r");
	
//	while(1){
//      // Delay before entering the code to determine which FSM state to 
//      // transistion to.
//      debounce_wait();
//			pressed = sw1_debounce_fsm();		
//			if(pressed){
//					for(addr = ADDR_START; addr <(ADDR_START+NUM_BYTES); addr++)
//					{
//							values[ addr - ADDR_START] = rand();
//							printf("Writing %i\n\r",values[addr-ADDR_START]);
//							eeprom_byte_write(I2C1_BASE,addr, values[addr-ADDR_START]);
//					}
//					for(addr = ADDR_START; addr <(ADDR_START+NUM_BYTES); addr++)
//					{
//							eeprom_byte_read(I2C1_BASE,addr, &read_val);
//							printf("Reading %i\n\r",read_val);
//					}
//			}
//			
//			touch_event = ft6x06_read_td_status();
//			printf("touch event: %d\n", touch_event);
//			
//			if(get_x_data){
//				spi_select(MODULE_1);
//				x_data = accel_read_x();
//				printf("%d\n", x_data);
//			}
//	}		
			lcd_clear_screen(LCD_COLOR_BLACK);
			draw_timer(game_timer);
			lcd_draw_image(LCD_WIDTH/2,borderWidthPixels,BORDER_HEIGHT,borderHeightPixels,borderBitmaps,draw_color,LCD_COLOR_BLACK);
			PADDLE_X_COORD = 120;
			PADDLE_Y_COORD = 319 - paddleHeightPixels/2 - PADDLE_PADDING;
			
			puck_here = (SEND_FIRST ? true : false);		
			if(SEND_FIRST)
				lcd_draw_image(PUCK_X_COORD,puckWidthPixels,PUCK_Y_COORD,puckHeightPixels,puckBitmaps,LCD_COLOR_RED,LCD_COLOR_BLACK);
			lcd_draw_image(PADDLE_X_COORD,paddleWidthPixels,PADDLE_Y_COORD,paddleHeightPixels,paddleBitmaps,draw_color,LCD_COLOR_BLACK);
			
			draw_score(0,0);
			speed_count = 5;
	
			while(game_timer > 0){
				spi_select(MODULE_1);
				if(get_x_data){
					get_x_data = false;
					// get data from other board
					receive();
					update_paddle();		
				}
				if(move_paddle){
					lcd_draw_image(PADDLE_X_COORD,paddleWidthPixels,PADDLE_Y_COORD,paddleHeightPixels,paddleBitmaps,LCD_COLOR_RED,LCD_COLOR_BLACK);
					move_paddle = false;	
				}
				if(draw_puck){
					draw_puck = false;
					if(puck_here && !pause) {
						update_puck();		
						move_puck = true;					
					}		
				}
				if(move_puck && puck_here && !pause){
						DisableInterrupts();
						lcd_draw_image(PUCK_X_COORD,puckWidthPixels,PUCK_Y_COORD,puckHeightPixels,puckBitmaps,LCD_COLOR_RED,LCD_COLOR_BLACK);
						EnableInterrupts();
						move_puck = false;	
				}
				if(AlertOneSec){
					game_timer = game_timer - 1;
					draw_timer(game_timer); 
					if(game_timer == 0)
						break;
					AlertOneSec = false;					
				}
				if(button_pushed) {
					button_pushed = false;
					if(push_buttons == UP_BUTTON && power == FULL_POWER){
						power = 0;
						fast = true;
						speed_count = 2;
						MCP23017_write_leds(0x00);
					} else if(push_buttons == DOWN_BUTTON) {
						pause = false;
					}
					EnableInterrupts();
				}
				if(print_bytes) {
					print_bytes = false;
					printf("bytes_sent : %d\n", bytes_sent);
					printf("bytes_received: %d\n", bytes_received);
				}
				// send the data to other board if required
				transmit();		
			}
}