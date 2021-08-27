#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "table.h"

#define STEP 9

/***********************************************************************************************/
//Реализация функций хеш-таблицы, разрешение коллизий -- перемешивание с шагом STEP
//Одинаковые ключи допустимы
/***********************************************************************************************/

unsigned int hash2 (int key, int SIZE_OF_SPACE) {
	return key%SIZE_OF_SPACE;
}

int find_key_ks2 (Table *table, char* key) { //пусть вернет последний релиз
	int hash, i, release;
	release = 0;
	hash = i = hash2(strlen(key), table->max_size);
	while (table->ks2[i].info || (table->ks2[i].info && table->ks2[i].info->busy >= 0)) {
		if (table->ks2[i].info && table->ks2[i].info->busy == 1 && (strcmp(table->ks2[i].key, key) == 0)) {
			++release;
		}
		i = (i + STEP) % table->max_size;
		if (i == hash) {
			break;
		}
	}
	return release;
}

Table* Tfind_key_ks2 (Table* table, char* key) { //выводит таблицу и удаляет (потом сделать это в мейне)
	int hash, i;
	Table* search_table = create_table(table->max_size);
	hash = i = hash2(strlen(key), table->max_size);
	while (table->ks2[i].info && table->ks2[i].info->busy == 1) {
		if (table->ks2[i].info && table->ks2[i].info->busy == 1 && (strcmp(table->ks2[i].key, key) == 0)) {
			char* str_info;
			char* str_key;
			str_info = (char*)calloc(strlen(table->ks2[i].info->string)+1, sizeof(char));
			str_key = (char*)calloc(strlen(table->ks2[i].key)+1, sizeof(char));
			strcpy(str_key, table->ks2[i].key);
			strcpy(str_info, table->ks2[i].info->string);
			add_in_table_ks2(search_table, str_key, str_info);
		}
		i = (i + STEP) % table->max_size;
		if (i == hash) {
			break;
		}
	}
	show_data_in_ks2(search_table);
	delete_ks2(search_table);
	free(search_table->ks1);
	free(search_table);
	return NULL;
}

KeySpace2* add_in_table_ks2 (Table* table, char* key2, char* str_info) {
	if (table->size2 == table->max_size) {
		puts("\nTable 2 is full.\n");
		return NULL;
	}
	int i, hash;
	hash = i = hash2(strlen(key2), table->max_size);
	while (table->ks2[i].info && table->ks2[i].info->busy == 1) {
		i = (i + STEP) % table->max_size;
		if (i == hash) {
			return NULL; //прошли всю таблицу до исходного значения hash
		}
	}
	Info* info = (Info*)calloc(1, sizeof(Info));
	if (info == NULL) {
		puts("\nMemory error!\n");
		return NULL;
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
	//заполнили поле информации !!!!!нет ссылки
	/*******************************/
	if (table->ks2[i].info && table->ks2[i].info->busy == -1) {
		free(table->ks2[i].key);
		free(table->ks2[i].info);
	}
	/*******************************/
	table->ks2[i].info = info;
	table->ks2[i].key = key2;
	table->size2 = table->size2 + 1;
	return &table->ks2[i];
}

void show_data_in_ks2 (Table* table) {
	for(int i = 0; i < table->max_size; i++) {
		if (table->ks2[i].info != NULL && table->ks2[i].info->busy == 1) {
			//printf("\n ==%d== Key: %s | Information: %s | Release of info: %d\n", i, table->ks2[i].key, table->ks2[i].info->string, table->ks2[i].info->release);
			if (table->ks2[i].link) {
				printf("\n ==%d== Key: %s | Information: %s | Release of info: %d | Key in keyspace1: %d \n", i, table->ks2[i].key, table->ks2[i].info->string, table->ks2[i].info->release, table->ks2[i].link->key);
			} else {
				printf("\n ==%d== Key: %s | Information: %s | Release of info: %d\n", i, table->ks2[i].key, table->ks2[i].info->string, table->ks2[i].info->release);
			}
		}
	}
}

Info* find_release_item_ks2 (Table* table, char* key, int release) {
	int hash, i;
	hash = i = hash2(strlen(key), table->max_size);
	while (table->ks2[i].info || (table->ks2[i].info && table->ks2[i].info->busy >= 0)) {
		if (table->ks2[i].info->release == release && table->ks2[i].info->busy == 1 && (strcmp(table->ks2[i].key, key) == 0)) {
			return table->ks2[i].info;
		}
		i = (i + STEP) % table->max_size;
		if (i == hash) {
			break;
		}
	}
	return NULL;
}

KeySpace2* find_release_struct_ks2 (Table* table, char* key, int release) {
	int hash, i;
	hash = i = hash2(strlen(key), table->max_size);
	while (table->ks2[i].info || (table->ks2[i].info && table->ks2[i].info->busy >= 0)) {
		if (table->ks2[i].info->release == release && table->ks2[i].info->busy == 1 && (strcmp(table->ks2[i].key, key) == 0)) {
			return &table->ks2[i];
		}
		i = (i + STEP) % table->max_size;
		if (i == hash) {
			break;
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
	KeySpace2* to_delete;
	Info* info = NULL;
	to_delete = find_release_struct_ks2(table, key, release);
	if (!to_delete) {
		return;
	}
	int last_release = find_key_ks2(table, key);
	if (last_release > 0) {
		int u = release + 1;
		for (u; u <= last_release; u++) {
			info = find_release_item_ks2(table, key, u);
			if (info) {
				info->release--;
			}
		}
		if (to_delete) {
			if (to_delete->info && to_delete->info->busy != 0) {
				if (to_delete->link && to_delete->link->link) {
					to_delete->link->link = NULL;
				}
				to_delete->info->busy = -1;
				free(to_delete->info->string);
				to_delete->info->string = NULL;
				to_delete->info->release = -10;
			}
		}
		table->size2--;
	}
}

void delete_items_ks2 (Table* table, char* key) {
	int last_release;
	last_release = find_key_ks2(table, key);
	if (last_release > 0) {
		for (int i = last_release; i >= 1; i--) {
			delete_release_item_ks2(table, key, i);
		}
		puts("\nAll information deleted\n");
	}
}

void delete_ks2 (Table* table) {
	for (int i = 0; i < table->max_size; i++) {
		if (table->ks2[i].info != NULL) {
			free(table->ks2[i].info->string);
		}
		free(table->ks2[i].info);
		free(table->ks2[i].key);
	}
	free(table->ks2);
	table->size2 = 0;
}