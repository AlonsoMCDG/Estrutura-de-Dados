#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

/* Binary Tree related code */
typedef struct binarytree {
    int value;
    struct binarytree *right_child;
    struct binarytree *left_child;
} BTree;

BTree *new_node(int value)
{
    BTree *tree = (BTree *)malloc(sizeof(BTree));
    tree->left_child = NULL;
    tree->right_child = NULL;
    tree->value = value;
    return tree;
}

BTree *insert(BTree *tree, int value)
{
    if (!tree)
        return new_node(value);
    
    if (value <= tree->value) // insere na esquerda
        tree->left_child = insert(tree->left_child, value);
    else // insere na direita
        tree->right_child = insert(tree->right_child, value);
}

void print_tree(BTree *tree, int tab)
{
    if (!tree) return;

    printf("%*d\n", tab, tree->value);
    print_tree(tree->left_child, tab + 2);
    print_tree(tree->right_child, tab + 2);
}

/**/

/***************************************************/
/* EXERCÍCIO 2 */
/*
2) Escreva uma função em C para verificar se uma árvore binária é estrita.
Definição: Uma árvore binária A é estrita se tiver ou 0 ou 2 filhos
*/

bool estrita(BTree *tree)
{
    if (!tree)
        return true;
    
    int filhos = (tree->left_child != NULL) + (tree->right_child != NULL);

    return (filhos != 1) && (estrita(tree->left_child) && estrita(tree->right_child));
}

/***************************************************/

/* EXERCÍCIO 3 */
/*
3) Crie uma árvore B de grau mínimo t = 3. Insira as seguintes chaves na 
ordem fornecida:
10, 20, 30, 40, 50, 60, 70, 80, 90, 100, 110, 120, 130, 140, 150, 160, 
170, 180, 190, 200.

4) Da árvore resultante do exercíco 3, remova as seguintes chaves:
a) 60
b) 130
c) 140
d) 100
e) 50.,.,,,,,............................
*/



/***************************************************/



int main()
{
    

    return 0;
}