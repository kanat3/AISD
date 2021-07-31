#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "table.h"

#define SIZE_OF_SPACE 4
#define EMPTY_KEY 65535

int find_key_ks1 (Table *table, unsigned int key) { //вернет релиз послeднего совпадающего ключа, если он есть

	int k = 0;

    for (int i = 0; i < SIZE_OF_SPACE; ++i) {
        if (table->ks1[i].key == key && table->ks1[i].info) {
        	k++;
        }
    }

    if (k == 0) {
    	return -1;
    }

    return k;
}

void show_item_ks1 (Table* table, unsigned int key, int release) { //проверки на корректность данных нет
	
	Info* info;
	info = find_release_item_ks1(table, key, release);

	if (info->busy == 1) {
		printf("\nKey: %d | Information: %s | Release of info: %d\n", key, info->string, info->release);
	} else {
		puts("\nThis information may have been deleted\n");
	}
}

Info* find_release_item_ks1 (Table* table, unsigned int key, int release) {

	int k;
	k = find_key_ks1(table, key); // есть ли такой ключ и какой релиз

	if (release > k || release < 0) {

		puts("\nThere are not this release\n");
		return NULL;

	}

	if (k < 0) {

		puts("\nThere is no such key\n");
		return NULL;

	} else {

		for (int i = 0; i < SIZE_OF_SPACE; i++) {

			if (table->ks1[i].key == key) {

				if ((table->ks1[i].info) && (table->ks1[i].info->release == release) && (table->ks1[i].info->busy == 1)) {

					return table->ks1[i].info;
				}
			}
		}
	}

	puts("\nInformation is deleted\n");
	return NULL;
}

int add_in_table_ks1 (Table* table, unsigned int key1, char* info) {

	if (table->size1 == SIZE_OF_SPACE) {
		puts("\nTable 1 is full.\n");
		return -2;
	}

	int i = 0;

	i = table->size1 + 1;

	table->ks1[i].info = (Info*)calloc(1, sizeof(Info));

	if (table->ks1[i].info == NULL) {
		puts("\nMemory error!\n");
		return -2;
	}

	int rel;
	rel = find_key_ks1(table, key1); // есть ли такой ключ и какой релиз

	if (rel >= 1) {
		table->ks1[i].info->release = ++rel;
	} else {
		table->ks1[i].info->release = 1;
	}

	table->ks1[i].key = key1;
	table->ks1[i].info->string = info;
	table->ks1[i].info->busy = 1;
	table->size1++;

}

void show_data_in_ks1 (Table* table) {

	int i = 0;

	while (i != SIZE_OF_SPACE) {

		if (table->ks1[i].info != NULL && table->ks1[i].info->busy != 0) {
			printf("\nKey: %d | Information: %s | Release of info: %d\n", table->ks1[i].key, table->ks1[i].info->string, table->ks1[i].info->release);
		}

		i++;
	}
}

int delete_release_item_ks1 (Table* table, unsigned int key, int _release) {

	Info* info = NULL;
	Info* info2 = NULL;
	int k;
	k = find_key_ks1(table, key);

	if (k >= 0) {
		puts("\nKey in table\n");
	} else {
		puts("\nThere no this key\n");
		return -5;
	}

	info = find_release_item_ks1(table, key, _release);

	if (info == NULL) {
		puts("\nUnable to delete\n");
		return -3;
	}

	info->busy = 0; 

	for (int i = _release; i <= k; i++) {
		info2 = find_release_item_ks1(table, key, i);
		if (info2) {
			info2->release--;
		}
	} //релизы меняются после удаления

	return 1;
}

int delete_items_ks1 (Table* table, unsigned int key) { //после удаление должна быть сборка мусора

	int k;
	k = find_key_ks1(table, key); //последний имеющийся релиз

	if (k < 0) {
		puts("\nKey is not find\n");
		return -4;
	} 

	Info* info = NULL;

	for (int i = 1; i <= k; i++) {
		info = find_release_item_ks1(table, key, i);
		if (info && info->busy) {
			info->busy = 0;
		}
	}

	puts("\nAll information ready for deletion (busy = 0)\n");

	return 1;
}

/*void collect_deleted_items (Table* table) {

	if (table->size1 == 0) {
		return;
	}

	for (int i = 0; i < SIZE_OF_SPACE; i++) {
		if ((table->ks1[i].info != NULL) && (table->ks1[i].info->busy == 0)) {
			free(table->ks1[i].info->string);
			free(table->ks1[i].info);
			table->ks1[i].key = EMPTY_KEY;
			table->ks1[i].info = NULL;
			table->size1--;
		}
	}

	if ((table->ks1[i].info != NULL) && (table->ks1[i].info->busy != 0) )
}*/

void collect_deleted_items (Table* table) {

	if (table->size1 == 0) {
		return;
	}

	int t = -1;

	for (int i = 0; i < table->size1; i++) {
		if ((table->ks1[i].info != NULL) && (table->ks1[i].info->busy == 0)) {
			free(table->ks1[i].info->string);
			free(table->ks1[i].info);
			table->ks1[i].key = EMPTY_KEY;
			table->ks1[i].info = NULL;
			table->size1--;
			t = i; //один раз пройдет
		}
	}

	if ((table->ks1[t + 1].info != NULL) && (table->ks1[t + 1].info->busy != 0) && t >= 0) {
		table->ks1[t].info = (Info*)calloc(1, sizeof(Info));
		table->ks1[t].key = table->ks1[t + 1].key;
		table->ks1[t].info->busy = table->ks1[t + 1].info->busy;
		table->ks1[t].info->release = table->ks1[t + 1].info->release;
		char* str_info;
		str_info = (char*)calloc(strlen(table->ks1[t + 1].info->string)+1, sizeof(char));
		strcpy(str_info, table->ks1[t + 1].info->string);
		table->ks1[t].info->string = str_info;
		table->ks1[t + 1].info->busy = 0;
		table->ks1[t].info->busy = 1;
		free(table->ks1[t + 1].info->string);
		free(table->ks1[t + 1].info);
		table->ks1[t + 1].info = NULL;
	}
}

void delete_ks1 (Table* table) {

	for (int i = 0; i < SIZE_OF_SPACE; i++) {
		if (table->ks1[i].info != NULL) {
			free(table->ks1[i].info->string);
			free(table->ks1[i].info);
		} else {
			free(table->ks1[i].info);
			table->ks1[i].key = EMPTY_KEY;
			table->ks1[i].info = NULL;
			table->size1--;
		}
	}
	free(table->ks1);
}
