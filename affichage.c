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

/*
uint8_t get_ligne(void) {
  uint8_t partie_basse = inb(0x3D5);
  uint8_t ligne = partie_basse % 80;
  return ligne;
}

uint8_t get_colonne(void) {
  uint8_t partie_basse = inb(0x3D5);
  uint8_t colonne = (partie_basse - (partie_basse % 80)) / 80;
  return colonne;
}
*/

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

//let's go global
uint32_t ligne = 0;
uint32_t colonne = 0;

void traite_car(char c) {
    //c doit être positif; on ignore tous les caractères >127; on ignore le caractère 127
    c = (int)c;
    if(c < 0 || c > 126) {
        return;
    }
    //get les coordonnées (ligne -> ligne, colonne -> colonne)
    //uint32_t ligne = get_ligne();
    //uint32_t colonne = get_colonne();
    if(c < 32) {
        switch(c) {
        case 8: // '\b'
            if(colonne) {
                place_curseur(ligne, --colonne);
            }
            break;
        case 9: // '\t'
            if(colonne < 72) {
                //avance à la prochaine valeur de la chaine 8
                colonne = colonne + (8 - (colonne % 8));
            } else {
                colonne = 79;
            }
            place_curseur(ligne, colonne);
            break;
        case 10: // '\n'
	    colonne = 0;
	    if(ligne + 1 < 25) {
                place_curseur(++ligne, colonne);
	    } else {
	        defilement();
		place_curseur(ligne, colonne);
	    }
	    break;
        case 12: // '\f'
            efface_ecran();
	    ligne = 0;
	    colonne = 0;
            place_curseur(ligne, colonne);
            break;
        case 13: // '\r'
   	    colonne = 0;
            place_curseur(ligne, colonne);
            break;
        default:
            //ignore les autres caractères
            break;
        }
    } else {
        ecrit_car(ligne, colonne, c);
        if(colonne + 1 < 80) {
	    place_curseur(ligne, ++colonne);
	} else {
	    colonne = 0;
	  if(ligne + 1 < 25) {
	      place_curseur(++ligne, colonne);
	  } else {
	      defilement();
	      place_curseur(ligne, colonne);
	  }
	}
    }
    return;
}

void defilement(void) {
    uint8_t i;
    uint8_t j;
    //uint16_t *destination = ptr_mem(0, 0);
    //uint16_t *source = ptr_mem(1, 0);
    for(i = 0; i < 24; i++) {
        memmove(ptr_mem(i, 0), ptr_mem(i + 1, 0), 160);
    }
    for(j = 0; j < 80; j++) { 
        ecrit_car(24, j, ' ');
    }
}

void console_putbytes(char *chaine, int32_t taille) {
    uint32_t i;
    for(i = 0; i < taille; i++) {
	traite_car(chaine[i]);
    }
}

void affiche_heure(char *chaine, int32_t taille) {
    uint32_t xligne = ligne;
    uint32_t xcolonne = colonne;
    ligne = 0;
    colonne = 80 - strlen(chaine);
    console_putbytes(chaine, taille);
    ligne = xligne;
    colonne = xcolonne;
    place_curseur(ligne, colonne);
}
