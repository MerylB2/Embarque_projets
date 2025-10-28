# Comment calculer les itérations pour un délai précis

## 1. Comprendre la relation fondamentale

```
Temps = Nombre de cycles / Fréquence du CPU
```

**Exemple :**
- CPU à 16 MHz = 16 000 000 cycles par seconde
- Si j'exécute 16 000 000 cycles → ça prend 1 seconde
- Si j'exécute 8 000 000 cycles → ça prend 0.5 seconde (500ms)

---

## 2. Méthode de calcul théorique

### Étape 1 : Déterminer le temps voulu
```
Objectif : 500ms = 0.5 seconde
```

### Étape 2 : Calculer le nombre de cycles nécessaires
```
Cycles nécessaires = Temps voulu × Fréquence CPU
Cycles nécessaires = 0.5s × 16 000 000 Hz
Cycles nécessaires = 8 000 000 cycles
```

### Étape 3 : Estimer les cycles par itération de boucle

Analysons cette boucle simple :
```c
for (uint16_t j = 0; j < 50000; j++)
{
    asm volatile("nop");
}
```

**Cycles par itération :**
```
1. nop                    : 1 cycle
2. j++                    : 1-2 cycles (INC instruction)
3. Comparaison j < 50000  : 1-2 cycles (CMP instruction)
4. Branchement (saut)     : 1-2 cycles (BRLT instruction)

Total approximatif : 4-6 cycles par itération
```

Prenons **5 cycles** comme moyenne.

### Étape 4 : Calculer le nombre d'itérations

```
Nombre d'itérations = Cycles nécessaires / Cycles par itération
Nombre d'itérations = 8 000 000 / 5
Nombre d'itérations = 1 600 000 itérations
```

### Étape 5 : Répartir avec des boucles imbriquées

**Problème :** `uint16_t` ne peut stocker que jusqu'à 65 535

**Solution :** Boucles imbriquées
```c
for (uint8_t i = 0; i < X; i++)      // Boucle externe
{
    for (uint16_t j = 0; j < Y; j++)  // Boucle interne
    {
        asm volatile("nop");
    }
}

Total itérations = X × Y
```

**Exemples de combinaisons pour 1 600 000 :**

| X (externe) | Y (interne) | Total         | Note   |
|-------------|-------------|-------        |------  |
| 32          | 50 000      | 1 600 000     | ✅ Bon |
| 40          | 40 000      | 1 600 000     | ✅ Bon |
| 20          | 80 000      | 1 600 000     | ❌ Dépassement uint16_t |
| 160         | 10 000      | 1 600 000     | ❌ Dépassement uint8_t |

**Choix dans le code :** `20 × 50 000 = 1 000 000`
- Plus simple à retenir
- Donne environ 312ms avec les cycles réels
- Donc on l'exécute ~2 fois dans la fonction pour approcher 500ms

---

## 3. Méthode empirique (la plus pratique)

En réalité, calculer théoriquement est compliqué car :
- Le compilateur optimise différemment
- Les boucles ont un overhead (coût supplémentaire)
- Les flags de compilation changent tout

### Approche par essai-erreur

**Étape 1 : Commencer avec une estimation grossière**
```c
void delay_test(void)
{
    for (uint32_t i = 0; i < 1000000; i++)
    {
        asm volatile("nop");
    }
}
```

**Étape 2 : Tester sur le hardware**
- Flasher le code
- Chronométrer avec un chronomètre ou un oscilloscope
- Observer la LED

**Étape 3 : Ajuster**
```
Si la LED clignote trop vite → augmenter le nombre d'itérations
Si la LED clignote trop lentement → diminuer le nombre d'itérations
```

**Étape 4 : Affiner progressivement**
```
Essai 1 : 500 000 itérations → trop rapide
Essai 2 : 1 000 000 itérations → encore un peu rapide
Essai 3 : 1 500 000 itérations → presque bon
Essai 4 : 1 600 000 itérations → parfait !
```

---

## 4. Exemple de calcul complet

Faisons un calcul détaillé pour comprendre :

### Code analysé :
```c
void delay_500ms(void)
{
    for (uint8_t i = 0; i < 20; i++)           // 20 itérations
    {
        for (uint16_t j = 0; j < 50000; j++)   // 50 000 itérations
        {
            asm volatile("nop");                // 1 cycle
        }
    }
}
```

### Calcul pas à pas :

**1. Boucle interne (une seule exécution) :**
```
Itérations : 50 000
Cycles par itération : ~5 cycles
Total : 50 000 × 5 = 250 000 cycles
```

**2. Boucle externe :**
```
La boucle interne s'exécute 20 fois
Total : 20 × 250 000 = 5 000 000 cycles
```

**3. Overhead de la boucle externe :**
```
Pour chaque itération de la boucle externe :
- Incrémentation i++
- Comparaison i < 20
- Saut

Environ 3-5 cycles × 20 itérations = 60-100 cycles (négligeable)
```

**4. Temps total :**
```
Cycles totaux : ~5 000 000 cycles
Fréquence CPU : 16 000 000 Hz

Temps = 5 000 000 / 16 000 000
Temps = 0.3125 secondes
Temps = 312.5 ms
```

**Conclusion :** Cette fonction donne environ **312ms**, pas 500ms !

---

## 5. Comment j'ai trouvé les valeurs de mon code

**Honnêtement ?** J'ai utilisé des valeurs standard qu'on trouve souvent dans les exemples Arduino/AVR :

```c
// Valeurs "magiques" courantes pour 16MHz :
- Pour ~1ms  : 2 000 - 4 000 itérations
- Pour ~10ms : 20 000 - 40 000 itérations  
- Pour ~100ms: 200 000 - 400 000 itérations
- Pour ~500ms: 1 000 000 - 2 000 000 itérations
```

Ces valeurs varient selon :
- Le compilateur (avr-gcc version)
- Les flags d'optimisation (`-Os`, `-O2`, `-O3`)
- La structure du code

---

## 6. La vraie méthode professionnelle

Pour être précis, il faut :

### A. Regarder le code assembleur généré

```bash
avr-gcc -S -O2 main.c -o main.s
```

Puis analyser le fichier `.s` :

```assembly
LBB0_1:
    nop           ; 1 cycle
    inc r24       ; 1 cycle (incrémentation)
    cpi r24, 200  ; 1 cycle (comparaison)
    brne LBB0_1   ; 2/1 cycles (saut si ≠)
```

**Calcul précis :**
- Itérations 0-199 : 1+1+1+2 = 5 cycles
- Dernière itération : 1+1+1+1 = 4 cycles (pas de saut)

### B. Utiliser un oscilloscope ou analyseur logique

Connecter sur la pin de la LED et mesurer :
```
Temps HIGH = ?
Temps LOW = ?
Fréquence = ?
```

Ajuster le code jusqu'à obtenir exactement 1 Hz.

---

## 7. Tableau récapitulatif pour 16MHz

| Délai voulu | Cycles nécessaires | Itérations (5 cycles/it) | Exemple boucles |
|-------------|------------------- |-------------------------|----------------- |
| 1 ms        | 16 000             | 3 200                   | 1 × 3200         |   
| 10 ms       | 160 000            | 32 000                  | 1 × 32000        |
| 100 ms      | 1 600 000          | 320 000                 | 10 × 32000       |
| 500 ms      | 8 000 000          | 1 600 000               | 32 × 50000       |
| 1 s         | 16 000 000         | 3 200 000               | 64 × 50000       |

---

## Conclusion

**Pour cet exercice :**
1. Commence avec des valeurs approximatives (essai-erreur)
2. Teste sur le hardware
3. Ajuste si nécessaire
4. L'important est que ça clignote environ 1 fois par seconde !

**Pour être précis :**
- Utilise des TIMERS hardware (exercices suivants)
- Analyse le code assembleur
- Mesure avec un oscilloscope

Le délai logiciel est **imprécis par nature**, c'est pourquoi on l'utilise seulement pour l'apprentissage. En production, on utilise toujours les timers hardware ! ⏱️