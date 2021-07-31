#pragma once

typedef struct Node {

	int key;
	struct Info* info;
	struct Node* left;
	struct Node* right;
	struct Node* parent;

} Node;

typedef struct Info {

	float a;
	float b;
	char* string;
	struct Info* next;

} Info;

Node* create_node (int _key, Info* _info, Node* _parent);
Info* create_info (float _a, float _b, char* _string);
void insert (Node** _node, Info* _info, int _key);
void show_tree (Node* _node, int n, int tree_height);
Node* search_by_key (Node** _node, int _key);
void show_info (Info* f, int _key);
int height (Node* _node);
Node* min (Node* _node);
Node* max (Node* _node);
void delete_node (Node** _node, int _key);
void free_info (Node** _node);
void delete_tree(Node** _node);
int find_mismatched_key (Node *root, int _key);
void find_diff (Node* _node, int _key, int* min_diff, int* min_diff_key);