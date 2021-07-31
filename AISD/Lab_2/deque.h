#pragma once

typedef struct dq_container {
	#ifdef DQ_LIST
	struct List* list;
	#else 
	struct Vector* vector;
	int head;
	int tail;
	int count;
	#endif
} dq_container;

typedef struct Patients {
	char priority; //0 - priority, 1 - not
	char* id;
	int ta;
	int ts;
} Patients;

dq_container* create_dq (void);
void delete_dq (dq_container* dq);
int read_id (void); //имя пациента
void separation (char** string, char* save_str, Patients* patient);
char* get_str (Patients* patient);
char* check_input (char* string, char* save_str, Patients* patient, int q, int q_of_lex);

