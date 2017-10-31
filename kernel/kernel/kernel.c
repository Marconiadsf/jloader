#include <stdio.h>
#include <stdlib.h>
#include <kernel/tty.h>




void kernel_main(unsigned long magic, unsigned long addr) 
{
	terminal_initialize();
	printf("Hello World!");
	
}
