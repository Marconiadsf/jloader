#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include <kernel/tty.h>
#include <kernel/arch/i386/boot.h>



void kernel_main(unsigned long magic, unsigned long addr) 
{
	uint32_t free_adr,avail_ram,initrd_adr;
	terminal_initialize();
	free_adr=(uint32_t )multiboot_init(magic,addr);
	initrd_adr=multiboot_getinitrd();
	avail_ram=multiboot_getram();
	printf("Free memory starts at: 0x%x ; Initial ramdisk is at: 0x%x ; Memory size: %d Kbytes",free_adr,initrd_adr, avail_ram/1024);
}
