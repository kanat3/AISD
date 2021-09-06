#include <stdio.h>
#include <stdlib.h>
#include "graph.h"
#include "dialog.h"
#include <time.h>

#define DIALOG_CHOICES 11
#define GRAPH_FILE "graph1.txt"
#define WGRAPH_FILE "wgraph1.txt"
#define TEST_FILE "test.txt"

int dialog (void) {
	puts("\n");
	puts("1. Insert graph from file\n");
	puts("2. Insert new vertex in the graph\n");
	puts("3. Insert new edge in the graph\n");
	puts("4. Show graph\n");
	puts("5. Delete vertex\n");
	puts("6. Delete edge\n");
	puts("7. Dijkstra\n");
	puts("8. Find vertix by DFS\n");
	puts("9. Save graph in the file\n");
	puts("10. DOT file\n");
	puts("11. Exit\n");
	puts("\nMake your choice: \n");
	int k, choice;
	k = scanf("%d", &choice);
	if (_feof() == 0) {
		exit(-1);
	}
	getchar();
	if (k <= 0) {
		puts("\nYou're wrong. Try again!\n");
		while (getchar() != '\n') {}
		return -1;
	}
	if ((choice < 1) || (choice > DIALOG_CHOICES)) {
		puts("\nYou're wrong. Try again!\n");
		return -2;
	}
	return choice;
}

int main (void) {
	clock_t start_add, end_add;
	int VERTEX_COUNT = 0;
	Vertex* Graph[HASH_TABLE_SIZE];
	for (int i = 0; i < HASH_TABLE_SIZE; i++) {
		Graph[i] = NULL;
	}
	int choise = -1;
	while (choise < 0 || choise != DIALOG_CHOICES) {
		do {
			choise = dialog();
		} while (choise < 0);

		if (choise == 1) {
			FILE* file = fopen(GRAPH_FILE, "r");
		    if (!file) {
		        printf("\n> Can't open file <\n");
		       	return 0;
		    }
		    read_file(Graph, file, &VERTEX_COUNT);
		    fclose(file);
		} else if (choise == 2) {
			Vertex *vertex;
			vertex = create_vertex();
			if (vertex) {
				if (add_vertex(Graph, vertex) == 2) {
					puts("> Can't add. The coordinates must be unique <");
				}
			}
		} else if (choise == 3) {
			int x1, x2, y1, y2, weight, check;
			puts("\nThe link is added to the first vertex entered!\n");
			puts("\nFirst vertex coordinates:");
			puts("x:");
			get_int(&x1);
			puts("y:");
			get_int(&y1);
			puts("\n");
			puts("\nSecond vertex coordinates:");
			puts("x:");
			get_int(&x2);
			puts("y:");
			get_int(&y2);
			weight = 1;

			check = add_edge(Graph, x1, x2, y1, y2, weight);
			if (check == 2) {
				printf("\n> Can't add <\n");
			} else if (check == 3) {
				puts("> The edge already exists <");
			}
		} else if (choise == 4) {
			show_graph(Graph);
		} else if (choise == 5) {
			int x, y, check;
			puts("\nVertex coordinates:");
			puts("x:");
			get_int(&x);
			puts("y:");
			get_int(&y);

			check = delete_vetrex(Graph, x, y);
			if (check) {
				printf("\n> Can't delete <\n");
			}
		} else if (choise == 6) {
			int x1, x2, y1, y2, check;
			puts("\nFirst vertex coordinates:");
			puts("x:");
			get_int(&x1);
			puts("y:");
			get_int(&y1);
			puts("\nSecond vertex coordinates:");
			puts("x:");
			get_int(&x2);
			puts("y:");
			get_int(&y2);

			check = delete_edge(Graph, x1, x2, y1, y2);
			if (check == 2 || check == 1) {
				printf("\n> Can't delete < \n");
			}
		} else if (choise == 8) {
			//start_dfs = clock();
			init_dfs(Graph);
			DFS_SHELL(Graph);
			//end_dfs = clock();
			//printf("\n==7== Search time: %.12fs\n", (end_dfs - start_dfs) / (double)CLOCKS_PER_SEC);
		} else if (choise == 9) {
			FILE* wfile;
			wfile = fopen(WGRAPH_FILE, "w");
		    if (!wfile) {
		        printf("\n> Can't open file <\n");
		       	return 0;
		    }
			write_file(Graph, wfile);
			fclose(wfile);
		} else if (choise == 11) {
			delete_graph(Graph);
		} else if (choise == 7) {
			Dijkstra_SHELL(Graph);
		} else if (choise == 10) {
			DOT(Graph);
		}
	}
	return 0;
}