#include <stdio.h>
#include <stdlib.h>


#define NO_ARGUMENTS (1)


int main(int argc, char* argv[])
{
    if ( argc == NO_ARGUMENTS )
    {
        printf("wzip: file1 [file2 ...]\n");
        return 1;
    }

    int  count    = 1;
    char previous = '\0';

    for (int i = 1; argv[i] != NULL; ++i)
    {
        FILE *file = fopen(argv[i], "r");

        if ( !file )
        {
            printf("wzip: cannot open file\n");
            return 1;
        }

        char current = '\0';

        while ( (current = fgetc(file)) != EOF )
        {
            if ( previous == '\0' )
            {
                previous = current;
                count    = 1;
                continue;
            }

            if ( current == previous )
            {
                ++count;
                continue;
            }

            fwrite(&count,    sizeof(int),  1, stdout);
            fwrite(&previous, sizeof(char), 1, stdout);
            previous = current;
            count    = 1;

        }

        /// Undo the read of EOF
        if ( current == EOF )
        {
            current = previous;
        }

        fclose(file);
    }

    /// Output the remaining character after  
    fwrite(&count,    sizeof(int),  1, stdout);
    fwrite(&previous, sizeof(char), 1, stdout);

    return 0;
}