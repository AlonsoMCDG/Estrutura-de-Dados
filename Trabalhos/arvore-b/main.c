#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define GRAU_MINIMO 2
#define NUMERO_MAXIMO_DE_FILHOS (2 * GRAU_MINIMO)
#define NUMERO_MAXIMO_DE_CHAVES (2 * GRAU_MINIMO - 1)

#define DEBUG_

typedef struct ArvoreB
{
    int numero_chaves;
    int numero_filhos;
    int chaves[NUMERO_MAXIMO_DE_CHAVES];
    struct ArvoreB *pai;
    struct ArvoreB *filhos[NUMERO_MAXIMO_DE_FILHOS];
    bool eh_folha;
    int id;
} ArvoreB;

ArvoreB *novo_no();
ArvoreB *inserir(ArvoreB *arv, int chave);
ArvoreB *copiar_chaves_e_filhos(ArvoreB *destino, ArvoreB *fonte, int inicio, int fim);
void liberar(ArvoreB *arv);
void imprime(ArvoreB *arv);
void dfs_imprime(ArvoreB *arv, int tabulacao, int nivel, int colunas_vazias);
int busca_binaria(int *vet, int valor, int esq, int dir);


int nos_criados = 0;

ArvoreB *novo_no()
{
    ArvoreB *arv = malloc(sizeof(ArvoreB));
    arv->numero_chaves = 0;
    arv->numero_filhos = 0;
    arv->pai = NULL;
    arv->eh_folha = true;
    arv->id = nos_criados ++;
    printf("no com id = %d, criado -> total = %d\n", arv->id, nos_criados);

    for (int i = 0; i < NUMERO_MAXIMO_DE_FILHOS; i ++)
        arv->filhos[i] = NULL;

    return arv;
}

void liberar(ArvoreB *arv)
{
    if (!arv)
        return;

    for (int i = 0; i < NUMERO_MAXIMO_DE_FILHOS; i ++)
        liberar(arv->filhos[i]);

    free(arv);
}

int busca_binaria(int *vet, int valor, int esq, int dir)
{
    if (esq > dir)
        return -1;
    
    int meio = (esq + dir) / 2;

    if (valor < vet[meio])
        return busca_binaria(vet, valor, esq, meio - 1);

    if (valor > vet[meio])
        return busca_binaria(vet, valor, meio + 1, dir);
    
    return meio;
}

int buscar_indice_de_insercao(int *vet, int valor, int esq, int dir)
{
    if (esq > dir)
        return esq;

    int meio = (esq + dir) / 2;

    if (valor < vet[meio])
        return buscar_indice_de_insercao(vet, valor, esq, meio - 1);
    
    return buscar_indice_de_insercao(vet, valor, meio + 1, dir);
}

ArvoreB *copiar_chaves_e_filhos(ArvoreB *destino, ArvoreB *fonte, int inicio, int fim)
{
    for (int i = inicio; i < fim; i ++)
    {
        int j = inicio - i;
        destino->chaves[j] = fonte->chaves[i];
        destino->numero_chaves ++;
        destino->filhos[j] = fonte->filhos[i];
        destino->filhos[j + 1] = fonte->filhos[i + 1];

        if (destino->filhos[j] || destino->filhos[j + 1])
            destino->eh_folha = false;
    }

    return destino;
}

/* Função auxiliar que insere uma chave em uma árvore-B */
ArvoreB *inserir_chave(ArvoreB *arv, int chave, ArvoreB *pai)
{
    if (!arv)
    {
        arv = novo_no();
    }

    printf("inserindo %d em no %d, ", chave, arv->id);
    if (pai) printf("pai = %d\n", pai->id);
    else printf("pai = null\n");

    /* split */    
    bool esta_cheio = (arv->numero_chaves == NUMERO_MAXIMO_DE_CHAVES);

    if (esta_cheio)
    { // fazer o split do nó quand este estiver cheio
        printf("no id = %d cheio\n", arv->id); // debug
        int chave_promovida = arv->chaves[GRAU_MINIMO - 1]; // chave a ser promovida para o nó pai

        ArvoreB *filho_esq; // filho à esquerda da chave promovida para o nó pai
        ArvoreB *filho_dir; // filho à direita da chave promovida para o nó pai

        if (pai == NULL) // o nó atual é a raiz
        {
            printf("nova raiz\n"); // debug
            // (se 'arv' é a raiz e está cheia, promove 'arv' para uma nova raiz com apenas a chave promovida)

            filho_dir = novo_no();
            filho_esq = novo_no();

            /* copia as informações relevantes para o filho esquerdo da chave promovida */
            copiar_chaves_e_filhos(filho_esq, arv, 0, GRAU_MINIMO - 1);
            
            /* copia as informações relevantes para o filho direito da chave promovida */
            copiar_chaves_e_filhos(filho_dir, arv, GRAU_MINIMO, NUMERO_MAXIMO_DE_CHAVES);

            // insere no novo nó raiz apenas a chave promovida
            arv->chaves[0] = chave_promovida;
            arv->numero_chaves = 1;

            /* insere a chave em um dos novos filhos */
            if (chave < chave_promovida)
                filho_esq = inserir_chave(filho_esq, chave, arv);
            else
                filho_dir = inserir_chave(filho_dir, chave, arv);
            
            // insere os filhos no nó pai
            arv->filhos[0] = filho_esq;
            arv->filhos[1] = filho_dir;
            arv->eh_folha = false;

            return arv;
        }

        printf("id pai = %d\n", pai->id); // debug

        filho_esq = arv;
        filho_dir = novo_no();

        // índice de inserção da chave promovida
        int index = pai->numero_chaves;

        for (int i = pai->numero_chaves - 1; i >= 0; i --)
        {
            if (chave_promovida >= pai->chaves[i])
                break;
            index --;
            pai->chaves[i + 1] = pai->chaves[i];
            pai->filhos[index + 1] = pai->filhos[i + 1];
        }

        // insere a chave na posição
        pai->chaves[index] = chave_promovida;
        pai->numero_chaves ++;

        if (chave_promovida == 12)
            printf("chave 12 inserida no indice %d do no %d\n", index, pai->id);

        // copiar as chaves e filhos para o filho da direita da chave promovida
        filho_dir = copiar_chaves_e_filhos(filho_dir, arv, GRAU_MINIMO, NUMERO_MAXIMO_DE_CHAVES);

        // o filho da esquerda do nó pai é o próprio nó 'arv' atual
        // então, todas as chaves e filhos correspondentes já estão lá
        filho_esq->numero_chaves = GRAU_MINIMO - 1;
        filho_esq->eh_folha = true;
        for (int i = 0; i < filho_esq->numero_chaves; i ++)
        {
            if (filho_esq->filhos[i] || filho_esq->filhos[i + 1])
                filho_esq->eh_folha = false;
        }

        if (chave == 17)
        {
            printf("filho aa direita da chave %d, ANTES de inserir:\n", chave_promovida);
            imprime(filho_dir);
            printf("filho aa esquerda da chave %d, ANTES de inserir:\n", chave_promovida);
            imprime(filho_esq);
        }

        printf("pai->id = %d\n", pai->id);
        printf("filho_esq->id = %d\n", filho_esq->id);
        printf("filho_dir->id = %d\n", filho_dir->id);
        printf("arv->id = %d\n", arv->id);
        printf("\n");
        

        // insere a chave em um dos novos filhos
        if (chave < chave_promovida)
            filho_esq = inserir_chave(filho_esq, chave, pai);
        else
            filho_dir = inserir_chave(filho_dir, chave, pai);

        printf("pai->id = %d\n", pai->id);
        printf("filho_esq->id = %d\n", filho_esq->id);
        printf("filho_dir->id = %d\n", filho_dir->id);
        printf("\n");
        
        if (chave == 17)
        {
            printf("filho aa direita da chave %d, DEPOIS de inserir:\n", chave_promovida);
            imprime(filho_dir);
            printf("filho aa esquerda da chave %d, DEPOIS de inserir:\n", chave_promovida);
            imprime(filho_esq);
        }
        
        printf("pai->id = %d\n", pai->id);
        pai->filhos[index] = filho_esq;
        pai->filhos[index + 1] = filho_dir;

        printf("pai(%d)[%d] = filho_esq(%d)\n", pai->id, index, filho_esq->id);
        printf("pai(%d)[%d] = filho_dir(%d)\n", pai->id, index + 1, filho_dir->id);

        pai = NULL;

        return arv;
    }

    /* inserção na folha */
    if (arv->eh_folha)
    {
        int index = arv->numero_chaves;
        for (int i = arv->numero_chaves - 1; i >= 0; i --)
        {
            if (chave >= arv->chaves[i]) // achou a posição de inserção
                break;
            index --;
            arv->chaves[i + 1] = arv->chaves[i]; // abre espaço
        }
        arv->chaves[index] = chave;
        arv->numero_chaves ++;
    }
    else
    {
        int index = buscar_indice_de_insercao(arv->chaves, chave, 0, arv->numero_chaves - 1);
        arv->filhos[index] = inserir_chave(arv->filhos[index], chave, arv);
    }
    
    return arv;
}

/* Função que deve ser chamada para inserir uma chave em uma árvore-B */
ArvoreB *inserir(ArvoreB *arv, int chave)
{
    return inserir_chave(arv, chave, NULL);

    if (!arv)
    {
        arv = novo_no();
    }

    /* variável auxiliar para fazer a promoção de uma chave quando o nó estiver cheio */
    static ArvoreB *pai = NULL;

    /* split */    
    bool esta_cheio = (arv->numero_chaves == NUMERO_MAXIMO_DE_CHAVES);

    if (esta_cheio)
    { // fazer o split do nó quand este estiver cheio
        printf("no id = %d cheio\n", arv->id); // debug
        int chave_promovida = arv->chaves[GRAU_MINIMO - 1]; // chave a ser promovida para o nó pai

        if (chave_promovida == 12)
        {
            printf("\n\n\n========\n");
            printf("chave 12 promovida para o no %d\n", pai ? pai->id : arv->id);
            printf(pai ? "pai existe\n" : "nova raiz criada\n");
            printf("========\n\n\n");
        }

        ArvoreB *filho_esq = arv; // filho à esquerda da chave promovida para o nó pai
        ArvoreB *filho_dir = novo_no(); // filho à direita da chave promovida para o nó pai

        if (pai == NULL) // o nó atual é a raiz
        {
            printf("nova raiz\n"); // debug
            // (se 'arv' é a raiz e está cheia, promove 'arv' para uma nova raiz com apenas a chave promovida)

            filho_esq = novo_no();

            /* copia as informações relevantes para o filho esquerdo da chave promovida */
            copiar_chaves_e_filhos(filho_esq, arv, 0, GRAU_MINIMO - 1);
            
            /* copia as informações relevantes para o filho direito da chave promovida */
            copiar_chaves_e_filhos(filho_dir, arv, GRAU_MINIMO, NUMERO_MAXIMO_DE_CHAVES);

            pai = arv;
            /* insere a chave em um dos novos filhos */
            if (chave < chave_promovida)
                filho_esq = inserir(filho_esq, chave);
            else
                filho_dir = inserir(filho_dir, chave);

            // insere no novo nó raiz apenas a chave promovida
            arv->chaves[0] = chave_promovida;
            arv->numero_chaves = 1;
            arv->filhos[0] = filho_esq;
            arv->filhos[1] = filho_dir;
            arv->eh_folha = false;

            pai = NULL;

            return arv;
        }

        printf("id pai = %d\n", pai->id); // debug

        // índice de inserção da chave promovida
        int index = pai->numero_chaves;

        for (int i = pai->numero_chaves - 1; i >= 0; i --)
        {
            if (chave_promovida >= pai->chaves[i])
                break;
            index --;
            pai->chaves[i + 1] = pai->chaves[i];
            pai->filhos[index + 1] = pai->filhos[i + 1];
        }

        // insere a chave na posição
        pai->chaves[index] = chave_promovida;
        pai->numero_chaves ++;

        if (chave_promovida == 12)
        printf("chave 12 inserida no indice %d do no %d\n", index, pai->id);

        // copiar as chaves e filhos para o filho da direita da chave promovida
        filho_dir = copiar_chaves_e_filhos(filho_dir, arv, GRAU_MINIMO, NUMERO_MAXIMO_DE_CHAVES);

        // o filho da esquerda do nó pai é o próprio nó 'arv' atual
        // então, todas as chaves e filhos correspondentes já estão lá
        filho_esq->numero_chaves = GRAU_MINIMO - 1;
        filho_esq->eh_folha = true;
        for (int i = 0; i < filho_esq->numero_chaves; i ++)
        {
            if (filho_esq->filhos[i] || filho_esq->filhos[i + 1])
                filho_esq->eh_folha = false;
        }

        if (chave == 17)
        {
            printf("filho aa direita da chave %d, ANTES de inserir:\n", chave_promovida);
            imprime(filho_dir);
            printf("filho aa esquerda da chave %d, ANTES de inserir:\n", chave_promovida);
            imprime(filho_esq);
        }

        printf("pai->id = %d\n", pai->id);
        printf("filho_esq->id = %d\n", filho_esq->id);
        printf("filho_dir->id = %d\n", filho_dir->id);
        printf("\n");
        

        // insere a chave em um dos novos filhos
        if (chave < chave_promovida)
            filho_esq = inserir(filho_esq, chave);
        else
            filho_dir = inserir(filho_dir, chave);

        printf("pai->id = %d\n", pai->id);
        printf("filho_esq->id = %d\n", filho_esq->id);
        printf("filho_dir->id = %d\n", filho_dir->id);
        printf("\n");
        
        if (chave == 17)
        {
            printf("filho aa direita da chave %d, DEPOIS de inserir:\n", chave_promovida);
            imprime(filho_dir);
            printf("filho aa esquerda da chave %d, DEPOIS de inserir:\n", chave_promovida);
            imprime(filho_esq);
        }
        
        printf("pai->id = %d\n", pai->id);
        pai->filhos[index] = filho_esq;
        pai->filhos[index + 1] = filho_dir;

        printf("pai(%d)[%d] = filho_esq(%d)\n", pai->id, index, filho_esq->id);
        printf("pai(%d)[%d] = filho_dir(%d)\n", pai->id, index + 1, filho_dir->id);

        pai = NULL;

        return arv;
    }

    /* inserção na folha */
    if (arv->eh_folha)
    {
        int index = arv->numero_chaves;
        for (int i = arv->numero_chaves - 1; i >= 0; i --)
        {
            if (chave >= arv->chaves[i]) // achou a posição de inserção
                break;
            index --;
            arv->chaves[i + 1] = arv->chaves[i]; // abre espaço
        }
        arv->chaves[index] = chave;
        arv->numero_chaves ++;
    }
    else
    {
        int index = buscar_indice_de_insercao(arv->chaves, chave, 0, arv->numero_chaves - 1);
        pai = arv;
        arv->filhos[index] = inserir(arv->filhos[index], chave);
    }
    
    return arv;
}

void dfs_imprime(ArvoreB *arv, int tabulacao, int nivel, int colunas_vazias)
{
    /* tabulação para a impressão */
    char tab[41] = "                                        ";

    /* formatando a tabulação */
    for (int i = 0; i < nivel; i ++)
    {
        tab[2 * i] = 186; // 186: ║
        tab[2 * i + 1] = ' ';
    }

    tab[2 * (nivel - 1)] = 204; // 204: ╠
    tab[tabulacao - 1] = 205; // 205: ═
    tab[tabulacao] = '\0';
    /****/

    if (!arv)
    {
        // printf("%s(null)\n", tab);
        return;
    }

    // printf("%sno id = %d com %d chaves\n", tab, arv->id, arv->numero_chaves);

    /* impressão das chaves */
    for (int i = 0; i < arv->numero_chaves; i ++)
    {
        // se é a última chave, dfs_imprime um caractere diferente
        if (i == arv->numero_chaves - 1)
            tab[2 * (nivel - 1)] = 200; // tab[ultimo] = 200: ╚

        /* imprime a tabulação e informações do nó */
        printf("%s%d", tab, arv->chaves[i]);
        printf(" (lvl %d, i = %d, last = %d, folha = %d, node_id = %d)", nivel, i, i == arv->numero_chaves - 1, arv->eh_folha, arv->id);
        printf("\n");


        /* chamada recursiva para imprimir os filhos deste nó */

        if (i == arv->numero_chaves - 1) // é o último nó do galho
            colunas_vazias ++;

        if (i == 0)
            dfs_imprime(arv->filhos[i], tabulacao + 2, nivel + 1, colunas_vazias);

        dfs_imprime(arv->filhos[i + 1], tabulacao + 2, nivel + 1, colunas_vazias); // filho à direita da chave
    }
}

void imprime(ArvoreB *arv)
{
    printf("arvore:\n");
    dfs_imprime(arv, 0, 0, 0);
    printf("\n");
}

void testar_arvore(ArvoreB *arv)
{
    /* teste 2 */
    //*
    imprime(arv);
    printf("inserindo 1, 5 e 6\n");
    inserir(arv, 1);
    inserir(arv, 5);
    inserir(arv, 6);
    imprime(arv);

    printf("inserindo 7\n");
    inserir(arv, 7);
    imprime(arv);
    
    printf("inserindo 9\n");
    inserir(arv, 9);
    imprime(arv);
    
    printf("inserindo 10\n");
    inserir(arv, 10);
    imprime(arv);
    
    printf("inserindo 11\n");
    inserir(arv, 11);
    imprime(arv);
    
    printf("inserindo 12\n");
    inserir(arv, 12);
    imprime(arv);
    
    printf("inserindo 13\n");
    inserir(arv, 13);
    imprime(arv);

    printf("inserindo 14\n");
    inserir(arv, 14);
    imprime(arv);

    printf("inserindo 16\n");
    inserir(arv, 16);
    imprime(arv);
    
    printf("inserindo 15\n");
    inserir(arv, 15);
    imprime(arv);
    
    printf("inserindo 17\n");
    inserir(arv, 17);
    imprime(arv);
    
    printf("inserindo 20\n");
    inserir(arv, 20);
    imprime(arv);

    printf("inserindo 18\n");
    inserir(arv, 18);
    imprime(arv);
    
    printf("inserindo 19, 20\n");
    inserir(arv, 19);
    inserir(arv, 30);
    imprime(arv);
    
    printf("inserindo 40\n");
    inserir(arv, 40);
    imprime(arv);
    
    printf("fim programa\n");
    //*/

    /* teste 1 */
    /*
    inserir(arv, 10);
    inserir(arv, 30);
    inserir(arv, 20);
    imprime(arv);

    inserir(arv, 15);
    imprime(arv);

    inserir(arv, 35);
    inserir(arv, 55);
    imprime(arv);

    inserir(arv, 70);
    imprime(arv);

    inserir(arv, 85);
    imprime(arv);

    inserir(arv, 90);
    imprime(arv);

    inserir(arv, 125);
    imprime(arv);
    // */

    printf("fim programa\n");
}

int main()
{
    ArvoreB *arv = novo_no();

    testar_arvore(arv);
    liberar(arv);
    
    return 0;
}
