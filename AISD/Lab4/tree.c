#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "tree.h"

Node* create_node (int _key, Info* _info, Node* _parent) {

	Node* node = (Node*)calloc(1, sizeof(Node));

	if (node == NULL) {
		puts("\nMemory error\n");
		return NULL;
	}

	node->info = _info;
	node->key = _key;
	node->left = NULL;
	node->right = NULL;
	node->parent = _parent;

	return node;
}

Info* create_info (float _a, float _b, char* _string) {

	Info* info = (Info*)calloc(1, sizeof(Info));

	if (info == NULL) {
		puts("\nMemory error\n");
		return NULL;
	}

	info->a = _a;
	info->b = _b;
	info->string = _string;
	info->next = NULL;

	return info;
}

void insert (Node** _node, Info* _info, int _key) {

	Node* _parent = NULL;

	if ((*_node) == NULL) {
		(*_node) = create_node(_key, _info, _parent); //корень дерева
		return;
	}

	_parent = (*_node);

	if (_key < (*_node)->key) {

		if ((*_node)->left) {
			Node* a = (*_node)->left;
			insert(&a, _info, _key);
		} else if (!(*_node)->left) {
			(*_node)->left = create_node(_key, _info, _parent);
			_parent = (*_node)->left;
		}
	} else if (_key > (*_node)->key) {

		if ((*_node)->right) {
			Node* a = (*_node)->right;
			insert(&a, _info, _key);
		} else if (!(*_node)->right) {
			(*_node)->right = create_node(_key, _info, _parent);
			_parent = (*_node)->right;
		}
		
	} else if (_key = (*_node)->key) { //коллизия
		_info->next = (*_node)->info;
		(*_node)->info = _info;
	}
	return;
}

void show_tree (Node* _node, int n, int tree_height) {

  if (_node) {
  	if (_node->left) {
    	show_tree(_node->left, n + 2, tree_height + 1);
    }
    if (_node) {
	    Info* save =_node->info;
	    printf("%d: ", tree_height);
	    printf("%d ", _node->key);
	    while (_node->info->next != NULL) {
	    	printf("%d ", _node->key);
	    	_node->info = _node->info->next;
	    }
	    puts("\n");
	    _node->info = save;
	}
    if (_node->right) {
    	show_tree(_node->right, n + 2, tree_height + 1);
    }
  }
}

Node* search_by_key (Node** _node, int _key) {

	if ((*_node) == NULL) {
		return NULL;
	}
	if (_key < (*_node)->key) {
		if ((*_node)->left) {
			Node* left = (*_node)->left;
			search_by_key(&left, _key);
		}
	} else if (_key > (*_node)->key) {
		if ((*_node)->right) {
			Node* right = (*_node)->right;
			search_by_key(&right, _key);
		}
	} else if (_key == (*_node)->key) {
		return (*_node);
	}
}

void show_info (Info* f, int _key) {

	Info* save = f;

	if (f) {
		puts("\nYour info by key:\n");
		printf("%d, %.4f, %.4f, %s\n", _key, f->a, f->b, f->string);
		while (f->next != NULL) {
			f = f->next;
			printf("%d, %.4f, %.4f, %s\n", _key, f->a, f->b, f->string);
		}
	}

	f = save; //тоже ок
}

int height (Node* _node) {

	if (!_node) {
		return 0;
	} else {
		int left_height = height(_node->left);
		int right_height = height(_node->right);
		return left_height > right_height ? left_height + 1 : right_height + 1;
	}
}

Node* min (Node* _node) {
	
	if (_node == NULL) {
		return NULL;
	} 
	while (_node->left) {
		_node = _node->left;
	}
	return _node;
}

Node* max (Node* _node) {

    while (_node->right) {
        _node = _node->right;
    }
    return _node;
}

Node* next (Node* _node) {

	if (_node->right != NULL) {
		return min(_node->right);
	}
	Node* _node2 = _node->parent;
	while ((_node2 != NULL) && (_node->key == _node2->right->key)) {
		_node = _node2;
		_node2 = _node2->parent;
	}
	return _node2;
}


void free_info (Node** _node) {

 	if ((*_node)->info->next == NULL) {
		free((*_node)->info);
		(*_node)->info = NULL;
	} else {
		while ((*_node)->info->next != NULL) {
			Info* to_delete = (*_node)->info->next;
			free((*_node)->info);
			(*_node)->info = NULL;
			(*_node)->info = to_delete;
		}
		free((*_node)->info);
		(*_node)->info = NULL;
	}
	return;
}

void delete_node(Node** _node, int _key) {
	if ((*_node) == NULL) {
		return;
	}
	if (_key < (*_node)->key) {
		delete_node(&(*_node)->left, _key);
	} else if (_key > (*_node)->key) {
		delete_node(&(*_node)->right, _key);
	} else {
		if ((*_node)->left == NULL) {
			if ((*_node)->right) { 
				if ((*_node) == (*_node)->parent->left) {
					(*_node)->parent->left = (*_node)->right;
				} else {
					(*_node)->parent->right = (*_node)->right;
				}
			}
			free_info(_node);
			free((*_node));
			(*_node) = NULL;
			return;

		} else if ((*_node)->right == NULL) {
			if ((*_node)->left) { 
				if ((*_node) == (*_node)->parent->left) {
					(*_node)->parent->left = (*_node)->left;
				} else {
					(*_node)->parent->right = (*_node)->left;
				}
			}
			free_info(_node);
			free((*_node));
			(*_node) = NULL;
			return;
		} else {
			// node with two children
			Node* temp = min((*_node)->right);
			Node* left_child = (*_node)->left;
			Node* right_child = (*_node)->right;
			left_child->parent = temp;
			if (right_child != temp) {
				right_child->parent = temp;
			} else {
				if (temp->right) {
					right_child = temp->right;
				} else {
					right_child = NULL;
				}
			}
			temp->parent->left = NULL;
	        temp->parent = NULL;
			free_info(_node);
			free((*_node));
			(*_node) = temp;
			(*_node)->left = left_child;
			(*_node)->right= right_child;
			return;
		}
	}
}

void delete_tree (Node** _node) {

	if ((*_node)) {
		if ((*_node)->left) {
	    	delete_tree(&(*_node)->left);
	    }
	    if ((*_node)->right) {
	    	delete_tree(&(*_node)->right);
	    }
	    if ((*_node)) {
			free_info(_node);
			free((*_node));
			(*_node) = NULL;
	    }
	}
}

void find_diff (Node* _node, int _key, int* min_diff, int* min_diff_key) {

    if (_node == NULL)
        return;
  
    if (_node->key != _key) {
	    if (*min_diff > abs(_node->key - _key))
	    {
	        *min_diff = abs(_node->key - _key);
	        *min_diff_key = _node->key;
	    }
	}

    if (_key < _node->key) {
        find_diff(_node->left, _key, min_diff, min_diff_key);
    }
    else {
        find_diff(_node->right, _key, min_diff, min_diff_key);
    }
}

int find_mismatched_key (Node *root, int _key) {

    int min_diff = 255;
    int min_diff_key = -1;
    find_diff(root, _key, &min_diff, &min_diff_key);
  
    return min_diff_key;
}
