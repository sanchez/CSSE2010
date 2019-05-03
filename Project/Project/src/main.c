#include "serial.h"

int main (void)
{
	init_uart(19200);
	printf("Loading\n");
	
	while(1);
}
