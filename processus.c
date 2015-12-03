#include "processus.h"

t_proc procs[2];

void init_procs(void) {
  procs[0].pid = 0;
  strcpy(procs[0].nom, "idle");
  procs[0].mon_etat = ELU;
  procs[1].pid = 0;
  strcpy(procs[1].nom, "proc1");
  procs[1].mon_etat = ACTIVABLE;
  procs[1].regs[1] = (uint32_t)&procs[1].pile[511];
  procs[1].pile[511] = (uint32_t)proc1;
}

void idle(void) {
  for(uint8_t i = 0; i < 3; i++) { 
    printf("[idle] je tente de passer la main a proc1 ...\n");
    ctx_sw(procs[0].regs, procs[1].regs);
    printf("[idle] proc1 m'a redonne la main ...\n");
  }
  printf("[proc1] j'arrete le systeme!\n");
  hlt();
}

void proc1(void) {
  while(1) {
    printf("[proc1] idle m'a donne la main!\n");
    printf("[proc1] je tente de lui redonner la main!\n");
    ctx_sw(procs[1].regs, procs[0].regs);
  }
}

