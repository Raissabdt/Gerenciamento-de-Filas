#ifndef FILA_CONFIGS_H
#define FILA_CONFIGS_H

#include <stdio.h>
#include <stdlib.h>

// Structs definidas em relat_configs.c

typedef struct especialidades {
    char nome[50];
    int qtd;
} especialidades;

typedef struct vetorDinamico {
    int tamanho;
    especialidades *elementos;
} vetorDinamico;

// Structs definidas em fila_configs.c

typedef struct ficha ficha;

struct ficha {
    int senha;
    int prioridade;
    char especialidade[50];
    int tempo;
    char nome[50];
    ficha *proximo;
};

typedef struct filaEspera {
    ficha *primeiro;
    ficha *ultimo;
} filaEspera;

// Structs definadas em arv_configs.c

typedef struct noPrioridade noPrioridade;

struct noPrioridade {
    int chave;
    filaEspera *fila;
    noPrioridade *direito;
    noPrioridade *esquerdo;
    noPrioridade *pai;
};

typedef struct abbPrior {
    noPrioridade *raiz;
} abbPrior;

// Structs definidas em atend_configs.c

typedef struct atendConfigs {
    int status;
    int intervalo;
} atendConfigs;

// Funções definidas em fila_configs.c
FILE *configs_abrirfila();
void configs_fechar(FILE *arquivo);

filaEspera *criar_fila();
void atualizar_fila(filaEspera *fila, ficha *nova_ficha, int *senha_atual);
void destruir_fila(filaEspera *fila);

ficha *criar_ficha(int prioridade, char *especialidade, int *senha_atual);
void salvar_ficha(ficha *nova_ficha);
ficha *ler_ficha();
ficha *remover_fila(filaEspera *fila);


// Funções definidas em atend_configs.c
FILE *configs_abriratend();
atendConfigs *configs_inicializar();
void configs_fechar_atend(FILE *arquivo);

void configs_ler(atendConfigs *atend);
void configs_salvar(atendConfigs *atend);
void configs_atualizar(atendConfigs *atend, int status, int intervalo);
void configs_mostrar(atendConfigs *atend);
void configs_destruir(atendConfigs *atend);

// Funções definidas em arv_configs.c
abbPrior *criar_arvore();
void atualizar_arvore(abbPrior *abb, ficha *nova_ficha, int *senha_atual);
void destruir_arvore(abbPrior *abb);

noPrioridade *criar_no(ficha *nova_ficha);
noPrioridade *buscar_no(abbPrior *abb, int chave);
void remover_no(abbPrior *abb, noPrioridade *no);
void inserir_no(abbPrior *abb, noPrioridade *novo_no);
ficha *ler_proximo_arv(abbPrior *abb);
noPrioridade *minimo(noPrioridade *node);

// Funções definidas em relat_configs.c

vetorDinamico *criar_vetor(int capacidade);
int buscar_elemento(vetorDinamico *vetor, char *nova_esp);
void atualizar_vetor(vetorDinamico *vetor, char *nova_esp);
void imprimir_relatorio(vetorDinamico *vetor);
void destruir_vetor(vetorDinamico *vetor);


#endif
