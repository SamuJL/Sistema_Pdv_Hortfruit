#include "Realizar_Venda.h"

#define MAX_PRODUTOS 100
#define ARQUIVO "produtos.txt"
#define MARGEM 0.20

int contadorVendas = 0; // Contador global de vendas
int contadorDias = 1;   // Contador global de dias

void realizarVenda(struct Produto produtos[], int numProdutos, int *contador){ {
    int idProduto;
    float quantidadeVenda;
    float total = 0.0;
    char opcadd;

    // Lista temporária para armazenar os produtos e quantidades vendidas
    struct Produto vendasTemp[MAX_PRODUTOS];
    float quantidadesVendidas[MAX_PRODUTOS]; // Quantidades vendidas de cada produto
    int vendasTempCount = 0; // Contador para vendas temporárias

    printf("Bem vindo ao sistema de PDV!\n");
    printf("Digite o ID do produto (0 para finalizar): ");

    while (1) {
        scanf("%d", &idProduto);

        // Se o ID for 0, finalizar o processo de venda
        if (idProduto == 0) {
            break;
        }

        // Procurar o produto pelo ID
        struct Produto *produto = procurarProduto(produtos, numProdutos, idProduto);

        if (produto != NULL) {
            printf("Quantidade a vender (Kg): ");
            if (scanf("%f", &quantidadeVenda) != 1) {
                printf("Entrada invalida para quantidade!\n");
                continue;
            }

            if (quantidadeVenda > 0 && quantidadeVenda <= produto->quantidade) {
                float precoFinal = produto->preco * (1 + MARGEM);
                float valorVenda = precoFinal * quantidadeVenda;
                printf("Preco do produto %s: R$%.2f\n", produto->nome, precoFinal);
                printf("Deseja adicionar um novo produto? s/n");
                scanf("%s", &opcadd);
                if ('s'== opcadd || 'S'== opcadd ){
                    return 1;
                } else {
                    return;
                }
                printf("Valor total da venda: R$%.2f\n", valorVenda);
                total += valorVenda;
                produto->quantidade -= quantidadeVenda;

                // Registrar a venda temporariamente
                vendasTemp[vendasTempCount] = *produto;
                quantidadesVendidas[vendasTempCount] = quantidadeVenda;
                vendasTempCount++;

                contadorVendas++; // Incrementa a contagem de vendas
                registrarVenda(produto->nome, precoFinal, quantidadeVenda, contadorDias); // Registrar a venda no dia correto
                
                }
            } else if (quantidadeVenda <= 0) {
                printf("A quantidade deve ser maior que zero.\n\n");
                break;
            } else {
                printf("Estoque insuficiente para o produto %s. Disponivel: %.2f Kg\n\n", produto->nome, produto->quantidade);
            }
        } else {
            printf("Produto com ID %d nao encontrado.\n\n", idProduto);
        }

        printf("Digite o ID do proximo produto (0 para finalizar): ");
    }

    printf("Total a pagar: R$%.2f\n\n", total);

    // Verificação se o total for igual a zero
    if (total == 0) {
        printf("O valor total não pode ser cobrado pois e igual a zero.\n");
        return;
    }

    // Adicionando opções de pagamento
    int metodoPagamento;
    printf("Escolha o metodo de pagamento:\n");
    printf("1. Dinheiro\n");
    printf("2. Cartao\n");
    printf("3. Pix\n");
    printf("Escolha uma opcao: ");
    scanf("%d", &metodoPagamento);

    while (metodoPagamento < 1 || metodoPagamento > 3) {
        printf("Opcao invalida. Tente novamente: ");
        scanf("%d", &metodoPagamento);
    }
    
    // Variável para a confirmação de pagamento
    char pagamentoAprovado;

    if (metodoPagamento == 2) {
        int tipoCartao;
        printf("Escolha o tipo de cartao:\n");
        printf("1. Credito\n");
        printf("2. Debito\n");
        printf("3. Voucher\n");
        printf("Escolha uma opcao: ");
        scanf("%d", &tipoCartao);

        while (tipoCartao < 1 || tipoCartao > 3) {
        printf("Opcao invalida. Tente novamente: ");
        scanf("%d", &tipoCartao);
        }

        
        printf("O pagamento foi aprovado? (s/n): ");
        scanf(" %c", &pagamentoAprovado);

        if (pagamentoAprovado == 'n' || pagamentoAprovado == 'N') {
            printf("Pagamento nao aprovado. Cancelando a venda.\n");

            // Reverter as vendas
            for (int i = 0; i < vendasTempCount; i++) {
                int idProdutoTemp = vendasTemp[i].id;
                struct Produto *produto = procurarProduto(produtos, numProdutos, idProdutoTemp);
                if (produto != NULL) {
                    produto->quantidade += quantidadesVendidas[i]; // Retorna ao estoque
                    printf("Produto %s retornado ao estoque.\n", produto->nome);
                }
            }
            return;  // Finaliza a função, cancelando a venda
        } else {
            printf("Pagamento aprovado e processado para o tipo de cartao.\n");
        }

    } else if (metodoPagamento == 1) {
        printf("Pagamento em dinheiro selecionado.\n");
        printf("O pagamento em dinheiro foi aprovado? (s/n): ");
        scanf(" %c", &pagamentoAprovado);

        if (pagamentoAprovado == 'n' || pagamentoAprovado == 'N') {
            printf("Pagamento em dinheiro nao aprovado. Cancelando a venda.\n");

            // Reverter as vendas
            for (int i = 0; i < vendasTempCount; i++) {
                int idProdutoTemp = vendasTemp[i].id;
                struct Produto *produto = procurarProduto(produtos, numProdutos, idProdutoTemp);
                if (produto != NULL) {
                    produto->quantidade += quantidadesVendidas[i]; // Retorna ao estoque
                    printf("Produto %s retornado ao estoque.\n", produto->nome);
                }
            }
            return;
        } else {
            printf("Pagamento em dinheiro realizado com sucesso.\n\n");
        }

    } else if (metodoPagamento == 3) {
        printf("Pagamento via Pix selecionado.\n");
        printf("O pagamento via Pix foi aprovado? (s/n): ");
        scanf(" %c", &pagamentoAprovado);

        if (pagamentoAprovado == 'n' || pagamentoAprovado == 'N') {
            printf("Pagamento via Pix nao aprovado. Cancelando a venda.\n");

            // Reverter as vendas
            for (int i = 0; i < vendasTempCount; i++) {
                int idProdutoTemp = vendasTemp[i].id;
                struct Produto *produto = procurarProduto(produtos, numProdutos, idProdutoTemp);
                if (produto != NULL) {
                    produto->quantidade += quantidadesVendidas[i]; // Retorna ao estoque
                    printf("Produto %s retornado ao estoque.\n", produto->nome);
                }
            }
            return;
        } else {
            printf("Pagamento via Pix realizado com sucesso.\n");
        }
    } else {
        printf("Metodo de pagamento invalido. Tente novamente.\n");
        return;
    }

    salvarProdutos(produtos, *contador); // Salva o estado atualizado dos produtos
}