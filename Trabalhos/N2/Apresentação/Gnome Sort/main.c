/* ESTRUTURA DE DADOS - Atividade para N2 - "Sala de aula invertida" */
/* Implementação do algoritmo de ordenação "Gnome Sort" */
/* Grupo:
    - Alonso Martins de Araújo
    - Elias da Cruz de Souza Junior
    - Nicolas Sampaio Pinheiro

Data: 17/03/2025
*/
#include <stdio.h>

void gnome_sort(int a[], int n)
{
    /* inicializa a posição do gnomo no primeiro índice do vetor */
    int index = 0;

    /* continua no algoritmo enquanto não chegou ao fim do vetor */
    while (index < n)
    {
        /* se está no início do vetor, avança */
        if (index == 0)
            index ++;

        /* senão, se os elementos estão na ordem correta, avança */
        else if (a[index] >= a[index - 1])
            index ++;

        /* os elementos estão na ordem incorreta */
        else
        {
            /* troca os dois elementos do vetor de posição */
            int temp = a[index];
            a[index] = a[index - 1];
            a[index - 1] = temp;
            
            /* volta uma posição */
            index --;
        }
    }
}

int main()
{
    int vet[10] = { 15, 2, 76, 45, 41, 53, 28, 39, 61, 78 };
    int n = 10;

    /* imprime o vetor orignial, antes de ordenar */
    printf("vetor original:\n");
    for (int i = 0; i < n; i ++)
        printf("%d%s", vet[i], i < n - 1 ? ", " : "\n");

    /* chamada para a função de ordenação */
    gnome_sort(vet, n);

    /* imprime o vetor final, após ter sido ordenado */
    printf("vetor ordenado:\n");
    for (int i = 0; i < n; i ++)
        printf("%d%s", vet[i], i < n - 1 ? ", " : "\n");

    return 0;
}
