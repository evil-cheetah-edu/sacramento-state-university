#include <stdio.h>
#include <stdlib.h>


#define BUFFER_SIZE (100)


int main(int argc, char* argv[])
{
    if (argc == 1)
        return 0;

    for (int i = 0; argv[i] != NULL; ++i)
    {
        FILE* file = fopen(argv[i], "r");

        if ( !file )
        {
            printf("wcat: cannot open file\n");
            exit(1);
        }

        for (
            char file_content[BUFFER_SIZE];
            fgets(file_content, BUFFER_SIZE, file) != NULL;
        )
        {
            printf("%s", file_content);
        }

        fclose(file);
    }

    return 0;
}