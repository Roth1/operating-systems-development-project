#include "affichage.h"

uint16_t *ptr_mem(uint32_t lig, uint32_t col) {
    //calcule la valeur du pointeur sur une case mémoire 
    uint16_t *ptr_case = ADRESSE_DE_DEBUT + (lig * 80 + col);
    return ptr_case;
}

void ecrit_car(uint32_t lig, uint32_t col, char c) {
    //get pointeur sur la case mémoire
    uint16_t *ptr_format = ptr_mem(lig, col);
    //set tous les bits de la case mémoire à 0
    *ptr_format &= ~65534;
    *ptr_format &= ~1;
    //format: fond noir, texte vert
    *ptr_format |= 512;
    //set caractère
    *ptr_format |= c;
    return;
}

void place_curseur(uint32_t lig, uint32_t col) {
    //calcule la position et sépare les 16 bit en deux parties
    uint16_t pos = (uint16_t)(col + lig * 80);
    uint8_t partie_basse = pos & 0xFF;
    uint8_t partie_haute = (pos & 0xFF00) >> 8;
    //commande les ports et envoie les parties de la position respectivement
    outb(0x0F, 0x3D4);
    outb(partie_basse, 0x3D5);
    outb(0x0E, 0x3D4);
    outb(partie_haute, 0x3D5);
    return;
}

void efface_ecran(void) {
    //écrit ' ' dans toutes les coordonnées possibles
    uint8_t i, j;
    for(i = 0; i < 25; i++) {
        for(j = 0; j < 80; j++) {
            ecrit_car(i, j, ' ');
        }
    }
    return;
}

/*** WHAT IF LIGNE >80 ? + GET POS_Y / POS_X ***/
void traite_car(char c) {
    //c doit être positif; on ignore tous les caractères >127; on ignore le caractère 127
    if(c < 0 || c > 126) {
        return;
    }
    //get les coordonnées (ligne -> pos_y, colonne -> pos_x)
    uint32_t pos_y = 0;
    uint32_t pos_x = 0;
    if(c < 32) {
        switch(c) {
        case 8: // '\b'
            if(pos_x) {
                place_curseur(pos_y, --pos_x);
            }
            break;
        case 9: // '\t'
            if(pos_x < 72) {
                //avance à la prochaine valeur de la chaine 8
                pos_x = pos_x + (8 - (pos_x % 8));
            } else {
                pos_x = 79;
            }
            place_curseur(pos_y, pos_x);
            break;
        case 10: // '\n'
            place_curseur(++pos_y, 0); //!!!
            break;
        case 12: // '\f'
            efface_ecran();
            place_curseur(0, 0);
            break;
        case 13: // '\r'
            place_curseur(pos_y, 0);
            break;
        default:
            //ignore les autres caractères
            break;
        }
    } else {
        ecrit_car(pos_y, pos_x, c);
        if(pos_x + 1 < 80) {
            place_curseur(pos_y, ++pos_x);
        } else {
            place_curseur(++pos_y, 0); //!!!
        }
    }
    return;
}
