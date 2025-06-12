#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <windows.h>
#include <time.h>
#include <locale.h>
#include "fila_configs.h"

void chamar_ficha(filaEspera *fila){
    if(fila->primeiro == NULL) { // Verifica se a fila está vazia!
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
    free(ficha_removida); // Libera a memória
    return;
}
int main() {

    setlocale(LC_ALL, "Portuguese");

    int senha_atual = 0; // Variável para controlar o número atual da ficha
    int status_anterior = 0; // Variável para não ficar repetindo os printf()'s
    int atendendo = 0; // Variável para não ficar repetindo os printf()'s

    atendConfigs *atend; // Cria um atendimento
    filaEspera *fila = criar_fila(); // Cria uma fila
    atend = configs_inicializar(); // Inicializa o atendimento

    if (!atend) { // Controle de erro
      printf("\nErro ao criar o atendimento\n");
      return 1;
    }

    configs_ler(atend); // Le as informações padrão do arquivo
    configs_mostrar(atend); // Mostra as nformações iniciais

    while(atend->status != 3) {
        sleep(1);
        atualizar_fila(fila, &senha_atual); // Atualiza a fila (mesmo se não estivar atendendo)
        if(atend->status == 1) { // Se estiver atendendo
            if(status_anterior != 1){
                printf("\nAtendimento iniciado! \n");
            }
            if(fila->primeiro != NULL) { // Se a fila não estivar vazia, chama a primeira ficha
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
        status_anterior = atend->status; // Salva o status anterior na variável de controle
        configs_ler(atend); // Le as novas informações no arquivo
    }

    // Se o aendimento for encerrado:

    destruir_fila(fila); // Destroi a fila
    configs_destruir(atend); // Destroi o atendimento
    DeleteFile("arquivo/configs_fila.dat"); // Deleta o arquivo de fila
    printf("\nAtendimento Encerrado. Até a próxima!\n");
    sleep(3);
    return 0;
}
