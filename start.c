#include <cpu.h>
#include "stdio.h"
#include <inttypes.h>
#include "affichage.h"
#include "temps.h"
#include "processus.h"

// on peut s'entrainer a utiliser GDB avec ce code de base
// par exemple afficher les valeurs de x, n et res avec la commande display

void kernel_start(void)
{
    // efface l'ecran    
    efface_ecran();
    // init l'heure
    regle_PIT();
    // démasquage des interruptions externes
    sti();
    // initialise les processus a utiliser
    init_procs();
    // defaut processus
    idle();
    // on ne doit jamais sortir de kernel_start
    while (1) {
        // cette fonction arrete le processeur
        hlt();
    }
}

