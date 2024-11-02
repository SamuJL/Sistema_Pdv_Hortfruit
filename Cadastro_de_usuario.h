#ifndef CADASTRO_DE_USUARIO_INCLUDED
#define CADASTRO_DE_USUARIO_INCLUDED

int contadorVendas = 0; // Contador global de vendas
int contadorDias = 1;   // Contador global de dias

struct Usuario { 
    char nomeUs[50]; 
    char senha[50]; 
    char funcionario; 
}

void limparTela();

int senhaExistente(char *senha);

void cadastrarUsuario();

int verificarUsuario(char *nome, char *senha);

int fazerLogin(char *nomeUsuario);

int verificarPermissao(char *nomeUsuario);

#endif
