#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "table.h"

/******************************************************/
//Реализация функций просматриваемой таблицы
//Одинаковые ключи допустимы
/******************************************************/

int find_key_ks1 (Table *table, unsigned int key) { //вернет релиз послeднего совпадающего ключа, если он есть
	int k = 0;
	for (int i = 0; i < table->max_size; ++i) {
		if (table->ks1[i].key == key && table->ks1[i].info  && table->ks1[i].info->busy == 1) {
			k++;
		}
	}
	if (k == 0) {
		return -1;
	}
	return k;
}

Info* find_release_item_ks1 (Table* table, unsigned int key, int release) {
	for (int i = 0; i < table->max_size; i++) {
		if (table->ks1[i].key == key && (table->ks1[i].info) && (table->ks1[i].info->release == release)) {
			return table->ks1[i].info;
		}
	}
	return NULL;
}

KeySpace1* find_release_struct_ks1 (Table* table, unsigned int key, int release) {
	for (int i = 0; i < table->max_size; i++) {
		if (table->ks1[i].key == key && (table->ks1[i].info) && (table->ks1[i].info->release == release)) {
			return &table->ks1[i];
		}
	}
	return NULL;
}

KeySpace1* add_in_table_ks1 (Table* table, unsigned int key1, char* info) {
	if (table->size1 == table->max_size) {
		puts("\nTable 1 is full.\n");
		return NULL;
	}
	int i;
	i = table->size1;
	table->ks1[i].info = (Info*)calloc(1, sizeof(Info));
	if (table->ks1[i].info == NULL) {
		puts("\nMemory error!\n");
		return NULL;
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
	return &table->ks1[i];
}

void show_data_in_ks1 (Table* table) {
	int i = 0;
	while (i != table->max_size) {
		if (table->ks1[i].info != NULL && table->ks1[i].info->busy != 0) {
			//printf("\n ==%d== Key: %d | Information: %s | Release of info: %d\n", i, table->ks1[i].key, table->ks1[i].info->string, table->ks1[i].info->release);
			if (table->ks1[i].link && table->ks1[i].link->info && table->ks1[i].link->info->busy == 1) {
				printf("\n ==%d== Key: %d | Information: %s | Release of info: %d | Key in keyspace2: %s \n", i, table->ks1[i].key, table->ks1[i].info->string, table->ks1[i].info->release, table->ks1[i].link->key);
			} else {
				printf("\n ==%d== Key: %d | Information: %s | Release of info: %d\n", i, table->ks1[i].key, table->ks1[i].info->string, table->ks1[i].info->release);
			}
		}
		i++;
	}
}

int delete_release_item_ks1 (Table* table, unsigned int key, int _release) {
	Info* info = NULL;
	Info* info2 = NULL;
	int k;
	k = find_key_ks1(table, key);
	if (k < 0) { // >=0
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
			collect_deleted_items(table);
		}
	}
	return 1;
}

void collect_deleted_items (Table* table) {
	if (table->size1 == 0) {
		return;
	}
	if (table->size1 == 1 && table->ks1[0].info && table->ks1[0].info->busy == 0) { //случай с одним элементом
		free(table->ks1[0].info->string);
		free(table->ks1[0].info);
		table->ks1[0].link->link = NULL;
		table->ks1[0].info = NULL;
		table->size1--;
		return;
	}
	int i_busy, i;
	i = -1;
	i_busy = -2;
	//поиск удаленной позиции
	do {
		i++;
		i_busy++;
	} while ((i <= table->max_size) && (table->ks1[i].info && table->ks1[i].info->busy == 1));
	puts("what");
	if (i_busy > -2 && (i != table->size1 - 1)) {
		i_busy = i;
		i++;
		free(table->ks1[i_busy].info->string); //очищаем "удаленную" позицию
		free(table->ks1[i_busy].info);
		if (table->ks1[i_busy].link && table->ks1[i_busy].link->link) {
			table->ks1[i_busy].link->link = NULL;
		}
	} else if (i == table->size1 - 1) { //случай с "удаленной" последней позицией
		free(table->ks1[i].info->string);
		free(table->ks1[i].info);
		table->ks1[i].info = NULL;
		if (table->ks1[i_busy].link && table->ks1[i_busy].link->link) {
			table->ks1[i_busy].link->link = NULL;
		}
		table->size1--;
		return;
	} else {
		return;
	}
	puts("here");
	for (i_busy; i_busy < table->size1 - 1; i_busy++) {
		table->ks1[i_busy].info = (Info*)calloc(1, sizeof(Info));
		table->ks1[i_busy].key = table->ks1[i].key;
		table->ks1[i_busy].link = table->ks1[i].link;
		table->ks1[i_busy].info->busy = 1;
		table->ks1[i_busy].info->release = table->ks1[i].info->release;
		char* str_info;
		str_info = (char*)calloc(strlen(table->ks1[i].info->string)+1, sizeof(char));
		strcpy(str_info, table->ks1[i].info->string);
		table->ks1[i_busy].info->string = str_info;
		free(table->ks1[i].info->string);
		free(table->ks1[i].info);
		table->ks1[i].info = NULL;
		i++;
	}
	table->size1--;
}

void delete_ks1 (Table* table) {
	for (int i = 0; i < table->size1; i++) {
		if (table->ks1[i].info != NULL) {
			free(table->ks1[i].info->string);
			free(table->ks1[i].info);
		} else {
			free(table->ks1[i].info);
			table->ks1[i].info = NULL;
			table->size1--;
		}
	}
	free(table->ks1);
}