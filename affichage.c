#include "affichage.h"

uint16_t *ptr_mem(uint32_t lig, uint32_t col) {
    uint16_t *ptr_case = ADRESSE_DE_DEBUT + 2 * (lig * 80 + col);
    return ptr_case;
}

void ecrit_car(uint32_t lig, uint32_t col, char c) {
    uint16_t *ptr_case = ptr_mem(lig, col);
    *ptr_case &= ~255;
    *ptr_case = c;
    return;
}
