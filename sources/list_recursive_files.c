#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <dirent.h>
#include <stdlib.h>
#include <stdbool.h>

typedef struct File
{
    char directory_name[50];
    struct File *next;
}File;

File* head_file = NULL;

void enfiler(File *file, char directory_name[]);
const char* defiler(File *file);

void list_recursive_files(int counter,int argc, char **argv, int nbre_arg, int n, int show_hidden_files)
{
	DIR* dirptr;
	struct dirent* entry;
	
	char directory[50];
	int i;
	
	i = counter;
	if (argc == nbre_arg)
	{
		strcpy(directory,".");
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
		
		printf("%s:\n", directory);
		char new_directory[50];
		while(head_file != NULL || i == nbre_arg || i == counter)
		{
			while((entry = readdir(dirptr)) != NULL)
			{
				if(entry->d_name[0] != '.' || show_hidden_files)
				{
					printf("%s  ", entry->d_name);
					if (entry->d_type == DT_DIR)
					{
						strcpy(new_directory, directory);
						strcat(new_directory, "/");
						strcat(new_directory, entry->d_name);
						enfiler(head_file, new_directory);
					}
				}
			}
			strcpy(directory, defiler(head_file));
			dirptr = opendir(directory);
			printf("\n\n");
			printf("%s:\n", directory);
		}
		
		printf("\n");
		if (i != argc-1)
		{
			printf("\n");
		}
		closedir(dirptr);
	}
}

void enfiler(File *file, char directory_name[])
{
    //File *nouveau = malloc(sizeof(*nouveau));
    File *nouveau = malloc(100);
    if (nouveau == NULL)
    {
        printf("Mémoire saturée.\n");
        exit(EXIT_FAILURE);
    }

    strcpy(nouveau->directory_name, directory_name);
    nouveau->next = NULL;

    if (head_file != NULL) 
    {
        File *elementActuel = head_file;
        while (elementActuel->next != NULL)
        {
            elementActuel = elementActuel->next;
        }
        elementActuel->next = nouveau;
    }
    else 
    {
        head_file = nouveau;
    }
}


const char* defiler(File *file)
{
    if (file == NULL)
    {
        printf("\n");
        exit(EXIT_FAILURE);
    }

    char* a;

    if (head_file != NULL)
    {
        File *elementDefile = head_file;

        strcpy(a, elementDefile->directory_name);
        head_file = elementDefile->next;
        free(elementDefile);
    }

    return a;
}
