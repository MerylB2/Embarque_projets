# Configuration du microcontrôleur
F_CPU		= 16000000UL
MCU			= atmega328p
PROG		= arduino
PORT		= /dev/ttyUSB0
BAUDRATE	= 115200

# Configuration du compilateur
CC			= avr-gcc
OBJCOPY		= avr-objcopy
AVRDUDE		= avrdude
CFLAGS		= -Wall -Os -DF_CPU=$(F_CPU) -mmcu=$(MCU) -DBAUD=$(BAUDRATE)

# Fichiers source
SRC			= main.c uart.c adc.c

#colors
RED			= \033[1;31m
GREEN		= \033[1;32m
YELLOW		= \033[1;33m
BLUE		= \033[1;34m
CYAN		= \033[1;36m
RESET		= \033[0m

# General rule
all: hex flash

# Compilation : .c -> .bin
main.bin: $(SRC)
	@echo "$(BLUE)=== Compilation des fichiers sources ===$(RESET)"
	@$(CC) $(CFLAGS) -o main.bin $(SRC)
	@echo "$(CYAN)✓ Fichier main.bin créé$(RESET)"

# Conversion: .bin -> .hex
hex: main.bin
	@echo "$(BLUE)=== Conversion en format HEX ===$(RESET)"
	@$(OBJCOPY) -O ihex main.bin main.hex
	@echo "$(CYAN)✓ Fichier main.hex créé$(RESET)"

# Flash du CPU
flash: hex
	@echo "$(YELLOW)=== Flashage du microcontrôleur ===$(RESET)"
	@$(AVRDUDE) -c $(PROG) -p $(MCU) -P $(PORT) -b $(BAUDRATE) -U flash:w:main.hex:i
	@echo "$(GREEN)✓ Programme flashé avec succès$(RESET)"

# Nettoyage
clean:
	@echo "$(BLUE)=== Nettoyage ===$(RESET)"
	@rm -f main.hex main.bin
	@echo "$(GREEN)✓ Fichiers supprimés$(RESET)"

# Informations sur le programme compilé
size: main.bin
	@echo "$(YELLOW)=== Taille du programme ===$(RESET)"
	@avr-size main.bin

.PHONY: all hex flash clean size