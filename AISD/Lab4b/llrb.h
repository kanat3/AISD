#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Info {
    float x;
    char* str;
    struct Info* next;
} Info;

typedef struct Node { 
    int key;
    struct Info* info;
    struct Node* left;
    struct Node* right;
    char c;
} Node;

void DOT(Node* _node);
void printInfo(Info* info);
void removeInfo(Info* info);
Info* makeInfo(char* str, float x);
Node* makeNode(unsigned int key, Info* info);
void addToList(Node* node, Info* info);
Node* rotateRight(Node* node);
Node* rotateLeft(Node* node);
void flipColors(Node* node);
int isRed(Node* node);
Node* fixUp(Node* node);
Node* moveRedLeft(Node* node);
Node* moveRedRight(Node* node);
Node* minNode(Node* node);
Node* maxNode(Node* node);
Node* insert(Node* node, unsigned int key, Info* info);
Node* removeMin(Node* node);
Node* removeNode(Node* node, unsigned int key);
Info* find(Node* node, unsigned key);
Node* deleteTree(Node* node);
void printTree(Node* leaf, unsigned int a, unsigned int b);
void print(Node* leaf, int level);
Node* inputFromFile(FILE* file);
void test(int n);
int find_mismatched_key (Node *root, int _key);
void find_diff (Node* _node, int _key, int* min_diff, int* min_diff_key);