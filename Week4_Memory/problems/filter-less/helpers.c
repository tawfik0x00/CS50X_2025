#include "helpers.h"
#include <math.h>
// Convert image to grayscale
void grayscale(int height, int width, RGBTRIPLE image[height][width])
{
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            int average =
                round((image[i][j].rgbtBlue + image[i][j].rgbtGreen + image[i][j].rgbtRed) / 3.0);

            // apply the averge to each color
            image[i][j].rgbtBlue = average;
            image[i][j].rgbtGreen = average;
            image[i][j].rgbtRed = average;
        }
    }
    return;
}

// Convert image to sepia
void sepia(int height, int width, RGBTRIPLE image[height][width])
{
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            // get the original RGB vlues
            BYTE originalRed = image[i][j].rgbtRed;
            BYTE originalGreen = image[i][j].rgbtGreen;
            BYTE originalBlue = image[i][j].rgbtBlue;

            // Calculate the new sepia values
            int sepiaRed =
                round(0.393 * originalRed + 0.769 * originalGreen + 0.189 * originalBlue);
            int sepiaGreen =
                round(0.349 * originalRed + 0.686 * originalGreen + 0.168 * originalBlue);
            int sepiaBlue =
                round(0.272 * originalRed + 0.534 * originalGreen + 0.131 * originalBlue);

            image[i][j].rgbtRed = sepiaRed > 255 ? 255 : sepiaRed;
            image[i][j].rgbtGreen = sepiaGreen > 255 ? 255 : sepiaGreen;
            image[i][j].rgbtBlue = sepiaBlue > 255 ? 255 : sepiaBlue;
        }
    }
    return;
}

// Reflect image horizontally
void reflect(int height, int width, RGBTRIPLE image[height][width])
{
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width / 2; j++)
        {
            // swap pixels
            RGBTRIPLE temp = image[i][j];
            image[i][j] = image[i][width - j - 1];
            image[i][width - j - 1] = temp;
        }
    }
    return;
}

// Blur image
void blur(int height, int width, RGBTRIPLE image[height][width])
{
    RGBTRIPLE temp[height][width];
    for (int row = 0; row < height; row++)
    {
        for (int clmn = 0; clmn < width; clmn++)
        {
            temp[row][clmn] = image[row][clmn];
        }
    }

    for (int row = 0; row < height; row++)
    {
        for (int clmn = 0; clmn < width; clmn++)
        {
            // define the sum of values for each color
            int redSum = 0, greenSum = 0, blueSum = 0, counter = 0;
            // define loop over grid of 3 * 3 box around the pixel
            for (int rowi = -1; rowi <= 1; rowi++)
            {
                for (int clmni = -1; clmni <= 1; clmni++)
                {

                    int nrindx = row + rowi;
                    int ncindx = clmn + clmni;

                    // check if out of bounds
                    if ((nrindx >= 0 && nrindx < height) && (ncindx >= 0 && ncindx < width))
                    {
                        redSum += temp[nrindx][ncindx].rgbtRed;
                        greenSum += temp[nrindx][ncindx].rgbtGreen;
                        blueSum += temp[nrindx][ncindx].rgbtBlue;
                        counter++;
                    }
                }
            }

            image[row][clmn].rgbtRed = round((float) redSum / counter);
            image[row][clmn].rgbtGreen = round((float) greenSum / counter);
            image[row][clmn].rgbtBlue = round((float) blueSum / counter);
        }
    }
}
