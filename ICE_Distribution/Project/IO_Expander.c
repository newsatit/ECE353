#include "IO_Expander.h"

//*****************************************************************************
// Writes a single byte of data out to the  MCP23017 IO Expander.  
//
// Paramters
//    i2c_base:   a valid base address of an I2C peripheral
//
//    address:    8-bit address of the byte being written.
//
//    data:       Data written to the MCP23017.
//
// Returns
// I2C_OK if the byte was written to the MCP23017.
//*****************************************************************************
i2c_status_t MCP23017_byte_write
( 
  uint32_t  i2c_base,
  uint8_t  address,
  uint8_t   data
)
{
  i2c_status_t status;
  
  // Before doing anything, make sure the I2C device is idle
  while ( I2CMasterBusy(i2c_base)) {};

  //==============================================================
  // Set the I2C address to be the MCP23017
  //==============================================================
	status = i2cSetSlaveAddr(i2c_base, IO_EXPANDER_DEV_ID, I2C_WRITE);
	if(status != I2C_OK)
		return status;
  
  //==============================================================
  // Send the address
  //==============================================================
	status = i2cSendByte(i2c_base, address, I2C_MCS_START | I2C_MCS_RUN);
	if(status != I2C_OK)
		return status;

  //==============================================================
  // Send the Byte of data to write
  //==============================================================
	status = i2cSendByte(i2c_base, data, I2C_MCS_RUN | I2C_MCS_STOP);
  return status;
}

//*****************************************************************************
// Reads a single byte of data from the  MCP23017 IO Expander  
//
// Paramters
//    i2c_base:   a valid base address of an I2C peripheral
//
//    address:    8-bit address of the byte being read. 
//
//    data:       data read from the MCP23017 is returned to a uint8_t pointer.
//
// Returns
// I2C_OK if the byte was read from the MCP23017.
//*****************************************************************************
i2c_status_t MCP23017_byte_read
( 
  uint32_t  i2c_base,
  uint8_t  address,
  uint8_t   *data
)
{
  i2c_status_t status;
  
  // Before doing anything, make sure the I2C device is idle
  while ( I2CMasterBusy(i2c_base)) {};

  //==============================================================
  // Set the I2C slave address to be the MCP23017 and in Write Mode
  //==============================================================
	status = i2cSetSlaveAddr(i2c_base, IO_EXPANDER_DEV_ID, I2C_WRITE);
	if(status != I2C_OK)
		return status;
		
  //==============================================================
  // Send the the address
  //==============================================================
	status = i2cSendByte(i2c_base, address, I2C_MCS_START | I2C_MCS_RUN);
	if(status != I2C_OK)
		return status;

  //==============================================================
  // Set the I2C slave address to be the MCP23017 and in Read Mode
	// ADD CODE
  //==============================================================
	status = i2cSetSlaveAddr(i2c_base, IO_EXPANDER_DEV_ID, I2C_READ);
	if(status != I2C_OK)
		return status;

  //==============================================================
  // Read the data returned by the IO_EXPANDER
	// ADD CODE
  //==============================================================
	status = i2cGetByte(i2c_base, data, I2C_MCS_START | I2C_MCS_RUN | I2C_MCS_STOP);
  return I2C_OK;
}


//*****************************************************************************
// Read the status of push buttons
//*****************************************************************************
uint8_t MCP23017_read_push_buttons(void)
{ 
	uint8_t data;

	MCP23017_byte_read(IO_EXPANDER_I2C_BASE, IO_EXPANDER_GPIOB, &data);
	
	return data;
} 

//*****************************************************************************
// Write to the LEDs
// 
// Paramters
//    pins:   a mask of led pins
//
//*****************************************************************************
void MCP23017_write_leds(uint8_t pins)
{ 
	MCP23017_byte_write(IO_EXPANDER_I2C_BASE, IO_EXPANDER_GPIOA, pins);
} 


// Config MCP23017 
void MCP23017_config(void)
{
	// config GPIOA as output
	MCP23017_byte_write(IO_EXPANDER_I2C_BASE, IO_EXPANDER_IODIRA, 0x00);
	// config GPIOB as input
	MCP23017_byte_write(IO_EXPANDER_I2C_BASE, IO_EXPANDER_IODIRB, 0xFF);

	// config GPB pull-up
	MCP23017_byte_write(IO_EXPANDER_I2C_BASE, IO_EXPANDER_GPPUB, 0xFF);



	// config GPIOF pin 0 for INTB
	gpio_enable_port(GPIOF_BASE);
	gpio_config_digital_enable(GPIOF_BASE, SW2_IO_EXPANDER_INT);
	gpio_config_enable_input(GPIOF_BASE, SW2_IO_EXPANDER_INT);
	gpio_config_enable_pullup(GPIOF_BASE, SW2_IO_EXPANDER_INT);
	// gpio_config_alternate_function(GPIOF_BASE, SW2_IO_EXPANDER_INT);
// 	gpio_config_port_control(GPIOF_BASE, 
	gpio_config_falling_edge_irq(GPIOF_BASE, SW2_IO_EXPANDER_INT);         
  // Set the Priority
  NVIC_SetPriority(GPIOF_IRQn, 1);
  // Enable the Interrupt in the NVIC
  NVIC_EnableIRQ(GPIOF_IRQn);
	
	// config interrupt INTB
	 MCP23017_byte_write(IO_EXPANDER_I2C_BASE, IO_EXPANDER_GPINTENB, IO_EXPANDER_BUTTONS_M);
	 MCP23017_byte_write(IO_EXPANDER_I2C_BASE, IO_EXPANDER_INTCONB, 0);	

	
	
}
