#include <stdio.h>
#include <stdlib.h>


#define FILES_ARGUMENT_INDEX (1)
#define STRUCT_ITEMS         (2)


struct EncodedData
{
    uint32_t number_of_characters;
    char     character;          
};


int main(int argc, char* argv[])
{
    /// Check for NO Arguments
    if (argc == 1)
    {
        printf("wunzip: file1 [file2 ...]\n");
        exit(1);
    }

    /// Iterate over Arguments
    for (uint32_t i = FILES_ARGUMENT_INDEX; argv[i] != NULL; ++i)
    {
        FILE* file = fopen(argv[i], "r");

        /// Check if File opens
        if ( !file )
        {
            printf("wcat: cannot open file\n");
            exit(1);
        }

        /// Iterate over the File
        while( !feof(file) )
        {
            struct EncodedData* data = malloc(sizeof *data);

            int scan_result = fscanf(
                file,
                "%d%c",
                &data->number_of_characters,
                &data->character
            );

            if ( scan_result == EOF )
                break;

            if ( scan_result != STRUCT_ITEMS )
            {
                printf("\n");
                printf("An archive seems to be corrupt...\n");
                printf("Terminating...\n");

                return 1;
            }

            for (uint32_t i = 0; i < data->number_of_characters; ++i)
                printf("%c", data->character);
        }

        printf("\n");
        fclose(file);
    }

    return 0;
}