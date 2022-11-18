#ifndef FUNCTIONS_HEADERS_H_
#define FUNCTIONS_HEADERS_H_

#define SHOW_HIDDEN_FILES 1
#define NO_HIDDEN_FILES 0

void list_files(int counter,int argc, char **argv, int nbre_arg, int n, int show_hidden_files);
void list_recursive_files(int counter,int argc, char **argv, int nbre_arg, int n, int show_hidden_files);
void list_files_long_format(int counter,int argc, char **argv, int nbre_arg, int n, int show_hidden_files);

#endif
