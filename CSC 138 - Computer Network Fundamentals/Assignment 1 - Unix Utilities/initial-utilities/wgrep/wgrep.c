#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#define NO_ARGUMENTS             (1)
#define SEARCHTERM_ARGUMENT_ONLY (2)

#define STRING_LOOKUP_INDEX      (1)
#define FILES_ARGUMENT_INDEX     (2)


void print_matched_lines(FILE* source, const char* search_term);


int main(int argc, char* argv[])
{
    /// Check for NO Arguments
    if ( argc == NO_ARGUMENTS )
    {
        printf("wgrep: searchterm [file ...]\n");
        exit(1);
    }


    if ( argc == SEARCHTERM_ARGUMENT_ONLY )
    {
        print_matched_lines(stdin, argv[STRING_LOOKUP_INDEX]);
    }


    /// Iterate over Arguments
    for (int i = FILES_ARGUMENT_INDEX; argv[i] != NULL; ++i)
    {
        FILE* file = fopen(argv[i], "r");

        /// Check if File opens
        if ( !file )
        {
            printf("wgrep: cannot open file\n");
            exit(1);
        }

        print_matched_lines(file, argv[STRING_LOOKUP_INDEX]);

        fclose(file);
    }

    return 0;
}


void print_matched_lines(FILE* source, const char* search_term)
{
    char*   line   = NULL;
    size_t  size   = 0;
    ssize_t length = 0;

    while ( (length = getline(&line, &size, source)) != -1 )
    {
        if ( strstr(line, search_term) != NULL )
            printf("%s", line);

    }

    free(line);
}