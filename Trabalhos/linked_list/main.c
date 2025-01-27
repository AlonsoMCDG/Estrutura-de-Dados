/* 
 * Universidade Federal do Estado do Acre - UFAC
 * Centro de Ciências Exatas e Tecnológicas - CCET
 * Curso: Bacharelado em Sistemas de Informação
 * Disciplina: Estrutura de Dados
 * Docente: Claudionor Alencar
 * Discente: Alonso Martins de Araújo
 * Data: 13/01/2025
 * 
 *   ======== Estudo sobre Listas Encadeadas ========  
 * 
 * ATIVIDADE: Implementar a estrutura de lista
 * encadeada utilizando structs. Cada nó da lista
 * deve guardar dois números inteiros, sendo um deles
 * o identificador (id) do nó e o outro um valor.
 * Utilize ponteiros e 'malloc()' para alocar memória
 * para cada nó novo. (IMPORTANTE: Não esqueça de 
 * liberar a memória após o término do programa).
 * 
 * Adicione funções para:
 *  - criar um nó após o usuário informar um id e um
 *    valor
 *  - inserir um elemento no início da lista
 *  - inserir um elemento no fim da lista
 *  - imprimir a lista
 *  - procurar por um elemento na lista e, no caso
 *    de não encotrá-lo, retornar um valor nulo (NULL)
 *  - calcular a soma dos valores armazenados na lista
 *  - calcular a média dos valores armazenados na lista
 *  - calcular o tamanho da lista
 *  - saber se a lista está vazia
 *  - retornar o elemento guardado no meio da lista (se
 *    há uma quantidade par de elementos, retorne
 *    qualquer um dos elementos do meio)
 * 
 * Adicione outras funções que julgar necessário.
 * 
 * Na função principal crie um exemplo que utilize as
 * funções criadas. Imprima cada passo da execuçao do
 * programa.
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <locale.h>

#define avaliar(booleano) ((booleano) ? "True" : "False")

// Estrutura do nó de uma lista encadeada
typedef struct Node
{
    int id;
    int value;
    struct Node *next;
} Node;

// Estrutura que guarda a referência para o primeiro
// elemento de uma lista encadeada
typedef struct LinkedList
{
    struct Node *head;
    struct Node *tail;
} LinkedList;

LinkedList *new_linked_list();
LinkedList *invertida(LinkedList *);
Node *new_node(int, int);
Node *buscar(int, LinkedList *);
Node *elemento_no_meio(LinkedList *);
Node *elemento_no_fim(LinkedList *);
bool lista_vazia(LinkedList *);
bool contem_id(int, LinkedList *);
bool contem_valor(int, LinkedList *);
int frequencia(int, LinkedList *);
int menor_valor(LinkedList *);
int maior_valor(LinkedList *);
int size(LinkedList *);
int soma(LinkedList *);
double media(LinkedList *);
void imprime_lista(LinkedList *);
void imprime_node(Node *);
void insere_frente(Node *, LinkedList *);
void insere_fim(Node *, LinkedList *);
void liberar_lista(LinkedList *);

int main()
{
    printf("--- Inicio do programa\n");

    // ----- Declração da lista -----

    printf("\n--- Criando nova lista encadeada 'lista1'\n");
    LinkedList *lista1 = new_linked_list();
    imprime_lista(lista1);

    // ----- Populando a lista -----

    printf("\n========== POPULANDO A LISTA ==========\n");
    printf("\n--- Inserindo 2 elementos na frente da lista 'lista1'\n");
    for (int i = 0; i < 2; i ++)
        insere_frente(new_node(i, 2 * i), lista1);
    imprime_lista(lista1);

    printf("\n--- Inserindo 3 elementos no fim da lista 'lista1'\n");
    for (int i = 3; i < 6; i ++)
        insere_fim(new_node(i, 2 * i), lista1);
    imprime_lista(lista1);

    // ----- Funções de busca -----
    printf("\n========== BUSCANDO ELEMENTOS ==========\n");

    int id_busca;
    Node *node_busca;

    // Busca o nó com o id
    id_busca = 3;
    printf("\n--- Procurando o elemento com id %d na lista\n", id_busca);
    node_busca = buscar(id_busca, lista1);
    if (node_busca != NULL)
        printf("Encontrou: [id=%d, value=%d]\n", node_busca->id, node_busca->value);
    else
        printf("Elemento com id %d nao esta na lista\n", id_busca);

    id_busca = 10;
    printf("\n--- Procurando o elemento com id %d na lista\n", id_busca);
    node_busca = buscar(id_busca, lista1);
    if (node_busca != NULL)
        printf("Encontrou: [id=%d, value=%d]\n", node_busca->id, node_busca->value);
    else
        printf("Elemento com id %d nao esta na lista\n", id_busca);

    // A lista contém o valor x
    int valor_busca;

    valor_busca = 10;
    printf("\n--- Conferindo se a lista possui o valor %d\n", valor_busca);
    if (contem_valor(valor_busca, lista1))
        printf("A lista contem o valor %d\n", valor_busca);
    else
        printf("O valor %d nao esta na lista\n", valor_busca);

    valor_busca = 30;
    printf("\n--- Conferindo se a lista possui o valor %d\n", valor_busca);
    if (contem_valor(valor_busca, lista1))
        printf("A lista contem o valor %d\n", valor_busca);
    else
        printf("O valor %d nao esta na lista\n", valor_busca);

    // A lista contém o id x
    id_busca = 3;
    printf("\n--- Checando se a lista possui elemento com id %d\n", id_busca);
    if (contem_id(id_busca, lista1))
        printf("A lista contem o id %d\n", id_busca);
    else
        printf("A lista nao contem o id %d\n", id_busca);

    id_busca = 7;
    printf("\n--- Checando se a lista possui elemento com id %d\n", id_busca);
    if (contem_id(id_busca, lista1))
        printf("A lista contem o id %d\n", id_busca);
    else
        printf("A lista nao contem o id %d\n", id_busca);
    
    // Elemento do meio
    printf("\n--- Buscando o elemento no meio da lista:\n");
    imprime_lista(lista1);
    Node *meio = elemento_no_meio(lista1);
    printf("Elemento no meio: ");
    if (meio) printf("[id=%d, val=%d]\n", meio->id, meio->value);
    else printf("(null)\n");

    // Elemento no fim
    printf("\n--- Buscando o ultimo elemento da lista:\n");
    imprime_lista(lista1);
    Node *ultimo = elemento_no_fim(lista1);
    printf("Ultimo elemento: ");
    if (ultimo) printf("[id=%d, val=%d]\n", ultimo->id, ultimo->value);
    else printf("(null)\n");

    // ----- Operações -----
    printf("\n========== OPERACOES NUMERICAS ==========\n");

    printf("\n--- Calculando o tamanho da lista\n");
    int tam = size(lista1);
    printf("A lista possui %d elementos\n", tam);
    
    printf("\n--- Somando os elementos da lista\n");
    int sum = soma(lista1);
    printf("Soma = %d\n", sum);

    printf("\n--- Calculando a media\n");
    double m = media(lista1);
    printf("Media = %.2lf\n", m);

    // ----- Encerramento do programa -----
    printf("\n========== ENCERRANDO O PROGRAMA ==========\n");

    printf("\n--- Liberando a memoria alocada. . .\n");
    liberar_lista(lista1);
    printf("--- Memoria liberada\n");
    printf("--- Programa encerrado.\n");

    return 0;
}

// Cria uma nova lista encadeada vazia
LinkedList *new_linked_list()
{
    LinkedList *llist = (LinkedList *)malloc(sizeof(LinkedList));

    llist->head = NULL;
    llist->tail = NULL;

    return llist;
}

// Retorna uma nova lista com os elementos na ordem inversa
LinkedList *invertida(LinkedList *llist)
{
    LinkedList *llist_inversa = new_linked_list();
    Node *atual = llist->head;

    while (atual)
    {
        insere_frente(atual, llist_inversa);
        atual = atual->next;
    }

    return llist_inversa;
}

// Cria um novo nó
Node *new_node(int id, int value)
{
    Node *node = (Node *)malloc(sizeof(Node));

    node->id = id;
    node->value = value;
    node->next = NULL;

    return node;
}

// Retorna o elemento com o id especificado
Node *buscar(int id, LinkedList *llist)
{
    Node *atual = llist->head;

    while (atual)
    {
        if (atual->id == id)
            return atual;

        atual = atual->next;
    }

    return NULL;
}

// Retorna o elemento que está no meio da lista
Node *elemento_no_meio(LinkedList *llist)
{
    Node *aux_slow = llist->head; // tem passo 1 na iteração da lista
    Node *aux_fast = llist->head; // tem passo 2 na iteração da lista

    while (aux_fast && aux_fast->next)
    {
        aux_slow = aux_slow->next;       // +1
        aux_fast = aux_fast->next->next; // +2
    }

    return aux_slow;
}

// Retorna o elemento no fim da lista
Node *elemento_no_fim(LinkedList *llist)
{
    Node *atual = llist->head, *fim = atual;
    while (atual)
    {
        fim = atual;
        atual = atual->next;
    }
    return fim;

    // // Outra solução:
    // return llist->tail;
}

// Retorna verdadeiro se a lista está vazia
bool lista_vazia(LinkedList *llist)
{
    return llist->head == NULL;
}

// Retorna verdadeiro se há na lista um elemento com o mesmo id
bool contem_id(int id_node, LinkedList *llist)
{
    Node *atual = llist->head;

    while (atual)
    {
        if (atual->id == id_node)
            return true;

        atual = atual->next;
    }

    return false;
}

// Retorna verdadeiro se o valor está guardado na lista
bool contem_valor(int value, LinkedList *llist)
{
    Node *atual = llist->head;

    while (atual)
    {
        if (atual->value == value)
            return true;

        atual = atual->next;
    }

    return false;
}

// Conta a frequência do valor na lista
int frequencia(int value, LinkedList *llist)
{
    Node *atual = llist->head;
    int freq = 0;

    while (atual)
    {
        if (atual->value == value)
            freq ++;

        atual = atual->next;
    }

    return freq;
}

// Retorna o menor valor guardado na lista
int menor_valor(LinkedList *llist)
{
    if (lista_vazia(llist))
        return 0;

    Node *atual = llist->head;
    int menor = atual->value;

    while (atual)
    {
        if (atual->value < menor)
            menor = atual->value;

        atual = atual->next;
    }

    return menor;
}

// Retorna o maior valor guardado na lista
int maior_valor(LinkedList *llist)
{
    if (lista_vazia(llist))
        return 0;

    Node *atual = llist->head;
    int maior = atual->value;

    while (atual)
    {
        if (atual->value > maior)
            maior = atual->value;

        atual = atual->next;
    }

    return maior;
}

// Retorna a quantidade de elementos na lista
int size(LinkedList *llist)
{
    Node *atual = llist->head;
    int size = 0;

    while (atual)
    {
        size ++;
        atual = atual->next;
    }

    return size;
}

// Retorna a soma dos valores armazenados na lista
int soma(LinkedList *llist)
{
    Node *atual = llist->head;
    int soma = 0;

    while (atual)
    {
        soma += atual->value;
        atual = atual->next;
    }

    return soma;
}

// Retorna a média dos valores armazenados na lista
double media(LinkedList *llist)
{
    if (lista_vazia(llist))
        return 0;

    double sum = 0, tam = 0;
    Node *atual = llist->head;

    while (atual)
    {
        sum += atual->value;
        tam ++;
        atual = atual->next;
    }

    return sum / tam;

    // // Outra solução (menos eficiente):
    // return (double) soma(llist) / size(llist);
}

// Imprime os elementos da lista
void imprime_lista(LinkedList *llist)
{
    printf("Lista: ");
    Node *atual = llist->head;

    if (!atual)
    {
        printf("(vazia)\n");
        return;
    }

    while (atual)
    {
        // printf("id: %d, valor: %.3lf\n", atual->id, atual->value);
        imprime_node(atual);
        printf(" -> ");
        atual = atual->next;
    }
    printf("(null)\n");
}

// Imprime os valores dos atributos do node
void imprime_node(Node *node)
{
    printf("[id=%d, val=%d]", node->id, node->value);
}

// Insere um nó na frente da lista
void insere_frente(Node *node, LinkedList *llist)
{
    if (!node || !llist)
        return;

    node->next = llist->head;
    if (lista_vazia(llist))
        llist->tail = node;
    llist->head = node;

    printf("Elemento [id=%d, value=%d] inserido na frente da lista\n", node->id, node->value);
}

// Insere um nó no fim da lista
void insere_fim(Node *node, LinkedList *llist)
{
    if (!node || !llist)
        return;


    if (lista_vazia(llist))
        llist->head = node;
    else
        llist->tail->next = node;    
    
    llist->tail = node;

    // // Outro método:
    // Node *atual = llist->head;
    // while (atual->next)
    //     atual = atual->next;
    // atual->next = node;

    printf("Elemento [id=%d, value=%d] inserido no fim da lista\n", node->id, node->value);
}

// Libera a memória utilizada pela lista
void liberar_lista(LinkedList *llist)
{
    Node *atual = llist->head;

    while (atual)
    {
        Node *temp = atual->next;
        free(atual);
        atual = temp;
    }

    free(llist);
}

