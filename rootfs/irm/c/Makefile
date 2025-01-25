# SPDX-License-Identifier: GPL-2.0
KDIR ?= /lib/modules/`uname -r`/build
LLVM ?= ../../../llvm
kmod_name = irm-c

default:
	$(MAKE) -C $(KDIR) M=$$PWD modules 

memoryload:
	echo "#include <linux/module.h>" > tmp.c
	echo "#include <syscall.h>" >> tmp.c
	echo "#include <stdio.h>" >> tmp.c
	echo "" >> tmp.c
	xxd -i $(kmod_name).ko >> tmp.c
	echo "const char args[] = \"\\\0\";" >> tmp.c
	echo "" >> tmp.c
	cat stub.c >> tmp.c
	cat tmp.c | sed 's/example_ko/$(kmod_name)_ko/g' > load.c
	rm tmp.c
	gcc -o load load.c
	# $(MAKE) -C $(KDIR) M=$$PWD

modules_install: default
	$(MAKE) -C $(KDIR) M=$$PWD modules_install
