#include <stdio.h>
#include <sys/types.h>
#include <dirent.h>
#include <unistd.h>
#include <string.h>

#include "headers/functions_headers.h"

int main(int argc, char **argv)
{
	int option_val;
	
	while((option_val = getopt(argc, argv, "laRep")) != -1)
	{
		switch(option_val)
		{
			case 'l':
				list_files_long_format(1, argc, argv, optind, 3, NO_HIDDEN_FILES);
				break;
			case 'R':
				list_recursive_files(1, argc, argv, optind, 1, NO_HIDDEN_FILES);
				break;
			case 'a':
				list_files(1, argc, argv, optind, 3, SHOW_HIDDEN_FILES);
				break;
			case '?':
				printf("Unknown argument : %c\n",optopt);
		}
		return 0;
	}
	
	//In case there is no arguments
	if ((option_val = getopt(argc, argv, "laRep")) == -1)
	{
		list_files(0, argc, argv, 1, 2, NO_HIDDEN_FILES);
		return 0;
	}
	
	return 0;
}
