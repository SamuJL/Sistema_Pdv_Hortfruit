#include "Cadastro_de_usuario"

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
    FILE *arquivo = fopen("Usuario.txt", "a");
    if (arquivo == NULL) {
        printf("Erro ao abrir o arquivo!\n");
        return;
    }

    struct Usuario usuario;
    printf("Digite o nome do usuario: ");
    scanf("%49s", usuario.nomeUs);
    getchar();

    do {
        printf("Digite a senha do usuario: ");
        scanf("%49s", usuario.senha);
        getchar();

        if (senhaExistente(usuario.senha)) {
            printf("Esta senha ja existe! Digite uma senha diferente.\n");
        } else {
            break;
        }
    } while (1);

    printf("O usuario e funcionario? [S/N] ");
    scanf(" %c", &usuario.funcionario);
    if (usuario.funcionario == 'S' || usuario.funcionario == 's' ) {
        printf("Cadastro de usuario do tipo funcionario concluido\n");
    } else if (usuario.funcionario == 'N' || usuario.funcionario == 'n') {
        printf("Cadastro de usuario do tipo administrador concluido\n");
    } else {
        printf("Opção invalida\n");
        fclose(arquivo);
        return;
    }
    fprintf(arquivo, "%s %s %c\n", usuario.nomeUs, usuario.senha, usuario.funcionario); 
    fclose(arquivo);
    printf("Cadastro salvo com sucesso!\n");
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
        printf("\nAcesso Negado.\nLogin ou senha incorretos.\n\n"); 
        return 0; // Falha no login
    }
}

int verificarPermissao(char *nomeUsuario) {
    if (strcmp(nomeUsuario, "admin") == 0) {
        printf("\nbem-vindo admin!\n\n");
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
            if (usuario.funcionario == 'N' || usuario.funcionario == 'n') {
                printf("\nbem-vindo admin!\n\n");
                return 1; // administrador
            } else if (usuario.funcionario == 'S' || usuario.funcionario == 's') {
                printf("\nbem-vindo!\n\n");
                return 2; // funcionário
            }
        }
    }
    fclose(arquivo);
    return 0;
}