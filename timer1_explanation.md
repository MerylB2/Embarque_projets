# Exercice 01 - Timer1 LED Blink (1 Hz)

## 📌 Objectif
Faire clignoter la LED D2 (PB1) à une fréquence de **1 Hz** (500ms allumée, 500ms éteinte) en utilisant le **Timer1** de l'ATmega328P, sans toucher aux registres PORTX et avec une boucle `while(1)` vide.

---

## 🔧 Configuration Matérielle
- **Microcontrôleur** : ATmega328P
- **Fréquence CPU** : 16 MHz
- **LED** : D2 connectée sur **PB1** (broche 15 du chip, aussi appelée OC1A)
- **Timer utilisé** : Timer1 (16 bits)

---

## 🧮 Calculs Théoriques

### 1. Choix du Prescaler (Predivisor)
Le Timer1 peut compter jusqu'à 65535 (2^16 - 1). Pour obtenir une période de 0.5s avec une fréquence CPU de 16 MHz :

**Sans prescaler :**
- Comptage max : 65535 ticks
- Temps max : 65535 / 16 000 000 = **4.1 ms** ❌ (trop court pour 500ms)

**Avec prescaler 256 :**
- Fréquence timer = 16 MHz / 256 = **62 500 Hz**
- Temps pour un tick = 1 / 62 500 = **16 µs**
- Comptage max possible : 65535 × 16 µs = **1.048 s** ✅ (suffisant pour 500ms)

### 2. Calcul de OCR1A
Pour obtenir un toggle toutes les **0.5 secondes** :

```
Nombre de ticks = Temps désiré × Fréquence timer
                = 0.5 s × 62 500 Hz
                = 31 250 ticks
```

Comme le compteur commence à **0**, on doit soustraire 1 :
```
OCR1A = 31 250 - 1 = 31 249
```

### 3. Fréquence résultante
- **Toggle** : tous les 0.5s (exactement)
- **Période complète** : 1 seconde (ON + OFF)
- **Fréquence LED** : **1 Hz** ✅

---

## 📝 Explication du Code

### Configuration de la broche PB1 en sortie
```c
DDRB |= (1 << DDB1);
```
**Explication :**
- `DDRB` : Data Direction Register pour le port B
- `DDB1` : bit 1 du registre (correspond à PB1)
- `(1 << DDB1)` : crée un masque avec le bit 1 à 1 (0b00000010)
- `|=` : OR logique pour mettre le bit à 1 sans modifier les autres
- **Résultat** : PB1 est configurée en **sortie**

---

### Configuration du Timer1 - Registre TCCR1A
```c
TCCR1A = (1 << COM1A0);
```

**TCCR1A** (Timer/Counter1 Control Register A) :
| Bit    | 7      | 6      | 5      | 4      | 3 | 2 | 1     | 0     |
|-----   |---     |---     |---     |---     |---|---|---    |---    |
| Nom    | COM1A1 | COM1A0 | COM1B1 | COM1B0 | - | - | WGM11 | WGM10 |
| Valeur | 0      | **1**  | 0      | 0      | 0 | 0 | 0     | 0     |

**Bits importants :**
- **COM1A1:0 = 01** : Mode "Toggle OC1A on Compare Match"
  - Quand le timer atteint OCR1A, le hardware **bascule automatiquement** la broche PB1
  - 0→1 ou 1→0 sans intervention du CPU
- **WGM11:10 = 00** : Partie basse du mode de fonctionnement (complété par TCCR1B)

**Pourquoi ce mode ?**
- Pas besoin de toucher à `PORTB` manuellement
- Pas besoin d'interruption
- Le hardware fait tout automatiquement ✅

---

### Configuration du Timer1 - Registre TCCR1B
```c
TCCR1B = (1 << WGM12) | (1 << CS12);
```

**TCCR1B** (Timer/Counter1 Control Register B) :
| Bit     | 7     | 6     | 5 | 4     | 3     | 2     | 1    | 0    |
|-----    |---    |---    |---|---    |---    |---    |---   |---   |
| Nom     | ICNC1 | ICES1 | - | WGM13 | WGM12 | CS12  | CS11 | CS10 |
| Valeur  | 0     | 0     | 0 | 0     | **1** | **1** | 0    | 0    |  

**Bits importants :**
- **WGM13:12 = 01** : Combiné avec WGM11:10 = 00 → Mode **CTC (Clear Timer on Compare Match)**
  - Le compteur va de 0 à OCR1A puis repart à 0
  - TOP = OCR1A
- **CS12:10 = 100** : Prescaler (predivisor) = **256**
  - Timer clock = 16 MHz / 256 = 62 500 Hz

**Tableau des prescalers Timer1 (rappel doc) :**
| CS12  | CS11  | CS10  | Prescaler |
|------ |------ |------ |-----------|
| 0     | 0     | 0     | STOP      |
| 0     | 0     | 1     | /1        |
| 0     | 1     | 0     | /8        |
| 0     | 1     | 1     | /64       |
| **1** | **0** | **0** | **/256** ✅ |
| 1     | 0     | 1     | /1024     |

---

### Configuration de OCR1A
```c
OCR1A = 31249;
```

**OCR1A** (Output Compare Register 1 A) :
- Registre 16 bits qui définit la valeur de comparaison
- Quand `TCNT1` (compteur du Timer1) atteint cette valeur :
  - Le timer repart à 0 (mode CTC)
  - La broche OC1A (PB1) est **togglée** automatiquement (mode COM1A0)

**Calcul détaillé :**
```
OCR1A = (F_CPU / Prescaler / (2 × Fréquence_LED)) - 1
      = (16 000 000 / 256 / 2) - 1
      = (62 500 / 2) - 1
      = 31 250 - 1
      = 31 249
```

Le facteur `2` vient du fait qu'on veut **toggler** 2 fois par période complète :
- Toggle 1 : LED s'allume (0 → 1)
- Toggle 2 : LED s'éteint (1 → 0)

---

## 🔄 Chronologie du Fonctionnement

```
Temps (ms)    0      500     1000    1500    2000
              |       |       |       |       |
TCNT1:        0 → 31249 → 0 → 31249 → 0 → 31249
              ↓       ↓       ↓       ↓       ↓
PB1:          0   →   1   →   0   →   1   →   0
LED:          OFF     ON      OFF     ON      OFF
```

**Détails :**
1. Timer1 démarre à 0
2. Compte jusqu'à 31249 (500ms)
3. Compare Match : PB1 toggle (0→1), timer reset à 0
4. Compte à nouveau jusqu'à 31249 (500ms)
5. Compare Match : PB1 toggle (1→0), timer reset à 0
6. Répète indéfiniment...

---

## 🎯 Pourquoi la Boucle while(1) est Vide ?

```c
while (1)
{
    /* Rien à faire ici */
}
```

Le Timer1 en mode **hardware toggle** fonctionne de manière **autonome** :
1. **Pas besoin d'interruption** : Le registre TIMSK1 n'est pas utilisé
2. **Pas besoin de lire PORTB** : Le hardware bascule directement OC1A
3. **Pas besoin de vérifier des flags** : Tout est automatique

Le CPU peut rester **idle** ou faire d'autres tâches pendant que le Timer1 gère la LED.

---

## ⚙️ Registres Utilisés - Résumé

| Registre    | Valeur        | Rôle                              |
|----------   |--------       |------                             |
| **DDRB**    | `0b00000010`  | Configure PB1 en sortie           |
| **TCCR1A**  | `0b01000000`  | Mode Toggle OC1A on Compare Match |
| **TCCR1B**  | `0b00001100`  | Mode CTC + Prescaler 256          |
| **OCR1A**   | `31249`       | Valeur de comparaison pour 0.5s   |
| **TCNT1**   | Auto (0→31249)| Compteur du Timer1                |

---

## 📊 Modes de Fonctionnement du Timer1

### Mode CTC (Clear Timer on Compare Match)
- **WGM13:10 = 0100** (4 en décimal)
- Le compteur va de **0 à OCR1A** puis repart à 0
- Idéal pour générer des **fréquences précises**

### Mode COM (Compare Output Mode)
- **COM1A1:0 = 01** : Toggle OC1A on Compare Match
- **COM1A1:0 = 10** : Clear OC1A on Compare Match (0)
- **COM1A1:0 = 11** : Set OC1A on Compare Match (1)

Dans notre cas, **Toggle** est parfait car on veut alterner entre 0 et 1.

---

## 🚫 Ce qui est INTERDIT dans l'exercice

❌ **Utiliser PORTB dans le code :**
```c
// ❌ INTERDIT
PORTB ^= (1 << PB1);  // Toggle manuel
```
✅ **Solution :** Utiliser le mode hardware toggle (`COM1A0`)

❌ **Avoir du code dans while(1) :**
```c
// ❌ INTERDIT
while(1) {
    if (TIFR1 & (1 << OCF1A)) {
        // ...
    }
}
```
✅ **Solution :** Le hardware fait tout automatiquement

❌ **Utiliser les interruptions :**
```c
// ❌ INTERDIT (mais techniquement possible)
TIMSK1 |= (1 << OCIE1A);
ISR(TIMER1_COMPA_vect) { /* ... */ }
```
✅ **Solution :** Mode hardware toggle sans interruption

---

## 🔬 Vérification des Contraintes

| Contrainte | ✅ Respectée | Comment ? |
|------------|-------------|-----------|
| LED D2 (PB1) à 1 Hz | ✅ | OCR1A = 31249, prescaler 256 |
| Utiliser Timer1 | ✅ | TCCR1A, TCCR1B, OCR1A configurés |
| Boucle while(1) vide | ✅ | Hardware toggle autonome |
| Ne pas utiliser PORTX | ✅ | Mode COM1A0 gère PB1 automatiquement |
| Seul avr/io.h autorisé | ✅ | Pas d'autre bibliothèque |

---

## 📚 Pour Aller Plus Loin

### Autres Prescalers Possibles
- **Prescaler 64** : OCR1A = 124999 (dépassement ! 16 bits max = 65535) ❌
- **Prescaler 1024** : OCR1A = 7812 ✅ (possible mais moins précis)
- **Prescaler 256** : OCR1A = 31249 ✅ **(choix optimal)**

### Autres Fréquences
Pour obtenir d'autres fréquences avec prescaler 256 :
- **2 Hz** (0.25s) : OCR1A = 15624
- **0.5 Hz** (1s) : OCR1A = 62499 (dépassement !) → utiliser prescaler 1024
- **10 Hz** (0.05s) : OCR1A = 3124

---

## 🎓 Conclusion

Cette solution utilise les capacités **hardware** du Timer1 pour obtenir un clignotement précis sans aucune intervention du CPU. C'est la méthode la plus **efficace** et la plus **élégante** pour ce type d'application.

**Points clés :**
- ✅ Mode CTC pour un comptage précis
- ✅ Hardware toggle pour éviter d'utiliser PORTB
- ✅ Calcul exact de OCR1A pour 1 Hz
- ✅ Aucune interruption nécessaire
- ✅ Boucle while(1) complètement vide