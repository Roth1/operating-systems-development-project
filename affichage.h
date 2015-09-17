#ifndef AFFICHAGE_H_
#define AFFICHAGE_H_

#include <inttypes.h>

#define ADRESSE_DE_DEBUT (uint16_t *)0xB8000

//renvoyer un pointeur sur la case mémoire correspondant aux coordonnées fournies
uint16_t *ptr_mem(uint32_t lig, uint32_t col);

//écrire le caractère c aux coordonnées spécifiées
void ecrit_car(uint32_t lig, uint32_t col, char c);

//qui place le curseur à la position donnée
//void place_curseur(uint32_t lig, uint32_t col);

//effacer l'écran
//void efface_ecran(void);

//traiter un caractère donné (c'est à dire qui l'affiche si c'est un caractère normal ou qui implante l'effet voulu si c'est un caractère de contrôle)
//void traite_car(char c);

//remonter d'une ligne l'affichage à l'écran (il pourra être judicieux d'utiliser memmove définie dans string.h pour cela)
//void defilement(void);

//afficher une chaine de caractères à la position courante du curseur
//void console_putbytes(char *chaine, int32_t taille);

#endif
