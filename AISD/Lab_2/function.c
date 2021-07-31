#define _GNU_SOURCE
#include "function.h"
#include "deque.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void separation (char** string, char* save_str) {
	char* pch;
	char* str = *string;
	pch = strtok (str, " ");
	while (pch) {
		//write into deque
		check_input(pch, save_str);
		*string += strlen(pch);
		pch = strtok (NULL, " "); 
	}
	*string++;
}

int get_str (void) {
	int result;
	char* save_str;
	size_t len = 0;
	char* string = NULL;
	puts("\nEnter information about patients: ");
	result = getline(&string, &len, stdin);
	save_str = string;
	if (result < 0) {
		printf("\nError in getline()\n");
		free(save_str);
		return 1;
	}
	if (string) {
		printf("\nThis is your string: \n%s\n", string);
		separation(&string, save_str);
		free(save_str);
	}
	return 0;
}

char* check_input (char* string, char* save_str) {
	int i = 0;
	int len, t;
	t = 1;
	len = strlen(string);
	while (*(string + i) != '/' && (t <= len)) {
		if ((*(string + i) < '0' && *(string + i) > '9') || (*(string + i) < 'a' && *(string + i) > 'z') || (*(string + i) < 'A' && *(string + i) > 'Z')) {
			puts("\nInvalid input. Please enter by template: <priority>/<id>/<ta>/<ts> ");
			puts("where priority is '*' or '', id is a string, ta and ts are integer\n");
			free(save_str);
			exit (1);
		}
		i++;
		len--;
	}
	 len--;
	i++;
	while (*(string + i) != '/') {
		if (*(string + i) < '0' || *(string + i) > '9') {
			puts("\nInvalid input. Please enter by template: <priority>/<id>/<ta>/<ts> ");
			puts("where priority is '*' or '', id is a string, ta and ts are integer\n");
			free(save_str);
			exit (1);
		}
		i++;
		len--;
	}
	i++;
	len--;
	len--;
	while (len) {
		if (*(string + i) < '0' || *(string + i) > '9') {
			puts("\nInvalid input. Please enter by template: <priority>/<id>/<ta>/<ts> ");
			puts("where priority is '*' or '', id is a string, ta and ts are integer\n");
			free(save_str);
			exit (1);
		}
		i++;
		len--;
	}
}
