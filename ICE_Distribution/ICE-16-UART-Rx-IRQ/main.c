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


/******************************************************************************
 * Global Variables
 *****************************************************************************/

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
  init_serial_debug(true, false);
  EnableInterrupts();
}

//*****************************************************************************
//*****************************************************************************
int 
main(void)
{
	char input[80];
  initializeBoard();
  
	if( validate_ice(ICE_UART_IRQ_RX) == false)
	{
		// Error in configuing the board, so enter infinite loop.
		DisableInterrupts();
		while(1){};
	}
	
  put_string("**************************************\n\r");
  put_string("* ECE353: ICE-UART-Rx-IRQ DIST\n\r");
  put_string("**************************************\n\r");
  
  // Infinite Loop
  while(1){
		memset(input,0,80);
		put_string("\n\rEnter a string: ");
		scanf("%79[^\n]", input);
		put_string("You entered: ");
		put_string(input);
		put_string("\n\r");
  };
}
