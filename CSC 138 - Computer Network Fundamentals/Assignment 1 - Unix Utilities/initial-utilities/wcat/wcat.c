#include <stdio.h>
#include <stdlib.h>


#define BUFFER_SIZE          (100)

#define NO_ARGUMENTS           (1)
#define FILES_ARGUMENT_INDEX   (1)


int main(int argc, char* argv[])
{
    /// Check for NO Arguments
    if (argc == NO_ARGUMENTS)
        return 0;

    /// Iterate over Arguments
    for (int i = FILES_ARGUMENT_INDEX; argv[i] != NULL; ++i)
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