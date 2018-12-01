#include "IO_Expander.h"

static __inline void  I0_Expander_init(void)
{
	gpio_enable_port(GPIOB_BASE);
	gpio_config_digital_enable(GPIOB_BASE,RED_M|BLUE_M|GREEN_M|SW1_M|SW2_M);
	gpio_config_enable_output(GPIOB_BASE,RED_M|BLUE_M|GREEN_M);
	gpio_config_enable_input(GPIOB_BASE,SW1_M|SW2_M);
	gpio_config_enable_pullup(GPIOB_BASE,SW1_M|SW2_M);
}