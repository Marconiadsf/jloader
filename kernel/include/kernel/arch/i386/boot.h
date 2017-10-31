#ifndef __BOOT_H__
#define __BOOT_H__

unsigned int multiboot_init(unsigned long magic, unsigned long addr);
unsigned int multiboot_getram();
unsigned int multiboot_getinitrd();
#endif
