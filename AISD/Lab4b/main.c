#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <time.h>
#include "optionally.h"
#include "llrb.h"

#define DIALOG_CHOICES 10
#define TEST_FILE "data.txt"

int dialog (void) {
    puts("\n");
    puts("1. Insert information from file\n");
    puts("2. Insert new information\n");
    puts("3. Find information by key\n");
    puts("4. Find information in the range\n");
    puts("5. Delete information by key\n");
    puts("6. Search information by mismatched key\n");
    puts("7. Show\n");
    puts("8. Banchmark\n");
    puts("9. DOT file\n");
    puts("10. Exit\n");
    puts("\nMake your choice:\n");

    int k, choice;
    k = scanf("%d", &choice);
    if (_feof() == 0) {
        exit(-1);
    }
    getchar();
    if (k <= 0) {
        puts("\nYou're wrong. Try again!\n");
        while (getchar() != '\n') {}
        return -1;
    }
    if ((choice < 1) || (choice > DIALOG_CHOICES)) {
        puts("\nYou're wrong. Try again!\n");
        return -2;
    }
    return choice;
}

int main (void) {

    int choise = -1;
    Node* root = NULL;
    unsigned int key, key1, key2, n;
    char* str1 = NULL;
    float x;
    Info* info;
    FILE* file;
    Node* node;

    while (choise < 0 || choise != DIALOG_CHOICES) {
        do {
            choise = dialog();
        } while (choise < 0);

        if (choise == 1) {
            printf("\nOpen file %s\n", TEST_FILE);
            file = fopen(TEST_FILE, "rt");
            if (file) {
                root = deleteTree(root);
                root = inputFromFile(file);
                fclose(file);
                puts("==Ok==");
            }
            else {
                printf("==Can't open file==\n");
            }
        } else if (choise == 2) {
            key = get_int();
            x = get_float();
            puts("\nEnter a string:\n");
            str1 = get_str(stdin);
            info = makeInfo(str1, x);
            root = insert(root, key, info);
            root->c = 'b';
            free(str1);
        } else if (choise == 3) {
            key = get_int();
            info = find(root, key);
            if (info) {
                puts("\nKey exist:\n");
                printInfo(info);
            }
            else {
                puts("\n==Can't find==\n");
            }
        } else if (choise == 4) {
            puts("\nEnter range (int 1, int 2): \n");
            key1 = get_int();
            key2 = get_int();
            printTree(root, key1, key2);
        } else if (choise == 5) {
            key = get_int();
            root = removeNode(root, key);
            if (root) {
                root->c = 'b';
            }
        } else if (choise == 6) {
            int near_key;
            key = get_int();
            node = NULL;
            near_key = find_mismatched_key(root, key);
            if (near_key >= 0) {
                printf("\nThe nearest key is: %u\n", near_key);
            }
            else {
               printf("==Can't find==\n");
            }
        } else if (choise == 7) {
            print(root, 0);
        } else if (choise == 8) {
            printf("\nEnter the number of nodes:\n");
            n = get_int();
            test(n);
        } else if (choise == 10) {
            deleteTree(root);
        } else if (choise == 9) {
            printf("\ndigraph G {\n");
            DOT(root);
            printf("}");
        }
    }
    return 0;
}