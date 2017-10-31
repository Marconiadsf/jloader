---
layout: default
---

# 2.Getting multiboot info

## 2.1 Intro

The first step after our program is loaded, is to retrieve some information given by Grub.
The data location and magic value is stored in registers that we previously pushed to the stack in boot.S.

The values are accessed in kernel_main function as parameters and we can use that to validate multiboot structure and load some useful information about our modules and memory.

So, the first thing is to download a multiboot.h header that will provide us with the multiboot struct and some constants.

I decided to put his file under kernel/include/arch/i386. I s up to you to decide if this file fits better a non arch related dir (I had used kernel/include/boot).

Now it finally time to some coding. First things first, we are going to create a header to a new application we are about to create: boot.h.

**boot.h**
```c
#ifndef __BOOT_H__
#define __BOOT_H__

unsigned int multiboot_init(unsigned long magic, unsigned long addr);
unsigned int multiboot_getram();
unsigned int multiboot_getinitrd();
#endif
```

So, the first step in our _kernel_main_ function will be validate and initialize the multiboot struct pointer for us.

You can add a call to _multiboot_init_ in _kernel_main_ in the kernel.c file. Our kernel.c source will look like:

**kernel.c**
```c
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include <kernel/tty.h>
#include <kernel/arch/i386/boot.h>

void kernel_main(unsigned long magic, unsigned long addr) 
{
	uint32_t free_adr;
	terminal_initialize();
	free_adr=(uint32_t )multiboot_init(magic,addr);
}
```

Here, **free_adr** is the address where our initial ramdisk ends. **THAT'S NOT THE RIGHT WAY TO DO THAT!!!**. You should instead add some other checks to be sure free_adr is really pointing to free memory. Modules could have been loaded before JLoader for example. The current version just read through the modules information and gets the higher end address. As addition it check if the address is under the 2 MB mark. If yes it sets **free_ptr** to the 2 MB mark. This is working, thought it may break if the placement address of JLoader and/or initrd.tar plus their size grows too much.

Now we got what we need, lets implement _multiboot_init_.

## 2.2 Multiboot validation

All we need to do is check the values present in the multiboot struct
count modules, retrieve its ending address.

Create a new file in the kernel sources directory called **boot.c**.

**boot.c**
```c
#include <stdio.h>
#include <kernel/arch/i386/multiboot.h>

multiboot_info_t *mbi;
multiboot_module_t *mod;

multiboot_uint32_t multiboot_check(unsigned long magic, unsigned long addr)
{
	if (magic != MULTIBOOT_BOOTLOADER_MAGIC)
		return 0;
	/* Set MBI to the address of the Multiboot information structure. */
	mbi = (multiboot_info_t *) addr;
	  /* is there a boot device set? */
	/*if ((mbi->flags&MULTIBOOT_INFO_BOOTDEV)
	{
		//do what?
		//return 0;
	}*/
	/* Bits 4 and 5 are mutually exclusive! */
	if ((mbi->flags&MULTIBOOT_INFO_AOUT_SYMS) && (mbi->flags&MULTIBOOT_INFO_ELF_SHDR))
		return 0;
	/* Are mmap_* valid? */
	if (!(mbi->flags&MULTIBOOT_INFO_MEM_MAP))
		return 0;
	if(mbi->flags&MULTIBOOT_INFO_MODS)
		return  _skip_modules_((multiboot_module_t *) mbi->mods_addr);
	else
		return 0;
}

multiboot_uint32_t multiboot_init(unsigned long magic, unsigned long addr){
	multiboot_uint32_t mpos=multiboot_check(magic,addr);
	if(mpos==0)
		printf("\nMultiboot_check(): Error! Check failed! POINTER: 0X%x",mpos);
	return mpos;
}
```

We declare some globals that are going to be used during initialization. 

The _multiboot_check_ function sets the global pointer **mbi**, and check for some bits in the **mbi->flags** field. If something goes wrong a 0 value is returned. If everything is ok it calls a inline function _ _skip_modules_ _ that will check for possible modules loaded, and return the highest value of **mod[i]_mod_end**

The code for _ _skip_modules_ _ :
```c
inline multiboot_uint32_t _skip_modules_(multiboot_module_t *mod){
	multiboot_uint32_t lval=mod[0].mod_end;
	for (multiboot_uint32_t i = 0; i < mbi->mods_count; i++)
		if(mod[i].mod_end>lval)lval=mod[i].mod_end;
		
	return lval;			
}
```

Pretty straightforward. I did also implemented the following methods:

```c
multiboot_uint32_t multiboot_getinitrd(){
	if(!mbi)
		return 0;
	mod= (multiboot_module_t *) mbi->mods_addr;
	if(mbi->mods_count>1)
		printf ("\nWarning mods_count > 1 (%d). Only first module start adr will be returned.",(int) mbi->mods_count);
	return mod->mod_start;

}

multiboot_uint32_t multiboot_getram(){
	if(!mbi)
		return 0;
	if (mbi->flags&MULTIBOOT_INFO_MEMORY)
		return ((multiboot_uint32_t) mbi->mem_upper+mbi->mem_lower)*1024;
	else
		return 0;	
}
```

That are also self explanatory. The first one just return the address of the first module loaded, the second the amount of physical memory available.

You may need to change multiboot_getinitrd if you plan to load more than one module to your kernel. I decided however to keep things simple and put all needed files inside a .tar file called initrd.tar.

To make this code work you need to add some lines to your iso.sh file, so it can build a iso image with the initrd.tar file, and the according grub.cfg file. Modify your iso.sh file so it look something like this:

**iso.sh**

```shell
#!/bin/sh
set -e
. ./build.sh

mkdir -p isodir
mkdir -p isodir/boot
mkdir -p isodir/boot/grub

cp sysroot/boot/jloader isodir/boot/jloader
cp path_to_tarfile/initrd.tar isodir/boot/initrd.tar
cat > isodir/boot/grub/grub.cfg << EOF
menuentry "JAOS" {
	multiboot /boot/jloader
	module    /boot/initrd.tar
}
EOF
grub-mkrescue -o jaos.iso isodir
```

I strongly recommend that your "path_to_tarfile" points to somewhere out of the build dir, for let things simple clean. Remember that if you put this into sysroot, every time you run clean.sh the sysroot dir is removed.

You can create a tar file from any file using command line, just watch out against too big files.

The methods implemented here can be found at JLoader commit [552ed87](https://github.com/Marconiadsf/JLoader/tree/552ed879760b4f20e79739cce698ead6770397f5).
