#include "rope.h"
#include <stdio.h>
#include <string.h>

#define S_MAX_LEN 300001
#define TRUE 1
#define FALSE 0


Node *createNode(char value) {
    Node *node = malloc(sizeof(Node));
    node->value = value;
    node->parent = NULL;
    node->left = NULL;
    node->right = NULL;
    node->size = 1;
    return node;
}

SplayTree *createTree(void) {
    SplayTree *tree = malloc(sizeof(SplayTree));
    tree->root = NULL;
    tree->size = 0;
    return tree;
}

void postOrderFree(SplayTree *tree) {
    unsigned n = tree->size;
    Node **stack = malloc(n * sizeof(*stack));
    unsigned size = 0;
    char *boolStack = malloc(n * sizeof(*boolStack));
    unsigned boolSize = 0;
    char alreadyEncountered;
    Node *current = tree->root;
    while (current) {
        stack[size++] = current;
        boolStack[boolSize++] = 0;
        current = current->left;
    }
    while (size) {
        current = stack[size - 1];
        alreadyEncountered = boolStack[boolSize - 1];
        if (alreadyEncountered) {
            free(current);
            size--;
            boolSize--;
        }
        else {
            boolSize--;
            boolStack[boolSize++] = 1;
            current = current->right;
            while (current) {
                stack[size++] = current;
                boolStack[boolSize++] = 0;
                current = current->left;
            }
        }
    }
    tree->root = NULL;
    tree->size = 0;
    free(stack);
    free(boolStack);
}

void destroyTree(SplayTree *tree) {
    if (!tree) {
        return;
    } else if (!tree->root) {
        free(tree);
        return;
    }
    postOrderFree(tree);
    free(tree);
}

static char *inOrder(SplayTree *tree) {
    Node *current = tree->root;

    static char result[S_MAX_LEN];
    unsigned index = 0;
    if (!current) {
        return result;
    }
    Node **stack = malloc(tree->size * sizeof(*stack));
    size_t stackIndex = 0;
    while (TRUE) {
        while (current) {
            stack[stackIndex++] = current;
            current = current->left;
        }
        if (stackIndex) {
            current = stack[--stackIndex];
            result[index++] = current->value;
            current = current->right;
        } else {
            break;
        }
    }
    free(stack);
    return result;
}

static void _rotateRight(SplayTree *tree, Node *node) {
    Node *parent = node->parent;
    Node *Y = node->left;
    if (!Y) {
        return;
    }
    Node *B = Y->right;
    Y->parent = parent;
    if (parent) {
        if (node == parent->left) {
            parent->left = Y;
        } else {
            parent->right = Y;
        }
    } else {
        tree->root = Y;
    }

    node->parent = Y;
    Y->right = node;
    if (B) {
        B->parent = node;
    }
    node->left = B;

    node->size = (node->left ? node->left->size : 0) + (node->right ? node->right->size : 0) + 1;
    Y->size = (Y->left ? Y->left->size : 0) + (Y->right ? Y->right->size : 0) + 1;
}

static void _rotateLeft(SplayTree *tree, Node *node) {
    Node *parent = node->parent;
    Node *X = node->right;
    if (!X)
        return;
    Node *B = X->left;
    X->parent = parent;
    if (parent) {
        if (node == parent->left)
            parent->left = X;
        else
            parent->right = X;
    }
    else
        tree->root = X;

    node->parent = X;
    X->left = node;
    if (B)
        B->parent = node;
    node->right = B;

    node->size = (node->left ? node->left->size : 0) + (node->right ? node->right->size : 0) + 1;
    X->size = (X->left ? X->left->size : 0) + (X->right ? X->right->size : 0) + 1;
}

static void _splay(SplayTree *tree, Node *node) {
    if (!node)
        return;

    Node *parent = node->parent;

    while (parent) {

        Node *left = node->left;
        Node *right = node->right;
        Node *grandParent = parent->parent;

        if (!grandParent) {
            /* Zig */
            if (node == parent->left)
                _rotateRight(tree, parent);
            else
                _rotateLeft(tree, parent);
        }

        else if (node == parent->left) {
            if (parent == grandParent->left) {
                /* Zig-zig */
                _rotateRight(tree, grandParent);
                _rotateRight(tree, parent);
            }
            else {
                _rotateRight(tree, parent);
                _rotateLeft(tree, grandParent);
            }
        }

        else if (node == parent->right) {
            if (parent == grandParent->right) {
                _rotateLeft(tree, grandParent);
                _rotateLeft(tree, parent);
            }
            else {
                _rotateLeft(tree, parent);
                _rotateRight(tree, grandParent);
            }
        }

        parent = node->parent;
    }
}

Node *orderStatisticZeroBasedRanking(SplayTree *tree, unsigned k) {
#ifdef DEBUG
    if (k >= tree->size || k < 0) {
        printf("0 <= k < size of the whole tree\n");
        exit(-1);
    }
#endif // DEBUG
    Node *node = tree->root;
    while (node) {
        Node *left = node->left;
        Node *right = node->right;
        unsigned s = left ? left->size : 0;
        if (k == s)
            break;
        else if (k < s) {
            if (left) {
                node = left;
                continue;
            }
            break;
        }
        else {
            if (right) {
                k = k - s - 1;
                node = right;
                continue;
            }
            break;
        }
    }
    _splay(tree, node);
    return node;
}

static void insert(SplayTree *tree, unsigned rank, char value) {
#ifdef DEBUG
    if (rank > tree->size || rank < 0) {
        printf("0 <= rank <= size of the whole tree\n");
        exit(-1);
    }
#endif // DEBUG

    Node *node = createNode(value);

    if (rank == tree->size && tree->size > 0) {
        Node *last = orderStatisticZeroBasedRanking(tree, rank - 1);
        node->left = last;
        node->size = last->size + 1;
        last->parent = node;
        tree->size++;
        tree->root = node;
        return;
    }

    if (tree->size == 0) {
        tree->size++;
        tree->root = node;
        return;
    }
    Node *right = orderStatisticZeroBasedRanking(tree, rank);
    node->right = right;
    node->left = right->left;
    right->parent = node;
    right->left = NULL;
    right->size = (right->right ? right->right->size : 0) + 1;
    node->size = (node->left ? node->left->size : 0) + (node->right ? node->right->size : 0) + 1;
    tree->size++;
    tree->root = node;
}

static void insertSpecific(SplayTree *tree, char value) {
    Node *node = createNode(value);
    if (tree->root)
        tree->root->parent = node;
    node->left = tree->root;
    node->size = (node->left ? node->left->size : 0) + 1;
    tree->root = node;
    tree->size++;
}

static Node *subtreeMaximum(SplayTree *tree, Node *node) {
    if (!node)
        return NULL;
    while (node->right)
        node = node->right;
    _splay(tree, node);
    return node;
}

static SplayTree *merge(SplayTree *tree1, SplayTree *tree2) {
    if (!tree1 || !tree1->root)
        return tree2;
    if (!tree2 || !tree2->root)
        return tree1;
    Node *root2 = tree2->root;
    Node *root1 = subtreeMaximum(tree1, tree1->root);
    root2->parent = root1;
    root1->right = root2;
    root1->size = (root1->left ? root1->left->size : 0) + (root1->right ? root1->right->size : 0) + 1;
    tree1->size = root1->size;
    return tree1;
}

static void split(SplayTree *tree, unsigned rank, SplayTree **tree1, SplayTree **tree2) {
    Node *root1 = orderStatisticZeroBasedRanking(tree, rank);
    Node *root2 = root1->right;
    root1->right = NULL;
    root1->size = (root1->left ? root1->left->size : 0) + (root1->right ? root1->right->size : 0) + 1;
    *tree1 = createTree();
    (*tree1)->root = root1;
    (*tree1)->size = root1->size;
    *tree2 = createTree();
    if (root2) {
        root2->parent = NULL;
        (*tree2)->root = root2;
        (*tree2)->size = root2->size;
    }
    return;
}

void process(SplayTree **tree, unsigned i, unsigned j, unsigned k) {
     SplayTree *left = NULL, *middle = NULL, *right = NULL;
    split(*tree, j, &middle, &right);
    if (i > 0)
        split(middle, i - 1, &left, &middle);
    left = merge(left, right);
    if (k > 0)
        split(left, k - 1, &left, &right);
    else {
        right = left;
        left = NULL;
    }
    *tree = merge(merge(left, middle), right);
    return;
}
