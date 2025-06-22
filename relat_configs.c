#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <windows.h>
#include <locale.h>
#include <time.h>
#include "fila_configs.h"

// Relatório será um vetor dinâmico da struct especialidades

vetorDinamico *criar_vetor(int capacidade){

    vetorDinamico *vetor = (vetorDinamico*)malloc(sizeof(vetorDinamico)); // Aloca memória para o vetor
    if (vetor == NULL) {
        printf("Erro ao alocar memória para o vetor.\n");
        return NULL;
    }
    vetor->tamanho = 0;
    vetor->elementos = (especialidades*)malloc(capacidade * sizeof(especialidades)); // Aloca memória para os elementos do vetor
    if (vetor->elementos == NULL) {
        printf("Erro ao alocar memória para os elementos do vetor.\n");
        free(vetor);
        return NULL;
    }
    return vetor;
}

int buscar_elemento(vetorDinamico *vetor, char *nova_esp){
    int flag = -1, i;
	for(i=0; i<vetor->tamanho; i++){
	    if(strcmp(vetor->elementos[i].nome, nova_esp) == 0){ // Função compara strings caracter por caracter
	        flag = i;
	        break;
	    }
	}
    return flag;
}

void atualizar_vetor(vetorDinamico *vetor, char *nova_esp) {

    int busca = buscar_elemento(vetor, nova_esp);

    if(busca == -1){
        strcpy(vetor->elementos[vetor->tamanho].nome, nova_esp);
        vetor->elementos[vetor->tamanho].qtd = 1;
        vetor->tamanho++;
    } else {
        (vetor->elementos[busca].qtd)++;
    }
}

void imprimir_relatorio(vetorDinamico *vetor){
    // Configuração para Windows
    SetConsoleOutputCP(65001);  // UTF-8
    setlocale(LC_ALL, "Portuguese_Brazil.utf8");

    printf("\n----- Relatório de Especialidades Atendidas: -----\n");

    if(vetor->tamanho == 0){
        printf("Nenhuma ficha gerada.\n");
    }
    int i;
    for (i = 0; i < vetor->tamanho; i++) {
        printf("%s: %d\n", vetor->elementos[i].nome, vetor->elementos[i].qtd);
    }
    printf("\n");
}

void destruir_vetor(vetorDinamico *vetor){
    if (vetor != NULL) {
        free(vetor->elementos);
        free(vetor);
    }
}

