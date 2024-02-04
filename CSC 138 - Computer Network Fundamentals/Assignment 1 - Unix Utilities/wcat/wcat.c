#include <stdio.h>
#include <stdlib.h>


int main(int argc, char* argv[])
{
    FILE* file = fopen(argv[1], "r");

    if ( !file )
    {
        printf("wcat: cannot open file\n");

        exit(1);
    }

    fclose(file);

    return 0;
}