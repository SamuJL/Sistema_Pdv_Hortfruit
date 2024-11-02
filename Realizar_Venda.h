#ifndef REALIZAR_VENDA_INCLUDED
#define REALIZAR_VENDA_INCLUDED

#define MAX_PRODUTOS 100
#define ARQUIVO "produtos.txt"
#define MARGEM 0.20

int idProduto;
    float quantidadeVenda;
    float total = 0.0;
    char opcadd;

    // Lista temporária para armazenar os produtos e quantidades vendidas
    struct Produto vendasTemp[MAX_PRODUTOS];
    float quantidadesVendidas[MAX_PRODUTOS]; // Quantidades vendidas de cada produto
    int vendasTempCount = 0; // Contador para vendas temporárias

void realizarVenda(struct Produto produtos[], int numProdutos, int *contador);

struct Produto *produto = procurarProduto(produtos, numProdutos, idProduto);

for (int i = 0; i < vendasTempCount; i++);

#endif
