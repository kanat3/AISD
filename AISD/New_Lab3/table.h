#pragma once

typedef struct Info {

	char* string;
	int busy;		    // признак занятости элемента
	int release;		// номер версии элемента

} Info;

typedef struct KeySpace1 {

	unsigned int key;	// ключ элемента
	struct KeySpace2* link;		//ссылка на элемент 2-ого пространства
	Info* info;			// указатель на информацию

} KeySpace1;

typedef struct KeySpace2 {

	char* key;	 	 //ключ элемента
	struct KeySpace1* link;		//ссылка на элемент 1-ого пространства
	Info* info;		 //указатель на информацию

} KeySpace2;

typedef struct Table {

	KeySpace1* ks1;
	KeySpace2* ks2;
	int size1; 	//размер области пространства ключей
	int size2;
	int max_size; //максимальный размер таблиц

} Table;

Table* create_table (int size);
int get_release (void);
unsigned int hash2 (int key, int SIZE_OF_SPACE);
int find_key_ks1 (Table *table, unsigned int key);
KeySpace1* add_in_table_ks1 (Table* table, unsigned int key1, char* info);
char* get_key2 (void);
void get_key1 (unsigned int* key1);
char* get_str();
void show_data_in_ks1 (Table* table);
Info* find_release_item_ks1 (Table* table, unsigned int key, int release);
int delete_release_item_ks1 (Table* table, unsigned int key, int release);
int delete_items_ks1 (Table* table, unsigned int key);
void collect_deleted_items (Table* table);
void delete_ks1 (Table* table);
KeySpace2* add_in_table_ks2 (Table* table, char* key2, char* str_info);
int find_key_ks2 (Table *table, char* key);
Table* Tfind_key_ks2 (Table* table, char* key);
void show_data_in_ks2 (Table* table);
Info* find_release_item_ks2 (Table* table, char* key, int release);
void show_item_ks2 (Table* table, char* key, int release);
void delete_release_item_ks2 (Table* table, char* key, int release);
void delete_items_ks2 (Table* table, char* key);
void delete_ks2 (Table* table);
int general_insert (Table* table);
void general_search (Table* table);
void general_release_delete (Table* table);
void general_show (Table* table);
void general_release_search (Table* table);
void general_exit (Table* table);
void general_key_delete (Table* table);
int _feof (void);
void general_file (Table* table);
void Twrite (Table* table, char* file_name);
Table* Tread (Table* table, char* file_name);