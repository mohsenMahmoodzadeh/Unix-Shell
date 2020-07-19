#include<unistd.h> //fork()
#include<stdio.h> //stderr
#include<stdlib.h> //execvp(), exit(),EXIT_FAILURE,EXIT_SUCCESS, malloc(),realloc()
#include <signal.h>
#include<sys/wait.h>
#include<string.h>

#define BUFFER_SIZE 1024
#define TOKEN_BUFFER_SIZE 128
#define TOKEN_DELIMITERS " \t\r\n\a"
#define QUIT "quit"
#define USER_ID "mohsen"
#define SEPRATOR "@"
#define HOST_ID "rash"
#define TERMINATOR ":~"
#define DOLLAR_SIGN "$"
#define HOME_DIR "/home"
char cwd[1024];

int changeDirectory(char **args) {
    if ((args[1]== NULL)) { // if cd doesn't have argument
        chdir(HOME_DIR);
        printf("cd executed successfully!\n");
    } 
    
    else if(chdir(args[1]) < 0) {  // if chdir() failed
        fprintf(stderr, "No such file or directory");
        exit(EXIT_FAILURE);
    }
    
    return 1;
}

void printPrompt() {
    char prompt[512] = "";

    if (getcwd(cwd,sizeof(cwd)) != NULL) {
        strcat(prompt, USER_ID);
        strcat(prompt, SEPRATOR);
        strcat(prompt, HOST_ID);
        strcat(prompt, TERMINATOR);
        strcat(prompt, cwd);
        strcat(prompt, DOLLAR_SIGN);
        
        printf("%s", prompt);
    }
    
    
    
    
}

void checkAllocationError(char *buffer) {
    if (!buffer) {
        fprintf(stderr, "rash: allocation error!\n");
        exit(EXIT_FAILURE);
    }
    
}

void checkAllocationError(char **buffer) {
    if (!buffer) {
        fprintf(stderr, "rash: allocation error!\n");
        exit(EXIT_FAILURE);
    }
    
}

char *readLine(void) {
    int inputBufferSize = BUFFER_SIZE;
    int commandLetter;
    int index = 0;
    char *inputBuffer = (char*) malloc(inputBufferSize * sizeof(char));
    

    checkAllocationError(inputBuffer);

    while (1) {
        commandLetter = getchar();
        if (commandLetter == EOF) { // Ctrl+ D
            printf("\n");
            exit(EXIT_SUCCESS);
        }
        

        if (commandLetter == '\n') {
            inputBuffer[index] = '\0';
            return inputBuffer;
        } else {
            inputBuffer[index] = commandLetter;
        }
        index++;

        if(index >= inputBufferSize) {
            inputBufferSize += BUFFER_SIZE;
            inputBuffer = (char*) realloc(inputBuffer, inputBufferSize);
            checkAllocationError(inputBuffer);
        }
    }
    printf("\n");
}

char **parseLine(char *line) {
    int commandLength;
    commandLength = strlen(line); // too long Command
    if (commandLength > 512)
    {
        fprintf(stderr, "too long command!\n");
    }
    // else
    // {
    //     printf("command length is less than 512\n");
    // }
    
    
    int tokenBufferSize = TOKEN_BUFFER_SIZE;
    int index = 0;
    char **tokens = (char**) malloc(tokenBufferSize * sizeof(char*));
    char *token;

    checkAllocationError(tokens);

    //tokenizing...
    token = strtok(line,TOKEN_DELIMITERS);
    while (token != NULL)
    {
        tokens[index] = token;
        index++;

        if(index >= tokenBufferSize) {
            tokenBufferSize += BUFFER_SIZE;
            tokens = (char**) realloc(tokens, tokenBufferSize);
            checkAllocationError(tokens);
        }

        token = strtok(NULL, TOKEN_DELIMITERS);
    }

    tokens[index] = NULL;
    return tokens;
    
}

void segFault() {
    int *ptr = NULL;
    *ptr = 42;
}

void signalHandler(int signum) {
    if (signum == SIGINT) { //Ctrl+C implemented but not completely.
        printf("\n");
        // char c;
        // signal(SIGINT, SIG_IGN);
        // printf("\n");
        
        // c = getchar();
        // if (c == '\n') {
        //     printPrompt();
        // } else {
        //     signal(SIGINT, signalHandler);
        // }
        // getchar();
        
        
        
        //fflush(stdout);
        return;
    }
    
    // switch (s)
    // {
    // case SIGSEGV:
    //     fputs("Caught SIGSEGV: segFault.\n", stderr);
    //     exit(EXIT_FAILURE);
    //     break;
    
    // case SIGINT:
    //     // fputs("Caught SIGINT: interactive attention signal, probably a ctrl+c.\n",stderr);
    //     
    //     // printf("%s\n", prompt);
    //     break;
    // default:
    //     break;
    // }
}

int forker(char **args) {
    pid_t pid;
    
    
    pid = fork();

    if (pid < 0) { // if fork doesn't work
        perror("fork failed!");
        exit(EXIT_FAILURE);
    } else if (pid == 0) { // child process
        
        if (execvp(args[0], args) < 0) { // if execvp doesn't work
            perror("execvp Error!");
            exit(EXIT_FAILURE);
        }
       
        exit(EXIT_SUCCESS);
    } else { // parent process
        int status;
        wait(NULL); // waiting to execute child process
        if(WIFEXITED(status)) {
            printf("PARENT <%ld> Child with PID = <%ld >and exit status = <%d> terminated.\n", (long) getpid(), (long) pid, WEXITSTATUS(status));
        }
        
        
       
        return 1;
    }
    

}

int executeLine(char **args) {
    if (args[0] == NULL) { // empty command
        return 1;
    }
    if (strcmp(args[0],QUIT) == 0) { // quit command
        return 0;
    }

    if (strcmp(args[0], "cd") == 0) { // cd command
        return changeDirectory(args);
    }

    return forker(args);   
}

int main(void) {

    char *line;
    char **args;
    int status;
    signal(SIGINT,signalHandler);
    do
    {
        printPrompt();
        line = readLine();
        args = parseLine(line);
        status = executeLine(args);
        
        free(line);
        free(args);
        
        

    } while (status);
    
}

