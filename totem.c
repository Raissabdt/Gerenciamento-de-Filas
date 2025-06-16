#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <windows.h>
#include <time.h>
#include <locale.h>
#include "fila_configs.h"

int menu() { // Menu com informações
    printf("\n----- MENU INICIAL -----\n");
    printf("\n0. Sair\n1. Iniciar Atendimento\n2. Pausar Atendimento\n3. Encerrar Atendimento\n4. Retirar Ficha\n");
    printf("Informe a opção desejada: ");
    int aux;
    scanf("%d", &aux);
    while(getchar() != '\n'); // Limpa o buffer de entrada
    return aux;
}

int menu_prior(){ // Menu com as prioridades de atendimento

    printf("\nPrioridade de atendimento:\n");
    printf("\n1. Gestante\n2. Idoso\n3. Pessoa com necessidades especiais (PNE)\n4. Criança de colo\n5. Doença Crônica\n6. Demais pacientes\n");
    printf("Informe a opção desejada: ");
    int aux;
    scanf("%d", &aux);
    while(getchar() != '\n'); // Limpa o buffer de entrada
    return aux;
}

int main() {

  // Configuração para Windows
  SetConsoleOutputCP(65001);  // UTF-8
  setlocale(LC_ALL, "Portuguese_Brazil.utf8");

  atendConfigs *atend; // Cria um tad de atendimento
  int op;
  int senha_atual=0;

  atend = configs_inicializar(); // Inicializa o tad de atendimento
  if (!atend) {
    printf("\nErro ao criar o atendimento!\n");
    return 1;
  }
  do {
      op = menu(); // Chama o menu
      switch(op) {
          case 1: {
              configs_atualizar(atend,1,1);// Iniciar Atendimento
              break;
          }
          case 2: {
              configs_atualizar(atend,2,1);// Pausar Atendimento
              break;
          }
          case 3: {
              configs_atualizar(atend,3,1);// Encerrar Atendimento
              break;
          }
          case 4: {
                printf("\n--- Nova Ficha ---\n");

                int prioridade = menu_prior();

                while(!(prioridade >= 1 && prioridade <= 6)){
                    printf("\nOpção inválida!\n");
                    prioridade = menu_prior();
                }

                ficha *nova_ficha = criar_ficha(prioridade, &senha_atual); // Cria uma nova ficha
                if(nova_ficha != NULL) { // Se a ficha for criada com sucesso
                    salvar_ficha(nova_ficha); // Salva a ficha
                    free(nova_ficha);
                    printf("\nFicha criada com sucesso! Aguarde.\n");
                }
                sleep(1);
                break;
          }
          case 0: {
            configs_destruir(atend);
            DeleteFile("arquivo/configs_atend.dat");
            printf("\nAté a próxima!\n");
            break;
          }
          default: {
              printf("\nOpção inválida!\n");
          }
      }
  } while (op != 0);
  return 0;
}

