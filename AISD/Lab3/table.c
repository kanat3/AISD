#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "table.h"

#define SIZE_OF_SPACE 4

Table* create_table (void) {

	Table* table;

	table = (Table*)calloc(1, sizeof(Table));

	if (table == NULL) {
		puts("\nMemory error!\n");
		return NULL;
	}

	table->ks1 = (KeySpace1*)calloc(SIZE_OF_SPACE, sizeof(KeySpace1));
	if (table->ks1 == NULL) {
		puts("\nMemory error!\n");
		return NULL;
	}

	table->ks2 = (KeySpace2*)calloc(SIZE_OF_SPACE, sizeof(KeySpace2));
	if (table->ks2 == NULL) {
		puts("\nMemory error!\n");
		return NULL;
	}

	table->size1 = 0;
	table->size2 = 0;

	for (int i = 0; i < SIZE_OF_SPACE; i++) {
		table->ks1[i].info = NULL;
		table->ks2[i].info = NULL;
	}

	puts("\nTable is created\n");
	return table;
}

int _feof (void) {

	if (feof(stdin)){
		puts("\nEnd\n");
		return 0;
	}
	return -5;
}

char* get_str() {
	char buf[80] = {0};
	char* res = NULL;

	int len = 0;
	int n = 0;
	do {
		n = scanf("%80[^\n]",buf);
		if(n < 0){
			if(!res) return NULL;	
		} else if(n > 0){
			int chunk_len = strlen(buf);
			int str_len = len + chunk_len;
			res = realloc(res,str_len + 1);
			memcpy(res + len, buf, chunk_len);
			len = str_len;
		} else{
			scanf("%*c");
		}
	} while(n > 0);

	if(len > 0) res[len] = '\0';
	else res = calloc(1,sizeof(char));
	if (!_feof()) {
		free(res);
		return NULL;
	}
	return res;
}

void get_key1 (unsigned int* key1) {
	puts("\nEnter unsigned integer (key = 65535 for empty key space): \n");
	int k;
	do {
		k = scanf("%u", key1);
		if (!_feof()) {
			*key1 = -5;
			return;
		}
		if (k <= 0) {
			puts("\nError. try again\n");
			int c;
			while ((c = getchar()) != '\n') {}			
		}
	} while (k <= 0);
	while (getchar() != '\n') {}
}

char* get_key2 (void) {
	char* key2;
	puts("\nEnter a string: \n");
	key2 = get_str();
	if (!_feof()) {
		free(key2);
		return NULL;
	}
	return key2;
}

int get_release (void) {

	puts("\nEnter release of information: \n");
	int k;
	int release;
	do {
		k = scanf("%d", &release);
		if (!_feof()) {
			return -5;
		}
		if (k <= 0) {
			puts("\nError. try again\n");
			int c;
			while ((c = getchar()) != '\n') {}			
		}
	} while (k <= 0);
	return release;
}

int general_insert (Table* table, unsigned int key1, char* key2, int rel1) {

	//add ks1

	if (table->size1 == SIZE_OF_SPACE) {
		puts("\nTable 1 is full.\n");
		return -2;
	}

	char* info;
	info = get_str();

	if (!info) {
		free(info);
		return-3;
	}

	int i = table->size1;
	printf("size table %d\n", table->size1);

	table->ks1[i].info = (Info*)calloc(1, sizeof(Info));

	if (table->ks1[i].info == NULL) {
		puts("\nMemory error!\n");
		return -2;
	}

	if (rel1 >= 1) {
		table->ks1[i].info->release = ++rel1;
	} else {
		table->ks1[i].info->release = 1;
	}

	table->ks1[i].key = key1;
	table->ks1[i].info->string = info;
	table->ks1[i].info->busy = 1;
	table->size1++;
	printf("size table after add %d\n", table->size1);

	//add ks2

	if (table->size2 == SIZE_OF_SPACE) {
		puts("\nTable 2 is full.\n");
		return -2;
	}

	char* str_info;
	str_info = (char*)calloc(strlen(info)+2, sizeof(char));

	if (str_info == NULL) {
		puts("\nMemory error!\n");
		free(info);
		return -2;
	}

	strcpy(str_info, info);
	int w = 0;
	int hash = 0;
	for (w; w < SIZE_OF_SPACE; w++) {

		hash = hash2(w);

		if ((table->ks2[hash].info == NULL) || (table->ks2[hash].info->busy != 1)) {

			Info* info = (Info*)calloc(1, sizeof(Info));

			if (info == NULL) {
				puts("\nMemory error!\n");
				return -2;
			}
			info->string = str_info;
			info->busy = 1;
			int last_release;
			last_release = find_key_ks2(table, key2);

			if (last_release <= 0) {
				info->release = 1;
			} else {
				info->release = last_release + 1;
			}

			table->ks2[hash].info = info;
			table->ks2[hash].key = key2;
			table->size2 = table->size2 + 1;

			return 1;

		}
	}
}

void general_search (Table* table) {

	unsigned int key1;
	get_key1(&key1);

	if (key1 == -5) {
		return;
	}

	char* key2;
	key2 = get_key2();

	if (!key2) {
		free(key2);
		return;
	}

	if (find_key_ks1(table, key1) >= 0) {
		puts("\nKey 1 in table\n");

	} else {
		puts("\nThere no key 1\n");
	}

	if (find_key_ks2(table, key2) > 0) {
		puts("\nKey 2 in table\n");

	} else {
		puts("\nThere no key 2\n");
	}
	free(key2);
}

void general_release_delete (Table* table) {

	unsigned int key1;
	get_key1(&key1);

	if (key1 == -5) {
		return;
	}

	char* key2;
	key2 = get_key2();

	if (!key2) {
		free(key2);
		return;
	}

	int release;
	release = get_release();

	if (release == -5) {
		return;
	}

	if (delete_release_item_ks1(table, key1, release) > 0) {
		puts("\nInformation ready for deletion (busy = 0)\n");
	}

	delete_release_item_ks2(table, key2, release);
	free(key2);

}

void general_show (Table* table) {

	puts("\n--------------------------Keyspace 1------------------------------\n");
	show_data_in_ks1(table);
	puts("\n--------------------------Keyspace 2------------------------------\n");
	show_data_in_ks2(table);

}

void general_release_search (Table* table) {

	unsigned int key1;
	get_key1(&key1);

	if (key1 == -5) {
		return;
	}

	char* key2;
	key2 = get_key2();

	if (!key2) {
		free(key2);
		return;
	}

	int release;
	release = get_release();

	if (release == -5) {
		return;
	}

	Info* info1;
	info1 = find_release_item_ks1(table, key1, release);

	if (info1 != NULL) {
		printf("\nKey: %d | Information: %s | Release of info: %d\n", key1, info1->string, info1->release);
	} else {
		puts("\nCan't find\n");
	}

	Info* info2;
	info2 = find_release_item_ks2(table, key2, release);

	if (info2 != NULL) {
		printf("\nKey: %s | Information: %s | Release of info: %d\n", key2, info2->string, info2->release);
	}
	free(key2);
}

void general_exit (Table* table) {

	delete_ks1(table);
	delete_ks2(table);
	free(table);
	puts("\nExit\n");

}

void general_key_delete (Table* table) {

	unsigned int key1;
	get_key1(&key1);

	if (key1 == -5) {
		return;
	}

	char* key2;
	key2 = get_key2();

	if (!key2) {
		free(key2);
		return;
	}

	delete_items_ks1(table, key1);
	delete_items_ks2(table, key2);
	free(key2);

}