#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

#define BLOCK_SIZE 512

typedef uint8_t BYTE;

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        printf("Usage: ./recover image\n");
        return 1;
    }

    ////////////////
    FILE *input = fopen(argv[1], "r");
    BYTE buffer[512];
    FILE *output = NULL;

    int nameCounter = 0;

    char filename[8];

    if (input == NULL)
    {
        fprintf(stderr, "Could not open %s.\n", argv[1]);
        return 2;
    }

    while (true)
    {
        // fread(store in buffer, each element of 1 byte, of block size 512, from the input filw)
        // fread returns the number of items of size "size" that were read
        unsigned int bytesRead = fread(buffer, sizeof(BYTE), BLOCK_SIZE, input);

        //Check end of the file
        // so, if bytesRead is zero, then we've reached the end of the file.Also, feof() indicates file end
        if (bytesRead == 0 && feof(input))
        {
            break;
        }

        // bitwise arithmetic
        bool condition = buffer[0] == 0xff && buffer[1] == 0xd8 && buffer[2] == 0xff && (buffer[3] & 0xf0) == 0xe0;
        // ?? (buffer[3] & 0xf0) == 0xe0
        // if this condition is seen again and the size of the output file isn't zero, close the output file
        // increase the name counter
        if (condition && output != NULL)
        {
            fclose(output);
            nameCounter += 1;
        }

        if (condition)
        {
            //filename to which you're writing, placeholder and name of the file
            sprintf(filename, "%03i.jpg", nameCounter);
            output = fopen(filename, "w");
        }

        // as long as file isn't a null character and it is oen
        if (output != NULL)
        {
            /// (the pointer from which the data to write fill be received, the size which is 1 byte,
            //, number of bytes received from the input file, and the name of the output file)
            fwrite(buffer, sizeof(BYTE), bytesRead, output);
        }

    }
    // close output file
    fclose(output);
    //close input file
    fclose(input);
    // return 0 for success
    return 0;
}
