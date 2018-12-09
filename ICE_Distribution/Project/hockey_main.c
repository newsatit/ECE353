#include "hockey_main.h"

// for wireless
#define TX_MODE  true

/******************************************************************************
 * Global Variables
 *****************************************************************************/
#if TX_MODE
uint8_t myID[]      = { 1,1,1,2,3};
uint8_t remoteID[]  = { 2,2,3,1,4};
#else
uint8_t myID[]      = { 2,2,3,1,4};
uint8_t remoteID[]  = { 1,1,1,2,3};
#endif


volatile uint8_t push_buttons = 0;
volatile bool button_pushed = false;
volatile uint16_t x_data = 0;
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

void hockey_main(){
	
	uint16_t addr;
  uint8_t values[20];
  uint8_t read_val;
	uint8_t push_val;
	uint8_t touch_event;
	int16_t accel;
	bool pressed = false;
	int i;
	int16_t x,y,z;
	char msg[80];
	
	// wireless demo var
  wireless_com_status_t status;
  uint32_t data;
  bool validate;	 
	i = 0;
	
	printf("========hockey main===============\n");
	lp_io_init();
	MCP23017_write_leds(0xAA);


  
  
  
  printf("\n\r");
  printf("**************************************\n\r");
  printf("* ECE353 - SPI ICE\n\r");
  printf("**************************************\n\r");
  printf("\n\r");
  
  printf("MyID:%i%i%i%i%i\n\r",myID[0],myID[1],myID[2],myID[3],myID[4]);
  printf("RmID:%i%i%i%i%i\n\r",remoteID[0],remoteID[1],remoteID[2],remoteID[3],remoteID[4]);
  
  spi_select(NORDIC);
    
  wireless_configure_device(myID, remoteID ) ;
  
  printf("\n\r");
  
  if(TX_MODE)
  {
    printf("Tx Mode\n\r");
  }
  else
  {
    printf("Rx Mode\n\r");
  }

  // Infinite Loop
  while(1)
  {
			printf("loop\n");
			spi_select(NORDIC);

      if(TX_MODE && AlertOneSec)
      {
          printf("Sending: %d\n\r",i);
          status = wireless_send_32(true, true, i);
					printf("status: %d\n", status);
          if(status != NRF24L01_TX_SUCCESS)
          {
            printf("Error Message: %s\n\r",wireless_error_messages[status]);
          }
          AlertOneSec = false;
          i++;
      }
      else if (!TX_MODE)
      {
        status =  wireless_get_32(true, &data);
        if(status == NRF24L01_RX_SUCCESS)
        {
					printf("Received: %d\n\r", data);
        }
        
        AlertOneSec = false;
      }
		
			spi_select(MODULE_1);
			
			 accel = accel_read_x();
			 printf("%d\n", accel);
			
			if(button_pushed) {
					button_pushed = false;
					printf("%d\n", push_buttons);
					EnableInterrupts();
			}

   }	
	while(1){
      // Delay before entering the code to determine which FSM state to 
      // transistion to.
      debounce_wait();
			pressed = sw1_debounce_fsm();		
			if(pressed){
					for(addr = ADDR_START; addr <(ADDR_START+NUM_BYTES); addr++)
					{
							values[ addr - ADDR_START] = rand();
							printf("Writing %i\n\r",values[addr-ADDR_START]);
							eeprom_byte_write(I2C1_BASE,addr, values[addr-ADDR_START]);
					}
					for(addr = ADDR_START; addr <(ADDR_START+NUM_BYTES); addr++)
					{
							eeprom_byte_read(I2C1_BASE,addr, &read_val);
							printf("Reading %i\n\r",read_val);
					}
			}
			
//			touch_event = ft6x06_read_td_status();
//			printf("touch event: %d\n", touch_event);
			
			accel = accel_read_x();
			printf("%d\n", accel);
			
			if(button_pushed) {
					button_pushed = false;
					printf("%d\n", push_buttons);
					EnableInterrupts();
			}

	}	
}