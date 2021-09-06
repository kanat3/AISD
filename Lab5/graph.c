#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "dialog.h"
#include "graph.h"

#define LIMIT 50

int get_hash (int x, int y) { 
	return (x + y) % HASH_TABLE_SIZE;
}

int add_vertex (Vertex** list, Vertex* vertex) {
	if (is_vertex(list, vertex->coords.x, vertex->coords.y) != NULL) {
		free(vertex->name);
		free(vertex);
		return 2;
	}
	int hash = get_hash(vertex->coords.x, vertex->coords.y);
	Vertex* place = list[hash];
	if (!list[hash]) {
		list[hash] = (Vertex*)calloc(1, sizeof(Vertex));
		if (!list[hash]) {
			free(vertex->name);
			free(vertex);
			return 1;
		}
		*list[hash] = *vertex;
	} else {
		while (place->next) place = place->next;
		place->next = (Vertex*)calloc(1, sizeof(Vertex));
		if (!place->next) {
			free(vertex->name);
			free(vertex);
			return 1;
		}
		place = place->next;
		*place = *vertex;
	}
	
	free(vertex);
	return 0;
}

int add_edge (Vertex** list, int x1, int x2, int y1, int y2, int weight) {
	Vertex_list* ptr;
	Vertex_list* search;
	Vertex* vertex_1;
	Vertex* vertex_2;
	vertex_1 = is_vertex(list, x1, y1);
	vertex_2 = is_vertex(list, x2, y2);
	//проверка
	if (!vertex_1 || !vertex_2) {
		return 2;
	}
	//закинуть потом в мейн
	search = vertex_1->vert_list;
	if (search) {
		while (search && (search->vertex->coords.x != x2 && search->vertex->coords.y != y2)) {
			search = search->next;
		}
	}
	if (search != NULL) {
		return 3;
	}
	Vertex_list* a_vert_1 = (Vertex_list*)calloc(1, sizeof(Vertex_list)); 
	//добавляем связь только к первой вершине, тк орграф
	if (!a_vert_1) {
		free(a_vert_1);
		return 1;
	}
	a_vert_1->vertex = vertex_2;
	a_vert_1->weight = weight;
	if (!vertex_1->vert_list) {
		vertex_1->vert_list = a_vert_1;
	} else {
		ptr = vertex_1->vert_list;
		while (ptr->next) ptr = ptr->next;
		ptr->next = a_vert_1;
		a_vert_1->next = NULL;
	}

	return 0;
}

void delete_vertex_in_adj_list (Vertex* list, int x, int y) {
	Vertex* start_vertex = list;
	Vertex_list* ptr = NULL;
	Vertex_list* save = NULL;
	Vertex_list* prev = NULL;
	int check;
	while (start_vertex) {
		check = 0;
		save = start_vertex->vert_list;
		ptr = start_vertex->vert_list;
		if (ptr != NULL) {
			while (ptr && (ptr->vertex->coords.x != x && ptr->vertex->coords.y != y)) {
				prev = ptr;
				ptr = ptr->next;
			}
			if (ptr == save) {
				save = save->next;
				free(ptr);
				check = 1;
			} else if (ptr && (check != 1)) {
				prev->next = ptr->next;
				free(ptr);
			}
		}
		start_vertex->vert_list = save;
		start_vertex = start_vertex->next;
	}
}

int delete_vetrex (Vertex** list, int x, int y) {
	for (int i = 0; i < HASH_TABLE_SIZE; i++) {
		delete_vertex_in_adj_list(list[i], x, y);
	}
	int hash = get_hash(x, y);
	Vertex* vertex = list[hash];
	Vertex* prev_ver = NULL;
	Vertex_list* vert_list;
	Vertex_list* temp_adj;

	if (!vertex) {
		return 1;
	}
	while (vertex->coords.x != x && vertex->coords.y != y) {
		if (!vertex->next) {
			return 1;
		}
		prev_ver = vertex;
		vertex = vertex->next;
	}

	vert_list = vertex->vert_list;
	while (vert_list) {
		temp_adj = vert_list->next;
		delete_edge(list, x, vert_list->vertex->coords.x, y, vert_list->vertex->coords.y);
		vert_list = temp_adj;
	}

	if (!prev_ver) {
		list[hash] = vertex->next;
	} else {
		prev_ver->next = vertex->next;
	}
	
	free(vertex->name);
	free(vertex);

	return 0;
}

int delete_edge (Vertex** list, int x1, int x2, int y1, int y2) {
	Vertex* vertex_1;
	Vertex* vertex_2;
	Vertex_list* vert_list;
	Vertex_list* prev_vert_list;
	vert_list = prev_vert_list = NULL;
	int check_del = 0;

	vertex_1 = is_vertex(list, x1, y1);
	vertex_2 = is_vertex(list, x2, y2);
	if (!vertex_1 || !vertex_2) {
		return 1;
	}
	vert_list = vertex_1->vert_list;
	while (vert_list) {
		if (vert_list->vertex == vertex_2) {
			if (!prev_vert_list) {
				vertex_1->vert_list = vert_list->next;
			} else {
				prev_vert_list->next = vert_list->next;
			}
			free(vert_list);
			check_del = 1;
			break;
		}
		prev_vert_list = vert_list;
		vert_list = vert_list->next;
	}

	if (!check_del) {
		return 2;
	}
	prev_vert_list = NULL;
	vert_list = vertex_2->vert_list;
	while (vert_list) {
		if (vert_list->vertex == vertex_1) {
			if (!prev_vert_list) {
				vertex_2->vert_list = vert_list->next;
			} else {
				prev_vert_list->next = vert_list->next;
			}
			free(vert_list);
			break;
		}
		prev_vert_list = vert_list;
		vert_list = vert_list->next;
	}

	return 0;
}

Vertex* is_vertex (Vertex**  list, int x, int y) {
	int hash = get_hash(x, y);
	Vertex* vertex = list[hash];
	if (!vertex) {
		return NULL;
	}
	while (vertex->coords.x != x || vertex->coords.y != y) {
		if (!vertex->next) {
			return NULL;
		}
		vertex = vertex->next;
	}
	return vertex;
}

void show_graph (Vertex** list) {
	Vertex* ptr;
	Vertex_list* vert_list;
	puts("\n>>>>		Graph    	<<<<\n");
	for (int i = 0; i < HASH_TABLE_SIZE; i++) {
		printf("==%d==  ", i);
		ptr = list[i];
		while (ptr) {
			printf(" (%d; %d)", ptr->coords.x, ptr->coords.y);
			vert_list = ptr->vert_list;
			printf(" {");
			while (vert_list) {
				printf("(%d; %d)", vert_list->vertex->coords.x, vert_list->vertex->coords.y);
				vert_list = vert_list->next;
			}
			printf("}");
			ptr = ptr->next;
		}
		printf("\n");
	}
}

void show_Dgraph (Vertex** list) {
	Vertex* ptr;
	Vertex_list* vert_list;
	puts("\n\n>>>>		Graph    	<<<<\n");
	for (int i = 0; i < HASH_TABLE_SIZE; i++) {
		printf("==%d==  ", i);
		ptr = list[i];
		while (ptr) {
			printf(" (%d; %d ==%d==)", ptr->coords.x, ptr->coords.y, ptr->min_way);
			vert_list = ptr->vert_list;
			printf(" {");
			while (vert_list) {
				printf("(%d; %d)", vert_list->vertex->coords.x, vert_list->vertex->coords.y);
				vert_list = vert_list->next;
			}
			printf("}");
			ptr = ptr->next;
		}
		printf("\n");
	}
}

int write_file (Vertex** list, FILE *fd) {
	Vertex* vertex;
	Vertex_list* vert;
	char *FLAG = "#ADJ_LIST";
	for (int i = 0; i < HASH_TABLE_SIZE; i++) {
		vertex = list[i];
		while (vertex) {
			fprintf(fd, "%s\n", vertex->name);
			fprintf(fd, "%d %d\n", vertex->coords.x, vertex->coords.y);
			vertex = vertex->next;
		}
	}
	fprintf(fd, "%s\n", FLAG);
	for (int i = 0; i < HASH_TABLE_SIZE; i++) {
		vertex = list[i];
		while (vertex) {
			vert = vertex->vert_list;
			if (vert) {
				fprintf(fd, "%d %d", vertex->coords.x, vertex->coords.y);
				while (vert) {
					fprintf(fd, "| %d %d %d", vert->weight, vert->vertex->coords.x, vert->vertex->coords.y);
					vert = vert->next;
				}
				fprintf(fd, "%c", '\n');
			}
			vertex = vertex->next;
		}
	}
	return 0;
}

int read_file (Vertex** list, FILE* fd, int* VERTEX_COUNT) {
	char *str, check;
	char *FLAG = "#ADJ_LIST";
	Vertex* vertex;
	int x_vec, y_vec, weight, x, y;
	str = fget_str(fd);
	if (!str) {
		return -1;
	}
	while (strncmp(str, FLAG, 9)) {
		(*VERTEX_COUNT)++;
		vertex = (Vertex*)calloc(1, sizeof(Vertex));
		vertex->name = str;
		fscanf(fd, "%d", &vertex->coords.x);
    	fscanf(fd, "%d", &vertex->coords.y);
    	add_vertex(list, vertex);
    	str = fget_str(fd);
    	free(str);
		str = fget_str(fd);
		if (!str) {
			return 1;
		}
	}
	free(str);

	while (!feof(fd)) {
		fscanf(fd, "%d", &x_vec);
    	fscanf(fd, "%d", &y_vec);
    	fscanf(fd, "%c", &check);
    	while (check == '|') {
    		fscanf(fd, "%d", &weight);
    		fscanf(fd, "%d", &x);
    		fscanf(fd, "%d", &y);
    		add_edge(list, x_vec, x, y_vec, y, weight); 
    		fscanf(fd, "%c", &check);
    	}
	}
	return 1;
}

int delete_graph (Vertex** list) {
	Vertex* vertex;
	Vertex* temp_vertex;
	Vertex_list* vert_list;
	Vertex_list* temp_adj;
	for (int i = 0; i < HASH_TABLE_SIZE; i++) {
		vertex = list[i];
		while(vertex) {
			vert_list = vertex->vert_list;
			while (vert_list) {
				temp_adj = vert_list->next;
				free(vert_list);
				vert_list = temp_adj;
			}
			temp_vertex = vertex->next;
			free(vertex->name);
			free(vertex);
			vertex = temp_vertex;
		}
	}
	return 0;
}

void init_dfs (Vertex** list) {
	Vertex* n_list;
	for (int i = 0; i < HASH_TABLE_SIZE; i++) {
		if (list[i]) {
			n_list = list[i];
			list[i]->for_dfs.prev = NULL;
			list[i]->for_dfs.color = 0;
			while (n_list) {
				n_list->for_dfs.prev = NULL;
				n_list->for_dfs.color = 0;
				n_list = n_list->next;
			}
		}
	}
}

void DFS_one_vert (Vertex* v) {
	puts("\n---\n");
	Vertex_list* save = v->vert_list;
	Vertex_list* lvert = v->vert_list;
	v->for_dfs.color = 1;
	while (lvert) {
		//printf("adj vert %d, %d ==color %d==\n", lvert->vertex->coords.x, lvert->vertex->coords.y, lvert->vertex->for_dfs.color);
		if (lvert->vertex->for_dfs.color == 0) {
			lvert->vertex->for_dfs.prev = v;//!
			DFS_one_vert(lvert->vertex);
		}
		lvert = lvert->next;
	}
	v->for_dfs.color = 2;
	v->vert_list = save;
}

int DFS_vert_search (Vertex* v, Vertex* search, int* result) {
	Vertex_list* save = v->vert_list;
	Vertex_list* lvert = v->vert_list;
	v->for_dfs.color = 1;
	while (lvert) {
		if ((lvert->vertex->coords.x == search->coords.x) && (lvert->vertex->coords.y == search->coords.y)) {
			*result = 1;
		}
		if (lvert->vertex->for_dfs.color == 0) {
			lvert->vertex->for_dfs.prev = v;
			DFS_vert_search(lvert->vertex, search, result);
		}
		lvert = lvert->next;
	}
	v->for_dfs.color = 2;
	v->vert_list = save;
	return 0;
}

void init_Dijkstra (Vertex** list, int* count) {
	Vertex* ptr;
	*count = 0;
	for (int i = 0; i < HASH_TABLE_SIZE; i++) {
		if (list[i]) {
			(*count)++;
			list[i]->min_way = LIMIT;
			list[i]->visited = 0;
			ptr = list[i]->next;
			if (ptr) {
				(*count)++;
				ptr->min_way = LIMIT;
				ptr->visited = 0;
				ptr = ptr->next;		
			}
		}
	}
}

Vertex* find_Dijkstra_min (Vertex* v) {
	int d_min = LIMIT;
	int index = -1;
	Vertex_list* save = v->vert_list;
	Vertex_list* vert = v->vert_list;
	while (vert) {
		if ((vert->vertex->min_way <= d_min) && (vert->vertex->visited == 0)) {
			d_min = vert->vertex->min_way;
			index = 1;
		}
		vert = vert->next;
	}
	if (!index) {
		return NULL;
	}
	while (save) {
		if ((save->vertex->visited == 0) && (save->vertex->min_way == d_min)) {
			return save->vertex;
		}
		save = save->next;
	}
}

void Dijkstra (Vertex* v, Vertex** graph, int* i) {
	if ((*i) == 0) {
		v->min_way = 0;
		if (!v->vert_list) {
			return;
		}
	}
	(*i)++;
	int sum;
	Vertex_list* vert = v->vert_list;
	Vertex_list* save = v->vert_list;
	while (vert) {
		if (vert->vertex->visited == 0) {
			sum = v->min_way + vert->weight;
			if (sum < vert->vertex->min_way) {
				vert->vertex->min_way = sum;
			}
		}
		vert = vert->next;
	}
	v->visited = 1;
	Vertex* min_v = find_Dijkstra_min(v);
	if (!min_v) {
		return;
	}
	Dijkstra(min_v, graph, i);
}

void DOT (Vertex** list) {
	Vertex_list* vert;
	Vertex* ptr;
	printf("\ndigraph G {\n");
	for (int i = 0; i < HASH_TABLE_SIZE; i++) {
		if (list[i]) {
			printf("\t%s -> ", list[i]->name);
			printf("{");
			vert = list[i]->vert_list;
			while (vert) {
				printf("%s ", vert->vertex->name);
				vert = vert->next;
			}
			printf("}\n");
			ptr = list[i]->next;
			if (ptr) {
				printf("\t%s -> ", ptr->name);
				printf("{");
				vert = ptr->vert_list;
				while (vert) {
					printf("%s ", vert->vertex->name);
					vert = vert->next;
				}
				printf("}\n");	
				ptr = ptr->next;		
			}
		}
	}
	printf("}");
}