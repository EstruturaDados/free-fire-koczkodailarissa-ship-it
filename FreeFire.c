#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

// ============================================================================
//         PROJETO CÓDIGO DA ILHA – EDIÇÃO FREE FIRE
// ============================================================================
//
// OBJETIVOS:
// - Nível Novato: Inventário básico (adicionar, remover, listar).
// - Nível Aventureiro: Busca sequencial por nome.
// - Nível Mestre: Ordenação (insertion sort) e busca binária.
// - Modularização total em funções.
// - Uso de struct, enum, bool e boas práticas.
//
// ============================================================================

// --- Constantes Globais ---
#define MAX_ITENS 10
#define MAX_NOME 50
#define MAX_TIPO 30

// --- Estrutura de Dados ---
typedef struct {
    char nome[MAX_NOME];
    char tipo[MAX_TIPO];
    int quantidade;
    int prioridade; // 1 a 5
} Item;

// --- Enum para critérios de ordenação ---
typedef enum {
    ORDENAR_NOME,
    ORDENAR_TIPO,
    ORDENAR_PRIORIDADE
} CriterioOrdenacao;

// --- Variáveis globais ---
Item mochila[MAX_ITENS];
int numItens = 0;
bool ordenadaPorNome = false;
int comparacoes = 0;

// --- Protótipos ---
void limparTela();
void exibirMenu();
void inserirItem();
void removerItem();
void listarItens();
void buscarSequencial();
void menuDeOrdenacao();
void insertionSort(CriterioOrdenacao criterio);
void buscaBinariaPorNome();
int compararItens(Item a, Item b, CriterioOrdenacao criterio);

// --- Função Principal ---
int main() {
    int opcao;
    do {
        limparTela();
        exibirMenu();
        printf("Escolha: ");
        scanf("%d", &opcao);
        getchar(); // limpar buffer

        switch (opcao) {
            case 1: inserirItem(); break;
            case 2: removerItem(); break;
            case 3: listarItens(); break;
            case 4: menuDeOrdenacao(); break;
            case 5: buscarSequencial(); break; // nível aventureiro
            case 6: buscaBinariaPorNome(); break; // nível mestre
            case 0: printf("Saindo...\n"); break;
            default: printf("Opção inválida!\n");
        }
        printf("\nPressione ENTER para continuar...");
        getchar();
    } while (opcao != 0);
    return 0;
}

// --- Implementações ---
void limparTela() {
    for (int i = 0; i < 30; i++) printf("\n");
}

void exibirMenu() {
    printf("===== MENU MOCHILA =====\n");
    printf("1 - Adicionar item\n");
    printf("2 - Remover item\n");
    printf("3 - Listar itens\n");
    printf("4 - Ordenar itens\n");
    printf("5 - Buscar item (sequencial)\n");
    printf("6 - Buscar item (binária)\n");
    printf("0 - Sair\n");
    printf("-------------------------\n");
    printf("Itens atuais: %d\n", numItens);
    printf("Ordenada por nome: %s\n", ordenadaPorNome ? "Sim" : "Não");
}

void inserirItem() {
    if (numItens >= MAX_ITENS) {
        printf("Mochila cheia!\n");
        return;
    }
    Item novo;
    printf("Nome: ");
    fgets(novo.nome, MAX_NOME, stdin);
    novo.nome[strcspn(novo.nome, "\n")] = '\0';

    printf("Tipo: ");
    fgets(novo.tipo, MAX_TIPO, stdin);
    novo.tipo[strcspn(novo.tipo, "\n")] = '\0';

    printf("Quantidade: ");
    scanf("%d", &novo.quantidade);
    getchar();

    printf("Prioridade (1-5): ");
    scanf("%d", &novo.prioridade);
    getchar();

    mochila[numItens++] = novo;
    ordenadaPorNome = false;
    printf("Item adicionado!\n");
}

void removerItem() {
    char nome[MAX_NOME];
    printf("Digite o nome do item a remover: ");
    fgets(nome, MAX_NOME, stdin);
    nome[strcspn(nome, "\n")] = '\0';

    for (int i = 0; i < numItens; i++) {
        if (strcmp(mochila[i].nome, nome) == 0) {
            for (int j = i; j < numItens - 1; j++) {
                mochila[j] = mochila[j + 1];
            }
            numItens--;
            printf("Item removido!\n");
            return;
        }
    }
    printf("Item não encontrado!\n");
}

void listarItens() {
    printf("\n===== ITENS NA MOCHILA =====\n");
    printf("%-15s %-10s %-10s %-10s\n", "Nome", "Tipo", "Qtd", "Prioridade");
    for (int i = 0; i < numItens; i++) {
        printf("%-15s %-10s %-10d %-10d\n",
               mochila[i].nome, mochila[i].tipo,
               mochila[i].quantidade, mochila[i].prioridade);
    }
}

void buscarSequencial() {
    char nome[MAX_NOME];
    printf("Digite o nome do item a buscar: ");
    fgets(nome, MAX_NOME, stdin);
    nome[strcspn(nome, "\n")] = '\0';

    for (int i = 0; i < numItens; i++) {
        if (strcmp(mochila[i].nome, nome) == 0) {
            printf("Item encontrado: %s | Tipo: %s | Qtd: %d | Prioridade: %d\n",
                   mochila[i].nome, mochila[i].tipo,
                   mochila[i].quantidade, mochila[i].prioridade);
            return;
        }
    }
    printf("Item não encontrado!\n");
}

void menuDeOrdenacao() {
    int escolha;
    printf("Ordenar por:\n1 - Nome\n2 - Tipo\n3 - Prioridade\nEscolha: ");
    scanf("%d", &escolha);
    getchar();

    comparacoes = 0;
    switch (escolha) {
        case 1: insertionSort(ORDENAR_NOME); ordenadaPorNome = true; break;
        case 2: insertionSort(ORDENAR_TIPO); ordenadaPorNome = false; break;
        case 3: insertionSort(ORDENAR_PRIORIDADE); ordenadaPorNome = false; break;
        default: printf("Critério inválido!\n"); return;
    }
    printf("Itens ordenados! Comparações: %d\n", comparacoes);
}

void insertionSort(CriterioOrdenacao criterio) {
    for (int i = 1; i < numItens; i++) {
        Item chave = mochila[i];
        int j = i - 1;
        while (j >= 0 && compararItens(mochila[j], chave, criterio) > 0) {
            mochila[j + 1] = mochila[j];
            j--;
            comparacoes++;
        }
        mochila[j + 1] = chave;
    }
}

int compararItens(Item a, Item b, CriterioOrdenacao criterio) {
    switch (criterio) {
        case ORDENAR_NOME: return strcmp(a.nome, b.nome);
        case ORDENAR_TIPO: return strcmp(a.tipo, b.tipo);
        case ORDENAR_PRIORIDADE: return b.prioridade - a.prioridade; // maior prioridade primeiro
    }
    return 0;
}

void buscaBinariaPorNome() {
    if (!ordenadaPorNome) {
        printf("A mochila deve estar ordenada por nome!\n");
        return;
    }
    char nome[MAX_NOME];
    printf("Digite o nome do item a buscar: ");
    fgets(nome, MAX_NOME, stdin);
    nome[strcspn(nome, "\n")] = '\0';

    int inicio = 0, fim = numItens - 1;
    while (inicio <= fim) {
        int meio = (inicio + fim) / 2;
        int cmp = strcmp(mochila[meio].nome, nome);
        if (cmp == 0) {
            printf("Item encontrado: %s | Tipo: %s | Qtd: %d | Prioridade: %d\n",
                   mochila[meio].nome, mochila[meio].tipo,
                   mochila[meio].quantidade, mochila[meio].prioridade);
            return;
        } else if (cmp < 0) {
            inicio = meio + 1;
        } else {
            fim = meio - 1;
        }
    }
    printf("Item não encontrado!\n");
}
