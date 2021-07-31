#include <stdio.h>
#include "table.h"
#include <string.h>
#include <stdlib.h>

#define DIALOG_CHOICES 7

int dialog (void) {
	puts("\n");
	puts("1. Insert information\n");
	puts("2. Is your key in the table?\n");
	puts("3. Find information with key + release\n");
	puts("4. Show\n");
	puts("5. Delete information with key + release\n");
	puts("6. Delete information with key\n");
	puts("7. Exit\n");
	puts("\nMake your choice: \n");
	int k, choice;
	k = scanf("%d", &choice);
	if (!_feof()) {
		return 7;
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
	Table* table;

	puts("\n------------------------START------------------------\n");
	table = create_table();

	while (choise < 0 || choise != DIALOG_CHOICES) {
		do {

			choise = dialog();

		} while (choise < 0);

		if (choise == 1) {

			collect_deleted_items(table);

			unsigned int key1;
			char* key2;

			get_key1(&key1);
			key2 = get_key2();

			int rel1, rel2;
			rel1 = find_key_ks1(table, key1); // есть ли такой ключ и какой релиз
			rel2 = find_key_ks2(table, key2);

			if ((rel1) > 0 && (rel2 > 0)) {

				puts("\nKeys (key1, key2) in space 1 and 2 are exit\n");
				puts("\nCan't add\n");
				free(key2);

			} else {

				puts("\nEnter information:\n");
				general_insert(table, key1, key2, rel1);

			}

		} else if (choise == 2) {

			general_search(table);

		} else if (choise == 5) {

			general_release_delete(table);

		} else if (choise == 4) {

			general_show(table);

		} else if (choise == 3) {

			general_release_search(table);

		} else if (choise == 7) {

			collect_deleted_items(table);
			general_exit(table);
			return 6;

		} else if (choise == 6) {

			general_key_delete(table);

		}
	}
	return 0;
}