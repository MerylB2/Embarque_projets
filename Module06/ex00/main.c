#include "main.h"
#include "i2c.h"
#include "uart.h"

/* #include <util/delay.h> */

int	main(void)
{
	init_uart(F_CPU / (8 * 115200) - 1);
	i2c_init();
	i2c_start();
	i2c_stop();
	/* For testing that both connection works: */ 
	/* _delay_ms(2000); */
	/* But technically, this is illegal... */
	while (1) {}
}
