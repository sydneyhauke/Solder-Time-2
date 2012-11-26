# AVR-GCC Makefile

PROJECT=ST2
SOURCES=src/main.c src/hardwareFunctions.c src/display.c src/softwareFunctions.c
DESTINATION=bin/
CC=avr-gcc
OBJCOPY=avr-objcopy
PROGRAMMER=avrdude
CONFPROG=/usr/share/arduino/hardware/tools/avrdude.conf
MMCU=atmega328p
PORT=/dev/ttyUSB0
BITRATE=57600
CFLAGS=-mmcu=$(MMCU) -Werror -Os

$(PROJECT).hex: $(PROJECT).out
	$(OBJCOPY) -j .text -O ihex $(DESTINATION)$(PROJECT).out $(DESTINATION)$(PROJECT).hex

$(PROJECT).out: $(SOURCES)
	$(CC) $(CFLAGS) -I./ -o $(DESTINATION)$(PROJECT).out $(SOURCES)

upload: $(PROJECT).hex
	$(PROGRAMMER) -C $(CONFPROG) -p $(MMCU) -b $(BITRATE) -carduino -v -v -v -v -P $(PORT) -D -U flash:w:$(DESTINATION)$(PROJECT).hex:i

clean:
	rm -f $(DESTINATION)$(PROJECT).out
	rm -f $(DESTINATION)$(PROJECT).hex
