#include <stdio.h>
#include <string.h>

#define SHIFT 3
#define MAX_USERS 100

typedef struct {
    int id;
    char name[50];
    char email[50];
    char password[50];
} User;

void encrypt(char *data) {
    for (int i = 0; i < strlen(data); i++) {
        data[i] = data[i] + SHIFT;
    }
}

void decrypt(char *data) {
    for (int i = 0; i < strlen(data); i++) {
        data[i] = data[i] - SHIFT;
    }
}

void save_user_data(User users[], int num_users) {
    FILE *file = fopen("MANBOR.txt", "w");
    if (file == NULL) {
        printf("Erro ao abrir o arquivo!\n");
        return;
    }

    for (int i = 0; i < num_users; i++) {
        char encrypted_name[50], encrypted_email[50], encrypted_password[50];
        strcpy(encrypted_name, users[i].name);
        encrypt(encrypted_name);
        strcpy(encrypted_email, users[i].email);
        encrypt(encrypted_email);
        strcpy(encrypted_password, users[i].password);
        encrypt(encrypted_password);

        fprintf(file, "ID: %d\n", users[i].id);
        fprintf(file, "Nome (Criptografado): %s\n", encrypted_name);
        fprintf(file, "Nome (Descriptografado): %s\n", users[i].name);
        fprintf(file, "Email (Criptografado): %s\n", encrypted_email);
        fprintf(file, "Email (Descriptografado): %s\n", users[i].email);
        fprintf(file, "Senha (Criptografada): %s\n", encrypted_password);
        fprintf(file, "Senha (Descriptografada): %s\n", users[i].password);
        fprintf(file, "-----------------------------------------\n");
    }

    fclose(file);
}

int load_user_data(User users[], int *num_users) {
    FILE *file = fopen("MANBOR.txt", "r");
    if (file == NULL) {
        return 0;
    }

    while (fscanf(file, "ID: %d\n", &users[*num_users].id) != EOF) {
        fscanf(file, "Nome (Criptografado): %s\n", users[*num_users].name);
        fscanf(file, "Nome (Descriptografado): %s\n", users[*num_users].name);
        fscanf(file, "Email (Criptografado): %s\n", users[*num_users].email);
        fscanf(file, "Email (Descriptografado): %s\n", users[*num_users].email);
        fscanf(file, "Senha (Criptografada): %s\n", users[*num_users].password);
        fscanf(file, "Senha (Descriptografada): %s\n", users[*num_users].password);
        fscanf(file, "-----------------------------------------\n");

        (*num_users)++;
    }

    fclose(file);
    return 1;
}

void add_user(User users[], int *num_users) {
    User new_user;
    printf("Coloque o nome de usuario: ");
    scanf("%s", new_user.name);
    printf("Coloque o Email do usuario: ");
    scanf("%s", new_user.email);
    printf("Coloque a senha: ");
    scanf("%s", new_user.password);

    new_user.id = *num_users + 1;
    users[*num_users] = new_user;
    (*num_users)++;
    save_user_data(users, *num_users);
    printf("Usuario adicionado com sucesso!\n");
}

void modify_user(User users[], int num_users) {
    int id;
    printf("Coloque o ID do usuario para modificar: ");
    scanf("%d", &id);
    for (int i = 0; i < num_users; i++) {
        if (users[i].id == id) {
            printf("Coloque o novo nome de usuario: ");
            scanf("%s", users[i].name);
            printf("Coloque o novo Email do usuario: ");
            scanf("%s", users[i].email);
            printf("Coloque a nova senha: ");
            scanf("%s", users[i].password);

            save_user_data(users, num_users);
            printf("Usuario modificado com sucesso!\n");
            return;
        }
    }
    printf("Usuario nao encontrado!\n");
}

void delete_user(User users[], int *num_users) {
    int id;
    printf("Coloque o ID do usuario para excluir: ");
    scanf("%d", &id);
    for (int i = 0; i < *num_users; i++) {
        if (users[i].id == id) {
            for (int j = i; j < *num_users - 1; j++) {
                users[j] = users[j + 1];
            }
            (*num_users)--;

            save_user_data(users, *num_users);
            printf("Usuario excluido com sucesso!\n");
            return;
        }
    }
    printf("Usuario nao encontrado!\n");
}

void list_users(User users[], int num_users) {
    printf("Lista de usuarios:\n");
    for (int i = 0; i < num_users; i++) {
        printf("ID: %d, Nome: %s, Email: %s\n", users[i].id, users[i].name, users[i].email);
    }
}

int login(User users[], int num_users) {
    char name[50];
    char password[50];
    printf("Login\n");
    printf("Coloque o nome de usuario: ");
    scanf("%s", name);
    printf("Coloque a senha: ");
    scanf("%s", password);

    for (int i = 0; i < num_users; i++) {
        if (strcmp(users[i].name, name) == 0 && strcmp(users[i].password, password) == 0) {
            printf("Login bem-sucedido!\n");
            return 1;
        }
    }
    printf("Nome de usuario ou senha incorretos!\n");
    return 0;
}

void initialize_admin(User users[], int *num_users) {
    if (*num_users == 0) {
        printf("Nenhum usuario registrado. Crie uma conta de administrador.\n");
        add_user(users, num_users);
    }
}

int main() {
    User users[MAX_USERS];
    int num_users = 0;

    load_user_data(users, &num_users);

    initialize_admin(users, &num_users);

    int logged_in = 0;
    while (!logged_in) {
        logged_in = login(users, num_users);
    }

    int option;
    do {
        printf("\nSistema de Controle de Usuarios\n");
        printf("1. Adicionar usuario\n");
        printf("2. Modificar usuario\n");
        printf("3. Deletar usuario\n");
        printf("4. Lista de usuarios\n");
        printf("5. Sair\n");
        printf("Coloque a sua opcao desejada: ");
        scanf("%d", &option);

        switch (option) {
            case 1:
                add_user(users, &num_users);
                break;
            case 2:
                modify_user(users, num_users);
                break;
            case 3:
                delete_user(users, &num_users);
                break;
            case 4:
                list_users(users, num_users);
                break;
            case 5:
                printf("Saindo...\n");
                break;
            default:
                printf("Opcao invalida!\n");
        }
    } while (option != 5);

    return 0;
}

