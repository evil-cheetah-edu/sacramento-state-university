#include <stdio.h>
#include <stdlib.h>


int main(int argc, char* argv[])
{
    FILE* file = fopen(argv[1], "r");

    if ( !file )
    {
        printf("An error occurred while opening the file\n");

        exit(1);
    }

    fclose(file);

    return 0;
}