/*
 * Exercice 00 : Clignotant (Blink)
 * Chapitre III : Tic & tac
 * 
 * Description :
 * LED D2 (PB1) clignote à 1 Hz (0.5s ON, 0.5s OFF)
 * 
 * Hardware :
 * - LED D2 sur PB1
 * 
 * Contraintes :
 * - Pas de TIMER hardware
 * - Pas de _delay_ms (créer notre propre fonction de délai)
 * - Toggle avec UNE SEULE opération bitwise (XOR)
 * - Pas de if/else pour le toggle
 * - Commentaires obligatoires pour chaque registre
 */

#include <avr/io.h>

/*
 * === FONCTION DE DÉLAI MANUELLE ===
 * 
 * Cette fonction crée un délai en exécutant des boucles vides.
 * 
 * Principe :
 * - Le CPU exécute des cycles vides (boucles sans effet)
 * - Chaque itération prend un certain nombre de cycles CPU
 * - En comptant les itérations, on crée un délai temporel
 * 
 * Paramètre : centaines_ms
 * - Nombre de centaines de millisecondes à attendre
 * - Exemple : centaines_ms = 5 → 500ms de délai
 * 
 * Calcul du nombre d'itérations :
 * - F_CPU = 16 000 000 Hz (16 MHz)
 * - 1 milliseconde = 16 000 cycles
 * - 100 millisecondes = 1 600 000 cycles
 * 
 * Valeur 2000 choisie empiriquement pour obtenir ~100ms
 * (dépend de l'optimisation du compilateur)
 * 
 * IMPORTANT : Ce délai n'est PAS précis !
 * Pour un délai précis, il faudrait utiliser un TIMER hardware.
 */
void wait_centaines_ms(int centaines_ms)
{
    /*
     * Boucle externe : nombre de centaines de ms
     * 
     * Variable : i
     * Valeur : 0 à centaines_ms
     * 
     * Exemple : centaines_ms = 5 → boucle 5 fois
     * Chaque itération = environ 100ms
     * Total = 5 × 100ms = 500ms
     */
    for (int i = 0; i < centaines_ms; i++)
    {
        /*
         * Boucle interne : crée le délai de ~100ms
         * 
         * Variable : j
         * Valeur : 0 à 2000 (valeur empirique)
         * 
         * Cette boucle exécute 2000 itérations vides.
         * Chaque itération prend plusieurs cycles CPU :
         * - Comparaison (j < 2000)
         * - Incrémentation (j++)
         * - Saut conditionnel (retour au début)
         * 
         * ASTUCE : On peut ajuster la valeur 2000 pour
         * calibrer le délai si nécessaire.
         * 
         * Valeur plus grande → délai plus long
         * Valeur plus petite → délai plus court
         */
        for (int j = 0; j < 2000; j++)
        {
            /*
             * Boucle la plus interne : ralentit encore plus
             * 
             * Variable : k
             * Valeur : 0 à 200
             * 
             * Cette boucle supplémentaire multiplie le délai.
             * Sans elle, le délai serait trop court.
             * 
             * Au total : 2000 × 200 = 400 000 itérations
             * par centaine de ms.
             * 
             * Ces boucles sont VIDES : elles ne font rien
             * d'autre que consommer des cycles CPU.
             */
            for (int k = 0; k < 200; k++)
            {
                // Boucle vide (ne fait rien)
                // Le compilateur pourrait optimiser et supprimer
                // cette boucle. Pour éviter ça, on peut utiliser
                // une instruction "asm volatile" (voir note ci-dessous)
            }
        }
    }
}

int main(void)
{
    /*
     * === CONFIGURATION DE LA PIN ===
     * 
     * DDRB = Data Direction Register du Port B
     * 
     * Ce registre contrôle la direction (entrée/sortie) 
     * de chaque pin du Port B.
     * 
     * Valeur assignée : (1 << PB1)
     * - PB1 = 1 (constante définie dans avr/io.h)
     * - (1 << PB1) = 0b00000010 (bit 1 à 1)
     * - |= met le bit 1 à 1 sans toucher aux autres
     * 
     * Résultat : PB1 configuré en SORTIE (output)
     * → Permet de contrôler la LED D2
     */
    DDRB |= (1 << PB1);
    
    
    /*
     * === ÉTAT INITIAL DE LA LED ===
     * 
     * PORTB = Port B Data Register
     * 
     * Ce registre contrôle l'état électrique (HIGH/LOW)
     * de chaque pin configurée en sortie.
     * 
     * Valeur assignée : ~(1 << PB1)
     * - (1 << PB1) = 0b00000010 (bit 1 à 1)
     * - ~ inverse : 0b11111101 (bit 1 à 0, autres à 1)
     * - &= force le bit 1 à 0 sans toucher aux autres
     * 
     * Résultat : PB1 mis à LOW (0V)
     * → LED D2 éteinte au démarrage
     */
    PORTB &= ~(1 << PB1);
    
    
    /*
     * === BOUCLE INFINIE ===
     * 
     * Cette boucle s'exécute indéfiniment.
     * À chaque itération :
     * 1. On inverse l'état de la LED (toggle)
     * 2. On attend environ 500ms avec notre fonction
     * 
     * Résultat : LED clignote à environ 1 Hz
     * (1 Hz = 1 cycle par seconde = 0.5s ON + 0.5s OFF)
     */
    while (1)
    {
        /*
         * === TOGGLE DE LA LED (XOR) ===
         * 
         * PORTB ^= (1 << PB1)
         * 
         * Opération XOR (eXclusive OR) :
         * - Si le bit était 0 → devient 1 (LED s'allume)
         * - Si le bit était 1 → devient 0 (LED s'éteint)
         * 
         * Valeur assignée : (1 << PB1)
         * - (1 << PB1) = 0b00000010 (masque avec bit 1 à 1)
         * - ^= applique XOR puis assigne
         * 
         * Table de vérité XOR :
         * A | B | A XOR B
         * ──┼───┼────────
         * 0 | 1 |   1     (0 devient 1)
         * 1 | 1 |   0     (1 devient 0)
         * 
         * Résultat : État de PB1 inversé à chaque appel
         * → LED D2 change d'état (ON↔OFF)
         * 
         * IMPORTANT : Une seule opération bitwise, pas de if/else !
         */
        PORTB ^= (1 << PB1);
        
        
        /*
         * === DÉLAI DE ~500 MILLISECONDES ===
         * 
         * wait_centaines_ms(5)
         * 
         * Notre fonction de délai manuelle.
         * 
         * Paramètre : 5
         * - 5 centaines de ms = 5 × 100ms = 500ms
         * 
         * Mécanisme :
         * - Exécute des boucles vides imbriquées
         * - Consomme des cycles CPU sans rien faire
         * - Crée un délai temporel approximatif
         * 
         * Valeur : 5 (centaines de ms)
         * 
         * Résultat : Programme "dort" pendant ~0.5s
         * → LED reste dans son état actuel pendant ce temps
         * 
         * Timing total du cycle :
         * - Toggle (instant)
         * - Attente ~500ms
         * - Retour au toggle
         * = ~500ms par état → ~1 Hz
         * 
         * NOTE : Le délai n'est pas précis !
         * La valeur exacte dépend :
         * - De la fréquence du CPU (F_CPU)
         * - De l'optimisation du compilateur (-O0, -O1, -O2, -Os)
         * - Des instructions assembleur générées
         * 
         * Pour ajuster le délai :
         * - Augmenter 5 → délai plus long → fréquence plus basse
         * - Diminuer 5 → délai plus court → fréquence plus haute
         * - Modifier les constantes dans wait_centaines_ms()
         */
        wait_centaines_ms(5);
    }
    
    
    /*
     * === RETURN ===
     * 
     * Ce return ne sera JAMAIS atteint car la boucle while(1)
     * est infinie.
     * 
     * ATTENTION : L'exercice sera considéré invalide si le main
     * retourne ! Ce return est ici uniquement pour respecter
     * la signature de la fonction main().
     * 
     * En pratique, le microcontrôleur restera dans la boucle
     * infinie jusqu'à ce qu'il soit éteint ou reset.
     */
    return 0;
}


/*
 * === NOTE AVANCÉE : Empêcher l'optimisation ===
 * 
 * Le compilateur avec optimisation (-O2, -Os) pourrait supprimer
 * les boucles vides car elles ne font "rien".
 * 
 * Pour empêcher cela, on peut ajouter une instruction assembleur
 * dans la boucle la plus interne :
 * 
 * for (int k = 0; k < 200; k++)
 * {
 *     asm volatile ("nop");  // No Operation
 * }
 * 
 * "nop" est une instruction qui ne fait rien mais prend 1 cycle CPU.
 * "volatile" dit au compilateur de ne PAS l'optimiser.
 * 
 * Cependant, pour cet exercice, les boucles simples suffisent.
 */
