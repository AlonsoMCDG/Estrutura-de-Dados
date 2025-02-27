#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define GRAU_MINIMO 2
#define NUMERO_MAXIMO_DE_FILHOS (2 * GRAU_MINIMO)
#define NUMERO_MAXIMO_DE_CHAVES (2 * GRAU_MINIMO - 1)

typedef struct ArvoreB
{
    int numero_chaves_armazenadas;
    int chaves[NUMERO_MAXIMO_DE_CHAVES];
    struct ArvoreB *pai;
    struct ArvoreB *filhos[NUMERO_MAXIMO_DE_FILHOS];
    bool eh_folha;
} ArvoreB;

int busca_binaria(int *vet, int valor, int esq, int dir);
ArvoreB *novo_no();
ArvoreB *inserir(ArvoreB *arv, int chave);
void liberar(ArvoreB *arv);
void imprime(ArvoreB *arv, int tabulacao, int nivel);

ArvoreB *novo_no()
{
    ArvoreB *arv = malloc(sizeof(ArvoreB));
    arv->numero_chaves_armazenadas = 0;
    arv->pai = NULL;
    arv->eh_folha = true;

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

ArvoreB *inserir(ArvoreB *arv, int chave)
{
    if (!arv)
        arv = novo_no();
    
    /* variável auxiliar para fazer a promoção de uma chave quando o nó estiver cheio */
    static ArvoreB *pai = NULL;

    printf("inserindo %d\n", chave);
    
    /* confere se o nó armazena o limite de chaves */
    if (arv->numero_chaves_armazenadas == NUMERO_MAXIMO_DE_CHAVES)
    {
        /* promover a chave central para o nível acima caso o nó esteja cheio */
        
        ArvoreB *filho_esq = novo_no(); // filho esquerdo do nó
        ArvoreB *filho_dir = novo_no(); // filho direito do nó

        /* copiar para o nó da esquerda as chaves menores que a chave promovida, e seus filhos */
        for (int i = 0; i < GRAU_MINIMO - 1; i ++)
        {
            filho_esq->chaves[i] = arv->chaves[i];
            filho_esq->filhos[i] = arv->filhos[i];
            filho_esq->numero_chaves_armazenadas ++;
        }
        // copia o último filho
        filho_esq->filhos[GRAU_MINIMO - 1] = arv->filhos[GRAU_MINIMO - 1];

        /* copiar para o nó da direita as chaves maiores que a chave promovida, e seus filhos */
        for (int i = GRAU_MINIMO; i < NUMERO_MAXIMO_DE_CHAVES; i ++)
        {
            filho_dir->chaves[i - GRAU_MINIMO] = arv->chaves[i];
            filho_dir->filhos[i - GRAU_MINIMO] = arv->filhos[i];
            filho_dir->numero_chaves_armazenadas ++;
        }
        // copia o último filho
        filho_dir->filhos[GRAU_MINIMO - 1] = arv->filhos[NUMERO_MAXIMO_DE_FILHOS - 1];

        // a chave no meio do nó será promovida para o nó superior
        int chave_promovida = arv->chaves[GRAU_MINIMO - 1];

        /* promover a chave para o nó pai */
        if (pai)
        {
            // insere a chave promovida no nó pai
            pai->eh_folha = true; // para usar o mesmo algoritmo de inserção em folha
            inserir(pai, chave_promovida);
            pai->eh_folha = false; // desfaz a ação temporária
            
            // indice onde a chave foi inserida no nó pai
            int indice_inserido = busca_binaria(pai->chaves, chave_promovida, 0, pai->numero_chaves_armazenadas);
            
            // atualiza os filhos da chave promovida
            pai->filhos[indice_inserido] = filho_esq;
            pai->filhos[indice_inserido + 1] = filho_dir;

            free(arv); // liberar a memória do nó que não é mais utilizado

            if (chave < chave_promovida)
                pai->filhos[indice_inserido] = inserir(filho_esq, chave);
            else
                pai->filhos[indice_inserido + 1] = inserir(filho_dir, chave);
        }
        else
        {
            arv->chaves[0] = arv->chaves[GRAU_MINIMO];
            arv->numero_chaves_armazenadas = 1;
            arv->filhos[0] = filho_esq;
            arv->filhos[1] = filho_dir;
            arv->eh_folha = false;
        }
    }

    if (arv->eh_folha) // se eh folha, insira no nó
    {
        /* acha a posição onde a chave será inserida ordenadamente e abre espaço */
        int index = arv->numero_chaves_armazenadas;
        for (int i = arv->numero_chaves_armazenadas - 1; i >= 0; i --)
        {
            if (chave >= arv->chaves[i])
                break;
            index --;
            arv->chaves[i + 1] = arv->chaves[i];
            arv->filhos[i + 1] = arv->filhos[i];
        }
        
        // adiciona a chave na posição liberada
        arv->chaves[index] = chave;
        arv->numero_chaves_armazenadas ++;
        arv->filhos[index] = NULL;
        arv->filhos[index + 1] = NULL;
        pai = NULL;
    }
    else // tenta inserir no filho
    {
        int index = buscar_indice_de_insercao(arv->chaves, chave, 0, arv->numero_chaves_armazenadas - 1);
        pai = arv;
        arv->filhos[index] = inserir(arv->filhos[index], chave);
    }

    return arv;

    /* procura a posição para inserir a nova chave */
    // int i = 0;
    // while (i < arv->numero_chaves_armazenadas && chave > arv->chaves[i])
    //     i ++;

    /* abre espaço no vetor para inserir a chave naquela posição */
    // TO-DO

    /* [código a atualizar] insere na primeira posição livre */
    if (arv->numero_chaves_armazenadas < 2 * GRAU_MINIMO - 1)
        arv->chaves[arv->numero_chaves_armazenadas ++] = chave;
    else
    {
        arv->filhos[0] = inserir(arv->filhos[0], chave);
    }
    printf("inseriu %d\n", chave);

    return arv;
}

void imprime(ArvoreB *arv, int tabulacao, int nivel)
{
    if (!arv)
    {
        printf("(null)\n");
        return;
    }

    printf("chaves no node: %d\n", arv->numero_chaves_armazenadas);

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

    /* impressão das chaves */
    for (int i = 0; i < arv->numero_chaves_armazenadas; i ++)
    {
        // se é a última chave, imprime um caractere diferente
        if (i == arv->numero_chaves_armazenadas - 1)
            tab[2 * (nivel - 1)] = 200; // tab[ultimo] = 200: ╚

        printf("%s%d", tab, arv->chaves[i]);
        printf(" (lvl %d, i = %d, last = %d, folha = %d)", nivel, i, i == arv->numero_chaves_armazenadas - 1, arv->eh_folha);
        printf("\n");

        /* chamada recursiva para imprimir os filhos deste nó */
        imprime(arv->filhos[i], tabulacao + 2, nivel + 1);
    }
    
    // /* imprime o filho da direita da última chave */
    // imprime(arv->filhos[arv->numero_chaves_armazenadas], tabulacao + 2, nivel + 1);
}

void testar_arvore(ArvoreB *arv)
{
    inserir(arv, 10);
    inserir(arv, 30);
    inserir(arv, 20);
    imprime(arv, 0, 0);
    inserir(arv, 15);
    // inserir(arv, 35);
    // inserir(arv, 55);
    // inserir(arv, 70);
    // inserir(arv, 85);
    // inserir(arv, 90);
    // inserir(arv, 125);
    // inserir(arv, 1325);
    // inserir(arv, 1243);
    // inserir(arv, 12654);

    imprime(arv, 0, 0);
}

int main()
{
    ArvoreB *arv = novo_no();

    testar_arvore(arv);
    liberar(arv);
    
    return 0;
}
