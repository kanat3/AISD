#pragma once

typedef struct List {
	char priority;
	int id;
	int ta;
	int ts;
	struct List* next;
	struct List* prev;
} List;