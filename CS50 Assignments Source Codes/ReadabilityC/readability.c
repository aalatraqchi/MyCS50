#include <cs50.h>
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <math.h>

int count_letters(string text);
int count_words(string text);
int count_sentences(string text);

int main(void)
{
    // Prompt user for text
    string text = get_string("Text: ");

    // Receive number of letters, words, and sentences from the functions to store into variables
    int letters = count_letters(text);
    int words = count_words(text);
    int sentences = count_sentences(text);

    // Calculate L and S from the CL index formula
    float L = ((float) letters / words) * 100;
    float S = ((float) sentences / words) * 100;

    // Calculate the grade level using CL index

    int clindex = round((0.0588 * L) - (0.296 * S) - 15.8);

    // Print appropriate grade levels based on calculation of CL index
    if (clindex >= 16)
    {
        printf("Grade 16+\n");
    }
    else if (clindex < 1)
    {
        printf("Before Grade 1\n");
    }
    else
    {
        printf("Grade %i\n", clindex);
    }
}

int count_letters(string text)
{
    // Start at 0 letters
    int letters = 0;
    for (int i = 0, n = strlen(text); i < n; i++)
    {
        // If the character in the text is alphabetical, increase letter count by 1
        if (isalpha(text[i]))
        {
            letters++;
        }
    }
    return letters;
}

int count_words(string text)
{
    // Start at 0 words
    int words = 0;
    for (int i = 0, n = strlen(text); i < n; i++)
    {
        // If a character in the text is a SPACE or is the last letter of the string, increase word count by 1
        if (isspace(text[i]) || i == n - 1)
        {
            words++;
        }
    }
    return words;
}

int count_sentences(string text)
{
    // Start at 0 sentences
    int sentences = 0;
    for (int i = 0, n = strlen(text); i < n; i++)
    {
        if (text[i] == 33 || text[i] == 46 || text[i] == 63)
        {
            sentences++;
        }
    }
    return sentences;
}