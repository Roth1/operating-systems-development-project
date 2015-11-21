#include "processus.h"

t_proc procs[2];

void idle(void) {
    printf("[idle] je tente de passer  la main a proc1 ...\n");
    ctx_sw(procs[0].regs, procs[1].regs);
}

void proc1(void) {
    printf("[proc1] idle m'a donne la main!\n");
    printf("[proc1] j'arrete le systeme!\n");
    hlt();
}

