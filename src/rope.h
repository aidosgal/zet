#ifndef ROPE_H
#define ROPE_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define S_MAX_LEN 300001
#define TRUE 1
#define FALSE 0

typedef struct Node Node;

struct Node {
    char value;
    Node *parent, *left, *right;
    unsigned size;
};

typedef struct {
    Node *root;
    unsigned size;
} SplayTree;

Node *createNode(char value);

SplayTree *createTree(void);

void destroyTree(SplayTree *tree);

#endif // ROPE_H
