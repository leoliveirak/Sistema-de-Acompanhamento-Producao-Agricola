#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#ifndef HEADER_H
#define HEADER_H
/* header */

/*---------------------------------------------------------------------------------------*/
// Struct que representa a data
typedef struct data
{
    int dia;
    int mes;
    int ano;
} Data;

// Struct que que contém as informações do fardo
typedef struct fardo
{
    char cultivar[20];
    char tipoDeFeno;
    int diametro;
} Fardo;

// Struct que contém os dados de produção
typedef struct producao
{
    int codigo;
    Data dataProducao;
    Fardo tipoDeFardo;
    int qtDeFardos;
    float duracao;
} Producao;

// Cria o nó
typedef struct tnode
{
    Producao info;
    struct tnode *left, *right;

} vnode;

// inicia a árvore
typedef struct ttree
{
    vnode *root;
} vtree;

/*---------------------------------------------------------------------------------------*/

// MENU
char menu()
{
    char option;

    printf("\n========== Menu Principal ==========\n");
    printf("1 - Incluir Producao\n");
    printf("2 - Consultar\n");
    printf("3 - Alterar\n");
    printf("4 - Excluir\n");
    printf("5 - Listar todos\n");
    printf("6 - Sair\n");

    printf("\nEscolha uma opcao (1-6): \n");
    scanf(" %c", &option);

    if (option >= '1' && option <= '6')
        printf("opcao escolhida: %c\n", option);
    else
        printf("Opcao invalida\n");

    return option;
}

// Função que compara duas datas
int compararDatas(Data d1, Data d2)
{
    if (d1.ano != d2.ano)
        return d1.ano - d2.ano;

    if (d1.mes != d2.mes)
        return d1.mes - d2.mes;
    return d1.dia - d2.dia;
}

//Inicia a árvore
void initTree(vtree *t)
{
    t->root = NULL;
}

//Cria novo nó
vnode *newNode(Producao i)
{
    vnode *n;
    n = (vnode *)malloc(sizeof(vnode));
    n->info = i;
    n->left = NULL;
    n->right = NULL;
    return n;
}

//Profundidade das subárvores
/*---------------------------------------------------------------------------------------*/
int depthLeft(vnode *r)
{
    if (r == NULL)
        return 0;
    return 1 + depthLeft(r->left);
}
int depthRight(vnode *r)
{
    if (r == NULL)
        return 0;
    return 1 + depthRight(r->right);
}
int depthSubTree(vnode *root)
{
    int depthLeft, depthRight;

    if (root == NULL)
        return 0;

    // calcular a profundidade da subárvore esquerda
    depthLeft = depthSubTree(root->left);
    // calcular a profundidade da subárvore direita
    depthRight = depthSubTree(root->right);

    return 1 + (depthLeft > depthRight
                    ? depthLeft
                    : depthRight); // verifica a subárvore mais profunda e adiciona 1.
}
/*---------------------------------------------------------------------------------------*/

//Funções árvore AVL
/*---------------------------------------------------------------------------------------*/
vnode *ll(vnode *root)
{
    vnode *newRoot = root->left;
    root->left = newRoot->right;
    newRoot->right = root;

    return newRoot;
}

vnode *rr(vnode *root)
{
    vnode *newRoot = root->right;
    root->right = newRoot->left;
    newRoot->left = root;

    return newRoot;
}

vnode *lr(vnode *root)
{
    root->left = rr(root->left);
    return ll(root);
}

vnode *rl(vnode *root)
{
    root->right = ll(root->right);
    return rr(root);
}

vnode *balanceCheck(vnode *root)
{
    int rootBalance, childBalance;

    rootBalance = depthSubTree(root->left) - depthSubTree(root->right);

    if (rootBalance > 1)
    {
        // calcula o balanceamento do filho a esquerda
        childBalance =
            depthSubTree(root->left->left) - depthSubTree(root->left->right);
        root = (childBalance > 0) ? ll(root) : lr(root);
    }
    else if (rootBalance < -1)
    {
        // calcula o balanceamento do filho a direita
        childBalance =
            depthSubTree(root->right->left) - depthSubTree(root->right->right);
        root = (childBalance < 0) ? rr(root) : rl(root);
    }
    return root;
}
/*---------------------------------------------------------------------------------------*/

//Insere um nó na arvore binaria de busca com base na data
/*---------------------------------------------------------------------------------------*/
vnode *insertNodeDate(vnode *root, vnode *n)
{
    if (root == NULL)
        return n;

    if (compararDatas(n->info.dataProducao, root->info.dataProducao) < 0)
        root->left = insertNodeDate(root->left, n);
    else if (compararDatas(n->info.dataProducao, root->info.dataProducao) > 0)
        root->right = insertNodeDate(root->right, n);
    else
        return root; // Evita dados duplos

    // Realiza as operações de balanceamento
    return balanceCheck(root);
}
/*---------------------------------------------------------------------------------------*/

//Insere um nó a partir de um ID
vnode *insertNodeId(vnode *root, vnode *n)
{
    if (root == NULL)
        return n;

    if (n->info.codigo < root->info.codigo)
        root->left = insertNodeId(root->left, n);
    else if (n->info.codigo > root->info.codigo)
        root->right = insertNodeId(root->right, n);
    else
    {
        printf("O id informado ja esta cadastrado\n");
        printf("Nenhuma alteracao foi feita\n");
        return root;
    }
    root = balanceCheck(root);

    return root;
}
/*---------------------------------------------------------------------------------------*/

//Verifica se o parametro está na arvore (ID/DATA)
/*---------------------------------------------------------------------------------------*/
bool idIsIn(vnode *root, vnode *n)
{
    if (root == NULL)
        return false;

    if (n->info.codigo < root->info.codigo)
        return idIsIn(root->left, n);
    else if (n->info.codigo > root->info.codigo)
        return idIsIn(root->right, n);
    else
        return true; // O código foi encontrado na árvore
}

bool dataIsIn(vnode *root, Data data)
{
    if (root == NULL)
        return false;

    if (compararDatas(data, root->info.dataProducao) < 0)
        return dataIsIn(root->left, data);
    else if (compararDatas(data, root->info.dataProducao) > 0)
        return dataIsIn(root->right, data);
    else
        return true; // O código foi encontrado na árvore
}
/*---------------------------------------------------------------------------------------*/

//Imprime as informações principais
/*---------------------------------------------------------------------------------------*/
void printNodeInfo(vnode *node)
{
    if (node != NULL)
    {
        printf("\n%d/%d/%d: %s - %c - %d\n",
               node->info.dataProducao.dia,
               node->info.dataProducao.mes,
               node->info.dataProducao.ano,
               node->info.tipoDeFardo.cultivar,
               node->info.tipoDeFardo.tipoDeFeno,
               node->info.qtDeFardos);
    }
    else
    {
        printf("Producao nao encontrada.\n");
    }
}
/*---------------------------------------------------------------------------------------*/

//Busca um nó com a data como parâmetro
/*---------------------------------------------------------------------------------------*/
vnode *searchNodeByDate(vnode *root, Data data)
{
    if (root == NULL)
    {
        printf("Nao encontrado\n");
        return root;
    }

    if (compararDatas(data, root->info.dataProducao) < 0)
        return searchNodeByDate(root->left, data);
    else if (compararDatas(data, root->info.dataProducao) > 0)
        return searchNodeByDate(root->right, data);
    else
    {
        printNodeInfo(root);
        return root;
    }
}
/*---------------------------------------------------------------------------------------*/

//Busca um nó com o ID como parâmetro
/*---------------------------------------------------------------------------------------*/
Producao searchNodeById(vnode *root, int key)
{
    if (root == NULL)
    {
        printf("Nao encontrado\n");
        return root->info;
    }

    if (key < root->info.codigo)
        return searchNodeById(root->left, key);
    else if (key > root->info.codigo)
        return searchNodeById(root->right, key);
    else
    {
        printNodeInfo(root);
        return root->info;
    }
}
/*---------------------------------------------------------------------------------------*/

//Busca um nó com Cultivar como parâmetro
/*---------------------------------------------------------------------------------------*/
void searchNodeByCultivar(vnode *root, char *cultivar, int *somaA, int *somaB, int *somaC)
{
    if (root == NULL)
    {
        return;
    }

    searchNodeByCultivar(root->left, cultivar, somaA, somaB, somaC);

    if (strcmp(root->info.tipoDeFardo.cultivar, cultivar) == 0)
    {
        if (root->info.tipoDeFardo.tipoDeFeno == 'A')
            (*somaA) += root->info.qtDeFardos;
        else if (root->info.tipoDeFardo.tipoDeFeno == 'B')
            (*somaB) += root->info.qtDeFardos;
        else if (root->info.tipoDeFardo.tipoDeFeno == 'C')
            (*somaC) += root->info.qtDeFardos;
    }

    searchNodeByCultivar(root->right, cultivar, somaA, somaB, somaC);
}

// Função de conveniência para chamar a função inicializando as variáveis de soma
void searchAndPrintCultivar(vnode *root, char *cultivar)
{
    int somaA = 0, somaB = 0, somaC = 0;
    searchNodeByCultivar(root, cultivar, &somaA, &somaB, &somaC);

    printf("%s: A - %d\n", cultivar, somaA);
    printf("%s: B - %d\n", cultivar, somaB);
    printf("%s: C - %d\n", cultivar, somaC);
}
/*---------------------------------------------------------------------------------------*/


//Encontra o minimo
/*---------------------------------------------------------------------------------------*/
vnode *findMinR(vnode *root)
{
    while (root->left != NULL)
        root = root->left;
    return root;
}
/*---------------------------------------------------------------------------------------*/

//Deleta um nó com o id como parâmetro
/*---------------------------------------------------------------------------------------*/
vnode *delNodeId(vnode *root, int key)
{
    if (root == NULL)
    {
        printf("Nó não encontrado.\n");
        return root;
    }

    if (key < root->info.codigo)
        root->left = delNodeId(root->left, key);

    else if (key > root->info.codigo)
        root->right = delNodeId(root->right, key);

    else
    {
        // Nó não tem filhos - exclui
        if (root->left == NULL)
        {
            vnode *temp = root->right;
            free(root);
            return temp;
        }

        // Nó tem apenas um filho - substitui o nó pelo filho
        else if (root->right == NULL)
        {
            vnode *temp = root->left;
            free(root);
            return temp;
        }

        // Nó tem dois filhos, encontra o sucessor (menor valor à direita)
        vnode *temp = findMinR(root->right);

        // Copia os dados do sucessor para este nó
        root->info = temp->info;

        // Exclui o sucessor
        root->right = delNodeId(root->right, temp->info.codigo);
    }

    // Realiza as operações de balanceamento
    root = balanceCheck(root);

    return root;
}
/*---------------------------------------------------------------------------------------*/

//Deleta um nó com a data como parâmetro
/*---------------------------------------------------------------------------------------*/
vnode *delNodeData(vnode *root, Data data)
{
    if (root == NULL)
    {
        printf("Nó não encontrado.\n");
        return root;
    }

    if (compararDatas(data, root->info.dataProducao) < 0)
        root->left = delNodeData(root->left, data);

    else if (compararDatas(data, root->info.dataProducao) > 0)
        root->right = delNodeData(root->right, data);

    else
    {
        // Nó não tem filhos - exclui
        if (root->left == NULL)
        {
            vnode *temp = root->right;
            free(root);
            return temp;
        }

        // Nó tem apenas um filho - substitui o nó pelo filho
        else if (root->right == NULL)
        {
            vnode *temp = root->left;
            free(root);
            return temp;
        }

        // Nó tem dois filhos, encontra o sucessor (menor valor à direita)
        vnode *temp = findMinR(root->right);

        // Copia os dados do sucessor para este nó
        root->info = temp->info;

        // Exclui o sucessor
        root->right = delNodeData(root->right, temp->info.dataProducao);
    }

    // Realiza as operações de balanceamento
    root = balanceCheck(root);

    return root;
}
/*---------------------------------------------------------------------------------------*/

//Imprimir arvore data crescente
/*---------------------------------------------------------------------------------------*/
void showTreeRIn(vnode *root)
{
    if (root == NULL)
        return;

    showTreeRIn(root->left);
    printNodeInfo(root);
    showTreeRIn(root->right);
}
/*---------------------------------------------------------------------------------------*/

//Libera memória da árvore
/*---------------------------------------------------------------------------------------*/
void freeTree(vnode *n)
{
    if (n == NULL)
        return;
    freeTree(n->left);
    freeTree(n->right);
    free(n);
}
/*---------------------------------------------------------------------------------------*/

//"Menu" para seleção de tipo de feno
/*---------------------------------------------------------------------------------------*/
char selTipoDeFeno()
{
    char op;
    char tipode;

    // Loop para garantir uma entrada válida
    do
    {
        scanf(" %c", &op);
        switch (op)
        {
        case 'A':
        case 'B':
        case 'C':
            //Atribui o valor com base na escolha do usuário
            tipode = op;
            break;
        default:
            printf("As opcoes validas sao: A, B ou C\n");
            tipode = '\0'; // Caractere nulo para indicar opção inválida
        }
    } while (tipode == '\0');

    return tipode;
}
/*---------------------------------------------------------------------------------------*/

// Função para selecionar o tipo cultivar
/*---------------------------------------------------------------------------------------*/
const char *selcultivar()
{
    char op;
    const char *cultivar;

    // Loop para garantir uma entrada válida
    do
    {
        printf("Selecione o Cultivar:\n");
        printf("1 - Tifton 85\n");
        printf("2 - Florakirk\n");
        printf("3 - Jiggs\n");
        printf("4 - Coastcross\n");

        scanf(" %c", &op);

        // Atribui o tipo de feno com base na escolha do usuário
        switch (op)
        {
        case '1':
            cultivar = "Tifton 85";
            break;
        case '2':
            cultivar = "Florakirk";
            break;
        case '3':
            cultivar = "Jiggs";
            break;
        case '4':
            cultivar = "Coastcross";
            break;
        default:
            printf("Opcao invalida. Tente novamente.\n");
            cultivar = NULL;
        }
    } while (cultivar == NULL);

    return cultivar;
}
/*---------------------------------------------------------------------------------------*/

#endif