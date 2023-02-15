TARGET = kernel.elf
OBJS = main.o graphics.o font.o hankaku.o console.o newlib_support.o \
			 pci.o error.o asmfunc.o logger.o usb/xhci/xhci.o usb/xhci/memory.o \
			 usb/xhci/devmgr.o usb/xhci/port.o usb/xhci/ring.o

CFLAGS += -I$(HOME)/tiny_xHCI/x86_64-elf/include \
					-I$(HOME)/tiny_xHCI/x86_64-elf/freetype2 \
					-I$(HOME)/edk2/MdePkg/Include -I/$HOME/edk2/Include/X64
CFLAGS += -g -O0 -Wall --target=x86_64-elf -ffreestanding \
					-mno-red-zone -std=c11
#CFLAGS += -g -O2 -Wall --target=x86_64-elf -ffreestanding \
					-mno-red-zone -std=c11
LDFLAGS += -L$(HOME)/tiny_xHCI/x86_64-elf/lib
LDFLAGS += --entry KernelMain -z norelro --image-base 0x100000 --static

.PHONY: all
all: $(TARGET)

.PHONY: clean
clean:
	rm -rf *.o usb/xhci/*.o

kernel.elf: $(OBJS) Makefile
	ld.lld $(LDFLAGS) -o kernel.elf $(OBJS) -lc

%.o: %.c Makefile
	clang $(CPPFLAGS) $(CFLAGS) -c $< -o $@

%.o: %.asm Makefile
	nasm -f elf64 -o $@ $<

hankaku.o: hankaku.bin
	objcopy -I binary -O elf64-x86-64 -B i386:x86-64 $< $@
