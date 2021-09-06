#include <stdio.h>
#include <stdlib.h>
#include "deque.h"
#ifdef DQ_LIST
#include "list.h"
#else 
#include "vector.h"
#endif

dq_container* _dq;

int menu(const char *msgs[], const int n) {
	int choice;
	choice = 0;;
	for (int i = 0; i < n; ++i) {
		puts(msgs[i]);
	}
	puts("\nMake your choice: \n");
	int k;
	k = scanf("%d", &choice);
	getchar();
	if (k <= 0) {
		puts("\nYou're wrong. Try again!\n");
		while (getchar() != '\n') {}
		return 0;
	}
	if ((choice < 1) || (choice > 3)) {
		puts("\nYou're wrong. Try again!\n");
		return -1;
	}
	return choice;
}

int dialog (void) {
	const char *MSGS[] = {"1. Exit", "2. Display the status of the queue (not working) ", "3. Add a patient to the queue"};
	const int MSGS_SIZE = 3;
	int choice;
	do {
		choice = menu(MSGS, MSGS_SIZE);
		if (choice == 1) {
			exit (0);
		}
		if (choice == 2) {
			puts("\n> Display the status of the queue <\n");
		}
		if (choice == 3) {
			puts("\n> Add a patients to the queue <\n");
			_dq = create_dq();
			if (_dq == NULL) {
				return 1;
			}
			get_str(_dq->vector->patient);
			delete_dq(_dq);
		}
	} while (choice != 1);
	return 0;
}

int main (void) {
	dialog();
	return 0;
}
