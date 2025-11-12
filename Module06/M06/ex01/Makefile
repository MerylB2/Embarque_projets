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
SRC			= main.c i2c.c uart.c aht20.c

# Fichiers source pour la version debug
SRC_DEBUG	= main_debug.c i2c.c uart.c aht20.c

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

# Compilation : .c -> .bin (version debug)
debug.bin: $(SRC_DEBUG)
	@echo "$(BLUE)=== Compilation des fichiers sources (debug) ===$(RESET)"
	@echo "$(CYAN)Sources: $(SRC_DEBUG)$(RESET)"
	@$(CC) $(CFLAGS) -o debug.bin $(SRC_DEBUG)
	@echo "$(GREEN)✓ Fichier debug.bin créé$(RESET)"

# Conversion: .bin -> .hex (version production)
hex: main.bin
	@echo "$(BLUE)=== Conversion en format HEX ===$(RESET)"
	@$(OBJCOPY) -O ihex main.bin main.hex
	@echo "$(CYAN)✓ Fichier main.hex créé$(RESET)"

# Conversion: .bin -> .hex (version debug)
hex-debug: debug.bin
	@echo "$(BLUE)=== Conversion en format HEX (debug) ===$(RESET)"
	@$(OBJCOPY) -O ihex debug.bin debug.hex
	@echo "$(CYAN)✓ Fichier debug.hex créé$(RESET)"

# Flash du CPU (version production)
flash: hex
	@echo "$(YELLOW)=== Flashage du microcontrôleur (production) ===$(RESET)"
	@$(AVRDUDE) -c $(PROG) -p $(MCU) -P $(PORT) -b $(BAUDRATE) -U flash:w:main.hex:i
	@echo "$(GREEN)✓ Programme flashé avec succès$(RESET)"

# Flash du CPU (version debug)
flash-debug: hex-debug
	@echo "$(YELLOW)=== Flashage du microcontrôleur (debug) ===$(RESET)"
	@$(AVRDUDE) -c $(PROG) -p $(MCU) -P $(PORT) -b $(BAUDRATE) -U flash:w:debug.hex:i
	@echo "$(GREEN)✓ Programme debug flashé avec succès$(RESET)"

# Compile et flash la version debug
debug: hex-debug flash-debug
	@echo "$(GREEN)✓ Version debug compilée et flashée$(RESET)"

# Moniteur série
monitor:
	@echo "$(CYAN)=== Ouverture du moniteur série à $(BAUDRATE) baud ===$(RESET)"
	@echo "$(YELLOW)Appuyez sur Ctrl+A puis K pour quitter$(RESET)"
	@screen $(PORT) $(BAUDRATE)

# Nettoyage
clean:
	@echo "$(BLUE)=== Nettoyage ===$(RESET)"
	@rm -f main.hex main.bin debug.hex debug.bin
	@echo "$(GREEN)✓ Fichiers supprimés$(RESET)"

# Informations sur le programme compilé
size: main.bin
	@echo "$(YELLOW)=== Taille du programme ===$(RESET)"
	@avr-size main.bin

size-debug: debug.bin
	@echo "$(YELLOW)=== Taille du programme (debug) ===$(RESET)"
	@avr-size debug.bin

# Aide
help:
	@echo "$(CYAN)=== Makefile pour ATmega328P - AHT20 I2C ===$(RESET)"
	@echo ""
	@echo "$(YELLOW)Cibles disponibles:$(RESET)"
	@echo "  $(GREEN)all$(RESET)          - Compile et flash la version production (défaut)"
	@echo "  $(GREEN)debug$(RESET)        - Compile et flash la version debug (avec affichage des status I2C)"
	@echo "  $(GREEN)hex$(RESET)          - Compile et génère le fichier .hex (production)"
	@echo "  $(GREEN)hex-debug$(RESET)    - Compile et génère le fichier .hex (debug)"
	@echo "  $(GREEN)flash$(RESET)        - Flash la version production"
	@echo "  $(GREEN)flash-debug$(RESET)  - Flash la version debug"
	@echo "  $(GREEN)monitor$(RESET)      - Ouvre le moniteur série (115200 baud)"
	@echo "  $(GREEN)size$(RESET)         - Affiche la taille du programme production"
	@echo "  $(GREEN)size-debug$(RESET)   - Affiche la taille du programme debug"
	@echo "  $(GREEN)clean$(RESET)        - Supprime les fichiers générés"
	@echo "  $(GREEN)help$(RESET)         - Affiche cette aide"
	@echo ""
	@echo "$(YELLOW)Exemples:$(RESET)"
	@echo "  make              # Compile et flash la version production"
	@echo "  make debug        # Compile et flash la version debug"
	@echo "  make monitor      # Ouvre le moniteur série"
	@echo "  make clean        # Nettoie les fichiers"
	@echo ""
	@echo "$(YELLOW)Versions:$(RESET)"
	@echo "  $(CYAN)Production$(RESET)  - Lecture continue des données (main.c)"
	@echo "  $(CYAN)Debug$(RESET)       - Affichage détaillé des status I2C (main_debug.c)"

.PHONY: all hex hex-debug flash flash-debug debug monitor clean size size-debug help