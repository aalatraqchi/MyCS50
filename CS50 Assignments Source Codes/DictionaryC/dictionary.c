// Implements a dictionary's functionality

#include <ctype.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <strings.h>

#include "dictionary.h"

// Represents a node in a hash table
typedef struct node
{
    char word[LENGTH + 1];
    struct node *next;
}
node;

// TODO: Choose number of buckets in hash table
const unsigned int N = 26e3; // Account for 26 letters in the alphabet three times (first, second, and last letters)

// Hash table
node *table[N];
int word_count =
    0; // This global variable is declared in order to keep track during the load function then return it in the size function

// Returns true if word is in dictionary, else false
bool check(const char *word)
{
    unsigned int index = hash(word); // Start by getting the index of the word through the hash function
    node *cursor = table[index]; // Create a cursor pointer that points to the location of the index

    // While the cursor doesn't point at NULL, meaning a word is there, it will compare the words
    while (cursor != NULL)
    {
        // If a word is there it will compare it to the word that's passed into the function
        if (strcasecmp(word, cursor->word) == 0)
        {
            return true;
        }
        else // If they aren't the same, the cursor will move on to the next node
        {
            cursor = cursor->next;
        }
    }
    return false;
}

// Hashes word to a number
unsigned int hash(const char *word)
{
    unsigned int index = 0;
    int n = strlen(word);

    // Iterate over every letter in the word, add the first, second, and last letters to index value
    for (int i = 0; i < n; i++)
    {
        index += (toupper(word[i]) - 'A') + (toupper(word[i + 1]) - 'A') + (toupper(word[n]) - 'A');
    }
    return index % N; // Use mod to index at unique values in hashtable without going over value of N
}

// Loads dictionary into memory, returning true if successful, else false
bool load(const char *dictionary)
{
    FILE *d = fopen(dictionary, "r");
    if (d == NULL)
    {
        return false;
    }

    char word[LENGTH + 1];
    while (fscanf(d, "%s", word) != EOF)
    {
        node *n = malloc(sizeof(node));
        if (n == NULL)
        {
            return false;
        }
        strcpy(n->word, word); // Copies word into node
        unsigned int index = hash(word); // Determine where word is insterted in hashtable
        n->next = table[index]; // Set the new node's next to the head index
        table[index] = n; // Now head can point where the new node is pointing
        word_count++;
    }
    fclose(d);
    return true;
}

// Returns number of words in dictionary if loaded, else 0 if not yet loaded
unsigned int size(void)
{
    // The word count was already kept track during the load function, so just return that value
    return word_count;
}

// Unloads dictionary from memory, returning true if successful, else false
bool unload(void)
{
    // Go through all buckets
    for (int i = 0; i < N; i++)
    {
        // Create cursor that begins at the start of the hashtable
        node *cursor = table[i];
        while (cursor != NULL)
        {
            // For every node, a tmp pointer will be created where the cursor is pointing, allowing the cursor to move on to the next node then freeing that node
            node *tmp = cursor;
            cursor = cursor->next;
            free(tmp);
        }
        if (cursor == NULL && i == N - 1)
        {
            return true;
        }
    }
    return false;
}
