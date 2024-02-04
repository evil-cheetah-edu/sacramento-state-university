#include <stdio.h>
#include <stdlib.h>


#define BUFFER_SIZE (100)


int main(int argc, char* argv[])
{
    /// Check for NO Arguments
    if (argc == 1)
        return 0;

    /// Iterate over Arguments
    for (int i = 1; argv[i] != NULL; ++i)
    {
        FILE* file = fopen(argv[i], "r");

        /// Check if File opens
        if ( !file )
        {
            printf("wcat: cannot open file\n");
            exit(1);
        }

        /// Iterate over the File
        for (
            char file_content[BUFFER_SIZE] = "";
            fgets(file_content, BUFFER_SIZE, file) != NULL;
        )
        {
            printf("%s", file_content);
        }

        fclose(file);
    }

    return 0;
}