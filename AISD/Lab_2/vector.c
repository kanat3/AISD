#include "vector.h"
#include "deque.h"
#include <stdio.h>
#include <stdlib.h>
#define Q 5

int create_vector(dq_container* dq) {
	dq->vector = (Vector*)calloc(1, sizeof(Vector));
	if (dq->vector == NULL) {
		puts("\n> Memory allocation error in struct vector (based on Vector) <\n");
		return 1;
	}
	dq->vector->patient = (Patients*)calloc(Q, sizeof(Patients));
	if (dq->vector->patient == NULL) {
		puts("\n> Memory allocation error in struct patient (based on Vector) <\n");
		free(dq->vector);
		return 1;
	}
	return 0;
}

void delete_vector(dq_container* dq) {
	free(dq->vector->patient);
	free(dq->vector);
}

void show_vector (dq_container* dq) {
	if (dq->head < dq->tail) {
		for (int i = dq->head; i < dq->tail; i++) {
			printf("%s\n", dq->vector->patient[i].id);
		}
	} else if (dq->head > dq->tail || dq->count > 1) {
		for (int i = dq->head; i < Q; i++) {
			printf("%s\n", dq->vector->patient[i].id);
		}
		for (int i = 0; i < dq->tail; i++) {
			printf("%s\n", dq->vector->patient[i].id);
		}
	} else {
		printf("%s\n", dq->vector->patient->id);
	}
}

void push_back (dq_container* dq, Patients p) {
	if (dq->count) {
		if (++(dq->tail) == Q) {
			dq->tail = 0;
		}
	} else {
		dq->vector->patient[dq->tail] = p;
		dq->tail++;
		dq->count++; 
	}
}

void push_ahead (dq_container* dq, Patients p) {
	if (++(dq->tail) == Q) {
		dq->tail = 0;
	} else {
		dq->vector->patient[dq->tail] = p;
		dq->tail++;
		dq->count++;
	}
}

void delete_in_vector (dq_container* dq, int i) {
	free(dq->vector->patient[i].id);
	dq->vector->patient[i].ta = 0;
	dq->vector->patient[i].ts = 0;
}