#pragma once

#define HASH_TABLE_SIZE 16

typedef struct Coordinates {
	int x;
	int y;
} Coordinates;

typedef struct DFS_search {
	int color; //0 - white 1 - grey 2 - black
	struct Vertex* prev; //для меток времени
} DFS_search;

typedef struct Vertex {
	char* name;
	struct DFS_search for_dfs;
	int min_way; // для dijkstra
	int visited;
	struct Coordinates coords;
	struct Vertex_list* vert_list;
	struct Vertex* next;
} Vertex;

typedef struct Vertex_list {
	Vertex* vertex;
	int weight;
	struct Vertex_list* next;	
} Vertex_list;


int add_vertex(Vertex** list, Vertex* vertex);
int add_edge(Vertex** list, int x1, int x2, int y1, int y2, int weight);
int delete_vetrex(Vertex** list, int x, int y);
int delete_edge(Vertex** list, int x1, int x2, int y1, int y2);
Vertex* is_vertex(Vertex** list, int x, int y);
int get_hash(int x, int y);
void show_graph(Vertex** list);
int read_file(Vertex** list, FILE* fd, int* VERTEX_COUNT);
int delete_graph(Vertex** list);
void delete_vertex_in_adj_list (Vertex* list, int x, int y);
void DFS_one_vert (Vertex* v);
void init_dfs (Vertex** list);
int DFS_vert_search (Vertex* v, Vertex* search, int* result);
int write_file(Vertex** list, FILE *fd);
void init_Dijkstra (Vertex** list, int* count);
void Dijkstra (Vertex* v, Vertex** graph, int* i);
Vertex* find_Dijkstra_min (Vertex* v);
void show_Dgraph (Vertex** list);
Vertex* generate_vertex (void);
int generate_graph (Vertex** list, int k);
void DOT (Vertex** list);