#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <windows.h>
#include <time.h>
#include <locale.h>
#include "fila_configs.h"

void chamar_fichas(abbPrior *abb, filaEspera *fila){

    noPrioridade *no_minimo = NULL;
    ficha *ficha_removida = NULL;

    // Se árvore não estiver vazia!
    if(abb->raiz != NULL){
        no_minimo = minimo(abb->raiz);

        if(no_minimo != NULL){
            ficha_removida = remover_fila(no_minimo->fila);

            // Verificar se fila se tornou fazia para remover o nó
            if(no_minimo->fila->primeiro == NULL) {
                remover_no(abb, no_minimo);
            }
        } else {
            printf("Erro ao recurar os dados!");
        }
    } else if(fila->primeiro != NULL){
            ficha_removida = remover_fila(fila);
    } else {
        return;
    }

    if(ficha_removida != NULL){
        printf("\n - Chamando para Atendimento - \n");
        printf("Senha: %d \n", ficha_removida->senha);
        printf("Prioridade: %d \n", ficha_removida->prioridade);
        printf("Nome: %s", ficha_removida->nome);
        printf("Tempo de atendimento: %d \n", ficha_removida->tempo);
        sleep(ficha_removida->tempo); // Espera o tempo da ficha
    }
    free(ficha_removida);
    return;
}
int main() {

    // Configuração para Windows
    SetConsoleOutputCP(65001);  // UTF-8
    setlocale(LC_ALL, "Portuguese_Brazil.utf8");

    int senha_atual = 0;
    int status_anterior = 0;
    int atendendo = 0;

    atendConfigs *atend;
    filaEspera *fila = criar_fila();
    abbPrior *abb = criar_arvore();
    atend = configs_inicializar();

    if (!atend) {
      printf("\nErro ao criar o atendimento\n");
      return 1;
    }

    configs_ler(atend);
    configs_mostrar(atend);

    while(atend->status != 3) {
        sleep(1);

        ficha *nova_ficha = ler_ficha();

        if(nova_ficha != NULL){
            if(nova_ficha->prioridade == 6){
                atualizar_fila(fila, nova_ficha, &senha_atual);
            } else {
                atualizar_arvore(abb, nova_ficha, &senha_atual);
            }
        }

        if(atend->status == 1) {
            if(fila->primeiro != NULL || abb->raiz != NULL) {
                chamar_fichas(abb, fila);
                atendendo = 1;
            } else {
                if(atendendo == 1) {
                    printf("\nNenhuma ficha na fila. Aguardando...\n");
                    atendendo = 0;
                }
            }
        } else if(atend->status == 2) {
            if(status_anterior != 2){
                printf("\nEstamos em intervalo. Por favor, aguarde. \n");
            }
        }
        status_anterior = atend->status;
        configs_ler(atend);
    }

    destruir_fila(fila);
    destruir_arvore(abb);
    configs_destruir(atend);
    DeleteFile("arquivo/configs_fila.dat");
    printf("\nAté a próxima! \n");
    sleep(3);
    return 0;
}
