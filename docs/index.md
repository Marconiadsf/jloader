---
layout: default
---

# >JLoader web site

Welcome to JLoader (JAOS Loader )home page.

## >What is JLoader?

JLoader is a second stage boot loader for my 
Hobby operational system, JAOS.

Currently it depends on Grub to work. After
loading, it will search in a initrd.tar file
loaded as a module for the kernel file, load 
it in the higher half of virtual memory, at 
address 0xC0000000 (_however that's not a 
requirement, as the loading location will be
determined at execution time, based on the 
information given by the program headers in 
the elf file_).

Most of the code found here is a replication
of something that can be found in [OSDev](http://wiki.osdev.org/Main_Page)
or in [JamesM's kernel development tutorials](http://www.jamesmolloy.co.uk/tutorial_html/).

The base of the project itself is a clone of
[sortie's](https://github.com/sortie) [Meaty Skeleton](https://gitlab.com/sortie/meaty-skeleton.git).
repository. 

If you find any bug's, or missing license or 
credit information, please, feel free to contact-me.

