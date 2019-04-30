#include "serial.h"

int main (void)
{
	init_uart(115200);
	printf("Loading\n");
	
	while(1);
}
