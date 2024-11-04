#include <cs50.h>
#include <stdio.h>

int main(void)
{
    int n;
    do
    {
        n = get_int("Height: ");
    }
    while (n < 1 || n > 8); // Ask user to input height over and over again until they choose one between 1 and 8

    // For every row
    for (int i = 0; i < n; i++)
    {

        // For spaces in each row
        for (int j = n - i; j > 1; j--)
        {
            printf(" ");
        }

        // For each row, add hashes after spaces
        for (int k = 0; k < i + 1; k++)
        {
            printf("#");
        }


        // For each row, add two spaces after the hashes
        for (int l = 0; l < 2; l++)
        {
            printf(" ");
        }


        // For each row, add hashes after the two spaces
        for (int m = 0; m < i + 1; m++)
        {
            printf("#");
        }

        printf("\n");
    }

}