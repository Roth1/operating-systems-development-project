#include "processus.h"

/* define les variables globales */
// le tableau des processus
t_proc procs[NB_PROC];
// le nombre actuel des processus
int32_t nb_procs = 0;
// le processus actif
int32_t proc_actif = 0;
// le temps depuis le launch
uint32_t temps_up = 0;

// initialise le tableau des processus
void init_procs(void) {
    // init les processus comme morts 
    for(int32_t i = 0; i < NB_PROC; i++) {
      procs[i].mon_etat = MORT;
    }
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

// ordonnancer les processus
void ordonnance(void) {
    // si un processus doit se reveiller -> reveille-le
    for(uint32_t i = 0; i < NB_PROC; i++) {
        if((procs[i].mon_etat == ENDORMI) && (temps_up >= procs[i].reveil)) {
            // reveille le processus
            procs[i].mon_etat = ACTIVABLE;
	}
    }
    // cherche le processus suivant
    uint32_t next_proc = (proc_actif + 1) % NB_PROC;
    while(next_proc != proc_actif && procs[next_proc].mon_etat != ACTIVABLE) {
        next_proc = (next_proc + 1) % NB_PROC;
    }
    if(next_proc == proc_actif) {
	// si tous les autres processus sont endormis ou morts: reste avec l'état élu 
        //-> c'est forcement idle
        return;
    }
    // si le processus actif est ELU, son nouveau etat est ACTIVABLE
    // les processus endormis ou morts ne vont pas dans cet etat
    if(procs[proc_actif].mon_etat == ELU) {
        procs[proc_actif].mon_etat = ACTIVABLE;
    }
    // prochain processus est elu
    procs[next_proc].mon_etat = ELU;
    // swap, pour que proc_actif soit actuel
    int32_t old_actif = proc_actif;
    proc_actif = next_proc;
    // appelle context switch
    ctx_sw(procs[old_actif].regs, procs[next_proc].regs);
}

// get le nom du processus actif
char* mon_nom(void) {
    return procs[proc_actif].nom;
}

// get le pid du processus actif
int32_t mon_pid(void) {
    return procs[proc_actif].pid;
}

// cree un processus
int32_t cree_processus(void (*code)(void), char *nom) {
    // si on trop de processus -> return error (-1)
    if(nb_procs == NB_PROC) {
	return -1;
    }
    // if no process yet -> create first one from kernel -> c'est idle
    if(nb_procs == 0) {
        procs[0].pid = 0;
	strcpy(procs[0].nom, "idle");
	procs[0].mon_etat = ELU;
	procs[0].reveil = 0;
	proc_actif = 0;
    } else {
        // trouve un pid mort
        int32_t pid_mort;
        for(pid_mort = 1; pid_mort < NB_PROC; pid_mort++) {
	  if (procs[pid_mort].mon_etat == MORT)
            {
	      break;
            }
        }
	// standard process initialization -> pour le pid mort trouve
	procs[pid_mort].pid = pid_mort;
	strcpy(procs[pid_mort].nom, nom);
	procs[pid_mort].mon_etat = ACTIVABLE;
	procs[pid_mort].reveil = 0;
	procs[pid_mort].regs[1] = (uint32_t)&procs[pid_mort].pile[510];
	procs[pid_mort].pile[510] = (uint32_t)code;
	procs[pid_mort].pile[511] = (uint32_t)fin_processus;
    }
    // augmente le nombre des processus
    nb_procs++;
    return nb_procs;
}

// <you may idle here>
void idle(void) {
    for(;;) {
	sti();
	hlt();
	cli();
    }
}

// process numero uno
void proc1(void) {        
    for(int32_t i = 0; i < 10; i++) {
        printf("[temps %0u] processus %s pid = %i\n", temps_up, mon_nom(), mon_pid());
	dors(2);
    }
}

// deuxieme processus -> va creer un processus qui utilise cette fonction
// mais s'appelle proc1 (car on utilise le proc1 mort)
void proc2(void) {        
    for(int32_t i = 0; i < 10; i++) {
        printf("[temps %0u] processus %s pid = %i\n", temps_up, mon_nom(), mon_pid());
	dors(3);
    }
    if(cree_processus(proc2, "proc1") == -1) {
      printf("Impossible de creer le processus : nb_procs = %d\n", nb_procs);
    }
}

// dritter prozess
void proc3(void) {        
    for(int32_t i = 0; i < 10; i++) {
        printf("[temps %0u] processus %s pid = %i\n", temps_up, mon_nom(), mon_pid());
	dors(5);
    }
}

// get some sleep
void dors(uint32_t nbr_secs) {
    procs[proc_actif].mon_etat = ENDORMI;
    // tell him when to wake up
    procs[proc_actif].reveil = temps_up + nbr_secs;
    ordonnance();
}

// murder a process
void fin_processus() {
    procs[proc_actif].mon_etat = MORT;
    nb_procs--;
    ordonnance();
}

// affiche les processus avec leurs etats (en couleur!)
void affiche_procs() {
  // 4 processus avec des buffers individuel (car la couleur est different)
  char to_print[4][32];
  to_print[0][0] = '\0';
  to_print[1][0] = '\0';
  to_print[2][0] = '\0';
  to_print[3][0] = '\0';
  for(int32_t i = 0; i < NB_PROC; i++) {
    // affiche son nom
    sprintf(to_print[i], "%d: [%s] = ", i, procs[i].nom);
    switch(procs[i].mon_etat) {
    case ELU:
        // affiche etat et donne le couleur correspondant 
        sprintf(to_print[i] + strlen(to_print[i]), "ELU         ");
	for (uint8_t j = 0; j < strlen(to_print[i]); j++) {
	    ecrit_car(i, j+45, to_print[i][j], JAUNE);
        }
	break;
    case ACTIVABLE:
        sprintf(to_print[i] + strlen(to_print[i]), "ACTIVABLE  ");
	for (uint8_t j = 0; j < strlen(to_print[i]); j++) {
	    ecrit_car(i, j+45, to_print[i][j], BLANC);
        }
	break;
    case ENDORMI:
        sprintf(to_print[i] + strlen(to_print[i]), "ENDORMIS   ");
	for (uint8_t j = 0; j < strlen(to_print[i]); j++) {
	    ecrit_car(i, j+45, to_print[i][j], BLEU);
        }
	break;
    case MORT:
        sprintf(to_print[i] + strlen(to_print[i]), "MORT      ");
        for (uint8_t j = 0; j < strlen(to_print[i]); j++) {
	    ecrit_car(i, j+45, to_print[i][j], ROUGE);
        }
	break;
    default:
        break;
    }
  }
}
