#pragma once

unsigned int hash2 (int key, int SIZE_OF_SPACE);
int find_key_ks2 (Table *table, char* key);
Table* Tfind_key_ks2 (Table* table, char* key);
KeySpace2* add_in_table_ks2 (Table* table, char* key2, char* str_info);
void show_data_in_ks2 (Table* table);
Info* find_release_item_ks2 (Table* table, char* key, int release);
void show_item_ks2 (Table* table, char* key, int release);
void delete_release_item_ks2 (Table* table, char* key, int release);
void delete_items_ks2 (Table* table, char* key);
void delete_ks2 (Table* table);
KeySpace2* find_release_struct_ks2 (Table* table, char* key, int release);