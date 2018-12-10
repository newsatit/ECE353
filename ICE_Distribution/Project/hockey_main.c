#include "hockey_main.h"

// for wireless

#define LCD_WIDTH 240
#define LCD_HEIGHT 320
#define WORD_START 80

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

volatile uint32_t PADDLE_X_COORD = 120;
volatile uint32_t PADDLE_Y_COORD = 300;

volatile uint32_t PADDLE2_X_COORD = 120;
volatile uint32_t PADDLE2_Y_COORD = 20;

static const Direction  MOV_DIR[] = {DIR_LEFT, DIR_RIGHT};



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
	lcd_draw_image(LCD_WIDTH/2,puckWidthPixels,LCD_HEIGHT/2,puckHeightPixels,puckBitmaps,LCD_COLOR_RED,LCD_COLOR_BLACK);
	while(!touch_start){
			if(!ft6x06_read_td_status()){
				touch_start = true;
			}
	}
	lcd_clear_screen(LCD_COLOR_BLACK);
	//print select color
	lcd_draw_image(WORD_START,6,LCD_HEIGHT/2,7,letter_S,LCD_COLOR_RED,LCD_COLOR_BLACK);
	lcd_draw_image(WORD_START+7,6,LCD_HEIGHT/2,7,letter_e,LCD_COLOR_RED,LCD_COLOR_BLACK);
	lcd_draw_image(WORD_START+14,6,LCD_HEIGHT/2,7,letter_l,LCD_COLOR_RED,LCD_COLOR_BLACK);
	lcd_draw_image(WORD_START+21,6,LCD_HEIGHT/2,7,letter_e,LCD_COLOR_RED,LCD_COLOR_BLACK);
	lcd_draw_image(WORD_START+28,6,LCD_HEIGHT/2,7,letter_c,LCD_COLOR_RED,LCD_COLOR_BLACK);
	lcd_draw_image(WORD_START+35,6,LCD_HEIGHT/2,7,letter_t,LCD_COLOR_RED,LCD_COLOR_BLACK);
	lcd_draw_image(WORD_START+49,6,LCD_HEIGHT/2,7,letter_c,LCD_COLOR_RED,LCD_COLOR_BLACK);
	lcd_draw_image(WORD_START+56,6,LCD_HEIGHT/2,7,letter_o,LCD_COLOR_RED,LCD_COLOR_BLACK);
	lcd_draw_image(WORD_START+63,6,LCD_HEIGHT/2,7,letter_l,LCD_COLOR_RED,LCD_COLOR_BLACK);
	lcd_draw_image(WORD_START+70,6,LCD_HEIGHT/2,7,letter_o,LCD_COLOR_RED,LCD_COLOR_BLACK);
	lcd_draw_image(WORD_START+77,6,LCD_HEIGHT/2,7,letter_r,LCD_COLOR_RED,LCD_COLOR_BLACK);
	//print four colors
	lcd_draw_image(60,colorboxWidthPixels,80,colorboxHeightPixels,colorboxBitmaps,LCD_COLOR_BLUE,LCD_COLOR_BLACK);
	lcd_draw_image(180,colorboxWidthPixels,80,colorboxHeightPixels,colorboxBitmaps,LCD_COLOR_RED,LCD_COLOR_BLACK);
	lcd_draw_image(60,colorboxWidthPixels,240,colorboxHeightPixels,colorboxBitmaps,LCD_COLOR_GREEN,LCD_COLOR_BLACK);
	lcd_draw_image(180,colorboxWidthPixels,240,colorboxHeightPixels,colorboxBitmaps,LCD_COLOR_YELLOW,LCD_COLOR_BLACK);
	while(!color_selected){
			
		//wait for color selection
		//once color has been selected 
	}
			//transmit that player is ready
			//set player1_ready to true
	while(!player2_ready){		
		//wait for other player to be ready
	}
	
}
void game_timer(){
	uint16_t game_timer = 60;
	lcd_draw_image(WORD_START,6,LCD_HEIGHT/2,7,letter_T,LCD_COLOR_RED,LCD_COLOR_BLACK);
	lcd_draw_image(WORD_START+7,6,LCD_HEIGHT/2,7,letter_I,LCD_COLOR_RED,LCD_COLOR_BLACK);
	lcd_draw_image(WORD_START+14,6,LCD_HEIGHT/2,7,letter_M,LCD_COLOR_RED,LCD_COLOR_BLACK);
	lcd_draw_image(WORD_START+21,6,LCD_HEIGHT/2,7,letter_E,LCD_COLOR_RED,LCD_COLOR_BLACK);
	lcd_draw_image(WORD_START+28,6,LCD_HEIGHT/2,7,colon,LCD_COLOR_RED,LCD_COLOR_BLACK);
//while(1){	
	lcd_draw_image(WORD_START+35,7,LCD_HEIGHT/2,7,numbers[0],LCD_COLOR_RED,LCD_COLOR_BLACK);
	
//}

	
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
void hockey_main(){
	
	uint16_t addr;
  uint8_t values[20];
  uint8_t read_val;
	uint8_t push_val;
	uint8_t touch_event;
	int16_t accel;
	uint32_t i;
	bool pressed = false;
	int16_t x,y,z;
	char msg[80];
	
	// wireless demo var
  wireless_com_status_t status;
  uint32_t data;
  bool validate;	 
	i = 0;
	
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
				//lcd_draw_image(PADDLE_X_COORD,puckWidthPixels,PADDLE_Y_COORD,puckHeightPixels,puckBitmaps,LCD_COLOR_RED,LCD_COLOR_BLACK);
				//EnableInterrupts();
				//spi_select(NORDIC);
while(1){
			
			if(get_x_data){
			spi_select(MODULE_1);
			x_data = accel_read_x();		
				if(x_data > 3000){
					move_image(DIR_LEFT,&PADDLE_X_COORD,&PADDLE_Y_COORD);	
				}else if(x_data < -3000){
					move_image(DIR_RIGHT,&PADDLE_X_COORD,&PADDLE_Y_COORD);	
				}
			//send data
//				if(SEND_FIRST){
//						//spi_select(NORDIC);
//						printf("Sent: %d\n\r",i);
//						//DisableInterrupts();
//						do{
//							status = wireless_send_32(false, false, i);
//						}while(status == NRF24L01_TX_PCK_LOST);
//						//EnableInterrupts();
//						if(status != NRF24L01_TX_SUCCESS)
//						{
//							printf("Error Message: %s\n\r",wireless_error_messages[status]);
//						}else if(status == NRF24L01_TX_SUCCESS){
//							printf("Sent message\n");
//						}
//						//spi_select(NORDIC);
//						//DisableInterrupts();
//						status =  wireless_get_32(true, &PADDLE2_X_COORD);
//						//EnableInterrupts();
//						if(status == NRF24L01_RX_SUCCESS)
//							{
//								printf("Received: %d\n\r", PADDLE2_X_COORD);
//							}else{
//							printf("Recieve Error: %s\n\r",wireless_error_messages[status]);
//						}
//					}else{
//						//spi_select(NORDIC);
//						//DisableInterrupts();
//						status =  wireless_get_32(true, &PADDLE2_X_COORD);
//						//EnableInterrupts();
//						if(status == NRF24L01_RX_SUCCESS)
//						{
//								printf("Received: %d\n\r", PADDLE2_X_COORD);
//						}else{
//							printf("Recieve Error: %s\n\r",wireless_error_messages[status]);
//						}
//						//spi_select(NORDIC);
//						printf("Sent: %d\n\r",i);
//						//DisableInterrupts();
//						do{
//							status = wireless_send_32(false, false, i);
//						}while(status == NRF24L01_TX_PCK_LOST);
//						//EnableInterrupts();
//						if(status != NRF24L01_TX_SUCCESS)
//						{
//							printf("Error Message: %s\n\r",wireless_error_messages[status]);
//						}else if(status == NRF24L01_TX_SUCCESS){
//							printf("Sent message\n");
//						}
//				}
					i++;
			}
			if(move_paddle){
				DisableInterrupts();
				lcd_draw_image(PADDLE_X_COORD,paddle2WidthPixels,PADDLE_Y_COORD,paddle2HeightPixels,paddle2Bitmaps,LCD_COLOR_RED,LCD_COLOR_BLACK);
				move_paddle = false;
				EnableInterrupts();			
				}
			}
		}