#include "hockey_main.h"

// for wireless

#define LCD_WIDTH 240
#define LCD_HEIGHT 320
#define WORD_START 170
#define	COLOR_START 80
#define BORDER_HEIGHT 10
#define START_WIDTH 85
#define START_HEIGHT 160
#define TOUCH_MIN 200

/******************************************************************************
 * Global Variables
 *****************************************************************************/


volatile uint8_t push_buttons = 0;
volatile bool button_pushed = false;
volatile int16_t x_data = 0;
extern const bool SEND_FIRST;
volatile bool get_x_data;
bool player1_ready=false;
bool player2_ready=false;
bool color_selected = false;
bool touch_start = false;
bool move_paddle = false;
bool move_puck = false;
bool scored = false;

volatile uint32_t PADDLE_X_COORD;
volatile uint32_t PADDLE_Y_COORD;
volatile uint32_t PADDLE2_X_COORD;
volatile uint32_t PADDLE2_Y_COORD;
volatile uint32_t PADDLE_PADDING = 20;
volatile int32_t PUCK_X_COORD = 120;
volatile int32_t PUCK_Y_COORD = 100;
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
	uint16_t x_touch = 0;
	uint16_t y_touch = 0;
	uint8_t touch_event = 0;
	uint8_t touch_counter = 0;
	int a;
	lcd_draw_image(START_WIDTH,15,START_HEIGHT,13,start[2],LCD_COLOR_RED,LCD_COLOR_BLACK);
	lcd_draw_image(START_WIDTH+16,15,START_HEIGHT,13,start[3],LCD_COLOR_RED,LCD_COLOR_BLACK);
	lcd_draw_image(START_WIDTH+32,15,START_HEIGHT,13,start[0],LCD_COLOR_RED,LCD_COLOR_BLACK);
	lcd_draw_image(START_WIDTH+48,15,START_HEIGHT,13,start[1],LCD_COLOR_RED,LCD_COLOR_BLACK);
	lcd_draw_image(START_WIDTH+64,15,START_HEIGHT,13,start[3],LCD_COLOR_RED,LCD_COLOR_BLACK);		
	while(!touch_start){
			touch_event = ft6x06_read_td_status();
			if(touch_event == 1) {
				touch_counter++;
			} else {
				touch_counter = 0;
			}
			//printf("touch event: %d\n", touch_event);
			//printf("x=%d, y=%d\n", x_touch, y_touch);
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
	while(!color_selected){ //!color_selected
		touch_event = ft6x06_read_td_status();
		x_touch = ft6x06_read_x();
		y_touch = ft6x06_read_x();		
		if(touch_event == 1) {
			touch_counter++;
		} else {
			touch_counter = 0;
		}
		//printf("touch event: %d\n", touch_event);
		//printf("x=%d, y=%d\n", x_touch, y_touch);
		if(touch_counter > TOUCH_MIN){
				//color_selected = true;
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
			//transmit that player is ready
			//set player1_ready to true
//	while(!player2_ready){		
//		//wait for other player to be ready
//	}
	
}
void draw_timer(uint16_t time_value){
	uint16_t last_digit = 0;
	uint16_t first_digit = 0;
	//uint16_t game_timer = 75;
	
	//draw TIMER:
	lcd_draw_image(WORD_START,6,4,7,letter_T,LCD_COLOR_RED,LCD_COLOR_BLACK);
	lcd_draw_image(WORD_START+7,6,4,7,letter_I,LCD_COLOR_RED,LCD_COLOR_BLACK);
	lcd_draw_image(WORD_START+14,6,4,7,letter_M,LCD_COLOR_RED,LCD_COLOR_BLACK);
	lcd_draw_image(WORD_START+21,6,4,7,letter_E,LCD_COLOR_RED,LCD_COLOR_BLACK);
	lcd_draw_image(WORD_START+28,6,4,7,colon,LCD_COLOR_RED,LCD_COLOR_BLACK);	
	
	if(time_value >= 60){
		last_digit = (time_value-60) %10;
		first_digit = (time_value-60) / 10;
		lcd_draw_image(WORD_START+35,7,4,7,numbers[1],LCD_COLOR_RED,LCD_COLOR_BLACK);
		lcd_draw_image(WORD_START+43,6,4,7,colon,LCD_COLOR_RED,LCD_COLOR_BLACK);
		lcd_draw_image(WORD_START+50,7,4,7,numbers[first_digit],LCD_COLOR_RED,LCD_COLOR_BLACK);
		lcd_draw_image(WORD_START+57,7,4,7,numbers[last_digit],LCD_COLOR_RED,LCD_COLOR_BLACK);
	}else{
		last_digit = time_value %10;
		first_digit = time_value / 10;
		lcd_draw_image(WORD_START+35,7,4,7,numbers[0],LCD_COLOR_RED,LCD_COLOR_BLACK);
		lcd_draw_image(WORD_START+43,6,4,7,colon,LCD_COLOR_RED,LCD_COLOR_BLACK);
		lcd_draw_image(WORD_START+50,7,4,7,numbers[first_digit],LCD_COLOR_RED,LCD_COLOR_BLACK);
		lcd_draw_image(WORD_START+57,7,4,7,numbers[last_digit],LCD_COLOR_RED,LCD_COLOR_BLACK);	
	}
//	if(AlertOneSec){
//		game_timer = game_timer - 1;
//		//printf("time:%d\n\r",game_timer);
//		AlertOneSec = false;
//	}


	
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
		scored = true;
		lcd_draw_image(PUCK_X_COORD,puckWidthPixels,PUCK_Y_COORD,puckHeightPixels,puckBitmaps,LCD_COLOR_BLACK,LCD_COLOR_BLACK);
	} else if(PUCK_Y_COORD  - puckHeightPixels/2 == 0){
		scored = true;
		lcd_draw_image(PUCK_X_COORD,puckWidthPixels,PUCK_Y_COORD,puckHeightPixels,puckBitmaps,LCD_COLOR_BLACK,LCD_COLOR_BLACK);
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
	} else if(PUCK_Y_COORD - puckHeightPixels/2 == 0) {
		// top
		if(PUCK_DY < 0) {
			PUCK_DY = -PUCK_DY;
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
  wireless_com_status_t status;
  uint32_t data;
  bool validate;	 
	i = 0;
	
	game_timer = 60;
	
	printf("========hockey main===============\n");
	//lp_io_init();
	//MCP23017_write_leds(0xAA);

  printf("\n\r");
  printf("**************************************\n\r");
  printf("* ECE353 - SPI ICE\n\r");
  printf("**************************************\n\r");
  printf("\n\r");
  
  //printf("MyID:%i%i%i%i%i\n\r",myID[0],myID[1],myID[2],myID[3],myID[4]);
  //printf("RmID:%i%i%i%i%i\n\r",remoteID[0],remoteID[1],remoteID[2],remoteID[3],remoteID[4]);
  
  printf("\n\r");

  // Infinite Loop
//  while(1)
//  {
//			
//      if(TX_MODE && AlertOneSec)
//      {
//          printf("Sending: %d\n\r",i);
//					spi_select(NORDIC);
//					DisableInterrupts();
//          status = wireless_send_32(false, false, i);
//					EnableInterrupts();
//          if(status != NRF24L01_TX_SUCCESS)
//          {
//            printf("Error Message: %s\n\r",wireless_error_messages[status]);
//          }
//          AlertOneSec = false;
//          i++;
//      }
//      else if (!TX_MODE)
//      {
//				spi_select(NORDIC);
//        status =  wireless_get_32(false, &data);
//        if(status == NRF24L01_RX_SUCCESS)
//        {
//            printf("Received: %d\n\r", data);
//        }
//        
//        AlertOneSec = false;
//      }

//			if(get_x_data){
//			spi_select(MODULE_1);
//			x_data = accel_read_x();
//			printf("%d\n", x_data);
//			}
//			if(button_pushed) {
//					button_pushed = false;
//					printf("%d\n", push_buttons);
//					EnableInterrupts();
//			}

//   }	
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
//			spi_select(MODULE_1);
//			x_data = accel_read_x();
//			printf("%d\n", x_data);
//			}
//			
//			if(button_pushed) {
//					button_pushed = false;
//					printf("%d\n", push_buttons);
//					EnableInterrupts();
//			}

//	}		
				//DisableInterrupts();

				//EnableInterrupts();
				//spi_select(NORDIC);

			start_screen();
			lcd_clear_screen(LCD_COLOR_BLACK);
			draw_timer(game_timer);
			lcd_draw_image(LCD_WIDTH/2,borderWidthPixels,BORDER_HEIGHT,borderHeightPixels,borderBitmaps,draw_color,LCD_COLOR_BLACK);
			PADDLE2_X_COORD = 120;
			PADDLE2_Y_COORD = paddleHeightPixels/2 + PADDLE_PADDING;
			PADDLE_X_COORD = 120;
			PADDLE_Y_COORD = 319 - paddleHeightPixels/2 - PADDLE_PADDING;
			
			lcd_draw_image(PUCK_X_COORD,puckWidthPixels,PUCK_Y_COORD,puckHeightPixels,puckBitmaps,LCD_COLOR_RED,LCD_COLOR_BLACK);
			lcd_draw_image(PADDLE_X_COORD,paddleWidthPixels,PADDLE_Y_COORD,paddleHeightPixels,paddleBitmaps,LCD_COLOR_RED,LCD_COLOR_BLACK);
			lcd_draw_image(PADDLE2_X_COORD,paddleWidthPixels,PADDLE2_Y_COORD,paddleHeightPixels,paddleBitmaps,LCD_COLOR_RED,LCD_COLOR_BLACK);
				
			while(game_timer != 0){
				spi_select(MODULE_1);
				if(get_x_data){
					get_x_data = false;
			//send data
				if(SEND_FIRST){
						spi_select(NORDIC);
						printf("Sent: %d\n\r",PADDLE_X_COORD);
						//DisableInterrupts();
						//do{
							status = wireless_send_32(true, true, PADDLE_X_COORD);
						//}while(status == NRF24L01_TX_PCK_LOST);
						//EnableInterrupts();
						if(status != NRF24L01_TX_SUCCESS)
						{
							printf("Error Message: %s\n\r",wireless_error_messages[status]);
						}else if(status == NRF24L01_TX_SUCCESS){
							printf("Sent message\n");
						}
						spi_select(NORDIC);
						//DisableInterrupts();
						status =  wireless_get_32(true, &PADDLE2_X_COORD);
						//EnableInterrupts();
//						if(status == NRF24L01_RX_SUCCESS)
//							{
//								printf("Received: %d\n\r", PADDLE2_X_COORD);
//							}else{
//							printf("Recieve Error: %s\n\r",wireless_error_messages[status]);
//						}
					}else{
						spi_select(NORDIC);
						//DisableInterrupts();
						status =  wireless_get_32(false, &PADDLE2_X_COORD);
						//EnableInterrupts();
						if(status == NRF24L01_RX_SUCCESS)
						{
								printf("Received: %d\n\r", PADDLE2_X_COORD);
						}else{
							printf("Recieve Error: %s\n\r",wireless_error_messages[status]);
						}
							spi_select(NORDIC);
						printf("Sent: %d\n\r",PADDLE_X_COORD);
						//DisableInterrupts();
						//do{
							status = wireless_send_32(false, false, PADDLE_X_COORD);
						//}while(status == NRF24L01_TX_PCK_LOST);
						//EnableInterrupts();
						if(status != NRF24L01_TX_SUCCESS)
						{
							printf("Error Message: %s\n\r",wireless_error_messages[status]);
						}else if(status == NRF24L01_TX_SUCCESS){
							printf("Sent message\n");
						}
					}
					//i++;
					update_paddle();
					if(!scored)
					update_puck();				
				}

			if(move_paddle){
				DisableInterrupts();
				lcd_draw_image(PADDLE_X_COORD,paddleWidthPixels,PADDLE_Y_COORD,paddleHeightPixels,paddleBitmaps,LCD_COLOR_RED,LCD_COLOR_BLACK);
				move_paddle = false;
				EnableInterrupts();			
			}
			if(move_puck){
				if(!scored) {
					DisableInterrupts();
					lcd_draw_image(PUCK_X_COORD,puckWidthPixels,PUCK_Y_COORD,puckHeightPixels,puckBitmaps,LCD_COLOR_RED,LCD_COLOR_BLACK);
					move_puck = false;
					EnableInterrupts();						
				}
			if(AlertOneSec){
				game_timer = game_timer - 1;
				draw_timer(game_timer);
				AlertOneSec = false;
			}
			if(button_pushed) {
				button_pushed = false;
				if(push_buttons == 247){
					MCP23017_write_leds(0x08);
				}
				printf("%d\n", push_buttons);
				EnableInterrupts();
				}
			}
		}
}