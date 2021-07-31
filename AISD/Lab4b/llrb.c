#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <time.h>
#include "llrb.h"
#include "optionally.h"
#include <limits.h>

void printInfo (Info *info) { //вывод односвязного списка типа Info
    while (info) {
        printf("info:\t%.4f\t%s\n", info->x, info->str);
        info = info->next;
    }
}

void removeInfo (Info* info) { //удаление односвязного списка информации 
    Info* to_delete = info;
    while (to_delete) {
        info = info->next;
        free(to_delete->str);
        free(to_delete);
        to_delete = info;
    }
}

Info* makeInfo (char* str, float x) { //создание информационного поля
    Info* info = (Info*)calloc(1, sizeof(Info));
    info->x = x;
    info->str = (char*)calloc(strlen(str) + 1, sizeof(char));
    strcpy(info->str, str);
    info->next = NULL;
    return info;
}

Node* makeNode (unsigned int key, Info *info) { //создание узла
    Node* node = (Node*)calloc(1, sizeof(Node));
    node->left = node->right = NULL;
    node->key = key;
    node->info = info;
    node->c = 'r';
    return node;
}

void addToList (Node* node, Info* info) { //добавление информационного поля в конец односвязного списка
    Info* last = node->info;
    while (last->next) {
        last = last->next;
    }
    last->next = info;
}

Node* rotateRight (Node* node) { //вращение в вправо
    Node* x = node->left;
    node->left = x->right;
    x->right = node;
    x->c = node->c;
    node->c = 'r';
    return x;
}

Node* rotateLeft (Node* node) {  //вращение в влево
    Node* x = node->right;
    node->right = x->left;
    x->left = node;
    x->c = node->c;
    node->c = 'r';
    return x;
}

void flipColors (Node* node) { //переворот цветов
    if (node->c == 'r') {
        node->c = 'b';
    } else {
        node->c = 'r';
    }
    if (node->left->c == 'r') {
        node->left->c = 'b';
    } else {
        node->left->c = 'r';
    }
    if (node->right->c == 'r') {
        node->right->c = 'b';
    } else {
        node->right->c = 'r';
    }
}

int isRed (Node *node) {
    if (!node) {
        return 0;
    }
    if (node->c == 'r') {
        return 1;
    } else {
        return 0;
    }
}

Node* fixUp (Node* node) { //исправление правых красных связей
    if (isRed(node->right)) {
        node = rotateLeft(node);
    }
    if (isRed(node->left) && isRed(node->left->left)) {
        node = rotateRight(node);
    }
    if (isRed(node->left) && isRed(node->right)) {
        flipColors(node);
    }
    return node;
}

Node* moveRedLeft (Node* node) { //перемещение красного потомка влево
    flipColors(node);
    if (node->right && isRed(node->right->left)) {
        node->right = rotateRight(node->right);
        node = rotateLeft(node);
        flipColors(node);
    }
    return node;
}

Node* moveRedRight (Node* node) {  //перемещение красного потомка вправо
    flipColors(node);
    if (node->left && isRed(node->left->left)) {
        node = rotateRight(node);
        flipColors(node);
    }
    return node;
}

Node* minNode (Node *node) {
    while (node && node->left) {
        node = node->left;
    }
    return node;
}

Node* maxNode (Node *node) {
    while (node && node->right) {
        node = node->right;
    }
    return node;
}

Node* insert (Node *node, unsigned int key, Info *info) {  //включение нового элемента в таблицу
    if (!node) {
        return makeNode(key, info);
    }
    if (key == node->key) {
        addToList(node, info);
    }
    else {
        if (key < node->key) {
            node->left = insert(node->left, key, info);
        }
        else {
            node->right = insert(node->right, key, info);
        }
        if (isRed(node->right) && !isRed(node->left)) {
            node = rotateLeft(node);
        }
        if (isRed(node->left) && isRed(node->left->left)) {
            node = rotateRight(node);
        }
    }
    if (isRed(node->left) && isRed(node->right)) {
        flipColors(node);
    }
    return node;
}

Node* removeMin (Node* node) { //удаление минимального элемента
    if (!node) {
        return NULL;
    }
    if (!node->left) {
        removeInfo(node->info);
        free(node);
        return NULL;
    }
    if (!isRed(node->left) && !isRed(node->left->left)) {
        node =  moveRedRight(node);
    }
    node->left = removeMin(node->left);
    return fixUp(node);
}

Node* removeNode (Node* node, unsigned int key) {  //удаление узла
    if (!node) {
        return NULL;
    }
    if (key < node->key) {
        if (node->left) {
            if (!isRed(node->left) && !isRed(node->left->left)) {
                node = moveRedLeft(node);
            }
            node->left = removeNode(node->left, key);
        }
    }
    else {
        if (key == node->key && node->info->next) {
            removeInfo(node->info);
            free(node);
            node = NULL;
            return node;
        }
        if (isRed(node->left)) {
            node = rotateRight(node);
        }
        if (key == node->key && !node->right) {
            removeInfo(node->info);
            free(node);
            return NULL;
        }
        if (node->right) {
            if (!isRed(node->right) && !isRed(node->right->left)) {
                node = moveRedRight(node);
            }
            if (key == node->key) {
                Node *tmp = minNode(node->right);
                node->key = tmp->key;
                node->info = tmp->info;
                tmp->info = NULL;
                node->right = removeMin(node->right);
            }
            else {
                node->right = removeNode(node->right, key);
            }
        }
    }
    return fixUp(node);
}

Info* find (Node* node, unsigned key) {  //поиск информации по заданному ключу
    if (!node) {
        return NULL;
    }
    if (key == node->key) {
        return node->info;
    }
    if (key < node->key) {
        return find(node->left, key);
    }
    else {
        return find(node->right, key);
    }
}

Node* deleteTree (Node* node) {
    if (node) {
        deleteTree(node->left);
        deleteTree(node->right);
        removeInfo(node->info);
        free(node);
        node = NULL;
    }
    return NULL;
}

void printTree (Node* _node, unsigned int a, unsigned int b) { 
    if (_node != NULL) {
        if (_node->key >= a) {
            printTree(_node->left, a, b);
        }
        if (_node->key >= a && _node->key < b) {
            printf("\n==%u==\n", _node->key);
            printInfo(_node->info);
        }
        if (_node->key < b) {
            printTree(_node->right, a, b);
        }
    }
}

void DOT (Node* _node) {
    if (_node != NULL) {
        DOT(_node->left);
        printf("\t%d -> ", _node->key);
        printf("{");
        if (_node->right) {
            printf("%d ", _node->right->key);
        }
        if (_node->left) {
            printf("%d ", _node->left->key);
        }
        printf("}\n");
        DOT(_node->right);
    }
}

void print (Node* _node, int level) { //вывод в виде дерева
    if(_node) {
        print(_node->left, level+1);
        for(int i = 0; i < level; i++)
            printf("\t\t");
        printf("%u(%c)\n", _node->key, _node->c);
        print(_node->right, level+1);
    }
}

Node* inputFromFile (FILE *file) {
    Node* root = NULL;
    unsigned int key;
    float x;
    char* str;
    while (fscanf(file, "%u", &key) == 1) {
        fscanf(file, "%f", &x);
        str = get_str(file);
        Info* info = makeInfo(str, x);
        root = insert(root, key, info);
        free(str);
    }
    return root;
}

void find_diff (Node* _node, int _key, int* min_diff, int* min_diff_key) {
    if (_node == NULL) {
        return;
    }
    if (_node->key != _key) {
        if (*min_diff > abs(_node->key - _key))
        {
            *min_diff = abs(_node->key - _key);
            *min_diff_key = _node->key;
        }
    }
    if (_key < _node->key) {
        find_diff(_node->left, _key, min_diff, min_diff_key);
    }
    else {
        find_diff(_node->right, _key, min_diff, min_diff_key);
    }
}

int find_mismatched_key (Node *root, int _key) {
    int min_diff = INT_MAX;
    int min_diff_key = -1;
    find_diff(root, _key, &min_diff, &min_diff_key);
  
    return min_diff_key;
}

void test (int n) {
    Node* root = NULL;
    double avgTime = 0;
    unsigned int key = 0;
    for (int i = 0; i < n; i++) {
        key = rand()%500;
        Info *info = makeInfo("test", 1.1);
        clock_t t = clock();
        root = insert(root, key, info);
        t = clock() - t;
        avgTime += (double)t/CLOCKS_PER_SEC;
    }
    printf("Insert time: %lf s\n", avgTime);
    deleteTree(root);
}