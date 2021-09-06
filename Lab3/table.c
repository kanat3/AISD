#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "table.h"
#include "ks1.h"
#include "ks2.h"

#define FILE_NAME "test.bin"

/********************************************************/
//Объединение двух таблиц
/********************************************************/

Table* create_table (int size) {

	Table* table;

	table = (Table*)calloc(1, sizeof(Table));

	if (table == NULL) {
		puts("\nMemory error!\n");
		return NULL;
	}

	table->ks1 = (KeySpace1*)calloc(size, sizeof(KeySpace1));
	if (table->ks1 == NULL) {
		puts("\nMemory error!\n");
		return NULL;
	}

	table->ks2 = (KeySpace2*)calloc(size, sizeof(KeySpace2));
	if (table->ks2 == NULL) {
		puts("\nMemory error!\n");
		return NULL;
	}

	table->size1 = 0;
	table->size2 = 0;
	table->max_size = size;

	for (int i = 0; i < size; i++) {
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

char* get_str () {
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
	puts("\nEnter integer: \n");
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

int general_insert (Table* table) {
	if (table->size1 == table->max_size || table->size2 == table->max_size) {
		puts("One of the spaces if full");
		return -2;
	}
	//start
	unsigned int key1;
	int rel;
	KeySpace1* ks;
	char* key2;
	char* info1;
	char* info2;
	get_key1(&key1);
	key2 = get_key2();
	rel = find_key_ks1(table, key1);
	int i = 1;
	while (i <= rel) {
		ks = find_release_struct_ks1(table, key1, i);
		if (ks->link) {
			if (ks && strcmp(ks->link->key, key2) == 0) {
				puts("\nComposite key in table. Can't add\n");
				free(key2);
				return -6;
			} else {
				i++;
			}
		} else {
			i = rel + 1;
		}
	}
	puts("\nEnter information:\n");
	info1 = get_str();
	if (!info1) {
		free(info1);
		puts("\nTry again\n");
	}
	info2 = (char*)calloc(strlen(info1)+1, sizeof(char));
	strcpy(info2, info1);
	//add ks1
	KeySpace1* ks1;
	KeySpace2* ks2;
	ks1 = add_in_table_ks1(table, key1, info1);
	if (!ks1) { //вставка не удалась
		free(key2);
		free(info1);
		return -5;
	}
	ks2 = add_in_table_ks2(table, key2, info2);
	if (!ks2) { //вставка не удалась
		free(key2);
		free(info2);
		return -5;
	}
	ks1->link = ks2;
	ks2->link = ks1;
	return 1;
}

void general_search (Table* table) {
	puts("\n1. Find key in ks1\n");
	puts("2. Find key in ks2\n");
	puts("3. Find composite key (key1, key2)\n");
	int k, choice;
	do {
		k = scanf("%d", &choice);
		if (!_feof()) {
			return;
		}
		getchar();
		if (k <= 0) {
			puts("\nYou're wrong. Try again!\n");
			while (getchar() != '\n') {}
		}
		if ((choice < 1) || (choice > 3)) {
			k = -1;
			puts("\nYou're wrong. Try again!\n");
		}
	} while (k < 0);
	if (choice == 1) {
		unsigned int key1;
		get_key1(&key1);
		if (key1 == -5) {
			return;
		}
		if (find_key_ks1(table, key1) >= 0) {
			puts("\nKey 1 in table\n");
		} else {
			puts("\nThere no key 1\n");
		}
	} else if (choice == 2) {
		char* key2;
		key2 = get_key2();
		if (!key2) {
			free(key2);
			return;
		}
		if (find_key_ks2(table, key2) > 0) {
			puts("\nKey 2 in table\n");
		} else {
			puts("\nThere no key 2\n");
		}
		/*******************/
		//search table
		//Tfind_key_ks2(table, key2);
		/******************/
		free(key2);
	} else if (choice == 3) {
		unsigned int key1;
		int rel;
		char* key2;
		KeySpace1* ks;
		get_key1(&key1);
		if (key1 == -5) {
			return;
		}
		key2 = get_key2();
		if (!key2) {
			free(key2);
			return;
		}
		rel = find_key_ks1(table, key1);
		int i = 1;
		while (i <= rel) {
			ks = find_release_struct_ks1(table, key1, i);
			if (ks && strcmp(ks->link->key, key2) == 0) {
				puts("\nComposite key in table\n");
				free(key2);
				return;
			} else {
				i++;
			}
		}
		puts("\nThere no composite key\n");
		free(key2);
	}
}

void general_show (Table* table) {

	puts("\n===========================================Keyspace 1====================================================\n");
	show_data_in_ks1(table);
	puts("\n===========================================Keyspace 2====================================================\n");
	show_data_in_ks2(table);

}

void general_release_search (Table* table) {
	puts("\n1. Find key in ks1\n");
	puts("2. Find key in ks2\n");
	int k, choice;
	do {
		k = scanf("%d", &choice);
		if (!_feof()) {
			return;
		}
		getchar();
		if (k <= 0) {
			puts("\nYou're wrong. Try again!\n");
			while (getchar() != '\n') {}
		}
		if ((choice < 1) || (choice > 2)) {
			k = -1;
			puts("\nYou're wrong. Try again!\n");
		}
	} while (k < 0);
	if (choice == 1) {
		unsigned int key1;
		get_key1(&key1);
		if (key1 == -5) {
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
	} else if (choice == 2) {
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
		Info* info2;
		info2 = find_release_item_ks2(table, key2, release);
		if (info2 != NULL) {
			printf("\nKey: %s | Information: %s | Release of info: %d\n", key2, info2->string, info2->release);
		} else {
			puts("\nCan't find\n");
		}
		free(key2);
	}
}

void general_key_delete (Table* table) {
	puts("\n1. Delete key in ks1\n");
	puts("2. Delete key in ks2\n");
	puts("3. Delete composite key (key1, key2)\n");
	int k, choice;
	do {
		k = scanf("%d", &choice);
		if (!_feof()) {
			return;
		}
		getchar();
		if (k <= 0) {
			puts("\nYou're wrong. Try again!\n");
			while (getchar() != '\n') {}
		}
		if ((choice < 1) || (choice > 3)) {
			k = -1;
			puts("\nYou're wrong. Try again!\n");
		}
	} while (k < 0);
	if (choice == 1) {
		unsigned int key1;
		get_key1(&key1);
		if (key1 == -5) {
			return;
		}
		delete_items_ks1(table, key1); //ссылка на пространство меняется в функции
	} else if (choice == 2) {
		char* key2;
		key2 = get_key2();
		if (!key2) {
			free(key2);
			return;
		}
		delete_items_ks2(table, key2); //ссылка на пространство меняется в функции
		free(key2);
	} else if (choice == 3) {
		unsigned int key1;
		int rel, rel2;
		char* key2;
		KeySpace1* ks;
		get_key1(&key1);
		if (key1 == -5) {
			return;
		}
		key2 = get_key2();
		if (!key2) {
			free(key2);
			return;
		}
		rel = find_key_ks1(table, key1);
		int i = 1;
		while (i <= rel) {
			ks = find_release_struct_ks1(table, key1, i);
			if (ks && strcmp(ks->link->key, key2) == 0) {
				rel2 = ks->link->info->release;
				delete_release_item_ks1(table, key1, i);
				collect_deleted_items(table);
				delete_release_item_ks2(table, key2, rel2);
				free(key2);
				return;
			} else {
				i++;
			}
		}
		puts("\nThere no composite key\n");
		free(key2);
	}
}

void general_release_delete (Table* table) {
	puts("\n1. Delete key in ks1\n");
	puts("2. Delete key in ks2\n");
	int k, choice;
	do {
		k = scanf("%d", &choice);
		if (!_feof()) {
			return;
		}
		getchar();
		if (k <= 0) {
			puts("\nYou're wrong. Try again!\n");
			while (getchar() != '\n') {}
		}
		if ((choice < 1) || (choice > 2)) {
			k = -1;
			puts("\nYou're wrong. Try again!\n");
		}
	} while (k < 0);
	if (choice == 1) {
		unsigned int key1;
		get_key1(&key1);
		if (key1 == -5) {
			return;
		}
		int release;
		release = get_release();
		if (release == -5) {
			return;
		}
		if (delete_release_item_ks1(table, key1, release) > 0) {
			puts("\nInformation ready for deletion (busy = 0)\n");
			collect_deleted_items(table);
		}
	} else if (choice == 2) {
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
		delete_release_item_ks2(table, key2, release);
		free(key2);
	}
}

void general_exit (Table* table) {
	delete_ks1(table);
	delete_ks2(table);
	free(table);
}

void general_file (Table* table) {
	puts("\nEnter file name to read:");
	char* file_name = get_key2();
	if (!file_name) {
		free(file_name);
		return;
	}
	Tread(table, file_name);
}

void Twrite (Table* table, char* file_name) {
		int str_len, key_len;
		FILE* file = fopen(file_name, "wb+");
		if (!file) {
			free(file_name);
			return;
		}
		fwrite(&table->max_size, sizeof(int), 1, file);
		fwrite(&table->size1, sizeof(int), 1, file);
		fwrite(&table->size2, sizeof(int), 1, file);
		for (int i = 0; i < table->size1; i++) {
			fwrite(&table->ks1[i].key, sizeof(int), 1, file);
			str_len = strlen(table->ks1[i].info->string);
			char* save_str1 = (char*)calloc(str_len + 1, sizeof(char));
			strcpy(save_str1, table->ks1[i].info->string);
			fwrite(&str_len, sizeof(int), 1, file);
			fwrite(save_str1, sizeof(char), str_len, file);
			if (table->ks1[i].link && table->ks1[i].link->info->busy == 1) {
				fwrite(&table->ks1[i].link->info->release, sizeof(int), 1, file);
				key_len = strlen(table->ks1[i].link->key);
				char* save_str2 = (char*)calloc(key_len + 1, sizeof(char));
				strcpy(save_str2, table->ks1[i].link->key);
				fwrite(&key_len, sizeof(int), 1, file);
				fwrite(save_str2, sizeof(char), key_len, file);
				free(save_str2);
			} else {
				int no_link = 0;
				fwrite(&no_link, sizeof(int), 1, file);
			}
			free(save_str1);
		}
		for (int i = 0; i < table->max_size; i++) {
			if (table->ks2[i].info && table->ks2[i].info->busy == 1) {
				fwrite(&i, sizeof(int), 1, file);
				key_len = strlen(table->ks2[i].key);
				char* save_str3 = (char*)calloc(key_len + 1, sizeof(char));
				fwrite(&key_len, sizeof(int), 1, file);
				strcpy(save_str3, table->ks2[i].key);
				fwrite(save_str3, sizeof(char), key_len, file);
				free(save_str3);
				str_len = strlen(table->ks2[i].info->string);
				char* save_str4 = (char*)calloc(str_len + 1, sizeof(char));
				strcpy(save_str4, table->ks2[i].info->string);
				fwrite(&str_len, sizeof(int), 1, file);
				fwrite(save_str4, sizeof(char), str_len, file);
				free(save_str4);
				fwrite(&table->ks2[i].info->release, sizeof(int), 1, file);
			}
		}
		fclose(file);
}

Table* Tread (Table* table, char* file_name) {
		Table* read_table;
		FILE* file = fopen("test.bin", "rb");
		if (!file) {
			free(file_name);
			return NULL;
		}
		int m_size, size1, size2;
		fread(&m_size, sizeof(int), 1, file);
		fread(&size1, sizeof(int), 1, file);
		fread(&size2, sizeof(int), 1, file);
		read_table = create_table(m_size);
		read_table->size1 = 0;
		read_table->size2 = size2;

		int rel2, str_len, is_link_release, index, key1;
		for (int i = 0; i < size1; i++) {
			fread(&key1, sizeof(int), 1, file);
			fread(&str_len, sizeof(int), 1, file);
			char* str_info = (char*)calloc(str_len + 1, sizeof(char));
			fread(str_info, sizeof(char), str_len, file);
			fread(&is_link_release, sizeof(int), 1, file);
			add_in_table_ks1(read_table, key1, str_info);
			if (is_link_release > 0) {
				fread(&str_len, sizeof(int), 1, file);
				char* link_key = (char*)calloc(str_len + 1, sizeof(char));
				fread(link_key, sizeof(char), str_len, file);
				read_table->ks1[i].link = (KeySpace2*)calloc(1, sizeof(KeySpace2));
				read_table->ks1[i].link->info = (Info*)calloc(1, sizeof(Info));
				read_table->ks1[i].link->key = (char*)calloc(str_len + 1, sizeof(char)); ///тут все есть ок
				strcpy(read_table->ks1[i].link->key, link_key);
				read_table->ks1[i].link->info->release = is_link_release;
				read_table->ks1[i].link->info->busy = 1;
				free(link_key);
			} else {
				read_table->ks1[i].link = NULL;
			}
		}
		for (int i = 0; i < size2; i++) {
			fread(&index, sizeof(int), 1, file);
			fread(&str_len, sizeof(int), 1, file);
			char* key2 = (char*)calloc(str_len + 1, sizeof(char));
			fread(key2, sizeof(char), str_len, file);
			fread(&str_len, sizeof(int), 1, file);
			char* str = (char*)calloc(str_len + 1, sizeof(char));
			fread(str, sizeof(char), str_len, file);
			read_table->ks2[index].key = key2;
			read_table->ks2[index].info = (Info*)calloc(1, sizeof(Info));
			read_table->ks2[index].info->string = str;
			read_table->ks2[index].info->busy = 1;
			fread(&read_table->ks2[index].info->release, sizeof(int), 1, file);
		}
		fclose(file);
		//установка связей
		KeySpace2* ks;
		for (int i = 0; i < size1; i++) {
			if (read_table->ks1[i].link && read_table->ks1[i].link->info->busy == 1) {
				ks = find_release_struct_ks2(read_table, read_table->ks1[i].link->key, read_table->ks1[i].link->info->release);
				ks->link = &read_table->ks1[i];
				free(read_table->ks1[i].link->key);
				free(read_table->ks1[i].link->info);
				free(read_table->ks1[i].link);
				read_table->ks1[i].link = ks;
			}
		}
		return read_table;
}