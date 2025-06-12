#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <windows.h>
#include <time.h>
#include "fila_configs.h"

#define CONFIGS_ATEND "arquivo/configs_atend.dat"

// Função para abrir o arquivo do atendimento (usada pelos dois programas)
FILE *configs_abriratend() {
    FILE *arquivo;
    if( access(CONFIGS_ATEND, F_OK ) != -1 ) { // Caso o arquivo já exista, apenas abre
        arquivo = fopen(CONFIGS_ATEND, "rb+");
    } else {
        arquivo = fopen(CONFIGS_ATEND, "wb+"); // Caso o arquivo não exista, abre e escreve
        atendConfigs *atend = (atendConfigs *)malloc(sizeof(atendConfigs)); // Cria um atendimento
        atend->status = 2; // Define valores padrão
        atend->intervalo = 1;
        if (atend && arquivo) {
          fwrite(atend, sizeof(atendConfigs), 1, arquivo); // Escreve no arquivo, termina a função,
        }                                                  // perde o atendimento
    }
    return arquivo;
}

void configs_fechar_atend(FILE *arquivo){
    fclose(arquivo);
}

//Função que lê o atendimento (usada pelo programa TV.C)
void configs_ler(atendConfigs *atend) {
  FILE *arquivo = configs_abriratend();
  if (atend && arquivo) {
    fread(atend, sizeof(atendConfigs), 1, arquivo);
  }
  configs_fechar_atend(arquivo);
}

// Função que inicializa o atendimento
atendConfigs *configs_inicializar() {
    atendConfigs *atend = (atendConfigs *)malloc(sizeof(atendConfigs));
    if (!atend) {
      return NULL;
    }
    FILE *arquivo = configs_abriratend();
    if(arquivo) {
        configs_ler(atend); // Salva
        configs_fechar_atend(arquivo);
    } else {
        return NULL;
    }
    return atend; // Retorna o atendimento, dessa vez não é perdido!
}

void configs_salvar(atendConfigs *atend) {
    FILE *arquivo = configs_abriratend();
    if (atend && arquivo) {
      fwrite(atend, sizeof(atendConfigs), 1, arquivo);
    }
    configs_fechar_atend(arquivo);
}

void configs_atualizar(atendConfigs *atend, int status, int intervalo) {
  if (atend) {
      atend->status = status;
      atend->intervalo = intervalo;
      configs_salvar(atend);
  }
}

void configs_mostrar(atendConfigs *atend) {
  if (atend) {
      // Exibir configurações (carrega do arquivo existente se existir)
      printf("\nConfigurações:\n");
      printf(" - Status: %s\n", atend->status == 1 ? "Atendendo" : atend->status == 2 ? "Atendimento em intervalo" : "Atendimento Encerrado");
      printf(" - Intervalo: %d segundo\n\n", atend->intervalo);
  }
}

void configs_destruir(atendConfigs *atend) {
  if (atend) {
    free(atend);
  }
}
