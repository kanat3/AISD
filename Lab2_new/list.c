#include <stdlib.h>
#include <stdio.h>
#include "deque.h"

Deque* create_dq () {

	Deque *deque = (Deque*) malloc( sizeof(Deque) );
	deque->front = deque->rear = NULL;
	deque->size = 0;
	deque->clearTime = 0;

	return deque;
}

int push_front (Deque* deque, Patient patient) {

	Node* newNode = (Node*) malloc(sizeof(Node));
	newNode->data = patient;
	newNode->prev = newNode->next = NULL;
	if (newNode == NULL)
		return 0;
	if (deque->front == NULL)
		deque->rear = deque->front = newNode;
	else {  
		newNode->next = deque->front;
		deque->front->prev = newNode;
		deque->front = newNode;
	}
	++deque->size;
	return 1;
}

int push_rear (Deque* deque, Patient patient) {
	Node* newNode = (Node*) malloc(sizeof(Node));
	newNode->data = patient;
	newNode->prev = newNode->next = NULL;
	if (newNode == NULL)
		return 0;
	if (deque->rear == NULL)
		deque->rear = deque->front = newNode;
	else {
		newNode->prev = deque->rear;
		deque->rear->next = newNode;
		deque->rear = newNode;
	}
	++deque->size;
	return 1;
}

int pop_front (Deque* deque) {
	if (deque->front == NULL)
		return 0;
	Node *temp = deque->front;
	deque->front = deque->front->next;

	if (deque->front == NULL){
		deque->rear = NULL;
	}
	else {
		deque->front->prev = NULL;
	}
	free(temp);
	--deque->size;
	return 1;
}

int pop_rear (Deque* deque) {
	// если очередь пустая
	if (deque->front == NULL)
		return 0;
	// 1 3 5 3 [2]
	Node *temp = deque->rear;
	deque->rear = deque->rear->prev;
	if (deque->rear == NULL)
		deque->front = NULL;
	else
		deque->rear->next = NULL;
	free(temp);
	--deque->size;
	return 1;
}

Patient get_front (Deque* deque) {
	if (deque->front == NULL) {
		Patient patient;
		patient.appear = -1;
		patient.duration = 0;
		patient.tm = 0;
		return patient;
	}
	return deque->front->data;

}

Patient get_rear (Deque* deque) {
	if (deque->rear == NULL) {
		Patient patient;
		patient.appear = -1;
		patient.duration = 0;
		patient.tm = 0;
		return patient;
	}
	return deque->rear->data;
}

void show_patient (Patient* patient) {
	printf("%s/%d/%d  | waiting time: %d\n", patient->id, patient->appear, patient->duration, patient->tm);
}

int is_full (Deque* deque) {
	return 0;
}

int is_empty (Deque* deque) {
	return (deque->front == NULL);
}

void show_dq (Deque* deque) {
	Node *node = deque->front;
	while (node != NULL) {
		show_patient(&node->data);
		node = node->next;
	}
}

void time_up (Deque* deque, Patient* patient, int t) {

	int counter = 0;
	Node *node = deque->front;
	while (node->next != NULL) {

		if (counter > 1) {
			node->data.tm += patient->duration;
		}
		node = node->next;
		++counter;
	}
}

void time_down (Deque* deque, int t) {

	int koef = 0;
	if ( deque->front->data.appear == 1 && !deque->clearTime) {
			koef = 1;
	// 5 - 25 + 21 = 1
	}
	deque->front->data.tm = deque->front->data.duration - t + koef + deque->clearTime;
	Node* node = deque->front->next;
	while (node != NULL) {
		node->data.tm = node->prev->data.tm + node->data.duration;
		node = node->next;
	}
}


