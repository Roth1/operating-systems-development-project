#include "temps.h"

// variables globales
uint32_t compteur = 0;
//int32_t heure = 0;
int32_t minutes = 0;
int32_t secondes = 0;

// pour l'ordonnonceur
extern uint32_t temps_up;

void tic_PIT(void) {
    outb(0x20, 0x20);  
    char buffer[12];
    int32_t taille = 10;
    compteur++;
    if(compteur == 50) {
	temps_up++;
        secondes++;
	if(secondes == 60) {
	    minutes++;
	    secondes = 0;
	}
	compteur = 0;
    }
    
    taille = sprintf(buffer, "%02d:%02d", minutes, secondes);
    
    affiche_heure(buffer, taille);
    // ordonnancement des processus gere par l'interruption
    ordonnance();
}

void init_traitant_IT(int32_t numIT, void (*traitant)(void)) {
    uint32_t adP = (int32_t)traitant;
    uint32_t *p;
    p = (uint32_t *)(0x1000 + 8 * numIT);
    *p = (KERNEL_CS << 16) | (adP & 0xFFFF);
    *(p + 1) = (adP & 0xFFFF0000) | 0x8E00;
}

void masque_IRQ(uint32_t numIRQ, bool masque) {
    uint8_t m = 1 << numIRQ;
    uint8_t v = inb(0x21);
    if(masque) {
        outb((m | v), 0x21);
    } else {
      outb((~m & v), 0x21);
    }
}

//CPT 0xCD & 0xFF  -- CPT >> 8 egale a CPT /256
void regle_PIT(void) {
    uint32_t i = 0;
    outb(0x34, 0x43);  
    // QUARTZ/CLOCK % 256
    outb((QUARTZ/CLOCKFREQ & 0xFF), 0x40);
    //poids fort:
    outb(((QUARTZ/CLOCKFREQ) >> 8)& 0xFF, 0x40);
    init_traitant_IT(32, traitant_IT_32);
    masque_IRQ(i, false);
    for(i = 1; i < 8; i++) {
        masque_IRQ(i, true);
    }
}
