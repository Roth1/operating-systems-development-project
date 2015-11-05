#ifndef TEMPS_H_
#define TEMPS_H_

#include <cpu.h>
#include <stdbool.h>
#include "stdio.h"
#include <inttypes.h>
#include "affichage.h"
#include "segment.h"

extern uint32_t QUARTZ = 0x1234DD;
extern uint32_t CLOCKFREQ = 50;

void tic_PIT(void);

void init_traitant_IT(int32_t num_IT, void (*traitant)(void));

void masque_IRQ(uint32_t numIRQ, bool masque);

void regle_PIT(void);

#endif
