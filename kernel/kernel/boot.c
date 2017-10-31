/*
       boot.c - some rotines to check multiboot info
	   and load usefull information.
	   Strongly bases on JamesM's kernel development
	   series.

*/
#include <stdio.h>
#include <kernel/arch/i386/multiboot.h>

//multiboot_memory_map_t *mmap;
multiboot_info_t *mbi;
multiboot_module_t *mod;


inline multiboot_uint32_t _skip_modules_(multiboot_module_t *mod){

	multiboot_uint32_t lval=mod[0].mod_end;
	for (multiboot_uint32_t i = 0; i < mbi->mods_count; i++)
		if(mod[i].mod_end>lval)lval=mod[i].mod_end;
		
	return lval;			
}

	
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

multiboot_uint32_t multiboot_getinitrd(){

	if(!mbi)
		return 0;
	mod= (multiboot_module_t *) mbi->mods_addr;
	if(mbi->mods_count>1)
		printf ("\nWarning mods_count > 1 (%d). Only first module start adr will be returned.",
			(int) mbi->mods_count);
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


multiboot_uint32_t multiboot_init(unsigned long magic, unsigned long addr){

	
	multiboot_uint32_t mpos=multiboot_check(magic,addr);
	if(mpos==0)
		printf("\nMultiboot_check(): Error! Check failed! POINTER: 0X%x",mpos);
	return mpos;
}

