#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <windows.h>
#include <time.h>
#include <locale.h>
#include "fila_configs.h"

void chamar_ficha(filaEspera *fila){
    if(fila->primeiro == NULL) { // Verifica se a fila est� vazia!
        return;
    }

    ficha *ficha_removida = remover_fila(fila); // Remove o primeiro elemento

    if(ficha_removida != NULL){ // Se existir ficha removida, informa os dados
        printf("\n - Chamando para Atendimento - \n");
        printf("Senha: %d \n", ficha_removida->senha);
        printf("Prioridade: %d \n", ficha_removida->prioridade);
        printf("Nome: %s \n", ficha_removida->nome);
        printf("Tempo de atendimento: %d \n", ficha_removida->tempo);
        sleep(ficha_removida->tempo); // Espera o tempo de atendimento da ficha
    }
    free(ficha_removida); // Libera a mem�ria
    return;
}
int main() {

    setlocale(LC_ALL, "Portuguese");

    int senha_atual = 0; // Vari�vel para controlar o n�mero atual da ficha
    int status_anterior = 0; // Vari�vel para n�o ficar repetindo os printf()'s
    int atendendo = 0; // Vari�vel para n�o ficar repetindo os printf()'s

    atendConfigs *atend; // Cria um atendimento
    filaEspera *fila = criar_fila(); // Cria uma fila
    atend = configs_inicializar(); // Inicializa o atendimento

    if (!atend) { // Controle de erro
      printf("\nErro ao criar o atendimento\n");
      return 1;
    }

    configs_ler(atend); // Le as informa��es padr�o do arquivo
    configs_mostrar(atend); // Mostra as nforma��es iniciais

    while(atend->status != 3) {
        sleep(1);
        atualizar_fila(fila, &senha_atual); // Atualiza a fila (mesmo se n�o estivar atendendo)
        if(atend->status == 1) { // Se estiver atendendo
            if(status_anterior != 1){
                printf("\nAtendimento iniciado! \n");
            }
            if(fila->primeiro != NULL) { // Se a fila n�o estivar vazia, chama a primeira ficha
                chamar_ficha(fila);
                atendendo = 1;
            } else {
                if(atendendo == 1) { // Se a fila acabou de ficar vazia, informa:
                    printf("\nNenhuma ficha na fila. Aguardando...\n");
                    atendendo = 0;
                }
            }
        } else if(atend->status == 2) { // Se o atendimento estiver em pausa
            if(status_anterior != 2){
                printf("\nEstamos em intervalo. Por favor, aguarde. \n");
            }
        }
        status_anterior = atend->status; // Salva o status anterior na vari�vel de controle
        configs_ler(atend); // Le as novas informa��es no arquivo
    }

    // Se o aendimento for encerrado:

    destruir_fila(fila); // Destroi a fila
    configs_destruir(atend); // Destroi o atendimento
    DeleteFile("arquivo/configs_fila.dat"); // Deleta o arquivo de fila
    printf("\nAtendimento Encerrado. At� a pr�xima!\n");
    sleep(3);
    return 0;
}
