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
    printf("\n0. Sair\n1. Iniciar Atendimento\n2. Pausar Atendimento\n3. Encerrar Atendimento\n4. Retirar Ficha\n5. Gerar Relatório\n");
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

int menu_esp(){

    printf("\nEspecialidade de atendimento:\n");
    printf("\n1. Cardiologista\n2. Neurologista\n3. Oftalmologista\n4. Dermatologista\n5. Pediatra\n6. Outro\n");
    printf("Informe a opção desejada: ");
    int aux;
    scanf("%d", &aux);
    while(getchar() != '\n'); // Limpa o buffer de entrada
    return aux;
}

char *esp(){

    char *str = (char*)malloc(50 * sizeof(char));

    if(str){

        int op = menu_esp();

        while(!(op >= 1 && op <= 6)){
            printf("\nOpção inválida!\n");
            op = menu_esp();
        }

        switch(op){
            case 1: {
                strcpy(str, "Cardiologista");
                break;
            }
            case 2: {
                strcpy(str, "Neurologista");
                break;

            }
            case 3: {
                strcpy(str, "Oftalmologista");
                break;
            }
            case 4: {
                strcpy(str, "Dermatologista");
                break;
            }
            case 5: {
                strcpy(str, "Pediatra");
                break;
            }
            case 6: {
                strcpy(str, "Outro");
                break;
            }
        }

        return str;
    } else {
        return NULL;
    }
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

  vetorDinamico *relatorio = criar_vetor(6);

  if (!relatorio) {
    printf("\nErro ao criar o relatório de atendimento!\n");
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

                char *especialidade = esp();

                ficha *nova_ficha = criar_ficha(prioridade, especialidade, &senha_atual); // Cria uma nova ficha
                if(nova_ficha != NULL) { // Se a ficha for criada com sucesso
                    salvar_ficha(nova_ficha); // Salva a ficha
                    atualizar_vetor(relatorio, especialidade);
                    free(nova_ficha);
                    printf("\nFicha criada com sucesso, sua senha é %d! Aguarde.\n", senha_atual);
                }
                sleep(1);
                break;
          }
          case 5: {
            imprimir_relatorio(relatorio);
            break;
          }
          case 0: {
            configs_destruir(atend);
            destruir_vetor(relatorio);
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

