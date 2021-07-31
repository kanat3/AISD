#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "graph.h"
#include "dialog.h"

int _feof (void) {
	if (feof(stdin)) {
		puts("\nEnd\n");
		return 0;
	}
	return -5;
}

char* get_str (void) {
	char buf[81] = {0};
	char *res = NULL;
	char *check = NULL;
	int len = 0;
	int n = 0;

	do {
		n = scanf("%80[^\n]", buf);
		if (n < 0) {
			if (!res) {
				return NULL;
			}
		} else if (n > 0) {
			int chunk_len = strlen(buf);
		    int str_len = len + chunk_len;
		    check = res;
		    res = realloc(res, str_len + 1);
		    if (!res) {
		    	free(check);
		    	return NULL;
		    }
		    memcpy(res + len, buf, chunk_len);
		    len = str_len;
		} else {
			scanf("%*c");
		}
	} while (n > 0);

	if (len > 0) {
		res[len] = '\0';
	} else {
		res = calloc(1, sizeof(char));
	}
	return res;
}

char* fget_str (FILE* fd) {
    char buf[81] = {0};
    char* res = NULL;
    char* check = NULL;
    int len = 0;
    int n = 0;

    do {
        n = fscanf(fd, "%80[^\n]", buf);
        if (n < 0) {
            if (!res) {
                    return NULL;
            }
        } else if (n > 0) {
            int chunk_len = strlen(buf);
            int str_len = len + chunk_len;
            check = res;
            res = (char*)realloc(res, sizeof(char)*(str_len + 1));
            if (!res) {
            	free(check);
            	return NULL;
            }
            memcpy(res + len, buf, chunk_len);
            len = str_len;
        } else {
            fscanf(fd, "%*c");
        }

    } while (n > 0);

    if (len > 0) {
        res[len] = '\0';
    } else {
       	res = (char *)calloc(1, sizeof(char));
    }

    return res;
}

void get_int (unsigned int* key1) {
	int k;
	do {
		k = scanf("%u", key1);
		if (_feof() == 0) {
			exit(-1);
		}
		if (k <= 0) {
			puts("\nError. try again\n");
			int c;
			while ((c = getchar()) != '\n' && c != EOF) {}			
		}
	} while (k <= 0);
	while (getchar() != '\n') {}
}

void DFS_SHELL (Vertex** list) {
	int result = 0;
	Vertex* ptr;
	Vertex* first_dfs;
	Vertex* to_find;
	int x1, y1, x2, y2;
	printf("\nEnter first vertex for dfs:");
	printf("\nx:");
	get_int(&x1);
	printf("y:");
	get_int(&y1);
	first_dfs = is_vertex(list, x1, y1);
	if (!first_dfs) {
		puts("\nCan't find ur vertex");
		return;
	}
	printf("\nEnter vertex to find:");
	printf("\nx:");
	get_int(&x2);
	printf("y:");
	get_int(&y2);
	to_find = is_vertex(list, x2, y2);
	if (!to_find) {
		puts("\nCan't find ur second vertex");
		return;
	}
	/**************************/
	DFS_vert_search(first_dfs, to_find, &result);
	if (result == 0) {
		puts("\n> The route isn't exist <");
	} else {
		puts("\n> The route exists <");
	}
}

void DFS (Vertex** list) { //обход с начала хеш таблицы
	Vertex* ptr;
	for (int i = 0; i < HASH_TABLE_SIZE; i++) {
		if (list[i]) {
			printf("for all =>> %d %d ==color %d==\n", list[i]->coords.x, list[i]->coords.y, list[i]->for_dfs.color);
			if (list[i]->for_dfs.color == 0) {
				DFS_one_vert(list[i]);
			}
			ptr = list[i]->next;
			if (ptr) {
				printf("for all =>> %d %d ==color %d==\n", ptr->coords.x, ptr->coords.y, ptr->for_dfs.color);
				if (ptr->for_dfs.color == 0) {
					DFS_one_vert(ptr);
				}
				ptr = ptr->next;		
			}
		}
	}
}

void Dijkstra_SHELL (Vertex** Graph) {
	int count;
	int i = 0;
	int x, y;
	init_Dijkstra(Graph, &count);
	puts("\nEnter vertex for dijkstra:");
	puts("x:");
	get_int(&x);
	puts("y:");
	get_int(&y);
	puts("\n");
	Vertex* dj = is_vertex(Graph, x, y);
	if (!dj) {
		puts("\nCan't find ur vertex");
		return;
	}
	Dijkstra(dj, Graph, &i);
	show_Dgraph(Graph);
}

Vertex* create_vertex (void) {
	Vertex* vertex;
	vertex = (Vertex*)calloc(1, sizeof(Vertex));
	printf("\nVertex name: ");
	if (!(vertex->name = get_str())) {
		free(vertex);
		return NULL;
	}	
	printf("Coordinate x: ");
	get_int(&vertex->coords.x);
	if (!vertex->coords.x) {
		free(vertex->name);
		free(vertex);
		return NULL;
	}

	printf("Coordinate y: ");
	get_int(&vertex->coords.y);
	if (!vertex->coords.y) {
		free(vertex->name);
		free(vertex);
		return NULL;
	}
	return vertex;
}
