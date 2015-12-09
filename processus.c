#include "processus.h"

t_proc procs[NB_PROC];
int32_t nb_procs = 0;
int32_t proc_actif = 0;
uint32_t temps_up = 0;

void init_procs(void) {
    if(cree_processus(proc1, "proc1?") == -1) {
        printf("\nSorry, too many processes already!\n");
	return;
    }
    if(cree_processus(proc1, "proc1") == -1) {
        printf("\nSorry, too many processes already!\n");
	return;
    }
    if(cree_processus(proc2, "proc2") == -1) {
        printf("\nSorry, too many processes already!\n");
	return;
    }
    if(cree_processus(proc3, "proc3") == -1) {
        printf("\nSorry, too many processes already!\n");
	return;
    }
}

void ordonnance(void) {
    for(uint32_t i = 0; i < NB_PROC; i++) {
        if((procs[i].mon_etat == ENDORMI) && (temps_up >= procs[i].reveil)) {
            // reveille le processus
            procs[i].mon_etat = ACTIVABLE;
	}
    }
    uint32_t next_proc = (proc_actif + 1) % NB_PROC;
    while(next_proc != proc_actif && procs[next_proc].mon_etat != ACTIVABLE) {
        next_proc = (next_proc + 1) % NB_PROC;
    }
    if(next_proc == proc_actif) {
	// si tous les autres processus sont endormis ou morts: reste avec l'état élu 
        //-> c'est forcement idle
	return;
    }
    procs[proc_actif].mon_etat = ACTIVABLE;
    procs[next_proc].mon_etat = ELU;
    int32_t old_actif = proc_actif;
    proc_actif = next_proc;
    ctx_sw(procs[old_actif].regs, procs[next_proc].regs);
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

int32_t cree_processus(void (*code)(void), char *nom) {
    if(nb_procs == NB_PROC) {
	return -1;
    }
    // if no process yet -> create first one from kernel
    if(nb_procs == 0) {
        procs[0].pid = 0;
	strcpy(procs[0].nom, "idle");
	procs[0].mon_etat = ELU;
	procs[0].reveil = 0;
	proc_actif = 0;
    } else {
	// standard process initialization
	procs[nb_procs].pid = nb_procs;
	strcpy(procs[nb_procs].nom, nom);
	procs[nb_procs].mon_etat = ACTIVABLE;
	procs[nb_procs].reveil = 0;
	procs[nb_procs].regs[1] = (uint32_t)&procs[nb_procs].pile[511];
	procs[nb_procs].pile[510] = (uint32_t)code;
	procs[nb_procs].pile[511] = (uint32_t)fin_processus;
    }
    nb_procs++;
    return nb_procs;
}


void idle(void) {
    for(;;) {
	    //printf("[%s] pid = %i\n", mon_nom(), mon_pid());
	    //for(uint32_t i = 0; i < 100 * 1000 * 1000; i++);
	sti();
	hlt();
	cli();
    }
}

void proc1(void) {        
    for(int32_t i = 0; i < 2; i++) {
        printf("[temps %u] processus %s pid = %i\n", temps_up, mon_nom(), mon_pid());
	dors(2);
    }
    fin_processus();
}

void proc2(void) {        
    for(int32_t i = 0; i < 2; i++) {
        printf("[temps %u] processus %s pid = %i\n", temps_up, mon_nom(), mon_pid());
	dors(3);
    }
    fin_processus();
}

void proc3(void) {        
    for(int32_t i = 0; i < 2; i++) {
        printf("[temps %u] processus %s pid = %i\n", temps_up, mon_nom(), mon_pid());
	dors(5);
    }
    fin_processus();
}

void dors(uint32_t nbr_secs) {
    for(uint32_t i = 0; i < NB_PROC; i++) {
        if(procs[i].mon_etat == ELU) {
	    // get some sleep
	    procs[i].mon_etat = ENDORMI;
	    procs[i].reveil = temps_up + nbr_secs;
	    ordonnance();
	}
    }    
}

void fin_processus() {
    procs[proc_actif].mon_etat = MORT;
    nb_procs--;
    ordonnance();
}
