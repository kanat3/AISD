#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "deque.h"
#ifdef DQ_LIST
#include "list.h"
#else 
#include "vector.h"
#endif
#define Q 5

dq_container* dq;

#ifdef DQ_LIST
dq_container* create_dq (void) {
	dq = (dq_container*)calloc(1, sizeof(dq_container));
	if (dq == NULL) {
		puts("\n> Memory allocation error (based on List) <\n");
		exit(1);
	}
}
#else
dq_container* create_dq (void) {
	dq = (dq_container*)calloc(1, sizeof(dq_container));
	if (dq == NULL) {
		puts("\n> Memory allocation error (based on Vector) <\n");
		exit(1);
	}
	int k = create_vector(dq);
	if (k == 1) {
		free(dq);
		return NULL;
	}
	return dq;
}
#endif

void delete_dq (dq_container* dq) {
	delete_vector(dq);
	free(dq);
}

void separation (char** string, char* save_str, Patients* patient) {
	char* pch;
	char* test;
	char* str = *string;
	char* test_str = save_str;
	pch = strtok (str, " ");
	test = strtok (test_str, " ");
	int q = 0;
	int q_of_lex = 0;
	while (test!= 0) {
		q_of_lex++;
		*save_str += strlen(test_str);
		test = strtok (NULL, " "); 
	}
	printf("Q LEX %d\n", q_of_lex);
	while (pch != 0 || q < Q) {
		//write into deque
		check_input(pch, save_str, patient, q, q_of_lex);
		q++;
		*string += strlen(pch);
		pch = strtok (NULL, " "); 
	}
	*string++;
}

char* get_str (Patients* patient) {
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
		return NULL;
	}
	if (string) {
		printf("\nThis is your string: \n%s\n", string);
		separation(&string, save_str, patient);
		free(save_str);
	}
	return save_str;
}

char* check_input (char* string, char* save_str, Patients* patient, int q, int last_lex) {
	int i = 0;
	int len, t;
	t = 1;
	len = strlen(string);
	if (q >= 1) {
		i++;
		len--;
	}
	while (*(string + i) != '/' && (t <= len)) {
		if ((*(string + i) < '0' && *(string + i) > '9') || (*(string + i) < 'a' && *(string + i) > 'z') || (*(string + i) < 'A' && *(string + i) > 'Z')) {
			puts("\nInvalid input. Please enter by template: <priority>/<id>/<ta>/<ts> ");
			puts("where priority is '*' or '', id is a string, ta and ts are integer\n");
			free(save_str);
			exit(1);
		}
		i++;
		len--;
	}
	patient[q].id = (char*)calloc(i, sizeof(char));
	for (int w = 0; w < i; w++) {
		patient[q].id[w] = *(string + w);
	}
	int sl = i;
	len--;
	i++;
	while (*(string + i) != '/') {
		if (*(string + i) < '0' || *(string + i) > '9') {
			puts("\nInvalid input. Please enter by template: <priority>/<id>/<ta>/<ts> ");
			puts("where priority is '*' or '', id is a string, ta and ts are integer\n");
			free(save_str);
			exit(1);
		}
		i++;
		len--;
	}
	int k = 0;
	for (int p = sl + 1; p < i; p++) {
		k = k * 10 + (string[p] - '0');
	}
	patient->ta = k;
	sl = i; //slash 
	i++;
	len--;
	while (len) {
		if (*(string + i) < '0' || *(string + i) > '9') {
			puts("\nInvalid input. Please enter by template: <priority>/<id>/<ta>/<ts> ");
			puts("where priority is '*' or '', id is a string, ta and ts are integer\n");
			free(save_str);
			exit(1);
		}
		i++;
		len--;
	}
	k = 0;
	if (q == last_lex) {
		for (int p = sl + 1; p <= strlen(string) - 2; p++) {
			k = k * 10 + (string[p] - '0');
		}	
	} else {
		for (int p = sl + 1; p <= strlen(string) - 1; p++) {
			k = k * 10 + (string[p] - '0');
		}
	}
	patient->ts = k;
}

