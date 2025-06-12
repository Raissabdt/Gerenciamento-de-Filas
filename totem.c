#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <windows.h>
#include <time.h>
#include <locale.h>
#include "fila_configs.h"

int menu() { // Menu com as a��es poss�veis
    printf("\n0. Sair\n1. Iniciar Atendimento\n2. Pausar Atendimento\n3. Encerrar Atendimento\n4. Retirar Ficha\n");
    printf("Informe a op��o desejada: ");
    int aux;
    scanf("%d", &aux);
    while(getchar() != '\n'); // Limpa o buffer de entrada
    return aux;
}
int main() {

  setlocale(LC_ALL, "Portuguese");

  atendConfigs *atend; // Cria atendimento
  int op; // Vari�vel para armazenar o comando informado em menu()
  int senha_atual=0; // Vari�vel de controle para o n�mero atual da ficha

  atend = configs_inicializar(); // Inicializa o atendimento
  if (!atend) { // Controle de erro
    printf("Erro ao criar o atendimento!\n");
    return 1;
  }
  do {
      op = menu(); // Chama a fun��o menu(), aramzena em op
      switch(op) {
          case 1: {
              configs_atualizar(atend,1,1);// Atualiza em "Iniciar Atendimento"
              break;
          }
          case 2: {
              configs_atualizar(atend,2,1);// Atualiza em "Pausar Atendimento"
              break;
          }
          case 3: {
              configs_atualizar(atend,3,1);// Atualiza em "Encerrar Atendimento"
              break;
          }
          case 4: {
                printf("\n--- Nova Ficha ---\n");
                ficha *nova_ficha = criar_ficha(&senha_atual); // Cria uma nova ficha
                if(nova_ficha != NULL) { // Se a ficha for criada com sucesso
                    salvar_ficha(nova_ficha); // Salva a ficha
                    free(nova_ficha); // Libera a mem�ria
                    printf("\nFicha criada com sucesso! Aguarde.\n");
                }
                sleep(1);
                break;
          }
          case 0: {
            // Caso o usu�rio informe 0
            configs_destruir(atend); // Destroi o atendimento
            DeleteFile("arquivo/configs_atend.dat"); // Deleta o arquivo de atendimento
            printf("At� a pr�xima!\n");
            break;
          }
          default: {
              printf("Op��o inv�lida!\n");
          }
      }
  } while (op != 0);
  return 0;
}

