#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <windows.h>
#include <time.h>
#include "fila_configs.h"

// Função para inicializar uma árvore ABB
abbPrior *criar_arvore() {
    abbPrior *abb = (abbPrior*)malloc(sizeof(abbPrior));
    abb->raiz = NULL;
    return abb;
}

// Função para criar um novo nó de prioridade
// Essa função só vai ser chamada quando uma ficha com a nova prioridade
// for solicitada, portanto podemos já criar a fila do nó

noPrioridade *criar_no(ficha *nova_ficha) {
    if(nova_ficha == NULL){
        printf("Ficha inválida!");
        return NULL;
    }

    // Cria o nó da prioridade
    noPrioridade *novoNo = (noPrioridade*)malloc(sizeof(noPrioridade));
     if(novoNo == NULL){
        printf("Erro ao criar a prioridade!");
        return NULL;
    }

    // Cria a fila de espera da prioridade
    filaEspera *fila = (filaEspera *) malloc(sizeof(filaEspera));
    if(fila == NULL){
        printf("Erro ao criar a fila!");
        return NULL;
    }

    fila->primeiro = nova_ficha;
    fila->ultimo = nova_ficha;

    novoNo->fila = fila;
    novoNo->chave = nova_ficha->prioridade;
    novoNo->esquerdo = novoNo->direito = novoNo->pai = NULL;

    return novoNo;
}

// Função para inserir novo nó de prioridade da árvore
void inserir_no(abbPrior *abb, noPrioridade *novo_no) {

    if(abb == NULL || novo_no == NULL){
        printf("Parâmetros inválidos!");
        return;
    }
    
    // Verifica se a árvore está vazia, se sim, o novo nó é a raiz;
    if(abb->raiz == NULL){
        abb->raiz = novo_no;
        return;
    } 

    // Se a árvore não estiver vazia:
    // Começa a verificação pela raiz
    noPrioridade *no_atual = abb->raiz;

    do{
        // Se a chave do novo nó for maior que a chave do nó atual
        if(novo_no->chave>no_atual->chave){

            // Se encontrou o final da ramificação, insere o nó
            if(no_atual->direito == NULL){
                novo_no->pai = no_atual; // o pai do novo nó se torna o atual
                no_atual->direito = novo_no; // atual direito aponta para o novo nó
            } else {
                // Se não, como o novo nó é maior, seguimos para a direita
                no_atual = no_atual->direito;
            }

            // Se a chave do novo nó for menor que a chave do nó atual
        } else if(novo_no->chave<no_atual->chave){

            // Se encontrou o final da ramificação, insere o nó
            if(no_atual->esquerdo == NULL){
                novo_no->pai = no_atual; // o pai do novo nó se torna o atual
                no_atual->esquerdo = novo_no; // atual esquerdo aponta para o novo nó           
            } else {
                // Se não, como o novo nó é menor, seguimos para a esquerda
                no_atual = no_atual->esquerdo;
            }
        } 
    }while(novo_no->pai == NULL);

}

void removerNoFolha(abbPrior *abb, noPrioridade *no) {
    if (no == NULL) return;
    // Se o nó for a raiz da árvore
    if (no->pai == NULL) {
        abb->raiz = NULL;
    }
    // Se for filho à esquerda do pai
    else if (no->pai->esquerdo == no) {
        no->pai->esquerdo = NULL;
    }
    // Se for filho à direita do pai
    else if (no->pai->direito == no) {
        no->pai->direito = NULL;
    }
    free(no);
}

void removerNoComUmFilho(abbPrior *abb, noPrioridade *no) {
    if (no == NULL) return;
    // Identifica o filho (o que não for NULL)
    noPrioridade* filho = (no->esquerdo != NULL) ? no->esquerdo : no->direito;
    // Atualiza o ponteiro do pai do nó
    if (no->pai == NULL) {
    // O nó é a raiz
        abb->raiz = filho;
    } else if (no->pai->esquerdo == no) {
        no->pai->esquerdo = filho;
    } else if (no->pai->direito == no) {
        no->pai->direito = filho;
    }
    // Atualiza o ponteiro do filho para o novo pai
    filho->pai = no->pai;
    // Libera o nó original
    free(no);
}

// Função para encontrar o nó mínimo (mais à esquerda) 
noPrioridade *minimo(noPrioridade *node) {
    if(node == NULL){
        return NULL;
    }

    noPrioridade* no_atual = node;
    while(no_atual->esquerdo != NULL){
        no_atual = no_atual->esquerdo;
    }
    return no_atual;
}

// Função para encontrar o sucessor de um nó na BST
noPrioridade *sucessor(noPrioridade *atual) {
    // Caso 1: Se o nó tem um filho à direita,
    // o sucessor é o menor nó na subárvore direita
    if (atual->direito != NULL)
        return minimo(atual->direito);

    // Caso 2: Se o nó não tem filho à direita, subir na árvore usando o pai
    noPrioridade* ancestral = atual->pai;
    // Enquanto o atual for igual ao filho da direita do pai,
    // significa que ele é o filho da direita
    while (ancestral != NULL && atual == ancestral->direito) {
        atual = ancestral; // subindo na árvore
        ancestral = ancestral->pai; // subindo na árvore
    }

    return ancestral;
}

void removerNoComDoisFilhos(abbPrior* abb, noPrioridade* no) {
    if (no == NULL) return;
    // 1. Encontra o sucessor
    noPrioridade* suc = sucessor(no); // Deve retornar o menor da subárvore direita
    // 2. Copia o valor do sucessor para o nó atual
    no->chave = suc->chave;
    // 3. Remove o sucessor (que terá no máximo um filho)
    if (suc->esquerdo == NULL && suc->direito == NULL)
        removerNoFolha(abb, suc);
    else
        removerNoComUmFilho(abb, suc);
}

// Função para remover o nó que estiver com fila vazia
void remover_no(abbPrior *abb, noPrioridade *no){
  
    if (no == NULL) return;

    if (no->esquerdo == NULL && no->direito == NULL) {
        // Caso 1: nó folha
        removerNoFolha(abb, no);
    }
    else if (no->esquerdo == NULL || no->direito == NULL) {
        // Caso 2: nó com um único filho
        removerNoComUmFilho(abb, no);
    }
    else {
        // Caso 3: nó com dois filhos
        removerNoComDoisFilhos(abb, no);
    }
}


// Função para buscar um nó na árvore
noPrioridade *buscar_no(abbPrior *abb, int chave){

    noPrioridade* no_atual = abb->raiz;
    
    while(no_atual != NULL){
        if(no_atual->chave == chave){
            break;
        } else if(chave<no_atual->chave){
            no_atual = no_atual->esquerdo;
        } else {
            no_atual = no_atual->direito;
        }
    }

    return no_atual; // Se existir, retorna o nó, se não retorna NULL
}

// Nova ficha já foi verificada a prioridade no programa TV.C,
// Chamou a função atualizar árvore.
void atualizar_arvore(abbPrior *abb, ficha *nova_ficha, int *senha_atual){
    if(nova_ficha == NULL){
        printf("\nFicha inválida!\n");
        return;
    }

    if(abb == NULL){
        printf("\nÁrvore binária inválida!\n");
        return;
    }

    if(nova_ficha->senha == *senha_atual){
        free(nova_ficha);
        return;
    } 

    noPrioridade *no_insercao = buscar_no(abb, nova_ficha->prioridade);

    // Se o nó não foi encontrado na árvore
    if(no_insercao == NULL){
        // Cria um novo nó para a prioridade
        no_insercao = criar_no(nova_ficha); 
        // Insere o nó na árvore
        if(no_insercao == NULL){
            printf("\nErro ao criar a prioridade!");
            return;
        } else {
            inserir_no(abb, no_insercao);
        }
       
    } else {
        // Se o nó foi encontrado na árvore
        // Insere a ficha no final da fila
        no_insercao->fila->ultimo->proximo = nova_ficha;
        no_insercao->fila->ultimo = nova_ficha;
    }
    (*senha_atual)++;
}

// Função recursiva para destruir todos os nós da árvore
void destruir_no(noPrioridade *no_atual) {

    if (no_atual != NULL) {
        destruir_no(no_atual->esquerdo);
        destruir_no(no_atual->direito);
           
        if(no_atual->pai != NULL){
            if(no_atual->pai->esquerdo == no_atual){
                no_atual->pai->esquerdo = NULL;
            } else {
                no_atual->pai->direito = NULL;
            }
        }

        no_atual = NULL;
        free(no_atual);
    }
}

// Função para destruir a árvore
void destruir_arvore(abbPrior *abb) {
    if (abb == NULL) return;
    destruir_no(abb->raiz);
    abb = NULL;
    free(abb); 
}
