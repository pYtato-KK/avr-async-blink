
CC=avr-gcc
CFLAGS := -Wall -Warray-bounds=0 -ffreestanding -mmcu=avr128db48 -Iasync.h/
CFLAGS += -O3
LDFLAGS := 

SRCS := $(shell find src/ -name '*.c' -or -name '*.s')
OBJS := $(SRCS:%=build/%.o)


default: progmem.elf
warn: CFLAGS += -Wextra
warn: default
asm: $(SRCS:%=build/%.s)


progmem.elf: $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) $(LDFLAGS) -o $@


build/%.c.s: %.c 
	mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -S $< -o $@


build/%.c.o: %.c
	mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@


build/%.s.o: %.s
	mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

.PHONY: clean
clean:
	$(RM) -r build/
	$(RM) progmem.elf


.PHONY: upload
upload: progmem.elf
	avrdude -p avr128db48 -U flash:w:progmem.elf:e -c curiosity_updi
