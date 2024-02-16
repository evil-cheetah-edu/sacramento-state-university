#include <stdio.h>
#include <stdlib.h>


#define FILES_ARGUMENT_INDEX (1)
#define READ_SUCCESS         (1)


int main(int argc, char* argv[])
{
    /// Check for NO Arguments
    if (argc == 1)
    {
        printf("wunzip: file1 [file2 ...]\n");
        return 1;
    }

    /// Iterate over Arguments
    for (uint32_t i = FILES_ARGUMENT_INDEX; argv[i] != NULL; ++i)
    {
        FILE* file = fopen(argv[i], "r");

        /// Check if File opens
        if ( !file )
        {
            printf("wunzip: cannot open file\n");
            return 1;
        }

        int character_count;
        char character;

        /// Iterate over the File
        while( 
            fread(&character_count, sizeof(int),  1, file) == READ_SUCCESS &&
            fread(&character,       sizeof(char), 1, file) == READ_SUCCESS
        )
        {
            for (uint32_t i = 0; i < character_count; ++i)
                printf("%c", character);
        }

        if ( !feof(file) )
        {
            printf("wunzip: File is corrupt... Terminating...");
            exit(1);
        }


        fclose(file);
    }

    return 0;
}