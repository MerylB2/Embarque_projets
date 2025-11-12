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
SRC_TEST	= main_test_errors.c i2c.c uart.c aht20.c

#colors
RED			= \033[1;31m
GREEN		= \033[1;32m
YELLOW		= \033[1;33m
BLUE		= \033[1;34m
CYAN		= \033[1;36m
RESET		= \033[0m

# General rule
all: hex flash

# Compilation : .c -> .bin (version normale)
main.bin: $(SRC)
	@echo "$(BLUE)=== Compilation des fichiers sources ===$(RESET)"
	@echo "$(CYAN)Sources: $(SRC)$(RESET)"
	@$(CC) $(CFLAGS) -o main.bin $(SRC)
	@echo "$(GREEN)✓ Fichier main.bin créé$(RESET)"

# Compilation : .c -> .bin (version test erreurs)
test.bin: $(SRC_TEST)
	@echo "$(BLUE)=== Compilation version test erreurs ===$(RESET)"
	@echo "$(CYAN)Sources: $(SRC_TEST)$(RESET)"
	@$(CC) $(CFLAGS) -o test.bin $(SRC_TEST)
	@echo "$(GREEN)✓ Fichier test.bin créé$(RESET)"

# Conversion: .bin -> .hex (version normale)
hex: main.bin
	@echo "$(BLUE)=== Conversion en format HEX ===$(RESET)"
	@$(OBJCOPY) -O ihex main.bin main.hex
	@echo "$(CYAN)✓ Fichier main.hex créé$(RESET)"

# Conversion: .bin -> .hex (version test)
hex-test: test.bin
	@echo "$(BLUE)=== Conversion en format HEX (test) ===$(RESET)"
	@$(OBJCOPY) -O ihex test.bin test.hex
	@echo "$(CYAN)✓ Fichier test.hex créé$(RESET)"

# Flash du CPU (version normale)
flash: hex
	@echo "$(YELLOW)=== Flashage du microcontrôleur ===$(RESET)"
	@$(AVRDUDE) -c $(PROG) -p $(MCU) -P $(PORT) -b $(BAUDRATE) -U flash:w:main.hex:i
	@echo "$(GREEN)✓ Programme flashé avec succès$(RESET)"

# Flash du CPU (version test)
flash-test: hex-test
	@echo "$(YELLOW)=== Flashage du microcontrôleur (test erreurs) ===$(RESET)"
	@$(AVRDUDE) -c $(PROG) -p $(MCU) -P $(PORT) -b $(BAUDRATE) -U flash:w:test.hex:i
	@echo "$(GREEN)✓ Programme test flashé avec succès$(RESET)"

# Test des cas d'erreur I2C (compile + flash)
error_test: hex-test flash-test
	@echo "$(GREEN)✓ Version test des erreurs I2C flashée$(RESET)"
	@echo "$(CYAN)=== Ouverture du moniteur série ===$(RESET)"
	@echo "$(YELLOW)Appuyez sur Ctrl+A puis K pour quitter$(RESET)"

# Moniteur série
monitor:
	@echo "$(CYAN)=== Ouverture du moniteur série à $(BAUDRATE) baud ===$(RESET)"
	@echo "$(YELLOW)Appuyez sur Ctrl+A puis K pour quitter$(RESET)"
	@screen $(PORT) $(BAUDRATE)

# Nettoyage
clean:
	@echo "$(BLUE)=== Nettoyage ===$(RESET)"
	@rm -f main.hex main.bin test.hex test.bin
	@echo "$(GREEN)✓ Fichiers supprimés$(RESET)"

# Informations sur le programme compilé
size: main.bin
	@echo "$(YELLOW)=== Taille du programme ===$(RESET)"
	@avr-size main.bin

size-test: test.bin
	@echo "$(YELLOW)=== Taille du programme test ===$(RESET)"
	@avr-size test.bin

# Aide
help:
	@echo "$(CYAN)========================================$(RESET)"
	@echo "$(CYAN)  Makefile - EX00 Communication I2C/TWI    $(RESET)"
	@echo "$(CYAN)========================================$(RESET)"
	@echo ""
	@echo "$(YELLOW)Cibles principales:$(RESET)"
	@echo "  $(GREEN)make$(RESET)              Compile et flash le programme principal"
	@echo "  $(GREEN)make error_test$(RESET)   Compile, flash et teste les erreurs I2C"
	@echo "  $(GREEN)make monitor$(RESET)      Ouvre le moniteur série (115200 baud)"
	@echo "  $(GREEN)make clean$(RESET)        Supprime les fichiers générés"
	@echo "  $(GREEN)make help$(RESET)         Affiche cette aide"
	@echo ""
	@echo "$(YELLOW)Cibles avancées:$(RESET)"
	@echo "  $(GREEN)make hex$(RESET)          Compile uniquement le .hex principal"
	@echo "  $(GREEN)make hex-test$(RESET)     Compile uniquement le .hex de test"
	@echo "  $(GREEN)make flash$(RESET)        Flash le programme principal"
	@echo "  $(GREEN)make flash-test$(RESET)   Flash le programme de test"
	@echo "  $(GREEN)make size$(RESET)         Taille du programme principal"
	@echo "  $(GREEN)make size-test$(RESET)    Taille du programme de test"
	@echo ""
	@echo "$(YELLOW)Exemples:$(RESET)"
	@echo "  make && make monitor       # Programme principal"
	@echo "  make error_test            # Tests des erreurs I2C"
	@echo "  make clean && make         # Recompiler"
	@echo ""

.PHONY: all hex hex-test flash flash-test error_test monitor clean size size-test help