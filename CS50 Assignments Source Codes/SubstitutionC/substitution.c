#include <cs50.h>
#include <stdio.h>
#include <ctype.h>
#include <string.h>

void encrypt(string key);
void addcipher(char c, string key);

int main(int argc, string argv[])
{

    // If no key is used, return error
    if (argc != 2)
    {
        printf("Please enter a key.\n");
        return 1;
    }

    string key = argv[1];
    int n = strlen(key);

    // If 2 arguments are given and the second argument has 26 characters
    if (argc == 2 && n == 26)
    {
        // Check every character to see if it's alphabetical
        for (int i = 0; i < n; i++)
        {
            // Return error message if string is not alphabetical
            if (!isalpha(key[i]))
            {
                printf("Key must consist of only alphabetical characters.\n");
                return 1;
            }
            // Check the next character to make sure it doesn't match the previous character
            for (int j = i + 1; j < n; j++)
            {
                // If a character in the key matches the next character, return error message
                if (tolower(key[i]) == tolower(key[j]))
                {
                    printf("Key must not contain repeated characters.\n");
                    return 1;
                }
            }
        }
        encrypt(key);
        printf("\n");
    }

    // If the input key didn't consist of 2 arguments and 26 characters
    else
    {
        printf("Usage: ./substitution key (one argument containing 26 unique characters)\n");
        return 1;
    }

}

void encrypt(string key)
{
    string ptext = get_string("plaintext: "); // Prompt user for text
    int l = strlen(ptext);
    printf("ciphertext: "); // This is where the encrypted text will be

    for (int i = 0; i < l; i++) // Go through every character of plain text
    {
        char plain = ptext[i];

        // If the character in plain text is alphabetical, pass it to the function that adds cipher
        if (isalpha(plain))
        {
            addcipher(plain, key);
        }
        else
        {
            printf("%c", plain);
        }
    }

}

void addcipher(char c, string key)
{
    // Set up default alphabet
    string default_ = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    int n = strlen(default_);

    // For every character in default alphabet
    for (int i = 0; i < n; i++)
    {
        char def = default_[i];
        char sub = key[i];

        // If the character from plain text is upper and equal to default, it will print uppercase key character
        if (isupper(c) && c == def)
        {
            printf("%c", toupper(sub));
        }
        else if (islower(c) && c == tolower(def)) // If character is lowercase and equal to default, it will print lowercase key character
        {
            printf("%c", tolower(sub));
        }

    }

}