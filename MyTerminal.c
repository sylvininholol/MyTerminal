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
    pid_t pid = fork();

    if (pid == -1) {
        perror("fork");
        exit(EXIT_FAILURE);
    } 
    else if (pid == 0) {  // Processo filho
        char *args[MAX_INPUT_SIZE];
        int arg_count = 0;
        char *token = strtok(command, " ");

        while (token != NULL) {
            args[arg_count++] = token;
            token = strtok(NULL, " ");
        }

        args[arg_count] = NULL;

        // Executa o comando
        int bandeira = execvp(args[0], args);


        if(bandeira == -1){
        perror("execvp");
        exit(EXIT_FAILURE);
        }

    } 
    else {  // Processo pai
        int status;
        waitpid(pid, &status, 0);
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

        // Adiciona o comando ao histórico
        add_history(input);

        // Executa o comando
        execute_command(input);

        // Libera a memória alocada por readline
        free(input);
    }

    return 0;
}
