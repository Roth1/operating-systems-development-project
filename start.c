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
    printf("\n1Long-term gf just broke up with me");
    printf("\n2At coffee shop trying to think of how to get my shit together");
    printf("\n3notice cute girl is has been looking at me for some time");
    printf("\n4smile at her and wave a little");
    printf("\n5go back to staring at my coffee");
    printf("\n6lock up to see her sitting next to me");
    printf("\n7asks me what’s wrong");
    printf("\n8little reluctant, think it’s a trick");
    printf("\n9It isn’t");
    printf("\n10feelsgoodman.jpg");
    printf("\n11she was dumped recently too");
    printf("\n12accidentally stare at her cleavage as she’s talking");
    printf("\n13start to get hard");
    printf("\n14feel a hand run up my leg");
    printf("\n15see her breathing a bit hard");
    printf("\n16asks if I like what I see");
    printf("\n17guide her hand to my zipper");
    printf("\n18she unzips my fanny pack by mistake");
    printf("\n19spaghetti falls out");
    printf("\n20try to clean it up with my sock");
    printf("\n21start to cry");
    printf("\n22Long-term gf just broke up with me");
    printf("\n23At coffee shop trying to think of how to get my shit together");
    printf("\n24notice cute girl is has been looking at me for some time");
    printf("\n25smile at her and wave a little");
    printf("\n26go back to staring at my coffee");
    printf("\n27lock up to see her sitting next to me");
    printf("\n28asks me what’s wrong");
    printf("\n29little reluctant, think it’s a trick");
    printf("\n30It isn’t");
    printf("\n31feelsgoodman.jpg");
    printf("\n32she was dumped recently too");
    printf("\n33accidentally stare at her cleavage as she’s talking");
    printf("\n34start to get hard");
    printf("\n35feel a hand run up my leg");
    printf("\n36see her breathing a bit hard");
    printf("\n37asks if I like what I see");
    printf("\n38guide her hand to my zipper");
    printf("\n39she unzips my fanny pack by mistake");
    printf("\n40spaghetti falls out");
    printf("\n41try to clean it up with my sock");
    printf("\n42start to cry");
    
    // on ne doit jamais sortir de kernel_start
    while (1) {
        // cette fonction arrete le processeur
        hlt();
    }
}

