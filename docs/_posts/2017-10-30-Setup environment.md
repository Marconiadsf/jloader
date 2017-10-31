---
layout: default
---

# 1.Setup environment

The environment and tools needed to build
JLoader are the same used in building a hobby
OS kernel.

If you already have a cross compiler and is 
following this page just to find something neat, 
you can skip the steps below.

You shall build your own cross compiler. I am
currently working on a Ubuntu 16.04 host OS,
and was able to build GCC 7.2, along with 
binutils 2.29. The target is i686-elf. 
For more information please check OSDev wiki for 
how to build a gcc cross compiler.

After finishing creating your cross compiler,
next step is getting the sources of a kernel 
skeleton. For a initial set you can both clone 
Meaty Skeleton repository (more information at 
OSDev wiki) or clone JLoader commit <insertcommitwhenready>.

If you clone JLoader (recommended) you will get 
improved versions of tty initial driver, printf,
and some extra string functions.

For both options you need to change the following
line in your config.sh file:

```shell

export PATH="$HOME/opt/cross/bin:$PATH"

```

to the right path to your cross compiler.


