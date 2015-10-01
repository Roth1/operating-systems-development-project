#include <cpu.h>
#include "stdio.h"
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
    //place_curseur(0, 0);

    //console_putbytes
    printf("1Long-term gf just broke up with me");
    printf("\n2At coffee shop trying to think of how to get my shit together");
    printf("\n3notice cute girl is has been looking at me for some time");
    printf("\n4smile at her and wave a little");
    printf("\5ngo back to staring at my coffee");
    printf("\6nlock up to see her sitting next to me");
    printf("\nasks me what’s wrong");
    printf("\nlittle reluctant, think it’s a trick");
    printf("\nIt isn’t");
    printf("\nfeelsgoodman.jpg");
    printf("\nshe was dumped recently too");
    printf("\naccidentally stare at her cleavage as she’s talking");
    printf("\nstart to get hard");
    printf("\nfeel a hand run up my leg");
    printf("\nsee her breathing a bit hard");
    printf("\nasks if I like what I see");
    printf("\nguide her hand to my zipper");
    printf("\n18she unzips my fanny pack by mistake");
    printf("\nspaghetti falls out");
    printf("\ntry to clean it up with my sock");
    printf("\nstart to cry");
    printf("\nLong-term gf just broke up with me");
    printf("\nAt coffee shop trying to think of how to get my shit together");
    printf("\n24notice cute girl is has been looking at me for some time");
    printf("\n25smile at her and wave a little");
    printf("\n26go back to staring at my coffee");
    printf("\nlock up to see her sitting next to me");
    printf("\nasks me what’s wrong");
    printf("\nlittle reluctant, think it’s a trick");
    printf("\nIt isn’t");
    printf("\nfeelsgoodman.jpg");
    printf("\nshe was dumped recently too");
    printf("\naccidentally stare at her cleavage as she’s talking");
    printf("\nstart to get hard");
    printf("\nfeel a hand run up my leg");
    printf("\nsee her breathing a bit hard");
    printf("\nasks if I like what I see");
    printf("\nguide her hand to my zipper");
    printf("\nshe unzips my fanny pack by mistake");
    printf("\n40spaghetti falls out");
    printf("\ntry to clean it up with my sock");
    printf("\n42start to cry");

    // on ne doit jamais sortir de kernel_start
    while (1) {
        // cette fonction arrete le processeur
        hlt();
    }
}

