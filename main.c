#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

// Define new data type
typedef uint8_t BYTE;

int main(int argc, char *argv[])
{
    // Check usage
    if (argc != 2)
    {
        printf("Usage: ./recover IMAGE\n");
        return 1;
    }

    // Open the memory card
    FILE *infile = fopen(argv[1], "r");

    // Check if file is corrupt
    if (infile == NULL)
    {
        printf("Could not open file\n");
        return 2;
    }

    // File to store the data read and set it to NULL
    FILE *outfile = NULL;

    // Array to read from, that stores the 512 bytes of every block
    BYTE buffer[512];

    // Variable that keeps track of .jpg found
    int jpeg_found = 0;

    // String of size 8 (7 chars plus null)
    char filename[8];

    // Repeat until the end of the memory card
    while (fread(buffer, sizeof(BYTE), 512, infile) == 512)
    {
        // If pattern matches, JPG found
        if (buffer[0] == 0xff && buffer[1] == 0xd8 && buffer[2] == 0xff && (buffer[3] & 0xf0) == 0xe0)
        {
            // If JPEG was already found, close it
            if (outfile != NULL)
            {
                fclose(outfile);
            }

            // Open a new file
            sprintf(filename, "%03i.jpg", jpeg_found);
            outfile = fopen(filename, "w");
            jpeg_found ++;
        }

        // Copy data from buffer to outfile
        if (outfile != NULL)
        {
            fwrite(buffer, sizeof(BYTE), 512, outfile);
        }
    }

    // Close any remaining files
    fclose(infile);
    fclose(outfile);

    return 0;
}
