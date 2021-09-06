#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "tree.h"

#define DIALOG_CHOICES 7

int _feof (void) {

	if (feof(stdin)){
		puts("\nEnd\n");
		return 0;
	}
	return -5;
}

void get_key (unsigned int* key1) {
	puts("\nEnter integer: \n");
	int k;
	do {
		k = scanf("%u", key1);
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
}

void read_random_tree (Node** _node, char* test_file) {

	FILE* file = fopen(test_file, "rb");
	int key;
	float a, b;
	char string[10];
	while (!feof(file)) {
		fscanf(file, "%d%f%f%s", &key, &a, &b, string);
		Info* info = create_info(a, b, string);
		insert(_node, info, key);
	}
	fclose(file);
}

int dialog (void) {
	puts("\n");
	puts("1. Insert information\n");
	puts("2. Find information by key\n");
	puts("3. Find information in the range\n");
	puts("4. Delete information by key\n");
	puts("5. Search information by mismatched key\n");
	puts("6. Show\n");
	puts("7. Exit\n");
	puts("\nMake your choice: \n");
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

	while (choise < 0 || choise != DIALOG_CHOICES) {
		do {

			choise = dialog();

		} while (choise < 0);

		if (choise == 1) {

			/*test
			char* test_file = "test.bin";
			read_random_tree(&root, test_file);
			*/

			Info* info1 = create_info(1.6, 2.7, "Hello1");
			Info* info2 = create_info(5.8, 4.5, "Hello2");
			Info* info3 = create_info(7.6, 3.6, "Hello3");
			Info* info4 = create_info(73.6, 3.652, "Hello4");
			Info* info5 = create_info(74.655, 32.6253, "Hello5");
			Info* info6 = create_info(75.62, 342.632, "Hello6");
			Info* info7 = create_info(5.2, 2.2, "Hello7");
			Info* info8 = create_info(55.2, 2.52, "Hello8");
			Info* info9 = create_info(22.2, 6.95, "Hello9");
			Info* info10 = create_info(24.32, 2.65, "Hello10");
			Info* info11 = create_info(3.62, 8.9, "Hello11");
			insert(&root, info1, 50);
			insert(&root, info2, 24);
			insert(&root, info3, 31); 
			insert(&root, info4, 20);
			insert(&root, info5, 70);
			insert(&root, info6, 25);
			insert(&root, info7, 12);
			insert(&root, info8, 66);
			insert(&root, info9, 12);
			insert(&root, info10, 134);
			insert(&root, info11, 12);

		} else if (choise == 2) {

			int key;
			get_key(&key);
			Node* n = search_by_key(&root, key);
			if (n != NULL) {
				show_info(n->info, key);
			} else {
				puts("\nNo key\n");
			}

		} else if (choise == 3) {

			puts("\n\nRange (int 1, int 2): \n");
			puts("\nIf you enter int 2 < int 1. We will use range (int 1, int 2)");
			int range1;
			int range2;
			get_key(&range1);
			get_key(&range2);

			if (range2 < range1) {
				int k;
				k = range2;
				range2 = range1;
				range1 = k;
			}

			for (range1; range1 <= range2; range1++) {

				Node* n = search_by_key(&root, range1);
				if (n != NULL) {
					show_info(n->info, range1);
				}
			}


		} else if (choise == 4) {

			int key;
			get_key(&key);
			delete_node(&root, key);

		} else if (choise == 5) {
			int key;
			get_key(&key);
			printf("the nearest key is: %d\n", find_mismatched_key(root, key));

		} else if (choise == 6) {

			int n = 0;
			show_tree(root, n, 1);

		} else if (choise == 7) {

			delete_tree(&root);

		}
	}
	return 0;
}