#pragma once

int find_key_ks1 (Table *table, unsigned int key);
Info* find_release_item_ks1 (Table* table, unsigned int key, int release);
KeySpace1* add_in_table_ks1 (Table* table, unsigned int key1, char* info);
void show_data_in_ks1 (Table* table);
int delete_release_item_ks1 (Table* table, unsigned int key, int _release);
int delete_items_ks1 (Table* table, unsigned int key);
void collect_deleted_items (Table* table);
void delete_ks1 (Table* table);
KeySpace1* find_release_struct_ks1 (Table* table, unsigned int key, int release);