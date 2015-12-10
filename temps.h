#ifndef TEMPS_H_
#define TEMPS_H_

#include <cpu.h>
#include <stdbool.h>
#include "stdio.h"
#include <inttypes.h>
#include "affichage.h"
#include "segment.h"
#include "processus.h"

#define QUARTZ 0x1234DD
#define CLOCKFREQ 50

// fonction compteur pour gerer le temps
void tic_PIT(void);

// initialise le interrupt 32 avec la fonction traitant
void init_traitant_IT(int32_t num_IT, void (*traitant)(void));

// initialise l'interrupt 32 avec la fonction traitant
void traitant_IT_32(void);

// masque ou demasque la routine d'interruption
void masque_IRQ(uint32_t numIRQ, bool masque);

// regle l'interruption
void regle_PIT(void);

#endif
