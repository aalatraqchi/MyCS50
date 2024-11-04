#include "helpers.h"
#include <math.h>

// Convert image to grayscale
void grayscale(int height, int width, RGBTRIPLE image[height][width])
{
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            // For each pixel, the average of RGB are calculated then all three RGB values are set to that new average value
            float avg = round((image[i][j].rgbtBlue + image[i][j].rgbtGreen + image[i][j].rgbtRed) / 3.0);
            image[i][j].rgbtBlue = avg;
            image[i][j].rgbtGreen = avg;
            image[i][j].rgbtRed = avg;
        }
    }
    return;
}

// Convert image to sepia
void sepia(int height, int width, RGBTRIPLE image[height][width])
{
    int sBlue;
    int sGreen;
    int sRed;

    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            // Use the sepia formula for all three RGB values while making sure they cap at 255, for each pixel.
            sBlue = round(.272 * image[i][j].rgbtRed + .534 * image[i][j].rgbtGreen + .131 * image[i][j].rgbtBlue);
            if (sBlue > 255)
            {
                sBlue = 255;
            }
            sGreen = round(.349 * image[i][j].rgbtRed + .686 * image[i][j].rgbtGreen + .168 * image[i][j].rgbtBlue);
            if (sGreen > 255)
            {
                sGreen = 255;
            }
            sRed = round(.393 * image[i][j].rgbtRed + .769 * image[i][j].rgbtGreen + .189 * image[i][j].rgbtBlue);
            if (sRed > 255)
            {
                sRed = 255;
            }

            image[i][j].rgbtBlue = sBlue;
            image[i][j].rgbtGreen = sGreen;
            image[i][j].rgbtRed = sRed;
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
            // Make temporary array and store original values
            int tmp[] = {image[i][j].rgbtBlue, image[i][j].rgbtGreen, image[i][j].rgbtRed};

            // Change the original image pixels to the pixels on the opposite side
            image[i][j].rgbtBlue = image[i][width - j - 1].rgbtBlue;
            image[i][j].rgbtGreen = image[i][width - j - 1].rgbtGreen;
            image[i][j].rgbtRed = image[i][width - j - 1].rgbtRed;

            // Switch the pixels on the other side to the original pixels that are stored in the temporary array
            image[i][width - j - 1].rgbtBlue = tmp[0];
            image[i][width - j - 1].rgbtGreen = tmp[1];
            image[i][width - j - 1].rgbtRed = tmp[2];
        }
    }
    return;
}

int Blurcopy(int i, int j, int height, int width, RGBTRIPLE image[height][width], int color)
{
    int total = 0;
    float count = 0;

    for (int k = i - 1; k < i + 2; k++) // This will count starting at the row before row i and stop before row 4
    {
        for (int l = j - 1; l < j + 2; l++) // This will count starting at the column before column i and stop before reaching the 4th column
        {
            if (k < 0 || k >= height)
            {
                break; // This will make sure it doesn't count outside of the "grid" containing the entire image
            }
            if (l < 0 || l >= width)
            {
                continue; // This will make sure to continue to allow it to go to columns before and after a number
            }
            if (color == 0)
            {
                total += image[k][l].rgbtBlue;
            }
            else if (color == 1)
            {
                total += image[k][l].rgbtGreen;
            }
            else
            {
                total += image[k][l].rgbtRed;
            }
            count++;
        }
    }
    return round(total / count);
}

// Blur image
void blur(int height, int width, RGBTRIPLE image[height][width])
{
    // Make a copy of the original image
    RGBTRIPLE copy[height][width];

    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            copy[i][j] = image[i][j];
        }
    }

    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            image[i][j].rgbtBlue = Blurcopy(i, j, height, width, copy, 0);
            image[i][j].rgbtGreen = Blurcopy(i, j, height, width, copy, 1);
            image[i][j].rgbtRed = Blurcopy(i, j, height, width, copy, 2);
        }
    }
    return;
}