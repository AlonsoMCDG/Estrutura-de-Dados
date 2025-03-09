#include <stdio.h>
#include <stdlib.h>


/* Interface da estrutura de pilha */

typedef struct PilhaNo
{
    float chave;
    struct PilhaNo *prox;
} PilhaNo;

typedef struct Pilha
{
    PilhaNo *topo;
} Pilha;

Pilha *pilha_cria();
Pilha *pilha_push(Pilha *p, float chave);
float pilha_pop(Pilha *p);
int pilha_vazia(Pilha *p);
void libera(Pilha *p);

void libera(Pilha *p)
{
    if (!p) return;
    while (!pilha_vazia(p))
        pilha_pop(p);
    free(p);
}

int pilha_vazia(Pilha *p)
{
    return p->topo == NULL;
}

Pilha *pilha_push(Pilha *p, float chave)
{
    PilhaNo *novo = (PilhaNo *)malloc(sizeof(PilhaNo));
    novo->chave = chave;
    novo->prox = p->topo;
    p->topo = novo;
    return p;
}

float pilha_pop(Pilha *p)
{
    float v = p->topo->chave;
    PilhaNo *topo_antigo = p->topo;
    p->topo = p->topo->prox;
    free(topo_antigo);
    return v;
}

Pilha *pilha_cria()
{
    Pilha *p = (Pilha *)malloc(sizeof(Pilha));
    p->topo = NULL;

    return p;
}

/**********/

/* FUNÇÕES */

/* Retorna uma cópia da pilha */
Pilha *pilha_copia(Pilha *p)
{
    Pilha *copia = pilha_cria();
    Pilha *aux = pilha_cria();

    // copiando para aux os elementos na ordem inversa da pilha original
    while (!pilha_vazia(p))
        pilha_push(aux, pilha_pop(p));
    
    while (!pilha_vazia(aux))
    {
        float v = pilha_pop(aux);
        pilha_push(copia, v); // inserindo os elementos na pilha cópia na ordem correta
        pilha_push(p, v); // retornando os elementos para manter a pilha original
    }

    return copia;
}

/* Retorna uma nova pilha com os elementos em ordem crescente */
Pilha *pilha_ordena(Pilha *p)
{
    if (!p)
        return pilha_cria();

    if (pilha_vazia(p))
        return p;

    Pilha *pilha_fonte = pilha_copia(p);
    Pilha *pilha_aux = pilha_cria();
    Pilha *pilha_ordenada = pilha_cria();
    float topo_ordenada;

    while (!pilha_vazia(pilha_fonte))
    {
        float v = pilha_pop(pilha_fonte);

        // abre espaço para inserir 'v' na posição correta
        while (!pilha_vazia(pilha_ordenada))
        {
            topo_ordenada = pilha_pop(pilha_ordenada);
            pilha_push(pilha_aux, topo_ordenada);

            if (topo_ordenada >= v)
            {
                pilha_push(pilha_ordenada, pilha_pop(pilha_aux)); // devolve de 'aux' para 'ordenada'
                break;
            }
        }

        // insere o topo de 'fonte' em 'ordenada'
        pilha_push(pilha_ordenada, v);

        // devolve para 'ordenada' os valores que foram retirados de lá
        while (!pilha_vazia(pilha_aux))
        {
            pilha_push(pilha_ordenada, pilha_pop(pilha_aux));
        }
    }

    return pilha_ordenada;
}

/* Imprime os elementos da pilha */
void pilha_imprime(Pilha *p)
{
    printf("Pilha:");
    if (pilha_vazia(p)) printf(" (vazia)\n");
    while (!pilha_vazia(p))
        printf(" %.2lf", pilha_pop(p));
    printf("\n");
}

int main()
{
    Pilha *p = pilha_cria();

    pilha_push(p, 20);
    pilha_push(p, 30);
    pilha_push(p, 10);
    pilha_push(p, 25);
    pilha_push(p, 50);

    pilha_imprime(pilha_copia(p));
    pilha_imprime(pilha_ordena(p));
    
    return 0;
}