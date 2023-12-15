#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "header.h"

int main()
{
    char escolha;
    vtree Treedata, Treeid;
    initTree(&Treedata);
    initTree(&Treeid);
    do
    {
        escolha = menu();
        if (escolha == '1')
        {
            vnode *temp;
            temp = (vnode *)malloc(sizeof(vnode)); // Aloca espaço para um novo vnode

            printf("Codigo: \n");
            scanf("%d", &temp->info.codigo);

            printf("Data: \n");
            scanf("%d %d %d", &temp->info.dataProducao.dia, &temp->info.dataProducao.mes, &temp->info.dataProducao.ano);

            strcpy(temp->info.tipoDeFardo.cultivar, selcultivar());

            printf("Tipo de feno: \n");
            temp->info.tipoDeFardo.tipoDeFeno = selTipoDeFeno();

            printf("Diametro: \n");
            scanf("%d", &temp->info.tipoDeFardo.diametro);

            printf("Quantidade de fardos: \n");
            scanf("%d", &temp->info.qtDeFardos);

            printf("Duracao: \n");
            scanf("%f", &temp->info.duracao);

            if (!idIsIn(Treedata.root, temp) && !dataIsIn(Treedata.root, temp->info.dataProducao))
            {
                Treedata.root = insertNodeDate(Treedata.root, newNode(temp->info));
                Treeid.root = insertNodeId(Treeid.root, newNode(temp->info));
            }
            else
            {
                printf("Data ou id ja existentes\n");
                printf("Nenhuma alteracao foi feita\n");
            }
        }
        else if (escolha == '2')
        {
            char op2;
            Data datatemp2;
            char cultivartemp2[20];

            printf("Selecione o tipo de busca\n");
            printf("1 - Pesquisar por data\n");
            printf("2 - Pesquisar por cultivar\n");
            scanf(" %c", &op2);
            if (op2 == '1')
            {
                printf("Insira a data: \n");
                scanf("%d %d %d", &datatemp2.dia, &datatemp2.mes, &datatemp2.ano);
                searchNodeByDate(Treedata.root, datatemp2);
            }
            else if (op2 == '2')
            {
                strcpy(cultivartemp2, selcultivar());
                searchAndPrintCultivar(Treedata.root, cultivartemp2);
            }

            else
                printf("Nao encontrado\n");
        }
        else if (escolha == '3')
        {
            Producao temp3;
            printf("Digite o ID que deseja alterar:\n");
            scanf("%d", &temp3.codigo);

            // Busca o nó na árvore pelo ID
            vnode *nodeToModify = (vnode *)malloc(sizeof(vnode));
            nodeToModify->info = searchNodeById(Treeid.root, temp3.codigo);
            temp3 = nodeToModify->info;

            if (nodeToModify != NULL)
            {
                printf("Nao e possivel alterar o ID ou Data por aqui, caso queira, exclua o registro de producao e crie outro com o ID/data desejado\n");
                printf("Caso queira voltar, repita os dados de producao\n");

                // Remove o nó das árvores
                Treeid.root = delNodeId(Treeid.root, temp3.codigo);
                Treedata.root = delNodeData(Treedata.root, nodeToModify->info.dataProducao);

                // Coleta novos dados de produção
                strcpy(temp3.tipoDeFardo.cultivar, selcultivar());
                printf("Novo tipo de feno: \n");
                scanf(" %c", &temp3.tipoDeFardo.tipoDeFeno);
                printf("Novo diametro: \n");
                scanf("%d", &temp3.tipoDeFardo.diametro);
                printf("Nova quantidade de fardos: \n");
                scanf("%d", &temp3.qtDeFardos);
                printf("Nova duracao: \n");
                scanf("%f", &temp3.duracao);

                // Insere o novo nó nas árvores
                Treeid.root = insertNodeId(Treeid.root, newNode(temp3));
                Treedata.root = insertNodeDate(Treedata.root, newNode(temp3));

                printf("Producao alterada\n");
            }
            else
            {
                printf("ID nao encontrado. Nenhuma alteracao foi feita.\n");
            }
        }

        else if (escolha == '4')
        {
            int idtemp4;
            char escolha4;
            vnode *nodeaux4;
            nodeaux4 = (vnode *)malloc(sizeof(vnode));

            printf("ID da producao que deseja excluir: \n");
            scanf("%d", &idtemp4);

            // Chama a função uma vez e armazena o resultado em nodeaux4
            nodeaux4->info = searchNodeById(Treeid.root, idtemp4);

            if (nodeaux4 != NULL)
            {
                printf("Deseja excluir a seguinte producao? (S/n)\n");

                scanf(" %c", &escolha4);
                if (escolha4 == 'S')
                {
                    Treeid.root = delNodeId(Treeid.root, idtemp4);
                    Treedata.root = delNodeData(Treedata.root, nodeaux4->info.dataProducao);
                }
                else
                    printf("Nenhuma alteracao foi feita\n");
            }
        }

        else if (escolha == '5')
        {
            showTreeRIn(Treedata.root);
        }
    } while (escolha != '6');

    freeTree(Treedata.root);
    freeTree(Treeid.root);

    printf("Encerrando o programa...\n");

    return 0;
}