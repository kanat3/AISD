#pragma once

typedef struct Info {

	char* string;
	int busy;		    // признак занятости элемента
	int release;		// номер версии элемента				

} Info;

typedef struct KeySpace1 {
			
	unsigned int key;	// ключ элемента
	Info* info;			// указатель на информацию		

} KeySpace1;

typedef struct KeySpace2 {
		
	char* key;	 	 //ключ элемента			
	Info* info;		 //указатель на информацию	

} KeySpace2;

typedef struct Table {

	KeySpace1* ks1;			
	KeySpace2* ks2;					
	int size1; 	//размер области пространства ключей 
	int size2;				

} Table;

Table* create_table (void);

int get_release (void);

unsigned int hash2 (int i);

int find_key_ks1 (Table *table, unsigned int key);

int add_in_table_ks1 (Table* table, unsigned int key1, char* info);

char* get_key2 (void);

void get_key1 (unsigned int* key1);

char* get_str();

void show_data_in_ks1 (Table* table);

Info* find_release_item_ks1 (Table* table, unsigned int key, int release);

void show_item_ks1 (Table* table, unsigned int key, int release);

int delete_release_item_ks1 (Table* table, unsigned int key, int release);

int delete_items_ks1 (Table* table, unsigned int key);

void collect_deleted_items (Table* table);

void delete_ks1 (Table* table);

int add_in_table_ks2 (Table* table, char* key2, char* str_info);

int find_key_ks2 (Table *table, char* key);

void show_data_in_ks2 (Table* table);

Info* find_release_item_ks2 (Table* table, char* key, int release);

void show_item_ks2 (Table* table, char* key, int release);

void delete_release_item_ks2 (Table* table, char* key, int release);

void delete_items_ks2 (Table* table, char* key);

void delete_ks2 (Table* table);

int general_insert (Table* table, unsigned int key1, char* key2, int rel1);

void general_search (Table* table);

void general_release_delete (Table* table);

void general_show (Table* table);

void general_release_search (Table* table);

void general_exit (Table* table);

void general_key_delete (Table* table);

int _feof (void);