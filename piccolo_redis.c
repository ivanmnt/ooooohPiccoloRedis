#include "piccolo_redis.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define HASH_TABLE_SIZE 100

struct HashNode {
    char key[MAX_KEY_SIZE];
    struct KeyValue keyValue;
    struct HashNode* next;
};

struct HashTable {
    struct HashNode* buckets[HASH_TABLE_SIZE];
};

struct HashTable database;

unsigned int hash(const char* key) {
    unsigned int hashValue = 0;
    unsigned int prime = 31;

    while (*key) {
        hashValue = (hashValue * prime) + (*key);
        key++;
    }

    return hashValue % HASH_TABLE_SIZE;
}

struct HashNode* getHashNode(const char* key) {
    unsigned int index = hash(key);
    struct HashNode* currentNode = database.buckets[index];

    while (currentNode != NULL) {
        if (strcmp(currentNode->key, key) == 0) {
            return currentNode;
        }
        currentNode = currentNode->next;
    }

    return NULL;
}

char* get(const char* key) {
    struct HashNode* node = getHashNode(key);
    if (node != NULL) {
        return node->keyValue.value;
    }
    return NULL;
}

void set(const char* key, const char* value) {
    struct HashNode* node = getHashNode(key);

    if (node != NULL) {
        strcpy(node->keyValue.value, value);
    } else {
        unsigned int index = hash(key);
        struct HashNode* newNode = (struct HashNode*)malloc(sizeof(struct HashNode));
        strcpy(newNode->key, key);
        strcpy(newNode->keyValue.key, key);
        strcpy(newNode->keyValue.value, value);
        newNode->next = database.buckets[index];
        database.buckets[index] = newNode;
    }
}

void del(const char* key) {
    unsigned int index = hash(key);
    struct HashNode* currentNode = database.buckets[index];
    struct HashNode* previousNode = NULL;

    while (currentNode != NULL) {
        if (strcmp(currentNode->key, key) == 0) {
            if (previousNode == NULL) {
                database.buckets[index] = currentNode->next;
            } else {
                previousNode->next = currentNode->next;
            }
            free(currentNode);
            return;
        }
        previousNode = currentNode;
        currentNode = currentNode->next;
    }
}
