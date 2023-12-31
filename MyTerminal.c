#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <readline/readline.h>
#include <readline/history.h>

#define ANSI_RESET   "\x1b[0m"
#define ANSI_BLUE    "\x1b[34m"
#define ANSI_YELLOW  "\x1b[33m"
#define MAX_INPUT_SIZE 1024

void execute_command(char *command) {
    // Tokenize a entrada usando ";" como delimitador
    char *commands[MAX_INPUT_SIZE];
    int command_count = 0;
    char *token = strtok(command, ";");

    while (token != NULL) {
        // Remove espaços em branco no início
        while (*token == ' ' || *token == '\t') {
            ++token;
        }

        // Adiciona o comando à lista
        commands[command_count++] = token;
        token = strtok(NULL, ";");
    }

    for (int i = 0; i < command_count; ++i) {
        // Executa cada comando
        pid_t pid = fork();

        if (pid == -1) {
            perror("fork");
            exit(EXIT_FAILURE);
        } 
        else if (pid == 0) {  // Processo filho
            char *args[MAX_INPUT_SIZE];
            int arg_count = 0;
            token = strtok(commands[i], " ");

            while (token != NULL) {
                args[arg_count++] = token;
                token = strtok(NULL, " ");
            }

            args[arg_count] = NULL;

            // Executa o comando
            int bandeira = execvp(args[0], args);

            if (bandeira == -1) {
                perror("execvp");
                exit(EXIT_FAILURE);
            }
        } 
        else {  // Processo pai
            int status;
            waitpid(pid, &status, 0);
        }
    }
}

int main() {
    while (1) {
        // Obtém o comando do usuário usando readline
        char *input = readline(ANSI_BLUE "Terminal do Sylvino" ANSI_RESET ANSI_YELLOW "->" ANSI_RESET);

        // Verifica se o usuário digitou "exit" para sair do shell
        if (input == NULL || strcmp(input, "exit") == 0) {
            free(input);
            break;
        }

        /* Apenas uma feature para deixar mais elegante, com o include do readline podemos usar as setas para
         recuperar o histórico do que escrevemos.*/
        add_history(input);
        execute_command(input);
        free(input);
    }

    return 0;
}
