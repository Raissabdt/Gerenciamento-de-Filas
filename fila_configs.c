#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <windows.h>
#include <time.h>
#include "fila_configs.h"

#define CONFIGS_FILA "arquivo/configs_fila.dat"

// Função para abrir o arquivo da fila (usada pelos dois programas)
FILE *configs_abrirfila() {

    FILE *arquivo;
    if( access(CONFIGS_FILA, F_OK ) != -1 ) {
        arquivo = fopen(CONFIGS_FILA, "rb+");
    } else {
        arquivo = fopen(CONFIGS_FILA, "wb+");
    }
    return arquivo;
}

// Função para fechar ambos os arquivos (usada pelos dois programas)
void configs_fechar(FILE *arquivo) {
    fclose(arquivo);
}

// Função para criar a fila (usada pelo programa TV.C)
filaEspera *criar_fila(){

    filaEspera *fila = (filaEspera *) malloc(sizeof(filaEspera));
    if(fila == NULL){
        printf("Erro ao criar a fila!");
        return NULL;
    }
    fila->primeiro = NULL;
    fila->ultimo = NULL;
    return fila;
}

// Função para criar ficha (usada pelo programa TOTEM.C)
ficha *criar_ficha(int *senha_atual){

    (*senha_atual)++;
    srand(time(NULL));

    char nome[50];
    printf("\nDigite o nome: ");
    fgets(nome, sizeof(nome), stdin);

    ficha *nova_ficha = (ficha *)malloc(sizeof(ficha));
    nova_ficha->senha = (*senha_atual);
    nova_ficha->prioridade = 1;
    nova_ficha->tempo = (rand() % 10) + 1;
    strcpy(nova_ficha->nome, nome);
    nova_ficha->proximo = NULL;

    return nova_ficha;
}

// Função para ler a ficha do arquivo fila (usada pelo programa TV.C)
ficha *ler_ficha(){

    FILE *arquivo_fila;
    arquivo_fila = configs_abrirfila();
    if(arquivo_fila == NULL){
        printf("\nErro ao abrir o arquivo!\n");
        return NULL;
    }

    // Verifica se o arquivo está vazio
    fseek(arquivo_fila, 0, SEEK_END);
    long tamanho = ftell(arquivo_fila);
    rewind(arquivo_fila);

    if(tamanho == 0) {
        configs_fechar(arquivo_fila);
        return NULL;
    }

    ficha *ficha_lida = (ficha *)malloc(sizeof(ficha));
    if(ficha_lida == NULL) {
        printf("\nErro de alocação!\n");
        configs_fechar(arquivo_fila);
        return NULL;
    }

    if(fread(ficha_lida, sizeof(ficha), 1, arquivo_fila) != 1) {
        printf("\nErro ao ler ficha!\n");
        free(ficha_lida);
        configs_fechar(arquivo_fila);
        return NULL;
    }

    configs_fechar(arquivo_fila);
    return ficha_lida;
}

// Função salvar ficha no arquivo da fila (usada pelo programa TOTEM.C)
void salvar_ficha(ficha *nova_ficha){

    FILE *arquivo_fila;
    arquivo_fila = configs_abrirfila();
    if(arquivo_fila == NULL){
        printf("\nErro ao abrir o arquivo!\n");
        return;
    }
    int teste = fwrite(nova_ficha, sizeof(ficha), 1, arquivo_fila);
    if(teste != 1){
        printf("\nErro ao salvar a ficha!\n");
        return;
    }
    configs_fechar(arquivo_fila);
}

// Função para inserir ficha no final da fila (usada pelo programa TV.C)
void atualizar_fila(filaEspera *fila, int *senha_atual){
    if(fila == NULL){
        printf("\nFila inválida!\n");
        return;
    }

    ficha *nova_ficha = ler_ficha();
    if(nova_ficha == NULL) {
        return;
    }

    if(nova_ficha->senha == *senha_atual){
        free(nova_ficha);
    } else {
        // Se fila vazia
        if(fila->primeiro == NULL) {
            fila->primeiro = nova_ficha;
            fila->ultimo = nova_ficha;
        } else {
            fila->ultimo->proximo = nova_ficha;
            fila->ultimo = nova_ficha;
        }
        (*senha_atual)++;
    }
}

// Função para remover ficha no início da fila (usada pelo programa TV.C)
ficha *remover_fila(filaEspera *fila){
    if(fila == NULL || fila->primeiro == NULL){
        return NULL;
    }
    ficha *ficha_removida = fila->primeiro;

    if(fila->primeiro == fila->ultimo){
        fila->primeiro = NULL;
        fila->ultimo = NULL;
    } else {
        fila->primeiro = ficha_removida->proximo;
    }

    ficha_removida->proximo = NULL;
    return ficha_removida;
}

// Função para destruir a fila (usada pelo programa TV.C)
void destruir_fila(filaEspera *fila) {
  if(fila == NULL){
    printf("Fila inválida!");
    return;
  }

  if (fila->primeiro == NULL) {
    free(fila);
    return;
  }

  ficha *atual = fila->primeiro;
  ficha *anterior;
  while (atual != NULL) {
    anterior = atual;
    atual = atual->proximo;
    anterior = NULL;
    free(anterior);
  }
  fila->primeiro = NULL;
  fila->ultimo = NULL;
  free(fila);
}

