#pragma once
#include "deque.h"

typedef struct Vector {
	Patients* patient;
	int dq_lenght;
} Vector;

int create_vector(dq_container* dq);
void show_vector (dq_container* dq);
void delete_vector(dq_container* dq);
void push_back (dq_container* dq, Patients p);
void push_ahead (dq_container* dq, Patients p);
void delete_in_vector (dq_container* dq, int i);