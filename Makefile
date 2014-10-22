.PHONY: all build clean program avrdude
all: build

PROJECT             := praktek2
MMCU                := atmega32a
CPU_FREQ            := 12000000

PRGORAMMER_TTY      := /dev/ttyUSB0

OBJECT              := $(addprefix src/, \
                           main.o \
                           tasks.o \
                       )

include mktools/template.mk
include mktools/avrdude.mk

build: $(PROJECT).elf

clean:
	@echo "Remove: *.o *.elf *.hex *.eep *.lss src/*.o"
	@rm -rf *.o *.elf *.hex *.eep *.lss src/*.o

program: $(PROJECT).hex
	$(AVRDUDE) -Uflash:w:"$<":i

avrdude:
	$(AVRDUDE)
