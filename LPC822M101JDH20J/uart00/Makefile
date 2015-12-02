
ARMGNU = arm-none-eabi
#ARMGNU = arm-linux-gnueabi

AOPS = --warn --fatal-warnings -mcpu=cortex-m0

all : uart00.bin

clean:
	rm -f *.bin
	rm -f *.o
	rm -f *.elf
	rm -f *.list
	rm -f *.hex

#---------------------------------

uart00.bin : uart00.s
	$(ARMGNU)-as $(AOPS) uart00.s -o uart00.o
	$(ARMGNU)-ld -o uart00.elf -Ttext 0x10000300 uart00.o
	$(ARMGNU)-objdump -D uart00.elf > uart00.list
	$(ARMGNU)-objcopy uart00.elf uart00.hex -O ihex
	$(ARMGNU)-objcopy uart00.elf uart00.bin -O binary

#---------------------------------




