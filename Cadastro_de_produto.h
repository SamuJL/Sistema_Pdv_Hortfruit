#ifndef CADASTRO_DE_PRODUTO_INCLUDED
#define CADASTRO_DE_PRODUTO_INCLUDED

#define MAX_PRODUTOS 100
#define ARQUIVO "produtos.txt"
#define MARGEM 0.20

int contadorVendas = 0; // Contador global de vendas
int contadorDias = 1;   // Contador global de dias

struct Produto {
    int id;               // ID do produto (posição no estoque)
    char nome[50];        // Nome do produto
    float preco;          // Preço do produto
    float quantidade;     // Quantidade em Kg no estoque
};

void salvarProdutos(struct Produto produtos[], int contador);

int carregarProdutos(struct Produto produtos[]);

void cadastrarProduto(struct Produto produtos[], int *contador, int modoAtualizacao, int idProdutoAtualizar);

void listarProdutos(struct Produto produtos[], int contador);

struct Produto* procurarProduto(struct Produto produtos[], int numProdutos, int id);

#endif
