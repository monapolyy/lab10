#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ALPHABET_SIZE 26

// Trie node structure
struct TrieNode
{
    struct TrieNode* children[ALPHABET_SIZE];
    int count;
};

// Trie structure
struct Trie
{
    struct TrieNode* root;
};

// Creates a new trie node
struct TrieNode* createNode()
{
    struct TrieNode* pNode = (struct TrieNode*)malloc(sizeof(struct TrieNode));
    pNode->count = 0;
    for (int i = 0; i < ALPHABET_SIZE; i++)
        pNode->children[i] = NULL;
    return pNode;
}

// Inserts the word into the trie structure
void insert(struct Trie* pTrie, char* word)
{
    struct TrieNode* pCrawl = pTrie->root;
    for (int i = 0; word[i]; i++)
    {
        int index = word[i] - 'a';
        if (!pCrawl->children[index])
            pCrawl->children[index] = createNode();
        pCrawl = pCrawl->children[index];
    }
    pCrawl->count++;
}

// Computes the number of occurrences of the word
int numberOfOccurances(struct Trie* pTrie, char* word)
{
    struct TrieNode* pCrawl = pTrie->root;
    for (int i = 0; word[i]; i++)
    {
        int index = word[i] - 'a';
        if (!pCrawl->children[index])
            return 0;
        pCrawl = pCrawl->children[index];
    }
    return pCrawl->count;
}

// Deallocates the trie structure
void deallocateNode(struct TrieNode* pNode)
{
    if (pNode == NULL)
        return;
    for (int i = 0; i < ALPHABET_SIZE; i++)
        deallocateNode(pNode->children[i]);
    free(pNode);
}

struct Trie* deallocateTrie(struct Trie* pTrie)
{
    if (pTrie == NULL)
        return NULL;
    deallocateNode(pTrie->root);
    free(pTrie);
    return NULL;
}

// Initializes a trie structure
struct Trie* createTrie()
{
    struct Trie* pTrie = (struct Trie*)malloc(sizeof(struct Trie));
    pTrie->root = createNode();
    return pTrie;
}

// Reads the words from the dictionary file and returns the number of words
int readDictionary(char* filename, char** pInWords)
{
    FILE* file = fopen(filename, "r");
    if (file == NULL)
    {
        printf("Failed to open the file.\n");
        return 0;
    }

    int numWords = 0;
    char word[100];
    while (fscanf(file, "%s", word) != EOF)
    {
        pInWords[numWords] = (char*)malloc(sizeof(char) * (strlen(word) + 1));
        strcpy(pInWords[numWords], word);
        numWords++;
    }

    fclose(file);
    return numWords;
}

int main(void)
{
    char* inWords[256];

    // Read the number of words in the dictionary
    int numWords = readDictionary("dictionary.txt", inWords);
    for (int i = 0; i < numWords; ++i)
    {
        printf("%s\n", inWords[i]);
    }

    struct Trie* pTrie = createTrie();
    for (int i = 0; i < numWords; i++)
    {
        insert(pTrie, inWords[i]);
    }

    // Parse line by line and insert each word into the trie data structure
    char* pWords[] = { "notaword", "ucf", "no", "note", "corg" };
    for (int i = 0; i < 5; i++)
    {
        printf("\t%s : %d\n", pWords[i], numberOfOccurances(pTrie, pWords[i]));
    }

    pTrie = deallocateTrie(pTrie);
    if (pTrie != NULL)
        printf("There is an error in this program\n");

    // Free the memory allocated for the words
    for (int i = 0; i < numWords; ++i)
    {
        free(inWords[i]);
    }

    return 0;
}