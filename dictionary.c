// Implements a dictionary's functionality

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <strings.h>
#include <ctype.h>

#include "dictionary.h"

// Represents a node in a hash table
typedef struct node
{
    char word[LENGTH + 1];
    struct node *next;
}
node;

const unsigned int tableSize = 1000;
node *hashTable[tableSize];
unsigned int numOfWords = 0;
bool hasLoaded = false;

// Given a word, this will return if the word is in the dictionary provided
bool check(const char *word)
{
    char checkWord[strlen(word)]; // Since the word being past in is const, we have to make a buffer to edit.
    strcpy(checkWord, word);
    for (int i = 0; checkWord[i] != '\0'; i++)
    {
        checkWord[i] = tolower(checkWord[i]); // change word to all lower case
    }

    int index = hash(checkWord); // save hash index in a varible
    if (hashTable[index] != NULL) // if it is not NULL then there is a word here
    {
        for (node *temp = hashTable[index]; temp != NULL; temp = temp->next)
        {
            if (strcmp(temp->word, checkWord) == 0)
            {
                return true; // If the words match return true;
            }
        }
    }

    return false;
}

// Hashes word to a number
unsigned int hash(const char *word)
{
    int hash = 0;

    int n;
    for (int i = 0; word[i] != '\0'; i++)
    {
        if (isalpha(word[i]))
        {
            n = word[i] - 'a' + 1;
        }
        else
        {
            n = 27;
        }
        // one of many hash equations that can be found online.
        hash = ((hash << 3) + n) % tableSize;
    }
    return hash;
}

// Loads dictionary into memory, returning true if successful, else false
bool load(const char *dictionary)
{
    FILE *inFile = fopen(dictionary, "r");
    if (inFile == NULL)
    {
        return false; // if the inFile is null, then there is no dictionary.
    }

    for (int i = 0; i < tableSize; i ++)
    {
        hashTable[i] = NULL; // Make sure table is empty before loading.
    }

    char word[LENGTH + 1]; // place to store word
    while (fscanf(inFile, "%s", word) != EOF) // while not of end file
    {
        numOfWords++;
        node *newNodePtr = malloc(sizeof(node));
        strcpy(newNodePtr->word, word);

        int index = hash(word);
        if (hashTable[index] == NULL)
        {
            newNodePtr->next = NULL;
            hashTable[index] = newNodePtr;
        }
        else
        {
            newNodePtr->next = hashTable[index];
            hashTable[index] = newNodePtr;
        }
    }
    fclose(inFile);
    hasLoaded = true;
    return hasLoaded;
}

// Returns number of words in dictionary if loaded, else 0 if not yet loaded
unsigned int size(void)
{
    if (!hasLoaded)
    {
        return 0;
    }
    return numOfWords;
}

// Unloads dictionary from memory, returning true if successful, else false
bool unload(void)
{
    if (!hasLoaded)
    {
        return false;
    }

    for (int i = 0; i < tableSize; i++)
    {
        if (hashTable[i] != NULL)
        {
            node *bufferNode = hashTable[i];

            while (bufferNode != NULL)
            {
                node *prevNode = bufferNode;
                bufferNode = bufferNode->next;
                free(prevNode);
            }
        }
    }
    return true;
}
