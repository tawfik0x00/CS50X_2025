#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#define BLOCK_SIZE 512

int main(int argc, char *argv[])
{
    // Accept a single command-line argument
    if (argc != 2)
    {
        printf("Usage: %s + FILE\n", argv[0]);
        return 1;
    }

    // open the memory card image
    FILE *card = fopen(argv[1], "r");
    if (card == NULL)
    {
        printf("Can't open the file\n");
        return 1;
    }

    // define the block we will read in the time of the card
    uint8_t buffer[512];
    int images = 0;
    FILE *image = NULL;

    // while there's still data left to read from the memory card
    while (fread(buffer, sizeof(uint8_t), BLOCK_SIZE, card) == 512)
    {
        // create JPEGS from the data
        if ((buffer[0] == 0xff && buffer[1] == 0xd8 && buffer[2] == 0xff) &&
            (buffer[3] >= 0xe0 && buffer[3] <= 0xee))
        {
            // close the previous image if it open
            if (image != NULL)
                fclose(image);
            // create new image and close previous image
            char jpeg[10];
            sprintf(jpeg, "%03i.jpg", images);
            image = fopen(jpeg, "w");
            if (image == NULL)
            {
                printf("Can't create a new file\n");
                return 1;
            }

            fwrite(buffer, sizeof(uint8_t), BLOCK_SIZE, image);
            images++;
        }
        else if (image != NULL)
        {
            fwrite(buffer, sizeof(uint8_t), BLOCK_SIZE, image);
        }
    }
    fclose(image);
    fclose(card);
}
