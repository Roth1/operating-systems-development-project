#ifndef PROCESSUS_H_
#define PROCESSUS_H_

#include <cpu.h>
#include <stdbool.h>
#include "stdio.h"
#include <inttypes.h>
#include "affichage.h"
#include "segment.h"

#define NB_PROC 4

typedef enum {ELU, ACTIVABLE, ENDORMI, MORT} etat;

typedef struct t_proc {
    //signed since error is -1
    int32_t pid;
    char nom[10];
    etat mon_etat;
    uint32_t reveil;
    uint32_t regs[5];
    uint32_t pile[512];
} t_proc;

t_proc procs[NB_PROC];

// initialise le tableau des processus
void init_procs(void);

// ordonnancer les processus
void ordonnance(void);

// get le nom d'un processus
char* mon_nom(void);

// get le process id d'un processus
int32_t mon_pid(void);

// creer un processus
int32_t cree_processus(void (*code)(void), char *nom);

// default process, <you may idle here>
void idle(void);

// a standard process
void proc1(void);

// another process
void proc2(void);

// and another process
void proc3(void);

// endormir un processus
void dors(uint32_t nbr_secs);

// terminer un processus
void fin_processus();

// context switcher entre les processus
void ctx_sw(uint32_t *ancien, uint32_t *nouveau);

#endif
