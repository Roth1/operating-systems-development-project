#include "processus.h"

t_proc procs[NB_PROC];

void init_procs(void) {
    procs[0].pid = 0;
    strcpy(procs[0].nom, "idle");
    procs[0].mon_etat = ELU;
    procs[1].pid = 1;
    strcpy(procs[1].nom, "proc1");
    procs[1].mon_etat = ACTIVABLE;
    procs[1].regs[1] = (uint32_t)&procs[1].pile[511];
    procs[1].pile[511] = (uint32_t)proc1;
}

void ordonnance(void) {
  for(uint32_t i = 0; i < NB_PROC; i++) {
    if(procs[i].mon_etat == ELU) {
      uint32_t j = (i + 1) % NB_PROC;
      procs[i].mon_etat = ACTIVABLE;
      procs[j].mon_etat = ELU;
      ctx_sw(procs[i].regs, procs[j].regs);
    }
  }
}

char* mon_nom(void) {
  for(uint32_t i = 0; i < NB_PROC; i++) {
    if(procs[i].mon_etat == ELU) {
      return procs[i].nom;
    }
  }
  return "unknown";
}

int32_t mon_pid(void) {
  for(uint32_t i = 0; i < NB_PROC; i++) {
    if(procs[i].mon_etat == ELU) {
      return procs[i].pid;
    }
  }
  return 0;
}

void idle(void) {
  for(;;) {
	printf("[%s] pid = %i\n", mon_nom(), mon_pid());
	for(uint32_t i = 0; i < 100000000; i++);
	ordonnance();
  }
}

void proc1(void) {
  for(;;) {
	printf("[%s] pid = %i\n", mon_nom(), mon_pid());
	for(uint32_t i = 0; i < 100000000; i++);
	ordonnance();
  }
}
