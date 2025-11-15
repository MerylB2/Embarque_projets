# Exercice 02 - Stockage Clé/Valeur Persistant en EEPROM

## 📋 Description Générale

Ce programme implémente un système de stockage **clé/valeur persistant** dans l'EEPROM de l'ATmega328P. Les données survivent aux redémarrages du microcontrôleur grâce à la mémoire EEPROM non-volatile.

### Caractéristiques
- **Interface** : Ligne de commande via UART (115200 bauds)
- **Stockage** : EEPROM de 1024 octets
- **Taille max** : 32 caractères ASCII standard par clé/valeur
- **Format** : Magic byte + longueurs + données
- **Commandes** : READ, WRITE, FORGET, PRINT, CLEAR

---

## 🗂️ Structure de Données en EEPROM

### Format de Stockage

Chaque paire clé/valeur est stockée séquentiellement :

```
[MAGIC_BYTE][key_len][key...][val_len][value...]
```

**Exemple concret** : `WRITE "lol" "je ne sais pas"`
```
Adresse   Données              Signification
0x00      7F                   Magic byte (0x7F = paire valide)
0x01      03                   Longueur de la clé (3 octets)
0x02-04   6C 6F 6C            "lol" en ASCII
0x05      0E                   Longueur de la valeur (14 octets)
0x06-13   6A 65 20 6E 65...   "je ne sais pas" en ASCII
0x14      FF                   Début de l'espace libre
```

### Magic Byte

- **Valeur `0x7F`** : Paire valide (non-ASCII standard pour éviter confusion)
- **Valeur `0x00`** : Paire supprimée (marquée pour réutilisation future)
- **Valeur `0xFF`** : Espace vide (état par défaut de l'EEPROM)

---

## 🔧 Fonctions Utilitaires

### `ft_strlen(const char *str)`
**Rôle** : Calcule la longueur d'une chaîne de caractères.

**Pourquoi** : Évite d'inclure `<string.h>` et garde le code léger.

```c
uint8_t ft_strlen(const char *str) {
    uint8_t len = 0;
    while (str[len])
        len++;
    return len;
}
```

### `ft_strcmp(const char *s1, const char *s2)`
**Rôle** : Compare deux chaînes (retourne 0 si égales).

**Pourquoi** : Alternative custom à `strcmp()` de la libc.

```c
int8_t ft_strcmp(const char *s1, const char *s2) {
    uint8_t i = 0;
    while (s1[i] && s2[i] && s1[i] == s2[i])
        i++;
    return s1[i] - s2[i];
}
```

---

## 🔍 Fonctions de Recherche

### `find_key(const char *key, uint16_t *data_addr)`
**Rôle** : Recherche une clé dans l'EEPROM.

**Retourne** :
- Adresse du magic byte si trouvée
- `0xFFFF` si non trouvée
- `data_addr` contient l'adresse de la valeur

**Algorithme** :
1. Parcourt l'EEPROM séquentiellement
2. Pour chaque entrée :
   - Lit le magic byte
   - Si `0xFF` → fin des données
   - Si `0x00` → paire supprimée, sauter
   - Si `0x7F` → vérifier si la clé correspond
3. Compare les clés octet par octet
4. Retourne l'adresse si match

**Points clés** :
- Gère les paires supprimées sans les réutiliser (juste saut)
- Arrêt dès qu'on trouve `0xFF` (optimisation)
- Comparaison complète : longueur + contenu

### `find_free_space(void)`
**Rôle** : Trouve la première adresse libre dans l'EEPROM.

**Retourne** :
- Adresse du premier `0xFF`
- `0xFFFF` si EEPROM pleine

**Algorithme** :
1. Parcourt l'EEPROM
2. Pour chaque entrée, calcule sa taille et saute
3. Retourne la première adresse contenant `0xFF`

---

## 📝 Commandes Principales

### `cmd_read(const char *key)`
**Syntaxe** : `READ clé` ou `READ "clé"`

**Fonctionnement** :
1. Appelle `find_key()` pour localiser la clé
2. Si non trouvée → affiche `empty`
3. Si trouvée :
   - Lit `val_len` à `data_addr - 1`
   - Lit et affiche la valeur entre guillemets
   - Format : `"valeur"`

**Exemple** :
```
> READ "lol"
"je ne sais pas"
```

### `cmd_write(const char *key, const char *value)`
**Syntaxe** : `WRITE "clé" "valeur"`

**Fonctionnement** :
1. Vérifie les longueurs (0 < len ≤ 32)
2. Vérifie si la clé existe déjà → `already exists`
3. Trouve un espace libre avec `find_free_space()`
4. Vérifie l'espace disponible
5. Écrit séquentiellement :
   - Magic byte `0x7F`
   - Longueur de la clé
   - Clé
   - Longueur de la valeur
   - Valeur
6. Affiche `done`

**Messages d'erreur** :
- `invalid key` : clé vide
- `invalid value` : valeur vide
- `too long` : > 32 caractères
- `already exists` : clé déjà présente
- `no space left` : EEPROM pleine

**Exemple** :
```
> WRITE "test" "abc"
done
```

### `cmd_forget(const char *key)`
**Syntaxe** : `FORGET clé` ou `FORGET "clé"`

**Fonctionnement** :
1. Recherche la clé avec `find_key()`
2. Si non trouvée → `not found`
3. Si trouvée :
   - Écrit `0x00` à la place du magic byte `0x7F`
   - Marque la paire comme supprimée
   - Affiche `done`

**Note** : Les données restent en mémoire mais sont ignorées. L'espace n'est pas réutilisé automatiquement.

**Exemple** :
```
> FORGET "lol"
done
```

### `cmd_print(void)`
**Syntaxe** : `PRINT`

**Fonctionnement** :
1. Parcourt l'EEPROM par lignes de 16 octets
2. Pour chaque ligne :
   - Vérifie si elle contient des données (≠ `0xFF`)
   - Si ligne vide → arrêt de l'affichage
   - Sinon, affiche :
     - Adresse sur 8 chiffres hexa (minuscule)
     - 16 octets par groupes de 2 (format word)
     - Partie ASCII entre `|...|`

**Format de sortie** :
```
00000000  7f03 6c6f 6c0e 6a65 206e 6520 7361 6973  |..lol.je ne sais|
00000010  2070 6173 ffff ffff ffff ffff ffff ffff  | pas............|
...
```

**Caractères ASCII** :
- Imprimables (`0x20`-`0x7E`) : affichés tels quels
- Non-imprimables : affichés comme `.`

### Commande Bonus : `CLEAR`
**Syntaxe** : `CLEAR`

**Fonctionnement** :
1. Écrit `0xFF` dans toute l'EEPROM (1024 octets)
2. Remet l'EEPROM à l'état vierge
3. Affiche `done`

**Attention** : Opération lente (~100ms) et consomme des cycles d'écriture.

---

## 🎯 Fonctions de Parsing

### `read_line(char *buffer, uint8_t max_len)`
**Rôle** : Lit une ligne complète depuis l'UART avec support du backspace.

**Fonctionnalités** :
- Echo des caractères tapés
- Support Backspace (`0x7F` ou `0x08`) avec effacement visuel
- Détection Enter (`\r` ou `\n`)
- Limite de taille pour éviter overflow

**Séquence Backspace** :
```c
uart_tx('\b');  // Recule le curseur
uart_tx(' ');   // Efface le caractère
uart_tx('\b');  // Repositionne le curseur
```

### `parse_command(const char *buffer, char *cmd)`
**Rôle** : Extrait le nom de la commande (premier mot).

**Exemple** : `"WRITE test abc"` → `cmd = "WRITE"`

### `parse_read_command(const char *buffer, char *key)`
**Rôle** : Extrait la clé pour READ/FORGET.

**Support** :
- Avec guillemets : `READ "ma clé"`
- Sans guillemets : `READ test`

**Algorithme** :
1. Saute le nom de commande
2. Détecte la présence de `"`
3. Extrait jusqu'au guillemet fermant ou espace

### `parse_write_command(const char *buffer, char *key, char *value)`
**Rôle** : Extrait clé ET valeur pour WRITE.

**Format attendu** : `WRITE "clé" "valeur"`

**Algorithme** :
1. Initialise `key` et `value` à vide
2. Saute "WRITE"
3. Extrait la clé entre guillemets
4. Extrait la valeur entre guillemets

**Important** : Les guillemets sont **obligatoires** pour WRITE (permet les espaces).

### `execute_command(const char *buffer)`
**Rôle** : Dispatcher qui appelle la bonne fonction selon la commande.

**Algorithme** :
1. Parse le nom de la commande
2. Compare avec `ft_strcmp()` :
   - `"PRINT"` → `cmd_print()`
   - `"CLEAR"` → effacement EEPROM
   - `"READ"` → parse puis `cmd_read()`
   - `"WRITE"` → parse puis `cmd_write()`
   - `"FORGET"` → parse puis `cmd_forget()`

---

## 🖥️ Fonction Principale

### `main(void)`
**Rôle** : Boucle principale du programme.

**Séquence** :
1. Initialise l'UART (115200 bauds, mode U2X)
2. Délai de stabilisation (100ms)
3. Boucle infinie :
   - Affiche le prompt `"> "`
   - Lit une ligne de commande
   - Exécute la commande
   - Recommence

---

## 🔐 Limitations de l'EEPROM

### Cycles d'Écriture
L'EEPROM de l'ATmega328P a une **endurance limitée** :
- **~100,000 cycles** d'écriture par cellule
- Les lectures sont illimitées

**Optimisations dans le code** :
- WRITE vérifie si la clé existe déjà (évite écritures inutiles)
- FORGET ne réécrit pas les données, juste le magic byte
- CLEAR à utiliser avec parcimonie

### Gestion de l'Espace
**Problème** : L'espace des paires supprimées n'est **pas réutilisé**.

**Exemple** :
```
WRITE "a" "test"     → écrit à 0x00
FORGET "a"           → marque 0x00 = supprimé
WRITE "b" "test"     → écrit APRÈS, pas à 0x00
```

**Solution future** : Implémenter un garbage collector qui compacte l'EEPROM.

### Fragmentation
Sans compactage, l'EEPROM peut se fragmenter rapidement avec des WRITE/FORGET successifs.

---

## 📊 Exemple d'Utilisation Complète

```
> CLEAR
done
> WRITE "user" "alice"
done
> WRITE "pass" "secret123"
done
> READ "user"
"alice"
> PRINT
00000000  7f04 7573 6572 0561 6c69 6365 7f04 7061  |..user.alice..pa|
00000010  7373 0973 6563 7265 7431 3233 ffff ffff  |ss.secret123....|
...
> FORGET "pass"
done
> READ "pass"
empty
> PRINT
00000000  7f04 7573 6572 0561 6c69 6365 0004 7061  |..user.alice..pa|
00000010  7373 0973 6563 7265 7431 3233 ffff ffff  |ss.secret123....|
...
```

**Note** : Après FORGET, le magic byte passe de `7f` à `00`.

---

## 🎓 Points d'Apprentissage

### 1. Gestion de Mémoire Non-Volatile
- Lecture/écriture EEPROM avec `avr/eeprom.h`
- Format de données personnalisé
- Gestion de l'espace et fragmentation

### 2. Parsing de Commandes
- Parsing manuel sans bibliothèques complexes
- Support de guillemets pour espaces
- Robustesse face aux entrées malformées

### 3. Interface Utilisateur
- Prompt et echo pour meilleure UX
- Messages d'erreur clairs
- Affichage hexdump lisible

### 4. Optimisation Embarquée
- Code compact (fonctions < 25 lignes)
- Pas de malloc/free (mémoire statique)
- Fonctions custom au lieu de libc

### 5. Débogage
- PRINT permet de voir l'état interne
- Format hexdump révèle la structure
- Points `.` montrent les données binaires

---

## 🚀 Améliorations Possibles

1. **Garbage Collection** : Compactage automatique des paires supprimées
2. **Checksums** : Validation d'intégrité des données
3. **Compression** : Optimisation de l'espace (RLE, Huffman)
4. **Index** : Table d'index en RAM pour recherche rapide
5. **Versioning** : Gestion de versions des paires
6. **Encryption** : Chiffrement des valeurs sensibles
7. **Auto-save** : Backup automatique avant écriture
8. **Stats** : Commande pour afficher utilisation mémoire

---

## 📚 Références

- **ATmega328P Datasheet** : Section 7.4 (EEPROM Data Memory)
- **avr/eeprom.h** : Documentation des fonctions autorisées
- **Format hexdump** : Standard Unix `hexdump -C`
- **ASCII Table** : Caractères imprimables 0x20-0x7E