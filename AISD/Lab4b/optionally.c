#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int _feof (void) {
    if (feof(stdin)){
        puts("\nEnd\n");
        return 0;
    }
    return -5;
}

char* get_str (FILE *file) {
    char *str = NULL;
    char buf[81];
    size_t size = 0;
    int res;
    do {
        res = fscanf(file, "%80[^\n]", buf);
        if (res == 0) {
            fscanf(file, "%*c"); 
            if (!str) {
                str = malloc(sizeof(char));
                str[0] = '\0';
            }
        } else if (res > 0) {
            size_t len = strlen(buf);
            str = realloc(str, (size+len+1)*sizeof(char));
            memcpy(str+size, buf, len);
            size += len;
            str[size] = '\0';
        }
    } while (res > 0);
    return str;
}

unsigned int get_int (void) {
    puts("\nEnter an integer: \n");
    int k;
    unsigned int key1;
    do {
        k = scanf("%u", &key1);
        if (_feof() == 0) {
            exit(-1);
        }
        if (key1 < 0) {
            puts("\nYou must enter unsigned integer!");
            k = 0;
        }
        if (k <= 0) {
            puts("\nError. Try again\n");
            int c;
            while ((c = getchar()) != '\n' && c != EOF) {}          
        }
    } while (k <= 0);
    while (getchar() != '\n') {}
    return key1;
}

float get_float (void) {
    puts("\nEnter a float: \n");
    int k;
    float key1;
    do {
        k = scanf("%f", &key1);
        if (_feof() == 0) {
            exit(-1);
        }
        if (k <= 0) {
            puts("\nError. try again\n");
            int c;
            while ((c = getchar()) != '\n' && c != EOF) {}          
        }
    } while (k <= 0);
    while (getchar() != '\n') {}
    return key1;
}