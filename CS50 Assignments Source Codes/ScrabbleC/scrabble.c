#include <ctype.h>
#include <cs50.h>
#include <stdio.h>
#include <string.h>

// Points assigned to each letter of the alphabet
int POINTS[] = {1, 3, 3, 2, 1, 4, 2, 4, 1, 8, 5, 1, 3, 1, 1, 3, 10, 1, 1, 1, 1, 4, 4, 8, 4, 10};

int compute_score(string word);

int main(void)
{
    // Get input words from both players
    string word1 = get_string("Player 1: ");
    string word2 = get_string("Player 2: ");

    // Score both words
    int score1 = compute_score(word1);
    int score2 = compute_score(word2);

    // Print the winner
    if (score1 > score2)
    {
        printf("Player 1 wins!\n");
    }
    else if (score1 < score2)
    {
        printf("Player 2 wins!\n");
    }
    else
    {
        printf("Tie!\n");
    }
}

int compute_score(string word)
{
    // Set default score to 0
    int score = 0;

    // For every character in the word
    for (int i = 0, n = strlen(word); i < n; i++)
    {
        if (isupper(word[i])) // If the character i in the word is uppercase
        {
            score += POINTS[word[i] -
                                    65]; // Increase the score by where the letter aligns in the points array (A would be position 0 because of 65-65)
        }
        else if (islower(word[i])) // If the character i in the word is lowercase
        {
            score += POINTS[word[i] - 
                                    97]; // Increase the score by where the letter aligns in the points array (a would be position 0 because of 97-97)
        }
    }
    return score;
}
