#include <sys/types.h>
#include <dirent.h>
#include <stdio.h>

int main(void)
{
	DIR *test;

	test = opendir("includes");

	printf("%s\n", (char*)test);  //cant print, use readdir?

	return (0);
}