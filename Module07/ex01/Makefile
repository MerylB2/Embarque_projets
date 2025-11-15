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
SRC			= main.c uart.c eeprom_parse.c

#colors
RED			= \033[1;31m
GREEN		= \033[1;32m
YELLOW		= \033[1;33m
BLUE		= \033[1;34m
CYAN		= \033[1;36m
RESET		= \033[0m

# General rule
all: hex flash

# Compilation : .c -> .bin (version production)
main.bin: $(SRC)
	@echo "$(BLUE)=== Compilation des fichiers sources (production) ===$(RESET)"
	@echo "$(CYAN)Sources: $(SRC)$(RESET)"
	@$(CC) $(CFLAGS) -o main.bin $(SRC)
	@echo "$(GREEN)✓ Fichier main.bin créé$(RESET)"

# Conversion: .bin -> .hex (version production)
hex: main.bin
	@echo "$(BLUE)=== Conversion en format HEX ===$(RESET)"
	@$(OBJCOPY) -O ihex main.bin main.hex
	@echo "$(CYAN)✓ Fichier main.hex créé$(RESET)"

# Flash du CPU (version production)
flash: hex
	@echo "$(YELLOW)=== Flashage du microcontrôleur ===$(RESET)"
	@$(AVRDUDE) -c $(PROG) -p $(MCU) -P $(PORT) -b $(BAUDRATE) -U flash:w:main.hex:i
	@echo "$(GREEN)✓ Programme flashé avec succès$(RESET)"

# Moniteur série
monitor:
	@echo "$(CYAN)=== Ouverture du moniteur série à $(BAUDRATE) baud ===$(RESET)"
	@echo "$(YELLOW)Appuyez sur Ctrl+A puis K pour quitter$(RESET)"
	@screen $(PORT) $(BAUDRATE)

# Nettoyage
clean:
	@echo "$(BLUE)=== Nettoyage ===$(RESET)"
	@rm -f main.hex main.bin
	@echo "$(GREEN)✓ Fichiers supprimés$(RESET)"

# Informations sur le programme compilé
size: main.bin
	@echo "$(YELLOW)=== Taille du programme ===$(RESET)"
	@avr-size main.bin

# Aide
help:
	@echo "$(CYAN)=== Makefile pour ATmega328P - EEPROM Hexdump ===$(RESET)"
	@echo ""
	@echo "$(YELLOW)Cibles disponibles:$(RESET)"
	@echo "  $(GREEN)all$(RESET)          - Compile et flash la version production (défaut)"
	@echo "  $(GREEN)hex$(RESET)          - Compile et génère le fichier .hex (production)"
	@echo "  $(GREEN)flash$(RESET)        - Flash la version production"
	@echo "  $(GREEN)monitor$(RESET)      - Ouvre le moniteur série (115200 baud)"
	@echo "  $(GREEN)size$(RESET)         - Affiche la taille du programme production"
	@echo "  $(GREEN)clean$(RESET)        - Supprime les fichiers générés"
	@echo "  $(GREEN)help$(RESET)         - Affiche cette aide"
	@echo ""
	@echo "$(YELLOW)Exemples:$(RESET)"
	@echo "  make              # Compile et flash la version production"
	@echo "  make monitor      # Ouvre le moniteur série"
	@echo "  make clean        # Nettoie les fichiers"
	@echo ""

.PHONY: all hex flash monitor clean size help