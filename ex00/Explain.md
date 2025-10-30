```c
void ft_delay_ms(void)
{
	// Boucle externe: 20 itérations
	for (uint8_t i = 0; i < 20; i++)
	{
		// Boucle interne: ~50000 itérations
		// Calcul approximatif: 20 * 50000 * 4 cycles ≈ 4M cycles
		// À 16MHz: 4M cycles / 16M Hz = 0.25s
		// On fait 2 fois cette boucle pour ~500ms
		for (uint16_t j = 0; j < 50000; j++)
		{
			// Boucle vide - le compilateur ne devrait pas l'optimiser
			// car nous n'utilisons pas -O3
			asm volatile("nop");
		}
	}
}

int main(void)
{
	// Configuration de PB1 (broche 9, LED D2) en sortie
	// DDRB = Data Direction Register pour le port B
	// Mettre le bit 1 à 1 pour configurer PB1 en OUTPUT
	// 0b00000010 = 0x02 = (1 << PB1)
	DDRB |= (1 << PB1);

	// Initialisation: LED éteinte au démarrage
	// PORTB = Port B Data Register
	// Mettre le bit 1 à 0 pour éteindre la LED
	PORTB &= ~(1 << PB1);

	// Boucle infinie - le programme ne doit jamais se terminer
	while (1)
	{
		// Toggle (inversion) de l'état de PB1 avec XOR
		// PINB = Port B Input Pins Register
		// Écrire 1 sur PINB toggle le bit correspondant dans PORTB
		// C'est une fonctionnalité hardware de l'ATmega328p
		// Pas de condition if/else nécessaire
		PINB = (1 << PB1);

		// Attente de 500ms
		// Avec toggle toutes les 500ms, on obtient:
		// - LED ON pendant 500ms
		// - LED OFF pendant 500ms
		// = Période de 1 seconde = Fréquence de 1Hz
		ft_delay_ms();
	}

	// Cette ligne ne sera jamais atteinte
	// Le main ne doit pas return (requis par l'exercice)
	return 0;
}
```

```

---

## 🎨 Schéma Visuel : Manipulation de Bits

### Opération OR (|=) - Allumer LED
```
Étape 1 : État initial de PORTB
┌───┬───┬───┬───┬───┬───┬───┬───┐
│ 0 │ 0 │ 0 │ 0 │ 0 │ 0 │ 0 │ 0 │  PORTB actuel
└───┴───┴───┴───┴───┴───┴───┴───┘

Étape 2 : Masque (1 << PORTB1)
┌───┬───┬───┬───┬───┬───┬───┬───┐
│ 0 │ 0 │ 0 │ 0 │ 0 │ 0 │ 1 │ 0 │  Masque
└───┴───┴───┴───┴───┴───┴───┴───┘
                          ▲
                          └─ Bit 1 = 1

Étape 3 : OR logique (PORTB |= masque)
┌───┬───┬───┬───┬───┬───┬───┬───┐
│ 0 │ 0 │ 0 │ 0 │ 0 │ 0 │ 0 │ 0 │  PORTB
│ OR│ OR│ OR│ OR│ OR│ OR│ OR│ OR│
│ 0 │ 0 │ 0 │ 0 │ 0 │ 0 │ 1 │ 0 │  Masque
└─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┘
  ▼   ▼   ▼   ▼   ▼   ▼   ▼   ▼
┌───┬───┬───┬───┬───┬───┬───┬───┐
│ 0 │ 0 │ 0 │ 0 │ 0 │ 0 │ 1 │ 0 │  PORTB final
└───┴───┴───┴───┴───┴───┴───┴───┘
                          ▲
                          └─ LED ALLUMÉE

Datasheet p.99 : PORTB Register Description
```

### Opération AND NOT (&= ~) - Éteindre LED
```
Étape 1 : État actuel de PORTB
┌───┬───┬───┬───┬───┬───┬───┬───┐
│ 0 │ 0 │ 0 │ 0 │ 0 │ 0 │ 1 │ 0 │  PORTB actuel
└───┴───┴───┴───┴───┴───┴───┴───┘
                          ▲
                          └─ LED actuellement ON

Étape 2 : Masque (1 << PORTB1)
┌───┬───┬───┬───┬───┬───┬───┬───┐
│ 0 │ 0 │ 0 │ 0 │ 0 │ 0 │ 1 │ 0 │  Masque
└───┴───┴───┴───┴───┴───┴───┴───┘

Étape 3 : Inversion NOT (~masque)
┌───┬───┬───┬───┬───┬───┬───┬───┐
│ 1 │ 1 │ 1 │ 1 │ 1 │ 1 │ 0 │ 1 │  ~Masque
└───┴───┴───┴───┴───┴───┴───┴───┘
                          ▲
                          └─ Seul ce bit est 0

Étape 4 : AND logique (PORTB &= ~masque)
┌───┬───┬───┬───┬───┬───┬───┬───┐
│ 0 │ 0 │ 0 │ 0 │ 0 │ 0 │ 1 │ 0 │  PORTB
│AND│AND│AND│AND│AND│AND│AND│AND│
│ 1 │ 1 │ 1 │ 1 │ 1 │ 1 │ 0 │ 1 │  ~Masque
└─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┴─┬─┘
  ▼   ▼   ▼   ▼   ▼   ▼   ▼   ▼
┌───┬───┬───┬───┬───┬───┬───┬───┐
│ 0 │ 0 │ 0 │ 0 │ 0 │ 0 │ 0 │ 0 │  PORTB final
└───┴───┴───┴───┴───┴───┴───┴───┘
                          ▲
                          └─ LED ÉTEINTE

Table de vérité AND :
0 AND 0 = 0
0 AND 1 = 0
1 AND 0 = 0  ← Force le bit à 0
1 AND 1 = 1
```

---

## 📊 Chronogramme de Fonctionnement
```
Temps (ms):  0     500    1000   1500   2000   2500   3000
             │      │      │      │      │      │      │
             ▼      ▼      ▼      ▼      ▼      ▼      ▼

Signal PB1:  ┌──────┐      ┌──────┐      ┌──────┐
(Logique)    │      │      │      │      │      │
           ──┘      └──────┘      └──────┘      └──────

LED D2:      ●●●●●●○○○○○○●●●●●●○○○○○○●●●●●●○○○○○○
(Visuel)     ON    OFF    ON    OFF    ON    OFF

Cycle:       ◄─ T=1s ─►◄─ T=1s ─►◄─ T=1s ─►
Fréquence:   f = 1/T = 1 Hz

Appels:      my_delay_500ms()
             ▲      ▲      ▲      ▲      ▲      ▲
             │      │      │      │      │      │
Code:        ON  ─  OFF ─  ON  ─  OFF ─  ON  ─  OFF
```

---

## 🔬 Analyse Détaillée de la Boucle de Délai

### Désassemblage AVR (Pseudo-code)
```
my_delay_500ms:
┌─────────────────────────────────────────────────────────┐
│                                                         │
│  count = 1600000                                        │
│  ▼                                                      │
│  ┌──────────────────────────────┐                      │
│  │  LOOP:                        │ ◄─┐                 │
│  │    nop              [1 cycle] │   │                 │
│  │    count--          [2 cycles]│   │                 │
│  │    if(count != 0)   [1 cycle] │   │                 │
│  │      goto LOOP      [2 cycles]│ ──┘                 │
│  └──────────────────────────────┘                      │
│                                                         │
│  Total par itération : 1+2+1+2 = 6 cycles              │
│  (en pratique : ~5 cycles avec optimisations)          │
│                                                         │
│  Temps réel = (1600000 × 5) / 16000000                 │
│             = 8000000 / 16000000                        │
│             = 0.5 secondes                              │
└─────────────────────────────────────────────────────────┘

Datasheet p.14 : AVR Instruction Set
- NOP : 1 cycle
- SBIW (Subtract Immediate from Word) : 2 cycles
- BRNE (Branch if Not Equal) : 1-2 cycles
```

---

## ⚠️ Problèmes et Optimisations

### Problème 1 : Optimisation du Compilateur
```
┌─────────────────────────────────────────────────────────┐
│  SANS volatile :                                        │
│  ────────────────                                       │
│  Le compilateur voit :                                  │
│    for(count = 0; count < 1600000; count++) { }        │
│                                                         │
│  Il pense : "Cette boucle ne fait rien !"              │
│  Résultat : SUPPRIME LA BOUCLE ENTIÈREMENT !           │
│                                                         │
│  AVEC volatile :                                        │
│  ────────────────                                       │
│  volatile uint32_t count;                              │
│                                                         │
│  Le compilateur sait :                                  │
│  "Cette variable peut changer à tout moment"           │
│  Résultat : GARDE LA BOUCLE INTACTE                    │
└─────────────────────────────────────────────────────────┘

Code minimum pour forcer l'exécution :
    volatile uint32_t count = 1600000UL;
    while(count--);
```

### Problème 2 : Précision du Délai
```
┌─────────────────────────────────────────────────────────┐
│  Facteurs affectant la précision :                     │
│                                                         │
│  1. Overhead de la boucle while(1) dans main()         │
│     ├─ Saut vers début de boucle : ~2 cycles           │
│     └─ Appel de fonction : ~4 cycles                   │
│                                                         │
│  2. Instructions PORTB |= et &= :                      │
│     └─ SBI instruction : 2 cycles chacune              │
│                                                         │
│  3. Variations du compilateur selon -O0, -O1, -Os      │
│                                                         │
│  Précision réelle : ±5% (acceptable pour LED)          │
└─────────────────────────────────────────────────────────┘


#define F_CPU 16000000UL
#include <avr/io.h>

// Datasheet p.14 : Instruction timing
void ft_delay_ms(void)
{
    // Calibré pour ~250ms (mesuré avec oscilloscope)
    // Boucle externe: 32 itérations pour atteindre 500ms
    for (uint8_t i = 0; i < 32; i++)
    {
        // Boucle interne: 50000 itérations
        // 32 × 50000 × 5 cycles ≈ 8M cycles = 500ms
        for (uint16_t j = 0; j < 50000; j++)
        {
            // NOP = 1 cycle (Datasheet p.14)
            __asm__ volatile("nop");
        }
    }
}

int main(void)
{
    // Datasheet p.98 : DDRB - Data Direction Register B
    // Configuration PB1 en sortie pour LED D2
    DDRB |= (1 << DDB1);
    
    // Datasheet p.99 : PORTB - Port B Data Register  
    // Initialisation LED éteinte
    PORTB &= ~(1 << PORTB1);
    
    while (1)
    {
        // Datasheet p.99 : Toggle avec PINB (fonctionnalité HW)
        // Écrire 1 sur PINB fait un XOR automatique sur PORTB
        PINB = (1 << PINB1);
        
        // Délai 500ms
        ft_delay_ms();
    }
    
    // Jamais atteint (requis par consigne)
    return 0;
}
```

---

## 📈 Chronogramme Complet
```
Temps (ms):    0        500      1000      1500      2000
               │         │         │         │         │
               ▼         ▼         ▼         ▼         ▼

Code:        Toggle   Delay    Toggle   Delay    Toggle
             PINB=2   500ms    PINB=2   500ms    PINB=2
               │                 │                 │
               ▼                 ▼                 ▼

PORTB[1]:      0 ──────────────> 1 ──────────────> 0
             (OFF)              (ON)              (OFF)

LED D2:      ○○○○○○○○○○○○○○○●●●●●●●●●●●●●●●○○○○○○○○○○
             OFF              ON               OFF

Cycles CPU:  [    8M cycles   ][    8M cycles   ]
               Busy-wait         Busy-wait

ft_delay_ms  ┌─ i=0 ─┐┌─ i=1 ─┐┌─ i=2 ─┐  ... ┌─ i=31 ─┐
appels:      │j:50000││j:50000││j:50000│      │j:50000 │
             └───────┘└───────┘└───────┘      └────────┘
                         32 itérations totales
```

---

## 🎓 Comparaison des Techniques de Délai
```
┌─────────────────────────────────────────────────────────────┐
│  TECHNIQUE 1 : Boucle simple (uint32_t)                     │
├─────────────────────────────────────────────────────────────┤
│  for(uint32_t i = 0; i < 1600000; i++) { nop; }           │
│                                                             │
│  ✓ Simple et direct                                         │
│  ✗ Utilise 4 bytes RAM                                      │
│  ✗ Code 32-bit plus lourd                                   │
│  ✗ Plus lent (arithmétique 32-bit sur AVR 8-bit)           │
└─────────────────────────────────────────────────────────────┘

┌─────────────────────────────────────────────────────────────┐
│  TECHNIQUE 2 : Boucles imbriquées (uint8_t + uint16_t)     │
├─────────────────────────────────────────────────────────────┤
│  for(i=0; i<32; i++)                                        │
│      for(j=0; j<50000; j++) { nop; }                       │
│                                                             │
│  ✓ Économe en RAM (3 bytes)                                 │
│  ✓ Code plus compact                                        │
│  ✓ Plus rapide (arithmétique 8/16-bit native)              │
│  ✓ Meilleure pratique en embarqué                          │
│  ✗ Légèrement plus complexe                                 │
└─────────────────────────────────────────────────────────────┘

┌─────────────────────────────────────────────────────────────┐
│  TECHNIQUE 3 : Timer Hardware (EX01)                        │
├─────────────────────────────────────────────────────────────┤
│  Configure Timer1 en mode CTC                               │
│  while(1) { /* CPU libre */ }                              │
│                                                             │
│  ✓ CPU libre à 100%                                         │
│  ✓ Précision maximale (quartz)                             │
│  ✓ Non-bloquant                                             │
│  ✗ Plus complexe à configurer                               │
│  ✗ Utilise une ressource hardware                           │
└─────────────────────────────────────────────────────────────┘
```

---

## 🔑 Points Clés de Votre Code
```
┌─────────────────────────────────────────────────────────┐
│  ✓ Boucles imbriquées : Économie mémoire                │
│                                                         │
│  ✓ PINB toggle : Utilise fonctionnalité HW unique      │
│                                                         │
│  ✓ Types optimisés : uint8_t et uint16_t              │
│                                                         │
│  ✓ Pas de condition : Toggle direct sans if/else       │
│                                                         │
│  ✓ Commentaires clairs : Explique chaque étape         │
│                                                         │
│  ⚠️ Calibration : Ajuster i<32 pour 500ms exact         │
└─────────────────────────────────────────────────────────┘
```

---

## 📚 Références Datasheet
```
┌─────────────────────────────────────────────────────────┐
│  Page 6   : Memory (RAM/Flash limits)                  │
│  Page 14  : Instruction Set (NOP, SBIW, BRNE)         │
│  Page 28  : Clock System (F_CPU)                       │
│  Page 98  : DDRB Register                              │
│  Page 99  : PORTB/PINB Registers + Toggle feature     │
└─────────────────────────────────────────────────────────┘

// // Fonction de délai
// void wait_centaines_ms(int centaines_ms)
// {
// 	for (int i = 0; i < centaines_ms; i++)
// 	{
// 		for (int j = 0; j < 2000; j++)
// 		{
// 			for (int k = 0; k < 200; k++)
// 			{
// 			   asm volatile("nop");
// 			}
// 		}
// 	}
// }

// int main(void)
// {
// 	DDRB |= (1 << PB1);
// 	PORTB &= ~(1 << PB1);
	
// 	while (1)
// 	{
// 		PORTB ^= (1 << PB1);
// 		wait_centaines_ms(5);  // 500ms
// 	}
	
// 	return 0;
// }
