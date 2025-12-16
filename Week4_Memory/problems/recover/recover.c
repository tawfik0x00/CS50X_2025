#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

// block of data in FAT 32 system
#define BLOCK_SIZE 512

int main(int argc, char *argv[])
{
    // accept single command-line argument
    if (argc != 2)
    {
        fprintf(stderr, "Usage: %s + FILE\n", argv[0]);
        return 1;
    }

    // open the memory card image
    FILE *card = fopen(argv[1], "r");
    if (card == NULL)
    {
        fprintf(stderr, "Error can't open the file %s \n", argv[1]);
        return 1;
    }
    // Buffer to store a block of data
    uint8_t buffer[BLOCK_SIZE];
    int image_count = 0;
    FILE *image = NULL;
    char filename[8];

    // Start Read data from the file until end the file
    while (fread(buffer, sizeof(uint8_t), BLOCK_SIZE, card) == BLOCK_SIZE)
    {
        // check the start of the block if an image found or not
        if ((buffer[0] == 0xff && buffer[1] == 0xd8 && buffer[2] == 0xff) &&
            (buffer[3] >= 0xe0 && buffer[3] <= 0xef))
        {
            // check if the image was opend
            if (image != NULL)
            {
                fclose(image);
            }

            // start creating a new image
            snprintf(filename, sizeof(filename), "%03i.jpg", image_count);
            image = fopen(filename, "w");
            if (image == NULL)
            {
                fprintf(stderr, "Error Can't create the file %s\n", filename);
                fclose(card);
                return 1;
            }
            image_count++;
        }

        if (image != NULL)
        {
            fwrite(buffer, sizeof(uint8_t), BLOCK_SIZE, image);
        }
    }
    fclose(image);
    fclose(card);
    return 0;
}
