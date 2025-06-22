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
        printf("Prioridade: %d (", ficha_removida->prioridade);
        switch(ficha_removida->prioridade){
            case 1: {
                printf("Gestante)\n");
                break;
            }
            case 2: {
                printf("Idoso)\n");
                break;
            }
            case 3: {
                printf("PNE)\n");
                break;
            }
            case 4: {
                printf("Criança de colo)\n");
                break;
            }
            case 5: {
                printf("Doença crônica)\n");
                break;
            }
            case 6: {
                printf("Demais pacientes)\n");
                break;
            }
        };
        printf("Especialidade: %s \n", ficha_removida->especialidade);
        printf("Nome: %s", ficha_removida->nome);
        printf("Tempo de atendimento: %d \n", ficha_removida->tempo);
        sleep(ficha_removida->tempo); // Espera o tempo da ficha
    }
    free(ficha_removida->especialidade);
    free(ficha_removida);
    return;
}

void mostrar_proximo(abbPrior *abb, filaEspera *fila){

    ficha *proximo_fila;

    if(abb->raiz == NULL){
        proximo_fila = fila->primeiro->proximo; // Se a árvore estiver vazia, significa que o último chamado foi da fila sem prioridade
    } else {                                    // Logo, como o chamado no momento será o primeiro da fila, então o próximo é o posterior
        proximo_fila = fila->primeiro; // Se a árvore não estiver vazia, significa que estão chamando a prioridade primeiro
                                       // Logo, o próximo a ser chamado será o primeiro
    }

    ficha *proximo_prior = ler_proximo_arv(abb);

    if(proximo_prior != NULL){
        printf("\n - Próximo da fila com prioridade: %d - \n", proximo_prior->senha);
    } else {
        printf("\n - Próximo da fila com prioridade: Fila vazia! - \n");
    }

    if(proximo_fila != NULL){
        printf("\n - Próximo da fila sem prioridade: %d - \n", proximo_fila->senha);
    } else {
        printf("\n - Próximo da fila sem prioridade: Fila vazia! - \n");
    }
}

int main() {

    // Configuração para Windows
    SetConsoleOutputCP(65001);  // UTF-8
    setlocale(LC_ALL, "Portuguese_Brazil.utf8");

    int senha_atual = 0;
    int status_anterior = 0;

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
            if(status_anterior != 1 && status_anterior != 0){
                printf("\nAtendimento Iniciado!\n");
                status_anterior = 1;
            }
            if(fila->primeiro != NULL || abb->raiz != NULL) {
                mostrar_proximo(abb, fila);
                chamar_fichas(abb, fila);
                status_anterior = 1;
            } else {
                if(status_anterior == 1) {
                    printf("\nNenhuma ficha na fila. Aguardando...\n");
                    status_anterior = 0;
                }
            }
        } else if(atend->status == 2) {
            if(status_anterior != 2){
                printf("\nEstamos em intervalo. Por favor, aguarde. \n");
                status_anterior = 2;
            }
        }
        configs_ler(atend);
    }

    destruir_fila(fila);
    destruir_arvore(abb);
    configs_mostrar(atend);
    configs_destruir(atend);
    DeleteFile("arquivo/configs_fila.dat");
    printf("Até a próxima! \n");
    sleep(3);
    return 0;
}
