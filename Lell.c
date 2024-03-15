#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define __USE_GNU // must be here
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
// this code belongs to Laura Mullings!
// Function Declarations
#define MAX_HISTORY 100  // Maximum number of commands to store
void loop(void);
char *read_line(void);
char **split_line(char *line);
int execute(char **args);
int launch(char **args);
int lell_cd(char **args);
int lell_exit(char **args);
int lell_history(char **args);
void add_to_history(char *command);
char *command_history[MAX_HISTORY];
int history_count = 0;  // Number of commands in history
int lell_pwd(char **args);



// List commands, followed by corresponding functions.
char *builtin_str[] = {
    "cd",
    "exit",
    "history",
    "pwd"};

int (*builtin_func[])(char **) = {
    &lell_cd,
    &lell_exit,
    &lell_history,
    &lell_pwd};

int num_builtins()
{
    return sizeof(builtin_str) / sizeof(char *);
}

// entry point
int main(int argc, char **argv)
{
    loop();

    return EXIT_SUCCESS;
}

void loop(void)
{
    char *line;
    char **args;
    int status;

    do
    {
        printf("lell> ");
        line = read_line();
        add_to_history(line);
        args = split_line(line);
        status = execute(args);

        free(line);
        free(args);
    } while (status);
}

char *read_line(void)
{
    char *line = NULL;
    ssize_t bufsize = 0; // getline allocate a buffer
    getline(&line, &bufsize, stdin);
    return line;
}

char **split_line(char *line)
{
    int bufsize = 64, position = 0;
    char **tokens = malloc(bufsize * sizeof(char *));
    char *token, **tokens_backup;

    if (!tokens)
    {
        fprintf(stderr, "lell: allocation error\n");
        exit(EXIT_FAILURE);
    }

    token = strtok(line, " \t\r\n");
    while (token != NULL)
    {
        tokens[position] = token;
        position++;

        if (position >= bufsize)
        {
            bufsize += 64;
            tokens_backup = tokens;
            tokens = realloc(tokens, bufsize * sizeof(char *));
            if (!tokens)
            {
                free(tokens_backup);
                fprintf(stderr, "lell: allocation error\n");
                exit(EXIT_FAILURE);
            }
        }

        token = strtok(NULL, " \t\r\n");
    }
    tokens[position] = NULL;
    return tokens;
}

int launch(char **args)
{
    pid_t pid, wpid;
    int status;

    pid = fork();
    if (pid == 0)
    {
        // Child
        if (execvp(args[0], args) == -1)
        {
            perror("lell");
        }
        exit(EXIT_FAILURE);
    }
    else if (pid < 0)
    {
        perror("lell");
    }
    else
    {
        // Parent
        do
        {
            wpid = waitpid(pid, &status, WUNTRACED);
        } while (!WIFEXITED(status) && !WIFSIGNALED(status));
    }

    return 1;
}

int execute(char **args)
{
    if (args[0] == NULL)
    {
        // An empty command was entered.
        return 1;
    }

    for (int i = 0; i < num_builtins(); i++)
    {
        if (strcmp(args[0], builtin_str[i]) == 0)
        {
            return (*builtin_func[i])(args);
        }
    }

    return launch(args);
}

int lell_cd(char **args)
{
    if (args[1] == NULL)
    {
        fprintf(stderr, "lell: expected argument to \"cd\"\n");
        // open current derr called . get its name then does pwd 
    }
    else
    {
        if (chdir(args[1]) != 0)
        {
            perror("lell");
        }
    }
    return 1;
}

void add_to_history(char *command)
{
    // free the oldest command if history is full
    if (history_count == MAX_HISTORY)
    {
        free(command_history[0]);
        for (int i = 1; i < MAX_HISTORY; i++)
        {
            command_history[i - 1] = command_history[i];
        }
        history_count--;
    }

    // allocate space for new command then add to history
    command_history[history_count] = strdup(command); // strdup allocates memory
    history_count++;
}

int lell_history(char **args)
{
    if (args[1] != NULL && strcmp(args[1], "-c") == 0)
    {
        for (int i = 0; i < history_count; i++)
        {
            free(command_history[i]);
            command_history[i] = NULL;
        }
        history_count = 0;
        add_to_history("history -c");
    }
    else if (args[1] != NULL)
    {
        int offset = atoi(args[1]);
        if (offset >= 0 && offset < history_count)
        {
            printf("Executing: %s\n", command_history[offset]);
        }
        else
        {
            printf("Invalid offset: %s\n", args[1]);
        }
    }
    else
    {
        // Display history
        for (int i = 0; i < history_count; i++)
        {
            printf("%d %s\n", i, command_history[i]);
        }
        add_to_history("history");
    }
    return 1;
}

int lell_pwd(char **args)
{
    char *cwd = get_current_dir_name();
    printf("%s\n", cwd);
    free(cwd);
    return 1;
}

int lell_exit(char **args)
{
    return 0;
}
