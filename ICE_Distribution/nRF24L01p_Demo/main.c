// Copyright (c) 2014, Joe Krachey
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

//*****************************************************************************
// main.c
// Author: jkrachey@wisc.edu
//*****************************************************************************
#include "main.h"

#define TX_MODE  false

/******************************************************************************
 * Global Variables
 *****************************************************************************/
#if TX_MODE
uint8_t myID[]      = { 3,5,3,6,8};
uint8_t remoteID[]  = { 3,5,3,6,7};
#else
uint8_t myID[]      = { 3,5,3,6,7};
uint8_t remoteID[]  = { 3,5,3,6,8};
#endif

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
//*****************************************************************************
void initializeBoard(void)
{
  DisableInterrupts();
  init_serial_debug(false, false);
	gp_timer_config_32(TIMER0_BASE, TIMER_TAMR_TAMR_1_SHOT, false, false);
  spi_select_init();
  SysTick_Config(10000);
  EnableInterrupts();
}


//*****************************************************************************
//*****************************************************************************
int 
main(void)
{                     
  wireless_com_status_t status;
  uint32_t data;
  int i = 0;
  bool validate;
  
  initializeBoard();
  
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

     /* if(TX_MODE && AlertOneSec)
      {
          printf("Sending: %d\n\r",i);
          status = wireless_send_32(false, false, i);
          if(status != NRF24L01_TX_SUCCESS)
          {
            printf("Error Message: %s\n\r",wireless_error_messages[status]);
          }
          AlertOneSec = false;
          i++;
      }
      else if (!TX_MODE)
      {
        status =  wireless_get_32(false, &data);
        if(status == NRF24L01_RX_SUCCESS)
        {
            printf("Received: %d\n\r", data);
        }
        
        AlertOneSec = false;
      }*/
			if(AlertOneSec) {
				if(TX_MODE){
						
						printf("Sent: %d\n\r",i);
						//DisableInterrupts();
						//do{
						spi_select(NORDIC);
							status = wireless_send_32(true, true, i);
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
						status =  wireless_get_32(true, &data);
						//EnableInterrupts();
//						if(status == NRF24L01_RX_SUCCESS)
//							{
//								printf("Received: %d\n\r", PADDLE2_X_COORD);
//							}else{
//							printf("Recieve Error: %s\n\r",wireless_error_messages[status]);
//						}
						i++;
					}else{
						spi_select(NORDIC);
						//DisableInterrupts();
						status =  wireless_get_32(true, &data);
						//EnableInterrupts();
						if(status == NRF24L01_RX_SUCCESS)
						{
								printf("Received: %d\n\r", data);
						}else{
							printf("Recieve Error: %s\n\r",wireless_error_messages[status]);
						}
							
						printf("Sent: %d\n\r",i);
						spi_select(NORDIC);
						//DisableInterrupts();
						//do{
							status = wireless_send_32(true, true, i);
						//}while(status == NRF24L01_TX_PCK_LOST);
						//EnableInterrupts();
						if(status != NRF24L01_TX_SUCCESS)
						{
							printf("Error Message: %s\n\r",wireless_error_messages[status]);
						}else if(status == NRF24L01_TX_SUCCESS){
							printf("Sent message\n");
						}
						i++;
					}
				AlertOneSec = false;
				gp_timer_wait(TIMER0_BASE, 5000000);
			}
			
    }
}
