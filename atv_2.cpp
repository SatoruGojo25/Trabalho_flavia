#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#define MAX_ITENS 100
#define TAM_NOME 50

typedef struct {
    int id;
    char nome[TAM_NOME];
    float preco;
    int ativo;
} Item;


Item inventario[MAX_ITENS];
int contador_itens = 0;
int total_ativos = 0;


void menu();
void cadastrar_item();
void consultar_item();
void listar_todos_itens();
void excluir_item();
void gerar_relatorio_txt();
int buscar_indice_por_id(int id);





void menu() {
    int escolha;
    do {
        printf("\n--- MENU DE GERENCIAMENTO DE ITENS ---\n");
        printf("1. Cadastrar novo item\n");
        printf("2. Consultar item por ID\n");
        printf("3. Consultar todos os itens (Lista)\n");
        printf("4. Excluir item por ID\n");
        printf("5. Gerar relatório em arquivo .txt\n");
        printf("0. Sair\n");
        printf("--------------------------------------\n");
        printf("Escolha uma opção: ");
        scanf("%d", &escolha);

        while (getchar() != '\n');

        switch (escolha) {
            case 1:
                cadastrar_item();
                break;
            case 2:
                consultar_item();
                break;
            case 3:
                listar_todos_itens();
                break;
            case 4:
                excluir_item();
                break;
            case 5:
                gerar_relatorio_txt();
                break;
            case 0:
                printf("\nSaindo do programa. Até logo!\n");
                break;
            default:
                printf("\nOpção inválida. Tente novamente.\n");
        }
    } while (escolha != 0);
}




int buscar_indice_por_id(int id) {
    for (int i = 0; i < contador_itens; i++) {

        if (inventario[i].id == id && inventario[i].ativo == 1) {
            return i;
        }
    }
    return -1;
}


void cadastrar_item() {
    if (total_ativos >= MAX_ITENS) {
        printf("\nERRO: Limite máximo de %d itens atingido.\n", MAX_ITENS);
        return;
    }

    Item novo;


    novo.id = contador_itens + 1;

    printf("\n--- CADASTRO DE NOVO ITEM ---\n");
    printf("Nome do item (máx %d caracteres): ", TAM_NOME - 1);

    if (fgets(novo.nome, TAM_NOME, stdin) == NULL) {
        printf("\nERRO de leitura do nome.\n");
        return;
    }

    novo.nome[strcspn(novo.nome, "\n")] = 0;

    printf("Preço do item: ");

    if (scanf("%f", &novo.preco) != 1) {
        printf("\nERRO: Entrada de preço inválida. Tente novamente.\n");
        while (getchar() != '\n');
        return;
    }
    while (getchar() != '\n');

    novo.ativo = 1;

    inventario[contador_itens] = novo;
    printf("\n✅ Item cadastrado com sucesso! ID: %d\n", novo.id);

    contador_itens++;
    total_ativos++;
}


void consultar_item() {
    int id_busca;
    printf("\n--- CONSULTAR ITEM POR ID ---\n");
    printf("Digite o ID do item: ");
    if (scanf("%d", &id_busca) != 1) {
        printf("\nERRO: Entrada de ID inválida.\n");
        while (getchar() != '\n');
        return;
    }
    while (getchar() != '\n');

    int indice = buscar_indice_por_id(id_busca);

    if (indice != -1) {
        Item item = inventario[indice];
        printf("\n--- DADOS DO ITEM ID: %d ---\n", item.id);
        printf("Nome: %s\n", item.nome);
        printf("Preço: R$ %.2f\n", item.preco);
        printf("Status: Ativo\n");
        printf("---------------------------\n");
    } else {
        printf("\n❌ Item com ID %d não encontrado ou excluído.\n", id_busca);
    }
}




void listar_todos_itens() {
    if (total_ativos == 0) {
        printf("\nO inventário está vazio. Nenhum item ativo para listar.\n");
        return;
    }

    printf("\n--- LISTA DE TODOS OS ITENS ATIVOS (%d itens) ---\n", total_ativos);
    printf("ID  | Nome%*s | Preço\n", TAM_NOME - 4, " ");
    printf("----|-%*s-|--------\n", TAM_NOME - 2, " ");

    int encontrado = 0;
    for (int i = 0; i < contador_itens; i++) {
        if (inventario[i].ativo == 1) {
            Item item = inventario[i];
            printf("%-4d| %-*s | R$ %.2f\n", item.id, TAM_NOME - 2, item.nome, item.preco);
            encontrado++;
        }
    }
    printf("--------------------------------------------------\n");
}



void excluir_item() {
    int id_excluir;
    printf("\n--- EXCLUIR ITEM POR ID ---\n");
    printf("Digite o ID do item a ser excluído: ");
    if (scanf("%d", &id_excluir) != 1) {
        printf("\nERRO: Entrada de ID inválida.\n");
        while (getchar() != '\n');
        return;
    }
    while (getchar() != '\n');

    int indice = buscar_indice_por_id(id_excluir);

    if (indice != -1) {

        inventario[indice].ativo = 0;
        total_ativos--;
        printf("\n✅ Item ID %d ('%s') excluído (marcado como inativo) com sucesso.\n",
               id_excluir, inventario[indice].nome);
    } else {
        printf("\n❌ Item com ID %d não encontrado ou já excluído.\n", id_excluir);
    }
}


void gerar_relatorio_txt() {
    FILE *arquivo;
    const char *nome_arquivo = "relatorio_inventario.txt";

    arquivo = fopen(nome_arquivo, "w");

    if (arquivo == NULL) {
        printf("\nERRO: Não foi possível abrir o arquivo '%s' para escrita.\n", nome_arquivo);
        return;
    }


    fprintf(arquivo, "==========================================\n");
    fprintf(arquivo, "RELATÓRIO DE ITENS ATIVOS NO INVENTÁRIO\n");
    fprintf(arquivo, "TOTAL DE ITENS ATIVOS: %d\n", total_ativos);
    fprintf(arquivo, "==========================================\n\n");
    fprintf(arquivo, "ID  | Nome%*s | Preço\n", TAM_NOME - 4, " ");
    fprintf(arquivo, "----|-%*s-|--------\n", TAM_NOME - 2, " ");

    int itens_salvos = 0;
    for (int i = 0; i < contador_itens; i++) {

        if (inventario[i].ativo == 1) {
            Item item = inventario[i];
            fprintf(arquivo, "%-4d| %-*s | R$ %.2f\n", item.id, TAM_NOME - 2, item.nome, item.preco);
            itens_salvos++;
        }
    }


    fclose(arquivo);

    printf("\n✅ Relatório gerado com sucesso!\n");
    printf("Arquivo salvo como: **%s** (%d itens registrados).\n", nome_arquivo, itens_salvos);
}


int main() {
    menu();
    return 0;
}