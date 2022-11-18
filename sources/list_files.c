#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <dirent.h>

void list_files(int counter,int argc, char **argv, int nbre_arg, int n, int show_hidden_files)
{
	DIR* dirptr;
	struct dirent* entry;
	
	char directory[50];
	int i;
	
	i = counter;
	if (argc == nbre_arg)
	{
		strcpy(directory,"./");
	}
	else
	{
		i = nbre_arg;
	}
	for(;i < argc; i++)
	{
		if (i != counter)
		{
			strcpy(directory, argv[i]);
		}
		dirptr = opendir(directory);
		if (dirptr == NULL)
		{
			perror("dirptr");
			return;
		}
		if (argc > n)
		{
			printf("%s:\n", argv[i]);
		}
		while((entry = readdir(dirptr)) != NULL)
		{
			if(entry->d_name[0] != '.' || show_hidden_files)
			{
				printf("%s  ", entry->d_name);
			}
		}
		printf("\n");
		if (i != argc-1)
		{
			printf("\n");
		}
		closedir(dirptr);
	}
}
