#include <stdio.h>
#include "table.h"
#include <string.h>
#include <stdlib.h>

#define DIALOG_CHOICES 8
#define FILE_NAME "test.bin"

int dialog (void) {
	puts("\n");
	puts("1. Insert information\n");
	puts("2. Is your key in the table?\n");
	puts("3. Find information with key + release\n");
	puts("4. Show\n");
	puts("5. Delete information with key + release\n");
	puts("6. Delete information with key\n");
	puts("7. Read table from file\n");
	puts("8. Exit\n");
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
	//bin file
	/*
	puts("\nEnter file name to write table:");
	char* file_name = get_key2();
	if (!file_name) {
		free(file_name);
		return;
	}
	*/
	//size_of_table
	/*
	int size;
	puts("Enter size of table: ");
	get_key1(&size);
	*/
	table = create_table(11); //size

	while (choise < 0 || choise != DIALOG_CHOICES) {
		do {
			choise = dialog();
		} while (choise < 0);
		if (choise == 1) {
			general_insert(table);
			Twrite(table, FILE_NAME);
		} else if (choise == 2) {
			general_search(table);
		} else if (choise == 5) {
 			general_release_delete(table);
 			Twrite(table, FILE_NAME);
		} else if (choise == 4) {
			general_show(table);
		} else if (choise == 3) {
			general_release_search(table);
		} else if (choise == 8) {
			general_exit(table);
			return 6;
		} else if (choise == 6) {
			general_key_delete(table);
			Twrite(table, FILE_NAME);
		} else if (choise == 7) {
			//general_file(table);
			general_exit(table);
			table = Tread(table, FILE_NAME);
			general_show(table);
		}
	}
	return 0;
}