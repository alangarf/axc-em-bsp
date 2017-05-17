# axc-em-bsp

This repo contains the basic that are needed to get Newlib working on an AMX AXC-EM card. Arguably it can be used for any m68k based board, you just need to create a linker script and a project to work on.

To set up a cross compiling environment easily for the m68k and Newlib I suggest using the [crosstool-ng](http://crosstool-ng.github.io/). I provides a complete setup and worked perfectly for me.

Basically do the following (as per the guide):

```
# get the latest crosstool-ng
$ git clone https://github.com/crosstool-ng/crosstool-ng
$ cd crosstool-ng
$ ./bootstrap

# build crosstool-ng
$ ./configure --prefix=/opt/crosstool-ng
$ make
$ make install
$ export PATH="${PATH}:/opt/crosstool-ng/bin"
```

The you need to create a workdir where the m68k env will be built (this can be deleted if you're done).

```
# make work dir
$ mkdir work-dir
$ cd work-dir

# this is the config "sample" I used - there are many others
$ ct-ng m68k-unknown-elf
$ ct-ng menuconfig
```

Here you will be presented with a menuconf arrangement that will let you set options before compliing. I basically used the defaults. The only option I needed to change was in "C library (newlib) > Disable the syscalls supplied with newlib".

To build the toolset is as easy as:
```
$ ct-ng build
```

Once it is done add the resulting `x-tools/m68k-unknown-elf/bin/` to your path and you're ready!

## Building this BSP

Once you have everything setup as above just do:
```
$ make
```

You should end up with a library in the directory called `libamx.a` which you can use in other projects with Newlib and your new m68k environment.

Good luck!
