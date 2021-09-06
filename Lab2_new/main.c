#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "deque.h"

#define STR_MAX 201

int get_str (char* string, int len) {

	puts("Add a patients to the queue: ");
	if (fgets(string, len, stdin) != NULL) {
		char* str;
		if ((str = strchr(string, '\n')) != NULL) {
			*str = '\0';
		} else {
			printf("Error.\n");
			return 0;
		}
	} else {
		printf("Error.\n");
		return 0;
	}

	return 1;
}

int remove_in_front (Deque* dq, int t) {

	if (is_empty(dq))
		return -1;
	int res = 0;
	Patient front = get_front(dq);
	while (front.tm == 0 && !is_empty(dq)) {
		time_down(dq, t);
		res += pop_front(dq);
		dq->clearTime = t;
		front = get_front(dq);
	}

	return res;
}

int add_to_dq (char* str, Deque* dq, int t) {

	Patient patient;
	int k = 1;
	size_t count = 0;
	char* pos = strtok(str, "/ ");
	patient.appear = 0;
	patient.duration = 0;
	while (pos != NULL) {
		if (k == 3) {
			patient.duration = atoi(pos);
			//проверка корректности данных
			if (patient.duration == 0) {
				puts("Error.");
				return -1;
			}
			if (patient.appear == 0 || patient.duration == 0 || strlen(patient.id) == 0
					|| (patient.id[0] == '*' && strlen(patient.id) == 1))
				return -1;
			if (patient.appear == t) {
				if (patient.id[0] == '*') {
					Patient front = get_front(dq);
					patient.tm = front.tm + patient.duration;
					pop_front(dq);
					push_front(dq, patient);
					push_front(dq, front);
					time_up(dq, &patient, t);
				} else {
					Patient pre = get_rear(dq);
					patient.tm = pre.tm + patient.duration;
					push_rear(dq, patient);
				}
				++count;
			}
			k = 0;
			patient.appear = 0;
			patient.duration = 0;
			patient.id[0] = '\0';
		} else if (k == 2) {
			patient.appear = atoi(pos);
		} else if (k == 1) {
			strcpy(patient.id, pos);
		}
		pos = strtok(NULL, "/ ");
		k++;
	}
	if (k != 1) {
		return -1;
	}

	return count;
}

int main (void) {

	char str[STR_MAX];
	get_str(str, STR_MAX);
	Deque* dq = create_dq();
	if (dq == NULL) {
		return 1;
	}
	int i = 1;
	while (1) {
		char buf[STR_MAX];
		strcpy(buf, str);
		if (i > 1) {
			time_down(dq, i);
		}
		int rem, add;
		rem = remove_in_front(dq, i);
		add = add_to_dq(buf, dq, i);
		if (add == -1) {
			puts("\nInvalid input. Please enter by template: <priority>/<id>/<ta>/<ts> ");
			puts("where priority is '*' or '', id is a string, ta and ts are integer\n");
			return 1;
		}
		if (is_empty(dq)) {
			printf(">> Iteration: %d << \n\n", i);
			puts("Now queue is empty\n");
			break;
		}
		if (rem > 0 || add > 0) {
			printf(">> Iteration: %d << \n\n", i);
			show_dq(dq);
			puts("");
		}
		++i;
	}

	return 1;
}
