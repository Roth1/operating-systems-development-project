#include "temps.h"

// variables globales
uint32_t compteur = 0;
//int32_t heure = 0;
int32_t minutes = 0;
int32_t secondes = 0;

// pour l'ordonnonceur
extern uint32_t temps_up;

// fonction compteur
void tic_PIT(void) {
    outb(0x20, 0x20);  
    char buffer[12];
    int32_t taille = 10;
    compteur++;
    // traduit en secondes/minutes
    if(compteur == 50) {
        temps_up++;
        secondes++;
	if(secondes == 60) {
	    minutes++;
	    secondes = 0;
	}
	compteur = 0;
    }
    // cree l'affichage du temps
    taille = sprintf(buffer, "%02d:%02d", minutes, secondes);
    // affichage de l'heure mis a niveau chaque tic 
    affiche_heure(buffer, taille);
    // ordonnancement des processus gere par l'interruption
    ordonnance();
    // affichage des procs mis a niveau chaque tic 
    affiche_procs();
}

// initialise l'interrupt 32 avec la fonction traitant
void init_traitant_IT(int32_t numIT, void (*traitant)(void)) {
    // adP pointe sur la fonction traitant
    uint32_t adP = (int32_t)traitant;
    uint32_t *p;
    // p pointe sur la position en memoire
    p = (uint32_t *)(0x1000 + 8 * numIT);
    // y met le poids faible (les LSB) de l'adresse de la fonction traitant
    *p = (KERNEL_CS << 16) | (adP & 0xFFFF);
    // y met le poids fort (les MSB) de l'adresse
    *(p + 1) = (adP & 0xFFFF0000) | 0x8E00;
}

// masque ou demasque la routine d'interruption
void masque_IRQ(uint32_t numIRQ, bool masque) {
    // push 1 a gauche selon le numero de l'interrupt  
    uint8_t m = 1 << numIRQ;
    //valeur actuelle de la masque
    uint8_t v = inb(0x21);
    // masque ou un demasque -> on force la valeur du bit N
    if(masque) {
        // masque le it
        outb((m | v), 0x21);
    } else {
      // demasque le it
      outb((~m & v), 0x21);
    }
}

// regle l'interruption
void regle_PIT(void) {
    uint32_t i = 0;
    outb(0x34, 0x43);  
    // poids faible (LSB)
    outb((QUARTZ/CLOCKFREQ & 0xFF), 0x40);
    // poids fort (MSB)
    outb(((QUARTZ/CLOCKFREQ) >> 8)& 0xFF, 0x40);
    // init it32 avec la fonction traitant_IT_32
    init_traitant_IT(32, traitant_IT_32);
    // demasque l'interruption routine 0
    masque_IRQ(i, false);
    // masque l'interruption routine 1-7
    for(i = 1; i < 8; i++) {
        masque_IRQ(i, true);
    }
}
