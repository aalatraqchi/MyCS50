#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

typedef uint8_t BYTE;

int main(int argc, char *argv[])
{
    // Remind of usage if CLA isn't correctly used
    if (argc != 2)
    {
        printf("Usage: ./recover IMAGE\n");
        return 1;
    }

    // Open the image
    FILE *file = fopen(argv[1], "r");

    if (file == NULL)
    {
        printf("Could Not Open File\n");
        return 2;
    }

    // Create output file, buffer, image count, and filename with default values
    FILE *fileout = NULL;
    BYTE buffer[512];
    int jpeg_count = 0;
    int BLOCK_SIZE = 512;
    char filename[8]; // 8 spaces to account for "###.jpg" plus the null character ('\0')

    while (fread(buffer, sizeof(BYTE), BLOCK_SIZE, file) == BLOCK_SIZE)
    {
        // Check for JPEG based on the first four bytes
        if (buffer[0] == 0xff && buffer[1] == 0xd8 && buffer[2] == 0xff && (0xe0 <= buffer[3] && buffer[3] <= 0xef))
        {
            // Go through process for first image
            if (jpeg_count == 0)
            {
                // Name image
                sprintf(filename, "%03d.jpg", jpeg_count);

                // Open file and write to it
                fileout = fopen(filename, "w");
                fwrite(buffer, sizeof(BYTE), BLOCK_SIZE, fileout);

                // Increment image count after first image
                jpeg_count++;
            }

            // After the first image, the previous image will start closing while new images are written into
            else if (jpeg_count > 0)
            {
                fclose(fileout);
                sprintf(filename, "%03d.jpg", jpeg_count);
                fileout = fopen(filename, "w");
                fwrite(buffer, sizeof(BYTE), BLOCK_SIZE, fileout);
                jpeg_count++;
            }
        }

        // This will continue writing the same file if it no longer fulfills the condition of first 4 bytes
        else if (jpeg_count > 0)
        {
            fwrite(buffer, sizeof(BYTE), BLOCK_SIZE, fileout);
        }
    }
    // Close files
    fclose(file);
    fclose(fileout);

    return 0;
}