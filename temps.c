#include "temps.h"

void tic_PIT(void) {
    char buffer[10];
    int32_t taille = 0;
    int32_t minutes = 0;
    int32_t secondes = 0;
    
    outb(0x20, 0x20);
    outb(secondes, 0x20);
    
    taille = sprintf(buffer, "%d:%d", minutes, secondes);
    affiche_heure(buffer, taille);
}

void init_traitant_IT(int32_t numIT, void (*traitant)(void)) {
    uint32_t adP = (int32_t)traitant;
    uint32_t *p;
    p = (uint32_t *)(0x1100 + 8 * numIT);
    *p = (KERNEL_CS << 16) | (adP & 0xFFFF);
    *(p + 1) = (adP & 0xFFFF0000) | 0x8E00;
}

void masque_IRQ(uint32_t numIRQ, bool masque) {
    uint8_t m = 1 << numIRQ;
    uint8_t v = inb(0x21);
    if(masque) {
        outb((m | v), 0x21);
    } else {
        outb((m & v), 0x21);
    }
}

//CPT 0xCD & 0xFF  -- CPT >> 8 egale a CPT /256
void regle_PIT(void) {
    outb(0x34, 0x43);
    // QUARTZ/CLOCK % 256
    outb((QUARTZ/CLOCKFREQ & 0xFF00) >> 8, 0x40);
    //poids fort:
    outb(QUARTZ/CLOCKFREQ, 0x40); 
}
