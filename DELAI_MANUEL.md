# Comment créer une fonction de délai sans _delay_ms ? ⏱️

## 🎯 Le défi

**Créer un délai de plusieurs centaines de millisecondes SANS utiliser `_delay_ms` !**

---

## 💡 Le principe

### Idée de base

Un **délai** = faire attendre le programme en exécutant des instructions qui ne font "rien".

```
CPU exécute des instructions → prend du temps → crée un délai
```

---

### Comment ça marche ?

1. **Le CPU a une fréquence** (ex: 16 MHz = 16 000 000 cycles/seconde)
2. **Chaque instruction prend des cycles**
3. **En exécutant beaucoup d'instructions**, on crée du délai

---

## 🔢 Les calculs

### Fréquence du CPU

```
F_CPU = 16 000 000 Hz (16 MHz)

1 seconde = 16 000 000 cycles
1 milliseconde = 16 000 cycles
100 millisecondes = 1 600 000 cycles
```

---

### Cycles d'une boucle

```c
for (int i = 0; i < 1000; i++)
{
    // Boucle vide
}
```

**Combien de cycles ?**

Chaque itération :
1. **Comparer** `i < 1000` → 1-2 cycles
2. **Incrémenter** `i++` → 1 cycle
3. **Sauter** au début → 1-2 cycles

**Total ≈ 3-5 cycles par itération**

Pour 1000 itérations : **≈ 3000-5000 cycles**

---

##  La solution : Boucles imbriquées

### Version simple (pas assez de délai)

```c
for (int i = 0; i < 1000; i++)
{
    // Trop rapide !
}
```

**Problème :** 1000 itérations × 5 cycles = 5000 cycles = 0.3ms seulement !

---

### Version avec boucles imbriquées (meilleure)

```c
void wait_centaines_ms(int centaines_ms)
{
    for (int i = 0; i < centaines_ms; i++)       // Boucle externe
    {
        for (int j = 0; j < 2000; j++)           // Boucle intermédiaire
        {
            for (int k = 0; k < 200; k++)        // Boucle interne
            {
                // Boucle vide
            }
        }
    }
}
```

**Calcul :**
- Boucle interne : 200 itérations
- Boucle intermédiaire : 2000 itérations
- **Total par centaine de ms :** 200 × 2000 = 400 000 itérations

**À 5 cycles/itération :**
- 400 000 × 5 = **2 000 000 cycles**
- 2 000 000 / 16 000 = **125 millisecondes** ≈ 1 centaine de ms ✓

---

## 📝 Code de la fonction

```c
/*
 * Fonction de délai manuelle
 * 
 * Paramètre : centaines_ms
 * Nombre de centaines de millisecondes (×100ms)
 * 
 * Exemple :
 * wait_centaines_ms(5)  → 500ms
 * wait_centaines_ms(10) → 1000ms (1 seconde)
 */
void wait_centaines_ms(int centaines_ms)
{
    // Boucle externe : nombre de centaines de ms
    for (int i = 0; i < centaines_ms; i++)
    {
        // Boucle intermédiaire : crée ~100ms
        for (int j = 0; j < 2000; j++)
        {
            // Boucle interne : multiplie le délai
            for (int k = 0; k < 200; k++)
            {
                // Boucle vide (consomme des cycles)
            }
        }
    }
}
```

---

## 🎯 Utilisation

```c
int main(void)
{
    DDRB |= (1 << PB1);
    PORTB &= ~(1 << PB1);
    
    while (1)
    {
        PORTB ^= (1 << PB1);        // Toggle LED
        wait_centaines_ms(5);       // Attendre 500ms
    }
    
    return 0;
}
```

**Résultat :** LED clignote à environ 1 Hz ! ✓

---

## ⚠️ Problèmes et limites

### 1. Délai pas précis

**Pourquoi ?**
- Le nombre de cycles par instruction varie
- L'optimisation du compilateur change le code
- On a utilisé des valeurs approximatives

**Solution :**
- Pour un délai précis → utiliser un TIMER hardware
- Pour cet exercice → approximation suffisante

---

### 2. Optimisation du compilateur

**Problème :**
```c
for (int k = 0; k < 200; k++)
{
    // Boucle vide → le compilateur peut la supprimer !
}
```

**Avec optimisation `-O2` :**
```
Le compilateur voit : "Cette boucle ne fait rien"
→ Il la supprime !
→ Plus de délai !
```

**Solution 1 : Compiler sans optimisation**
```makefile
CFLAGS = -Wall -O0  # Pas d'optimisation
```

**Solution 2 : Utiliser `volatile`**
```c
for (volatile int k = 0; k < 200; k++)
{
    // Le mot-clé "volatile" empêche l'optimisation
}
```

**Solution 3 : Ajouter une instruction assembleur**
```c
for (int k = 0; k < 200; k++)
{
    asm volatile ("nop");  // No OPeration (1 cycle)
}
```

---

### 3. Calibration nécessaire

**Les valeurs 2000 et 200 sont approximatives !**

Pour calibrer :
1. Compiler et tester
2. Mesurer la fréquence réelle (avec oscilloscope ou comptage)
3. Ajuster les valeurs

**Exemples d'ajustement :**
```c
// Trop rapide ? Augmenter les valeurs
for (int j = 0; j < 3000; j++)  // Au lieu de 2000
for (int k = 0; k < 300; k++)   // Au lieu de 200

// Trop lent ? Diminuer les valeurs
for (int j = 0; j < 1000; j++)  // Au lieu de 2000
for (int k = 0; k < 100; k++)   // Au lieu de 200
```

---

## 🔬 Version avec `asm volatile`

```c
void wait_centaines_ms(int centaines_ms)
{
    for (int i = 0; i < centaines_ms; i++)
    {
        for (int j = 0; j < 2000; j++)
        {
            for (int k = 0; k < 200; k++)
            {
                /*
                 * Instruction assembleur "nop"
                 * = No OPeration (ne fait rien)
                 * Prend exactement 1 cycle CPU
                 * "volatile" empêche l'optimisation
                 */
                asm volatile ("nop");
            }
        }
    }
}
```

**Avantage :** Plus précis, résiste à l'optimisation

---

## 📊 Comparaison des méthodes

| Méthode | Précision | Résiste à l'optimisation | Complexité |
|---------|-----------|--------------------------|------------|
| Boucles simples | Faible | Non | Simple |
| `volatile int` | Moyenne | Oui | Simple |
| `asm volatile` | Haute | Oui | Moyenne |
| TIMER hardware | Très haute | N/A | Complexe |

---

## 🎓 Pourquoi cet exercice ?

### Objectifs pédagogiques

1. **Comprendre comment fonctionnent les délais**
   - Les `_delay_ms` utilisent le même principe !

2. **Apprendre la relation CPU / temps**
   - Fréquence CPU → cycles → temps

3. **Découvrir l'optimisation du compilateur**
   - Le compilateur peut modifier votre code !

4. **Préparer aux TIMER hardware**
   - Plus tard, vous utiliserez des timers précis

---

## ✅ Résumé

```
Pour créer un délai SANS _delay_ms :

1. Utiliser des boucles imbriquées
2. Faire des boucles "vides"
3. Calculer le nombre d'itérations nécessaire
4. Tester et ajuster

Formule approximative :
Délai (ms) = (itérations × cycles) / (F_CPU / 1000)
```

---

## 🚀 Exemple complet

```c
#include <avr/io.h>

// Fonction de délai
void wait_centaines_ms(int centaines_ms)
{
    for (int i = 0; i < centaines_ms; i++)
    {
        for (int j = 0; j < 2000; j++)
        {
            for (int k = 0; k < 200; k++)
            {
                // Boucle vide
            }
        }
    }
}

int main(void)
{
    DDRB |= (1 << PB1);
    PORTB &= ~(1 << PB1);
    
    while (1)
    {
        PORTB ^= (1 << PB1);
        wait_centaines_ms(5);  // 500ms
    }
    
    return 0;
}
```

---

**Maintenant vous comprenez comment fonctionnent les délais ! ⏱️**


```c
#include <avr/io.h>

/**
 * Fonction de délai utilisant des boucles vides
 * Calibrée pour approximativement 500ms à 16MHz
 */
void delay_500ms(void)
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
		delay_500ms();
	}

	// Cette ligne ne sera jamais atteinte
	// Le main ne doit pas return (requis par l'exercice)
	return 0;
}
```