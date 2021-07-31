#pragma once

#define ID_MAX 30
#define DQ_MAX 5

typedef struct Patient {
	char id[ID_MAX];
	int appear;
	int duration;
	int tm;
} Patient;

typedef struct dq_vector {
	Patient arr[DQ_MAX];
	int front;
	int rear;
	int size;
	int clearTime;
} dq_vector;

typedef struct Node {
	Patient data;
	struct Node *prev;
	struct Node *next;
} Node;

typedef struct dq_list {
	Node *front;
	Node *rear;
	int size;
	int clearTime;
} dq_list;

#ifdef DQ_LIST
typedef dq_list Deque;
#else
typedef dq_vector Deque;
#endif


Deque* create_dq ();

int push_front (Deque* deque, Patient patient);

int push_rear (Deque* deque, Patient patient);

int pop_front (Deque* deque);

int pop_rear (Deque* deque);

Patient get_front (Deque* deque);

Patient get_rear (Deque* deque);

int is_full (Deque* deque);

int is_empty (Deque* deque);

void show_patient (Patient* patient);

void show_dq (Deque* deque);

void time_up (Deque* deque, Patient *patient, int t);

void time_down (Deque* deque, int t);
