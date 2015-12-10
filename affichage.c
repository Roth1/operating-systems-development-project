#include "affichage.h"

//let's go global
uint32_t ligne = 0;
uint32_t colonne = 0;

// renvoyer un pointeur sur la case mémoire correspondant aux coordonnées fournies
uint16_t *ptr_mem(uint32_t lig, uint32_t col) {
    //calcule la valeur du pointeur sur une case mémoire 
    uint16_t *ptr_case = ADRESSE_DE_DEBUT + (lig * 80 + col);
    return ptr_case;
}

// écrire le caractère c aux coordonnées spécifiées
void ecrit_car(uint32_t lig, uint32_t col, char c, int32_t COLOR) {
    //get pointeur sur la case mémoire
    uint16_t *ptr_format = ptr_mem(lig, col);
    //format: depend de la valeur de couleur / c est le charactere a ecrire
    *ptr_format = COLOR | c;
    return;
}

// qui place le curseur à la position donnée
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

// effacer l'ecran
void efface_ecran(void) {
    //écrit ' ' dans toutes les coordonnées possibles
    uint8_t i, j;
    for(i = 0; i < 25; i++) {
        for(j = 0; j < 80; j++) {
	    ecrit_car(i, j, ' ', VERT);
        }
    }
    return;
}

// traiter un caractère donné (c'est à dire qui l'affiche si c'est un caractère normal ou qui implante l'effet voulu si c'est un caractère de contrôle)
void traite_car(char c) {
    // c doit être positif; on ignore tous les caractères >127; on ignore le caractère 127
    c = (int)c;
    if(c < 0 || c > 126) {
        return;
    }
    // get les coordonnées (ligne -> ligne, colonne -> colonne)
    // selon les commandes on change la position du curseur
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
            // ignore tous les autres caractères
            break;
        }
    } else {
      // cas standard: ecrit normal d'un caractere (en faisant attention aux contraintes)
      ecrit_car(ligne, colonne, c, VERT);
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

// remonter d'une ligne l'affichage à l'écran (il pourra être judicieux d'utiliser memmove définie dans string.h pour cela)
void defilement(void) {
    uint8_t i;
    uint8_t j;
    // defilement: chaque ligne monte par 1
    for(i = 0; i < 24; i++) {
        memmove(ptr_mem(i, 0), ptr_mem(i + 1, 0), 160);
    }
    // nouvelle ligne vide en bas
    for(j = 0; j < 80; j++) { 
      ecrit_car(24, j, ' ', VERT);
    }
}

// mettre les bytes sur l'écran (utilisé par printf())
void console_putbytes(char *chaine, int32_t taille) {
    uint32_t i;
    // appel simple de traite_car
    for(i = 0; i < taille; i++) {
	traite_car(chaine[i]);
    }
}

// afficher l'uptime
void affiche_heure(char *chaine, int32_t taille) {
    // sauvegarde la position du curseur actuel
    uint32_t xligne = ligne;
    uint32_t xcolonne = colonne;
    // l'heure est en haut a droite
    ligne = 0;
    colonne = 80 - strlen(chaine);
    // affiche une chaine qui represente l'uptime
    console_putbytes(chaine, taille);
    // remet le curseur au bon endroit
    ligne = xligne;
    colonne = xcolonne;
    place_curseur(ligne, colonne);
}
