#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_PRODUTOS 100
#define ARQUIVO "produtos.txt"
#define MARGEM 0.20

int contadorVendas = 0; // Contador global de vendas
int contadorDias = 1;   // Contador global de dias

struct Usuario { 
    char nomeUs[50]; 
    char senha[50]; 
    char funcionario; 
};

void limparTela() {
    #ifdef _WIN32
        system("cls");
    #else
        system("clear");
    #endif
}

int senhaExistente(char *senha) {
    FILE *arquivo = fopen("Usuario.txt", "r");
    if (arquivo == NULL) {
        return 0; // Arquivo não encontrado, então nenhuma senha existe ainda
    }

    struct Usuario usuario;
    while (fscanf(arquivo, "%s %s %c", usuario.nomeUs, usuario.senha, &usuario.funcionario) != EOF) {
        if (strcmp(usuario.senha, senha) == 0) {
            fclose(arquivo);
            return 1; // Senha ja existe
        }
    }
    fclose(arquivo);
    return 0; // Senha não existe
}

void cadastrarUsuario() {    
    limparTela();
    
    int j = 0;
    char cr;
    
    FILE *arquivo = fopen("Usuario.txt", "a");
    if (arquivo == NULL) {
        printf("Erro ao abrir o arquivo!\n");
        return;
    }

    struct Usuario usuario;
    printf("Digite o nome do usuario: ");
    scanf("%49s", usuario.nomeUs);
    
    do {   
        printf("Digite a senha do usuario: ");
    
        while ((cr = getch()) != '\r') { // '\r' é o Enter
            if (cr == '\b') { // '\b' é o Backspace
                if (j > 0) {
                    j--;
                    printf("\b \b"); // Apaga o último caractere
                }
            } else if (j < 49){ // evita overflow 
                usuario.senha[j++] = cr; // Preenche senha
                printf("*"); // Exibe um asterisco
            }
        }
        usuario.senha[j] = '\0'; // Termina a string


        if (senhaExistente(usuario.senha)) { 
            // 20/       --------------------
            printf("\n-----------------------------------------------------------\n");
            printf("*Senha ja existente!!! Digite uma senha diferente.");
            printf("\n-----------------------------------------------------------\n\n");                
        } else {
            break;
        }
    } while (1);

    printf("\nO usuario e administrador? [S/N] ");
    scanf(" %c", &usuario.funcionario);
    limparTela();

    if (usuario.funcionario == 'S' || usuario.funcionario == 's' ) {
        printf("\n-----------------------------------------------------------\n");
        printf("*Cadastro de usuario do tipo administrador concluido.");
        printf("\n-----------------------------------------------------------\n\n");
    } else if (usuario.funcionario == 'N' || usuario.funcionario == 'n') {
        printf("\n-----------------------------------------------------------\n");
        printf("*Cadastro de usuario do tipo funcionario concluido.");
        printf("\n-----------------------------------------------------------\n\n");
    } else {
        printf("\n-----------------------------------------------------------\n");
        printf("*Opcao invalida!");
        printf("\n-----------------------------------------------------------\n\n");
        fclose(arquivo);
        return;
    }
    fprintf(arquivo, "%s %s %c\n", usuario.nomeUs, usuario.senha, usuario.funcionario); 
    fclose(arquivo);
    printf("\n-----------------------------------------------------------\n");
    printf("*Cadastro salvo com sucesso!");
    printf("\n-----------------------------------------------------------\n\n");
}

int verificarUsuario(char *nome, char *senha) {
    FILE *arquivo = fopen("Usuario.txt", "r");
    if (arquivo == NULL) {
        printf("Erro ao abrir o arquivo!\n");
        return -1;
    }

    struct Usuario usuario;
    while (fscanf(arquivo, "%s %s %c", usuario.nomeUs, usuario.senha, &usuario.funcionario) != EOF) {
        if (strcmp(usuario.nomeUs, nome) == 0 && strcmp(usuario.senha, senha) == 0) {
            fclose(arquivo);
            return 1; 
        }
    }
    fclose(arquivo);
    return 0;
}

int fazerLogin(char *nomeUsuario) { //funcao de fazer login, inicio do programa
    char senhaUsuario[50];
    int i = 0;
    char ch;
    printf("Digite a senha do usuario: ");

    while ((ch = getch()) != '\r') { // '\r' é o Enter
        if (ch == '\b') { // '\b' é o Backspace
            if (i > 0) {
                i--;
                printf("\b \b"); // Apaga o último caractere
            }
        } else if (i < 49){ // evita overflow 
            senhaUsuario[i++] = ch; // Preenche senha
            printf("*"); // Exibe um asterisco
        }
    }
    senhaUsuario[i] = '\0'; // Termina a string

    limparTela();

    // Validando o login admin para primeiro acesso 
    if (strcmp(nomeUsuario, "admin") == 0 && strcmp(senhaUsuario, "admin") == 0) {
        return 1; // Login bem-sucedido
    } else if (verificarUsuario(nomeUsuario, senhaUsuario)) {
        return 1;
    }else {
        printf("\n-----------------------------------------------------------\n");
        printf("*Acesso Negado. Nome ou senha de usuario incorretos."); 
        printf("\n-----------------------------------------------------------\n\n");
        return 0; // Falha no login
    }
}

int verificarPermissao(char *nomeUsuario) {
    if (strcmp(nomeUsuario, "admin") == 0) {
        printf("\nbem-vindo admin!\n");
        return 1; // Admin tem acesso total (menu de administrador)
    }

    FILE *arquivo = fopen("Usuario.txt", "r");
    if (arquivo == NULL) {
        printf("Erro ao abrir o arquivo!\n");
        return 0;
    }

    struct Usuario usuario;
    while (fscanf(arquivo, "%s %s %c", usuario.nomeUs, usuario.senha, &usuario.funcionario) != EOF) {
        if (strcmp(usuario.nomeUs, nomeUsuario) == 0) {
            fclose(arquivo);
            // Retorna 1 para administrador e 2 para funcionário
            if (usuario.funcionario == 'S' || usuario.funcionario == 's') {
                printf("\nbem-vindo admin!\n");
                return 1; // administrador
            } else if (usuario.funcionario == 'N' || usuario.funcionario == 'n') {
                printf("\nbem-vindo!\n");
                return 2; // funcionário
            }
        }
    }
    fclose(arquivo);
    return 0;
}

// Estrutura para armazenar as informações de um produto
struct Produto {
    int id;               // ID do produto (posição no estoque)
    char nome[50];        // Nome do produto
    float preco;          // Preço do produto
    float quantidade;     // Quantidade em Kg no estoque
};


// Função para salvar os produtos no arquivo
void salvarProdutos(struct Produto produtos[], int contador) {
    FILE *arquivo = fopen(ARQUIVO, "w");
    if (arquivo == NULL) {
        printf("Erro ao abrir o arquivo para escrita.\n");
        return;
    }

    for (int i = 0; i < contador; i++) {
        fprintf(arquivo, "ID:%d | Nome:%s | Preco por Kg: R$%.2f | Quantidade: %.2f Kg\n",
                produtos[i].id, produtos[i].nome, produtos[i].preco, produtos[i].quantidade);
    }

    fclose(arquivo);
}

// Função para carregar os produtos do arquivo
int carregarProdutos(struct Produto produtos[]) {
    FILE *arquivo = fopen(ARQUIVO, "r");
    if (arquivo == NULL) {
        printf("\n-----------------------------------------------------------\n");
        printf("*Nenhum Produto Cadastrado");
        printf("\n-----------------------------------------------------------\n");
        return 0;
    }

    int contador = 0;
    while (fscanf(arquivo, "ID:%d | Nome:%s | Preco por Kg: R$%f | Quantidade: %f Kg\n",
                  &produtos[contador].id, produtos[contador].nome,
                  &produtos[contador].preco, &produtos[contador].quantidade) == 4) {
        contador++;
    }

    fclose(arquivo);
    return contador;
}

// Função para cadastrar um novo produto ou apenas atualizar a quantidade
void cadastrarProduto(struct Produto produtos[], int *contador, int modoAtualizacao, int idProdutoAtualizar) {
    limparTela();
    
    if (modoAtualizacao == 0 && *contador >= MAX_PRODUTOS) {
        printf("\n-----------------------------------------------------------\n");
        printf("*Limite de produtos cadastrados atingido.");
        printf("\n-----------------------------------------------------------\n\n");
        return;
    }

    struct Produto novoProduto;
    if (modoAtualizacao == 1) {
        novoProduto = produtos[idProdutoAtualizar];
        printf("Atualizando quantidade do produto %s (ID %d)\n", novoProduto.nome, novoProduto.id);
    } else {
        novoProduto.id = *contador + 1; // Define a posição no estoque com base no contador atual
        printf("\nNome do produto: ");
        getchar(); // Para consumir o caractere de nova linha deixado pelo scanf anterior
        fgets(novoProduto.nome, sizeof(novoProduto.nome), stdin);
        novoProduto.nome[strcspn(novoProduto.nome, "\n")] = 0; // Remove a nova linha

        printf("Preco do produto por Kg: ");
        if (scanf("%f", &novoProduto.preco) != 1) {
            printf("\n-----------------------------------------------------------\n");
            printf("*Entrada invalida para preco!");
            printf("\n-----------------------------------------------------------\n\n");
            return;
        }
    }

    printf("Quantidade no estoque (Kg): ");
    if (scanf("%f", &novoProduto.quantidade) != 1) { // Alterado para float
        printf("\n-----------------------------------------------------------\n");
        printf("*Entrada invalida para quantidade!");
        printf("\n-----------------------------------------------------------\n\n");
        return;
    }

    if (modoAtualizacao == 1) {
        produtos[idProdutoAtualizar].quantidade += novoProduto.quantidade;
    } else {
        produtos[*contador] = novoProduto;
        (*contador)++;
    }

    printf("\n-----------------------------------------------------------\n");
    printf("*Produto atualizado com sucesso!");
    printf("\n-----------------------------------------------------------\n\n");

    // Salva os produtos no arquivo
    salvarProdutos(produtos, *contador);
}

// Função para listar todos os produtos cadastrados
void listarProdutos(struct Produto produtos[], int contador) {
    limparTela();
    if (contador == 0) {
        printf("\n-----------------------------------------------------------\n");
        printf("*Nenhum produto cadastrado.");
        printf("\n-----------------------------------------------------------\n\n");
        return;
    }

    printf("\nLista de produtos cadastrados:\n");
    for (int i = 0; i < contador; i++) {
        printf("\n---------------------------------------------------------------------\n");
        printf("ID: %d | Nome: %s | Preco por Kg: R$ %.2f | Quantidade: %.2f Kg",
               produtos[i].id, produtos[i].nome, produtos[i].preco, produtos[i].quantidade);
        printf("\n---------------------------------------------------------------------\n");
    }
}

// Função para procurar um produto pelo ID
struct Produto* procurarProduto(struct Produto produtos[], int numProdutos, int id) {
    for (int i = 0; i < numProdutos; i++) {
        if (produtos[i].id == id) {
            return &produtos[i];
        }
    }
    return NULL;
}

// Função para salvar o valor de contadorDias em um arquivo
void salvarContadorDias() {
    FILE *arquivo = fopen("contadorDias.txt", "w");
    if (arquivo == NULL) {
        printf("Erro ao salvar o contador de dias.\n");
        return;
    }
    fprintf(arquivo, "%d", contadorDias);
    fclose(arquivo);
}

// Função para carregar o valor de contadorDias a partir de um arquivo
void carregarContadorDias() {
    FILE *arquivo = fopen("contadorDias.txt", "r");
    if (arquivo == NULL) {
        printf("Nenhum contador de dias encontrado. Iniciando com o dia 1.\n");
        contadorDias = 1; // Se não houver arquivo, começa com o dia 1.
        return;
    }
    fscanf(arquivo, "%d", &contadorDias);
    fclose(arquivo);
}

// Função para registrar uma venda
void registrarVenda(const char *nomeProduto, float precoFinal, float quantidadeVendida, int dia) {
    if (precoFinal <= 0 || quantidadeVendida <= 0) {
        printf("Erro: Preço ou quantidade inválidos.\n");
        return; // Valida se preço e quantidade são positivos
    }

    char arquivoVendas[50];
    sprintf(arquivoVendas, "vendas_dia_%d.txt", dia); // Cria o nome do arquivo para o dia específico

    FILE *arquivo = fopen(arquivoVendas, "a");
    if (arquivo == NULL) {
        printf("Erro ao abrir o arquivo de vendas.\n");
        return;
    }

    fprintf(arquivo, "Produto: %s | Preco com margem: R$%.2f | Quantidade vendida: %.2f Kg\n", nomeProduto, precoFinal, quantidadeVendida);
    fclose(arquivo);
}

// Função para finalizar o dia e salvar o contador de dias
void finalizarDia() {
    limparTela();
    printf("\n-----------------------------------------------------------\n");
    printf("Finalizando o dia %d...\n", contadorDias);
    printf("Dia %d finalizado com sucesso!\n", contadorDias);
    printf("\n-----------------------------------------------------------\n\n");

    contadorDias++;
    salvarContadorDias(); // Salva o contador de dias no arquivo
}

void imprimirNotaFiscal(char vendedor[], struct Produto produtos[], int numProdutos, float pagamento) {

printf("                                                             ===============================================\n");
printf("                                                            |                    PEDIDO                     |\n");
printf("                                                            |-----------------------------------------------|\n");
printf("                                                            |                  HORTIFRUTI                   |\n");
printf("                                                            |                                               |\n");
printf("                                                            |-----------------------------------------------|\n");
printf("                                                            | VENDEDOR :  %-12s                      |\n", vendedor);
printf("                                                            |===============================================|\n");
printf("                                                            | ID   NOME            Qtd    R$/Kg      Total  |\n"); 
printf("                                                            |===============================================|\n"); 

// Laço para listar os produtos
for (int i = 0; i < numProdutos; i++) {
    printf("                                                            | %-4d %-15s %-6.2f %-10.2f %-8.2f |\n", produtos[i].id, produtos[i].nome, produtos[i].quantidade, produtos[i].preco, pagamento);
}

printf("                                                            |===============================================|\n");  
printf("                                                            | %02d ITEM(S)                          %.2f      |\n", numProdutos, pagamento);
printf("                                                            |===============================================|\n"); 
printf("                                                            |                   DINHEIRO :         %.2f     |\n", pagamento);
printf("                                                            |                             ------------------|\n");    
printf("                                                            |                                               |\n");
printf("                                                            |-----------------------------------------------|\n");
printf("                                                            | Obrigado pela preferencia.                    |\n");
printf("                                                             =============================================== \n\n\n"); 

}



// Função para realizar a venda com opções de pagamento
void realizarVenda(struct Produto produtos[], int numProdutos, int *contador, char vendedor[]) {
    limparTela();
    
    int idProduto;
    float quantidadeVenda;
    float total = 0.0;

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
                printf("\n-----------------------------------------------------------\n");
                printf("*Entrada invalida para quantidade!");
                printf("\n-----------------------------------------------------------\n\n");
                continue;
            }

            if (quantidadeVenda > 0 && quantidadeVenda <= produto->quantidade) {
                float precoFinal = produto->preco * (1 + MARGEM);
                float valorVenda = precoFinal * quantidadeVenda;
                printf("Preco do produto %s: R$%.2f\n", produto->nome, precoFinal);
                printf("Valor total da venda: R$%.2f\n", valorVenda);
                total += valorVenda;
                produto->quantidade -= quantidadeVenda;

                // Registrar a venda temporariamente
                vendasTemp[vendasTempCount] = *produto;
                quantidadesVendidas[vendasTempCount] = quantidadeVenda;
                vendasTempCount++;

                contadorVendas++; // Incrementa a contagem de vendas
                registrarVenda(produto->nome, precoFinal, quantidadeVenda, contadorDias); // Registrar a venda no dia correto

                // Verificar se o estoque zerou e perguntar se deseja atualizar
                if (produto->quantidade == 0) {
                    char resposta;
                    printf("\n-----------------------------------------------------------\n");
                    printf("*O estoque do produto %s acabou. Deseja repor? (s/n): ", produto->nome);
                    scanf(" %c", &resposta);
                    printf("\n-----------------------------------------------------------\n\n");
                    if (resposta == 's' || resposta == 'S') {
                        cadastrarProduto(produtos, contador, 1, produto->id - 1);
                    }
                }
            } else if (quantidadeVenda <= 0) {
                printf("\n-----------------------------------------------------------\n");
                printf("*A quantidade deve ser maior que zero.");
                printf("\n-----------------------------------------------------------\n\n");
                break;
            } else {
                printf("\n-----------------------------------------------------------\n");
                printf("*Estoque insuficiente para o produto %s. Disponivel: %.2f Kg\n\n", produto->nome, produto->quantidade);
                printf("\n-----------------------------------------------------------\n\n");
            }
        } else {
            printf("\n-----------------------------------------------------------\n");
            printf("*Produto com ID %d nao encontrado.\n\n", idProduto);
            printf("\n-----------------------------------------------------------\n\n");
        }

        printf("Digite o ID do proximo produto (0 para finalizar): ");
    }

    printf("Total a pagar: R$%.2f\n\n", total);

    // Verificação se o total é igual a zero
    if (total == 0) {
        printf("\n-----------------------------------------------------------\n");
        printf("*O valor total nao pode ser cobrado pois e igual a zero.\n");
        printf("\n-----------------------------------------------------------\n\n");
        return;
    }
    
    imprimirNotaFiscal(vendedor, vendasTemp, vendasTempCount, total);

    // Adicionando opções de pagamento
    int metodoPagamento;
    printf("Escolha o metodo de pagamento:\n");
    printf("1. Dinheiro\n");
    printf("2. Cartao\n");
    printf("3. Pix\n");
    printf("Escolha uma opcao: ");
    scanf("%d", &metodoPagamento);

    while (metodoPagamento < 1 || metodoPagamento > 3) {
        printf("\n-----------------------------------------------------------\n");
        printf("*Opcao invalida. Tente novamente: ");
        scanf("%d", &metodoPagamento);
        printf("\n-----------------------------------------------------------\n\n");
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
        printf("\n-----------------------------------------------------------\n");
        printf("*Opcao invalida. Tente novamente: ");
        scanf("%d", &tipoCartao);
        printf("\n-----------------------------------------------------------\n\n");
        }

        
        printf("O pagamento foi aprovado? (s/n): ");
        scanf(" %c", &pagamentoAprovado);

        if (pagamentoAprovado == 'n' || pagamentoAprovado == 'N') {
            printf("\n-----------------------------------------------------------\n");
            printf("*Pagamento nao aprovado. Cancelando a venda.");
            printf("\n-----------------------------------------------------------\n\n");

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
            printf("\n-----------------------------------------------------------\n");
            printf("*Pagamento aprovado e processado para o tipo de cartao.");
            printf("\n-----------------------------------------------------------\n\n");
        }

    } else if (metodoPagamento == 1) {
        printf("Pagamento em dinheiro selecionado.\n");
        printf("O pagamento em dinheiro foi aprovado? (s/n): ");
        scanf(" %c", &pagamentoAprovado);

        if (pagamentoAprovado == 'n' || pagamentoAprovado == 'N') {
            printf("\n-----------------------------------------------------------\n");
            printf("*Pagamento em dinheiro nao aprovado. Cancelando a venda.");
            printf("\n-----------------------------------------------------------\n\n");

            // Reverter as vendas
            for (int i = 0; i < vendasTempCount; i++) {
                int idProdutoTemp = vendasTemp[i].id;
                struct Produto *produto = procurarProduto(produtos, numProdutos, idProdutoTemp);
                if (produto != NULL) {
                    produto->quantidade += quantidadesVendidas[i]; // Retorna ao estoque
                    printf("\n-----------------------------------------------------------\n");
                    printf("*Produto %s retornado ao estoque.\n", produto->nome);
                    printf("\n-----------------------------------------------------------\n\n");
                }
            }
            return;
        } else {
            printf("\n-----------------------------------------------------------\n");
            printf("*Pagamento em dinheiro realizado com sucesso.");
            printf("\n-----------------------------------------------------------\n\n");
        }

    } else if (metodoPagamento == 3) {
        printf("Pagamento via Pix selecionado.\n");
        printf("O pagamento via Pix foi aprovado? (s/n): ");
        scanf(" %c", &pagamentoAprovado);

        if (pagamentoAprovado == 'n' || pagamentoAprovado == 'N') {
            printf("\n-----------------------------------------------------------\n");
            printf("*Pagamento via Pix nao aprovado. Cancelando a venda.");
            printf("\n-----------------------------------------------------------\n\n");

            // Reverter as vendas
            for (int i = 0; i < vendasTempCount; i++) {
                int idProdutoTemp = vendasTemp[i].id;
                struct Produto *produto = procurarProduto(produtos, numProdutos, idProdutoTemp);
                if (produto != NULL) {
                    produto->quantidade += quantidadesVendidas[i]; // Retorna ao estoque
                    printf("\n-----------------------------------------------------------\n");
                    printf("*Produto %s retornado ao estoque.\n", produto->nome);
                    printf("\n-----------------------------------------------------------\n\n");
                }
            }
            return;
        } else {
            printf("\n-----------------------------------------------------------\n");
            printf("*Pagamento via Pix realizado com sucesso.");
            printf("\n-----------------------------------------------------------\n\n");
        }
    } else {
        printf("\n-----------------------------------------------------------\n");
        printf("*Metodo de pagamento invalido. Tente novamente.");
        printf("\n-----------------------------------------------------------\n\n");
        return;
    }

    salvarProdutos(produtos, *contador); // Salva o estado atualizado dos produtos
    
}

int main() {
    char nomeUsuario[50];

    carregarContadorDias();

    printf("Digite o nome do usuario: ");
    scanf("%49s", nomeUsuario);
    while (getchar() != '\n');  // Limpa o buffer

    // Tentar fazer login 
    if (!fazerLogin(nomeUsuario)) {
        return 0; // Encerra o programa se o login falhar
    }

    int permissao = verificarPermissao(nomeUsuario);
    if (permissao == -1) {
        printf("Erro ao abrir o arquivo. Encerrando o programa.\n");
        return 1;
    } else if (permissao == 0) {
        printf("Erro: Usuario nao encontrado.\n");
        return 1;
    } // permissao 

    struct Produto produtos[MAX_PRODUTOS];
    int contador = carregarProdutos(produtos); // Carrega os produtos ao iniciar
    int opcao; // variável para a opção de cadastro

    // Loop para mostrar o menu até que o usuário escolha sair
    while (1) {
        // Menu de opções
        printf("Escolha uma das opcoes: \n");
        printf(" __________________________\n");
        if (permissao == 1) { // Menu para administradores
            printf("|1-Cadastrar usuario       |\n");
            printf("|2-Cadastrar novo produto  |\n");
            printf("|3-Listar Produtos         |\n");
            printf("|4-Venda                   |\n");
            printf("|5-Finalizar dia           |\n");
            printf("|6-Sair                    |\n");
        } else if (permissao == 2) { // Menu para funcionários
            printf("|1-Listar produtos         |\n");
            printf("|2-Venda                   |\n");
            printf("|3-Finalizar dia           |\n");
            printf("|4-Sair                    |\n");
        }
        printf("|__________________________|\n\n");
        scanf("%d", &opcao); // Lê a opção do usuário

        // Estrutura switch para opções
        switch (opcao) {
            case 1:
                if (permissao == 1) {
                    cadastrarUsuario(); //chama a função de cadastro de usuario, tipo void
                } else {
                    listarProdutos(produtos, contador);
                }
                break;
            case 2:
                if (permissao == 1) {
                    cadastrarProduto(produtos, &contador, 0, 0); // Cadastro de produto
                } else {
                    realizarVenda(produtos, contador, &contador, nomeUsuario); // Chama a funcao de venda
                }
                break;
            case 3:
                if (permissao == 1) {
                    listarProdutos(produtos, contador);
                } else {
                    finalizarDia(); // Chama a função de finalizar o dia
                }     
                break;
            case 4:
                if (permissao == 1) {
                    realizarVenda(produtos, contador, &contador, nomeUsuario); // Chama a funcao de venda
                } else {
                    printf("\n-----------------------------------------------------------\n");
                    printf("*Saindo do sistema...");
                    printf("\n-----------------------------------------------------------\n\n");
                    return 0;
                }
                break;
            case 5:
                if (permissao == 1) {
                    finalizarDia(); // Chama a função de finalizar o dia
                } else {
                    printf("\n-----------------------------------------------------------\n");
                    printf("*Opcao invalida. Tente novamente.");
                    printf("\n-----------------------------------------------------------\n\n");
                }
                break;
            case 6:
                if (permissao == 1) {
                    printf("\n-----------------------------------------------------------\n");
                    printf("*Saindo do sistema...");
                    printf("\n-----------------------------------------------------------\n\n");
                return 0;
                } else {
                    printf("\n-----------------------------------------------------------\n");
                    printf("*Opcao invalida. Tente novamente.");
                    printf("\n-----------------------------------------------------------\n\n");
                }
                break;
            default:
                printf("\n-----------------------------------------------------------\n");
                printf("*Opcao invalida. Tente novamente.");
                printf("\n-----------------------------------------------------------\n\n");
                break;
        }
        printf("\nPressione Enter para continuar...");
        getchar(); // Consumir newline restante
        getchar(); // Aguarda o Enter
        limparTela();
    }
    return 0;
}