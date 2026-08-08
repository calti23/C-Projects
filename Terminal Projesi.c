#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>

void color_changer(){
	system("COLOR 2");
}

char **parse_command(char *line) {
    int bufsize = 64, position = 0;
    char **tokens = (char **)malloc(bufsize * sizeof(char*));
    char *token;

    if(!tokens) {
        fprintf(stderr, "Memory Error!!!\n");
        exit(EXIT_FAILURE);
    }

    token = strtok(line, " \t\r\n\a");
    while(token != NULL) {
        tokens[position++] = token;

        if(position >= bufsize) {
            bufsize += 64;
            tokens = (char **)realloc(tokens, bufsize * sizeof(char*));
            if(!tokens) {
                fprintf(stderr, "Memory Error!!!\n");
                exit(EXIT_FAILURE);
            }
        }
        token = strtok(NULL, " \t\r\n\a");
    }
    tokens[position] = NULL;
    return tokens;
}

void launch_command(char *full_line) {
    STARTUPINFO si;
    PROCESS_INFORMATION pi;
    char cmd_line[1024];

    ZeroMemory(&si, sizeof(si));
    si.cb = sizeof(si);
    ZeroMemory(&pi, sizeof(pi));

    strcpy(cmd_line, full_line);

    if (!CreateProcess(NULL, cmd_line, NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi)) {
        printf("Command was not executed or was not found.\n");
        return;
    }

    WaitForSingleObject(pi.hProcess, INFINITE);

    CloseHandle(pi.hProcess);
    CloseHandle(pi.hThread);
}

int main() {
    char input[1024];
    char **args;
    
    color_changer();

    while(1) {
        printf("windows11-terminal> ");
        fflush(stdout);

        if(fgets(input, sizeof(input), stdin) == NULL) {
            break; 
        }

        input[strcspn(input, "\n")] = 0;

        if(strlen(input) == 0) {
            continue;
        }
        
        char input_copy[1024];
        strcpy(input_copy, input);

        args = parse_command(input_copy);
        if(args[0] == NULL) {
            free(args);
            continue;
        }

        if (strcmp(args[0], "cd") == 0) {
            if (args[1] == NULL) {
                fprintf(stderr, "windows11-terminal: arguement is not enough for cd.\n");
            } else {
                if (SetCurrentDirectory(args[1]) == 0) {
                    printf("The directory could not be changed.\n");
                }
            }
        }
        else if (strcmp(args[0], "exit") == 0) {
            free(args);
            exit(0);
        }
        else {
            launch_command(input);
        }
        free(args); 
    }
    return 0;
}
