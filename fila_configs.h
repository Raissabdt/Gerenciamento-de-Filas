#ifndef FILA_CONFIGS_H
#define FILA_CONFIGS_H

#include <stdio.h>
#include <stdlib.h>

// Structs definidas em fila_configs.c

typedef struct ficha ficha;

struct ficha {
    int senha;
    int prioridade;
    int tempo;
    char nome[50];
    ficha *proximo;
};


typedef struct filaEspera {
    ficha *primeiro;
    ficha *ultimo;
} filaEspera;

// Structs definidas em atend_configs.c

typedef struct atendConfigs {
    int status;
    int intervalo;
} atendConfigs;

// Funções definidas em fila_configs.c
FILE *configs_abrirfila();
filaEspera *criar_fila();
ficha *ler_ficha();
ficha *criar_ficha(int *senha_atual);
ficha *remover_fila(filaEspera *fila);
void salvar_ficha(ficha *nova_ficha);
void configs_fechar(FILE *arquivo);
void destruir_fila(filaEspera *fila);
void atualizar_fila(filaEspera *fila, int *senha_atual);

//Funções definidas em atend_configs.c
FILE *configs_abriratend();
atendConfigs *configs_inicializar();
void configs_fechar_atend(FILE *arquivo);
void configs_ler(atendConfigs *atend);
void configs_salvar(atendConfigs *atend);
void configs_atualizar(atendConfigs *atend, int status, int intervalo);
void configs_mostrar(atendConfigs *atend);
void configs_destruir(atendConfigs *atend);


#endif
