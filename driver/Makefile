MODFLAGS := -O2 -Wall -DMODULE -DLINUX -D__KERNEL__ -I /usr/src/linux/include/

.PHONY : clean global_clean module

mir.o: mir.c
	gcc $(MODFLAGS) -c $< -o $@ 

mir_emu.o: mir_emu.c
	gcc $(MODFLAGS) -c $< -o $@ 

/dev/mir :
	mknod /dev/mir c 254 0
	
module : mir.o Makefile
	insmod -f mir.o n_in=8 n_out=4 in_port=0x150 out_port=0x151

emulation_module : mir_emu.o Makefile
	insmod -f mir_emu.o n_in=8 n_out=4 in_port=0x150 out_port=0x151

global_clean: clean

clean: 
	rm -f *.o
