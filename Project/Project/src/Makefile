PORT=$(shell pavr2cmd --prog-port)

all: hex flash
	@echo "Compiling and uploading"

help:
	@echo "Help details"
	@echo "hex: compile hex file"
	@echo "flash: "install hex file"
	@echo "program: compile hex and install"

hex:
	avr-gcc -std=c99 -Os -mmcu=atmega324a -c *.c
	avr-gcc -std=c99 -mmcu=atmega324a -o project.elf *.o
	avr-objcopy -R .eeprom -O ihex project.elf project.hex

flash:
	avrdude -p m324a -c stk500v2 -P $(PORT) -B 5 -D -e
	avrdude -p m324a -c stk500v2 -P $(PORT) -B 5 -D -U flash:w:project.hex:i

clean:
	rm *.o
	rm *.elf
	rm *.hex
