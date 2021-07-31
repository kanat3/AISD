#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "table.h"

#define SIZE_OF_SPACE 4

unsigned int hash2 (int i) {
	return (i + 9) % SIZE_OF_SPACE;
}

int find_key_ks2 (Table *table, char* key) { //пусть вернет последний релиз

	int release = 0;

	for (int i = 0; i < SIZE_OF_SPACE; i++) {
		if (table->ks2[i].info != NULL) {
			if ((table->ks2[i].info->busy == 1) && (strcmp(table->ks2[i].key, key) == 0)) {
				release++;
			}
		}	
	}

	return release;
}

int add_in_table_ks2 (Table* table, char* key2, char* str_info) {

	if (table->size2 == SIZE_OF_SPACE) {
		puts("\nTable 2 is full.\n");
		return -2;
	}

	int i = 0;
	int hash;

	for (i; i < SIZE_OF_SPACE; i++) {

		hash = hash2(i);

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

void show_data_in_ks2 (Table* table) {

	for(int i = 0; i < SIZE_OF_SPACE; i++) {

		if (table->ks2[i].info != NULL) {

			if (table->ks2[i].info->busy == 1) {
				printf("\nKey: %s | Information: %s | Release of info: %d\n", table->ks2[i].key, table->ks2[i].info->string, table->ks2[i].info->release);
			}
		}
	}
}

Info* find_release_item_ks2 (Table* table, char* key, int release) {

	if (table->size2 = 0) {
		puts("\nTable is empty\n");
		return NULL;
	}

	int rel = find_key_ks2(table, key);

	if ((rel <= 0) || (release > rel) || (release <= 0)) {
		puts("\nCan't find\n");
		return NULL;
	}

	for (int i = 0; i < SIZE_OF_SPACE; i++) {
		if (table->ks2[i].info != NULL) {
			if ((strcmp(table->ks2[i].key, key) == 0)) {
				if (table->ks2[i].info->busy == 1 && table->ks2[i].info->release == release) {
					return table->ks2[i].info;
				}
			}
		}
	}

	return NULL;

}

void show_item_ks2 (Table* table, char* key, int release) {

	if (table->size2 = 0) {

		puts("\nTable is empty\n");

	} else {

		Info* info = NULL;
		info = find_release_item_ks2(table, key, release);

		if (info != NULL) {
			printf("\nKey: %s | Information: %s | Release of info: %d\n", key, info->string, info->release);	
		}
	}
}

void delete_release_item_ks2 (Table* table, char* key, int release) {

	Info* to_delete = NULL;
	to_delete = find_release_item_ks2(table, key, release);

	int last_release = find_key_ks2(table,key);

	if (last_release > 0) {

		int u = release + 1;

		for (u; u <= last_release; u++) {
			Info* info = NULL;
			info = find_release_item_ks2(table, key, u);
			info->release--;
		}

		if (to_delete != NULL && to_delete->busy != 0) {
			to_delete->busy = -1;
			free(to_delete->string);
			to_delete->string = NULL;
			to_delete->release = -10;
		}

		table->size2--;
	}
}

void delete_items_ks2 (Table* table, char* key) {

	int last_release;
	last_release = find_key_ks2(table, key);

	if (last_release > 0) {

		for (int i = 1; i <= last_release; ++i) {
			delete_release_item_ks2(table, key, i);
		}

		if (last_release > 1) {
			delete_release_item_ks2(table, key, 1);
		}

		puts("\nAll information deleted\n");		
	}

}

void delete_ks2 (Table* table) {

	for (int i = 0; i < SIZE_OF_SPACE; i++) {

		if (table->ks2[i].info != NULL) {
			free(table->ks2[i].info->string);
		}
		free(table->ks2[i].info);
		free(table->ks2[i].key);
	}
	free(table->ks2);	
	table->size2 = 0;
}