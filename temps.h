#ifndef TEMPS_H_
#define TEMPS_H_

#include <cpu.h>
#include <stdbool.h>
#include "stdio.h"
#include <inttypes.h>
#include "affichage.h"
#include "segment.h"

#define QUARTZ 0x1234DD
#define CLOCKFREQ 50

void tic_PIT(void);

void init_traitant_IT(int32_t num_IT, void (*traitant)(void));

void traitant_IT_32(void);

void masque_IRQ(uint32_t numIRQ, bool masque);

void regle_PIT(void);

#endif
