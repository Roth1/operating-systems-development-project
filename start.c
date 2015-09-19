#include <cpu.h>
#include <inttypes.h>
#include "affichage.h"

// on peut s'entrainer a utiliser GDB avec ce code de base
// par exemple afficher les valeurs de x, n et res avec la commande display

// une fonction bien connue
uint32_t fact(uint32_t n)
{
    uint32_t res;
    if (n <= 1) {
        res = 1;
    } else {
        res = fact(n - 1) * n;
    }
    return res;
}

void kernel_start(void)
{
    uint32_t x = fact(5);
    // quand on saura gerer l'ecran, on pourra afficher x
    (void)x;
    
    efface_ecran();

    ecrit_car(10, 39, 'X');
    ecrit_car(10, 40, 'Y');
    ecrit_car(10, 41, 'B');
    
    place_curseur(10, 42);

    // on ne doit jamais sortir de kernel_start
    while (1) {
        // cette fonction arrete le processeur
        hlt();
    }
}

