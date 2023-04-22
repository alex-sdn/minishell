#include <stdio.h>
#include <readline/readline.h>
#include <readline/history.h>

// compil : gcc -readline_test.c -lreadline

int main(void)
{
	char *line;

	line = readline("minishell$ ");

	printf("line = %s\n", line);
	free(line);
	return (0);
}