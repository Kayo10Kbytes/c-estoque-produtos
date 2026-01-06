#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_PRODUTOS 100
#define ARQ_ESTOQUE "data/estoque.csv"

typedef struct {
    int codigo;
    char nome[60];
    int qtd;
    float preco;
    int ativo;
} Produto;

void limparBuffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF) {}
}

void lerLinha(char *dest, int tam) {
    fgets(dest, tam, stdin);
    size_t n = strlen(dest);
    if (n > 0 && dest[n - 1] == '\n') dest[n - 1] = '\0';
}

int menu() {
    int op;
    printf("\n==== SISTEMA DE ESTOQUE ====\n");
    printf("1) Cadastrar produto\n");
    printf("2) Listar produtos\n");
    printf("3) Entrada de estoque\n");
    printf("4) Saida de estoque\n");
    printf("5) Relatorio: abaixo do minimo\n");
    printf("6) Salvar em arquivo\n");
    printf("7) Carregar do arquivo\n");
    printf("0) Sair\n");
    printf("Escolha: ");
    scanf("%d", &op);
    limparBuffer();
    return op;
}

int acharPorCodigo(Produto v[], int n, int codigo) {
    for (int i = 0; i < n; i++) {
        if (v[i].ativo == 1 && v[i].codigo == codigo) return i;
    }
    return -1;
}

void cadastrar(Produto v[], int *n) {
    if (*n >= MAX_PRODUTOS) {
        printf("Limite de produtos atingido.\n");
        return;
    }

    Produto p;
    p.ativo = 1;

    printf("Codigo (numero): ");
    scanf("%d", &p.codigo);
    limparBuffer();

    if (acharPorCodigo(v, *n, p.codigo) != -1) {
        printf("Codigo ja cadastrado.\n");
        return;
    }

    printf("Nome: ");
    lerLinha(p.nome, sizeof(p.nome));

    printf("Quantidade inicial: ");
    scanf("%d", &p.qtd);
    limparBuffer();

    printf("Preco: ");
    scanf("%f", &p.preco);
    limparBuffer();

    v[*n] = p;
    (*n)++;

    printf("Produto cadastrado.\n");
}

void listar(Produto v[], int n) {
    printf("\n--- PRODUTOS ---\n");
    int ok = 0;
    for (int i = 0; i < n; i++) {
        if (v[i].ativo == 1) {
            printf("Cod: %d | %s | Qtd: %d | Preco: %.2f\n",
                   v[i].codigo, v[i].nome, v[i].qtd, v[i].preco);
            ok = 1;
        }
    }
    if (!ok) printf("Nenhum produto cadastrado.\n");
}

void entrada(Produto v[], int n) {
    int codigo, qtd;
    printf("Codigo do produto: ");
    scanf("%d", &codigo);
    limparBuffer();

    int idx = acharPorCodigo(v, n, codigo);
    if (idx == -1) {
        printf("Produto nao encontrado.\n");
        return;
    }

    printf("Quantidade de entrada: ");
    scanf("%d", &qtd);
    limparBuffer();

    if (qtd <= 0) {
        printf("Quantidade invalida.\n");
        return;
    }

    v[idx].qtd += qtd;
    printf("Entrada registrada. Nova qtd: %d\n", v[idx].qtd);
}

void saida(Produto v[], int n) {
    int codigo, qtd;
    printf("Codigo do produto: ");
    scanf("%d", &codigo);
    limparBuffer();

    int idx = acharPorCodigo(v, n, codigo);
    if (idx == -1) {
        printf("Produto nao encontrado.\n");
        return;
    }

    printf("Quantidade de saida: ");
    scanf("%d", &qtd);
    limparBuffer();

    if (qtd <= 0) {
        printf("Quantidade invalida.\n");
        return;
    }

    if (qtd > v[idx].qtd) {
        printf("Estoque insuficiente. Qtd atual: %d\n", v[idx].qtd);
        return;
    }

    v[idx].qtd -= qtd;
    printf("Saida registrada. Nova qtd: %d\n", v[idx].qtd);
}

void relatorioMinimo(Produto v[], int n) {
    int minimo;
    printf("Defina o minimo (ex.: 5): ");
    scanf("%d", &minimo);
    limparBuffer();

    printf("\n--- ABAIXO DO MINIMO (%d) ---\n", minimo);
    int ok = 0;
    for (int i = 0; i < n; i++) {
        if (v[i].ativo == 1 && v[i].qtd < minimo) {
            printf("Cod: %d | %s | Qtd: %d\n", v[i].codigo, v[i].nome, v[i].qtd);
            ok = 1;
        }
    }
    if (!ok) printf("Nenhum produto abaixo do minimo.\n");
}

void salvar(Produto v[], int n) {
    FILE *f = fopen(ARQ_ESTOQUE, "w");
    if (!f) {
        printf("Erro ao abrir arquivo.\n");
        return;
    }
    fprintf(f, "codigo,nome,qtd,preco,ativo\n");
    for (int i = 0; i < n; i++) {
        fprintf(f, "%d,%s,%d,%.2f,%d\n",
                v[i].codigo, v[i].nome, v[i].qtd, v[i].preco, v[i].ativo);
    }
    fclose(f);
    printf("Salvo em %s\n", ARQ_ESTOQUE);
}

void carregar(Produto v[], int *n) {
    FILE *f = fopen(ARQ_ESTOQUE, "r");
    if (!f) {
        printf("Arquivo %s nao encontrado.\n", ARQ_ESTOQUE);
        return;
    }

    char linha[256];
    *n = 0;

    fgets(linha, sizeof(linha), f); // cabecalho

    while (fgets(linha, sizeof(linha), f) && *n < MAX_PRODUTOS) {
        Produto p;
        char *tok = strtok(linha, ",");
        if (!tok) continue;
        p.codigo = atoi(tok);

        tok = strtok(NULL, ",");
        if (!tok) continue;
        strncpy(p.nome, tok, sizeof(p.nome));
        p.nome[strcspn(p.nome, "\n")] = 0;

        tok = strtok(NULL, ",");
        if (!tok) continue;
        p.qtd = atoi(tok);

        tok = strtok(NULL, ",");
        if (!tok) continue;
        p.preco = (float)atof(tok);

        tok = strtok(NULL, ",");
        p.ativo = tok ? atoi(tok) : 1;

        v[*n] = p;
        (*n)++;
    }

    fclose(f);
    printf("Carregado de %s (%d registros)\n", ARQ_ESTOQUE, *n);
}

int main() {
    Produto produtos[MAX_PRODUTOS];
    int n = 0;

    int op;
    do {
        op = menu();
        switch (op) {
            case 1: cadastrar(produtos, &n); break;
            case 2: listar(produtos, n); break;
            case 3: entrada(produtos, n); break;
            case 4: saida(produtos, n); break;
            case 5: relatorioMinimo(produtos, n); break;
            case 6: salvar(produtos, n); break;
            case 7: carregar(produtos, &n); break;
            case 0: printf("Saindo...\n"); break;
            default: printf("Opcao invalida.\n"); break;
        }
    } while (op != 0);

    return 0;
}
